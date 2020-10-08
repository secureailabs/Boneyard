import * as async from 'async';
import { Router, Request, Response } from 'express';

import * as db from '../../models/db';
import Task from '../../analysis/models/Task';
import TaskDispatcher from '../../analysis/routes/TaskDispatcher';

interface IUserRequest extends Request {
  user: any;
}

interface IQuestion {
  id: number;
  question: string;
  answer: string[];
  type: string;
}

export class DNARouter {
  public router: Router;

  constructor() {
    this.router = Router();
    this.init();
  }

  public init(): void {
    this.router.get('/status', this.getDNAStatus);
    this.router.post('/register', this.registerDNAFile);
    this.router.get('/phenotypic-survey', this.getPhenotypicSurvey);
    this.router.post('/phenotypic-survey', this.insertPhenotypicSurveyAnswer);
    this.router.post('/phenotypic-survey/complete', this.startPhenotypicReport);
  }

  private getDNAStatus(req: IUserRequest, res: Response): void {
    const patientId: string = req.user.sub;
    async.waterfall(
      [
        // Check report_status db
        (callback: Function) => {
          db.query('SELECT genetic FROM report_status WHERE user_id = ?', [patientId], (err, rows) => {
            if (err) {
              callback(err);
            } else if (!rows || !rows.length) {
              callback({ status: 400, message: 'That user does not exist.' });
            } else {
              if (rows[0].genetic === 'none') {
                res.send({ status: 'none' });
              } else {
                res.send({ status: 'uploaded' });
              }
            }
          });
        }
      ],
      (err) => {
        if (err && err.status && err.message) {
          res.status(err.status).send(err.message);
        } else if (err) {
          res.status(500).send(err);
        }
      }
    );
  }

  private registerDNAFile(req: IUserRequest, res: Response): void {
    const patientId: string = req.user.sub;
    const source: string = req.body.source;

    async.waterfall(
      [
        // Check inputs
        function (callback: Function): void {
          if (patientId && source && ['23andme', 'ancestry'].includes(source)) {
            callback(null);
          } else {
            callback({ status: 400, message: 'Invalid params' });
          }
        },
        // Make sure user exists
        function (callback: Function): void {
          db.query('SELECT * FROM users WHERE id = ?', [patientId], (err, rows) => {
            if (err) {
              callback(err);
            } else if (!rows || !rows.length) {
              callback({ status: 400, message: 'Invalid user' });
            } else {
              callback(null, rows);
            }
          });
        },
        // Update report status for user
        function (rows: any, callback: Function): void {
          db.query(
            'UPDATE report_status SET genetic = ?, ancestry = ? WHERE user_id = ?',
            ['pending', 'pending', patientId],
            err => {
              callback(err);
            }
          );
        },
        // Construct new DNA analysis task pipeline
        function (callback: Function): void {
          let dnaAnalysis: Task = new Task({
            args: ['pullDownUpload.js', 'genetank-uploads', patientId],
            cmd: 'node',
            name: 'pull-down-upload'
          });
          let prepTask: Task = dnaAnalysis.addNextTask({
            args: ['createEnvForAnalysis.js', patientId, 'output/'],
            cmd: 'node',
            name: 'create-env-for-analysis'
          });
          let vcfTask: Task;
          if (source === '23andme') {
            vcfTask = prepTask.addNextTask({
              args: ['23andme/23andmeVersionBuild_convert_liftover2.py', patientId + '.zip', 'output/Patient_' + patientId, 'reference/'],
              cmd: 'python',
              name: 'convert-23andme-to-vcf'
            });
          } else {
            vcfTask = prepTask.addNextTask({
              args: ['test.sh', patientId],
              cmd: 'sh',
              name: 'convert-ancestry-to-vcf'
            });
          }
          vcfTask.addNextTask({
            args: ['VCFToPrediction.R', patientId, patientId, 'output/', 'reference/'],
            cmd: 'rscript',
            name: 'genotype-to-phenotype-prediction'
          }).addNextTask({
            args: ['storeGenotypeToPhenotypeResults.js', patientId, 'output/'],
            cmd: 'node',
            name: 'store-genotype-to-phenotype-results'
            // }).addNextTask({
            //   args: ['test.sh', patientId],
            //   cmd: 'sh',
            //   name: 'genotype-to-ancestry-prediction'
            // }).addNextTask({
            //   args: ['storeGenotypeToAncestryResults.js', patientId, username],
            //   cmd: 'node',
            //   name: 'store-genotype-to-ancestry-results'
          }).addNextTask({
            args: ['uploadRawAndVCF.js', 'genetank-dna-files', patientId, 'genetank-uploads', 'output/'],
            cmd: 'node',
            name: 'upload-raw-and-vcf'
          }).addNextTask({
            args: ['cleanIntermediateFiles.js', patientId, 'output/'],
            cmd: 'node',
            name: 'clean-intermediate-files'
          });

          // Kick off DNA analysis task
          TaskDispatcher.sendTask(dnaAnalysis);

          // Reply to requester
          res.sendStatus(200);
        }
      ],
      err => {
        if (err && err.status && err.message) {
          res.status(err.status).send(err.message);
        } else if (err) {
          res.status(500).send(err);
        }
      });
  }

