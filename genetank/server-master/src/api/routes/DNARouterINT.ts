// Node modules
import * as async from 'async';
import { Router, Request, Response } from 'express';

// Local modules
import { SQLWrapper } from '../../models/SqlWrapper';
import { IUserRequest, IQuestion } from '../../models/types';
import Task from '../../analysis/models/Task';
import TaskDispatcher from '../../analysis/routes/TaskDispatcher';

export class DNARouterINT {

  // NOTE, the db should already be initialized. I recommend doing it when setting
  // up the express app somewhere in index
  private static _db: SQLWrapper = SQLWrapper.getInstance();

  public static createRouter(): Router {
    const router: Router = Router();

    // Setup router handlers
    router.get('/status', this._getDNAStatus);
    router.post('/register', this._registerDNAFile);
    router.get('/phenotypic-survey', this._getPhenotypicSurvey);
    router.post('/phenotypic-survey', this._insertPhenotypicSurveyAnswer);
    router.post('/phenotypic-survey/complete', this._startPhenotypicReport);

    return router;
  }

  // Returns the current status of a user's DNA, either none or uploaded.
  // Input parameters: user
  // Output: status: string
  private static async _getDNAStatus(req: IUserRequest, res: Response): Promise<void> {
    const patientId: string = req.user.sub;

    // Init response params
    let message: string = null;

    try {
      // NOTE: Need to strongly type rows object
      const geneticRows: any = await this._db.query(
        'SELECT genetic \
                    FROM report_status WHERE user_id = ?',
        [patientId]);

      // Sanity check
      if (!geneticRows || !geneticRows.length) {
        message = 'That user does not exist.';
        res.status(400).json(message);
      } else {
        if (geneticRows[0].genetic === 'none') {
          message = 'none';
        } else {
          message = 'uploaded';
        }
        res.status(200).json(message);
      }

    } catch (err) {
      // All errors are handled here
      this._handleCatchError(err, res);
    }
  }

  // Associates a user's uploaded DNA with the user and kicks off the DNA analysis pipeline.
  // Input parameters: user
  // Output: response status
  private static async _registerDNAFile(req: IUserRequest, res: Response): Promise<void> {
    const patientId: string = req.user.sub;
    const source: string = req.body.source;

    // Init response params
    let message: string = null;

    // Check inputs
    if (!patientId || !source || !['23andme', 'ancestry'].includes(source)) {
      message = 'Invalid params.';
      res.status(400).json(message);
      return; // Stops code execution OR put rest of code in else block and remove return
    } else {
      // All ok, continue...
    }

    try {
      // Make sure user exists
      const userRows: any = await this._db.query('SELECT * FROM users WHERE id = ?', [patientId]);

      // Sanity check
      if (!userRows || !userRows.length) {
        message = 'Invalid user';
        res.status(400).json(message);
      } else {
        // All ok, continue.

        // Update report status for user
        await this._db.query(
          'UPDATE report_status SET genetic = ?, ancestry = ? WHERE user_id = ?',
          ['pending', 'pending', patientId]);

        // Construct new DNA analysis task pipeline
        const dnaAnalysis: Task = new Task({
          args: ['pullDownUpload.js', 'genetank-uploads', patientId],
          cmd: 'node',
          name: 'pull-down-upload'
        });
        const prepTask: Task = dnaAnalysis.addNextTask({
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
        res.status(200).json(message);
      }
    } catch (err) {
      // All errors are handled here
      this._handleCatchError(err, res);
    }
  }

  // Returns the questions for the phenotypic report survey.
  // Input parameters: none
  // Output: [IQuestion]
  private static _getPhenotypicSurvey(req: Request, res: Response): void {
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

  // Records the answers for the phenotypic survey, returning whether the answer was successfully recorded.
  // Input parameters: user
  // Output: response status
  private static async _insertPhenotypicSurveyAnswer(req: IUserRequest, res: Response): Promise<void> {
    const patientId: string = req.user.sub;

    // Init response params
    let message: string = null;

    // Check inputs
    if (!Array.isArray(req.body.response) || req.body.response.some(x => typeof x !== 'string')) {
      message = 'Expected different answer format.';
      res.status(400).json(message);
      return; // Stops code execution OR put rest of code in else block and remove return
    } else {
      // All ok, continue...
    }

    try {
      // Verify user existence
      const userRows: any = await this._db.query('SELECT 1 FROM users WHERE id = ?', [patientId]);

      // Sanity check
      if (!userRows || !userRows.length) {
        message = 'That user does not exist.';
        res.status(400).json(message);
      } else {
        // Read in the relevant diseases
        const diseaseRows: any = await this._db.query('SELECT id, name FROM diseases WHERE name IN (?)', [req.body.response]);

        const diseases: any = {};
        diseaseRows.forEach(row => {
          diseases[row.name] = row.id;
        });

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
          await this._db.query('INSERT INTO phenotype_answers_disease (user_id, disease_id) VALUES ?', [entries]);
        } else {
          // continue
        }

        res.status(200).json(message);
      }
    } catch (err) {
      this._handleCatchError(err, res);
    }
  }

  // Kicks off the phenotypic report analysis pipeline.
  // Input parameters: user
  // Output: response status
  private static async _startPhenotypicReport(req: IUserRequest, res: Response): Promise<void> {
    const patientId: string = req.user.sub;

    // TODO: Start the phenotypic report job.
    res.sendStatus(200);
    return;
  }

  private static _handleCatchError(error: any, response: Response): void {
    // Init response params
    let status: number = 500;
    let message: string = null;

    if (error && error.status && error.message) {
      status = error.status;
      message = error.message;
    } else if (error) {
      status = 500;
      message = error;
    }

    response.status(status).json(message);
  }
}
