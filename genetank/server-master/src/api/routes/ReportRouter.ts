import * as async from 'async';
import { Router, Request, Response } from 'express';

import * as db from '../../models/db';
import { IUserRequest } from '../../models/types';
import * as bodyMapData from './body_map';

export class ReportRouter {
  public router: Router;

  constructor() {
    this.router = Router();
    this.init();
  }

  public init(): void {
    this.router.get('/genetic', this.getGeneticReport);
    this.router.get('/body-map', this.getBodyMap);
    this.router.get('/phenotypic', this.getPhenotypicReport);
  }

  // Returns the current status of a user's genetic report, and results if the report is completed.
  // Input parameters: user
  // Output: { status: string, report: [{ disease: { id: string, name: string }, result: string }] }
  private getGeneticReport(req: IUserRequest, res: Response): void {
    const userId: string = req.user.sub;
    async.waterfall(
      [
        // Check report_status to see whether the report is completed
        (callback: Function) => {
          db.query('SELECT genetic FROM report_status WHERE user_id = ?', [userId], (err, rows) => {
            if (err) {
              callback(err);
            } else if (!rows || !rows.length) {
              callback({ status: 400, message: 'That user does not exist.' });
            } else {
              if (rows[0].genetic !== 'complete') {
                // Report not done yet, tell the user
                res.status(200).send({ status: rows[0].genetic });
              } else {
                // Report is complete
                callback(null);
              }
            }
          });
        },
        // Retrieve the report results from genetic_report
        (callback: Function) => {
          db.query('SELECT disease_id, result FROM genetic_report WHERE user_id = ?', [userId], (err, rows) => {
            if (err) {
              callback(err);
            } else {
              callback(null, rows);
            }
          });
        },
        // Get disease information to correspond ids to names
        (results: any, callback: Function) => {
          const ids: Array<string> = results.map((row) => row.disease_id);
          db.query('SELECT id, name FROM diseases WHERE id IN (?)', [ids], (err, rows) => {
            if (err) {
              callback(err);
            } else {
              // Create a map of diseases
              let diseases: any = {};
              rows.forEach((row) => {
                diseases[row.id] = row.name;
              });
              callback(null, results, diseases);
            }
          });
        },
        // Transform results into a JSON array
        (results: any, diseases: any, callback: Function) => {
          let reportResults: Array<any> = [];
          results.forEach((row) => {
            reportResults.push({ disease: { id: row.disease_id, name: diseases[row.disease_id] }, result: row.result });
          });
          res.send({ status: 'complete', report: reportResults });
        }
      ],
      (err) => {
        if (err && err.status && err.message) {
          res.status(err.status).send(err.message);
        } else if (err) {
          res.status(500).send(err);
        }
      });
  }

  // Returns a user's body map.
  // Input parameters: user
  // Output: IBodyMap
  private getBodyMap(req: IUserRequest, res: Response): void {
    res.json(bodyMapData);
  }

  // Returns the current status of a user's phenotypic report, and results if the report is completed.
  // Input parameters: user
  // Output: { status: string, report: [{ disease: { id: string, name: string }, result: string }] }
  private getPhenotypicReport(req: IUserRequest, res: Response): void {
    const userId: string = req.user.sub;
    async.waterfall(
      [
        // Check report_status to see whether the report is completed
        (callback: Function) => {
          db.query('SELECT phenotype FROM report_status WHERE user_id = ?', [userId], (err, rows) => {
            if (err) {
              callback(err);
            } else if (!rows || !rows.length) {
              callback({ status: 400, message: 'That user does not exist.' });
            } else {
              if (rows[0].phenotype !== 'complete') {
                // Report not done yet, tell the user
                res.status(200).send({ status: rows[0].phenotype });
              } else {
                // Report is complete
                callback(null);
              }
            }
          });
        },
        // Retrieve the report results from phenotype_report
        (callback: Function) => {
          db.query('SELECT disease_id, result FROM phenotype_report WHERE user_id = ?', [userId], (err, rows) => {
            if (err) {
              callback(err);
            } else {
              callback(null, rows);
            }
          });
        },
        // Get disease information to correspond ids to names
        (results: any, callback: Function) => {
          const ids: Array<string> = results.map((row) => row.disease_id);
          db.query('SELECT id, name FROM diseases WHERE id IN (?)', [ids], (err, rows) => {
            if (err) {
              callback(err);
            } else {
              // Create a map of diseases
              let diseases: any = {};
              rows.forEach((row) => {
                diseases[row.id] = row.name;
              });
              callback(null, results, diseases);
            }
          });
        },
        // Transform results into a JSON array
        (results: any, diseases: any, callback: Function) => {
          let reportResults: Array<any> = [];
          results.forEach((row) => {
            reportResults.push({ disease: { id: row.disease_id, name: diseases[row.disease_id] }, result: row.result });
          });
          res.send({ status: 'complete', report: reportResults });
        }
      ],
      (err) => {
        if (err && err.status && err.message) {
          res.status(err.status).send(err.message);
        } else if (err) {
          res.status(500).send(err);
        }
      });
  }
}

const reportRoutes: ReportRouter = new ReportRouter();
reportRoutes.init();

export default reportRoutes.router;