  private getPhenotypicSurvey(req: Request, res: Response): void {
    // TODO: Generate phenotypic survey from database.
    const sampleSurvey: Array<IQuestion> = [
      {
        answer: ['Follicular Lymphoma', 'Pink Disease', 'Hypokalemia', 'Amyotrophic Lateral Sclerosis'],
        id: 1,
        question: 'Which of these diseases do you have?',
        type: 'multiple-select'
      },
      {
        answer: ['Eating Disorder', 'Dysphasia', 'Cystic Fibrosis'],
        id: 2,
        question: 'Which of these diseases do you have?',
        type: 'multiple-select'
      }
    ];
    res.json(sampleSurvey);
  }

  private insertPhenotypicSurveyAnswer(req: IUserRequest, res: Response): void {
    const patientId: string = req.user.sub;
    async.waterfall(
      [
        // We expect the response to be an array of strings
        (callback: Function) => {
          if (!Array.isArray(req.body.response) || req.body.response.some(x => typeof x !== 'string')) {
            callback({ status: 400, message: 'Expected different answer format.' });
          } else {
            callback(null);
          }
        },
        // Verify user existence
        (callback: Function) => {
          db.query('SELECT 1 FROM users WHERE id = ?', [patientId], (err, rows) => {
            if (err) {
              callback(err);
            } else if (!rows || !rows.length) {
              callback({ status: 400, message: 'That user does not exist.' });
            } else {
              callback(null);
            }
          });
        },
        // Read in the relevant diseases
        (callback: Function) => {
          db.query('SELECT id, name FROM diseases WHERE name IN (?)', [req.body.response], (err, rows) => {
            if (err) {
              callback(err);
            } else {
              // Create a map of diseases
              let diseases: any = {};
              rows.forEach(row => {
                diseases[row.name] = row.id;
              });
              callback(null, diseases);
            }
          });
        },
        // Insert the selected diseases for this user
        (diseases: any, callback: Function) => {
          let entries: any = req.body.response.map(value => {
            if (diseases[value]) {
              return [patientId, diseases[value]];
            } else {
              return undefined;
            }
          });
          // Remove all the diseases that weren't in the database
          entries = entries.filter(element => {
            return element !== undefined;
          });
          if (entries.length) {
            // Bulk insert results into database
            db.query('INSERT INTO phenotype_answers_disease (user_id, disease_id) VALUES ?', [entries], err => {
              if (err) {
                callback(err);
              } else {
                res.sendStatus(200);
              }
            });
          } else {
            res.sendStatus(200);
          }
        }
      ],
      err => {
        if (err && err.status && err.message) {
          res.status(err.status).send(err.message);
        } else if (err) {
          res.status(500).send(err);
        }
      }
    );
  }

  private startPhenotypicReport(req: IUserRequest, res: Response): void {
    const patientId: string = req.user.sub;
    const source: string = req.body.source;

    // TODO: Start the phenotypic report job.
    res.sendStatus(200);
    return;
  }
}

const dnaRoutes: DNARouter = new DNARouter();
dnaRoutes.init();

export default dnaRoutes.router;
