import { Router, Request, Response } from 'express';

import * as db from '../../models/db';

export class SubscribeRouter {
  public router: Router;

  constructor() {
    this.router = Router();
    this.init();
  }

  public init(): void {
    this.router.post('/', this.subscribeEmail);
  }

  // Adds the email to the mailing list database.
  // Input parameters: email: string
  // Output: response status, string
  private subscribeEmail(req: Request, res: Response): void {
    if (req.body.email) {
      db.query('INSERT INTO subscribe (email) VALUES (?)', [req.body.email], (err) => {
        if (err) {
          res.status(500).send('Sorry, something went wrong. Please try again.');
        } else {
          res.status(200).send('Success! We\'ll be in touch.');
        }
      });
    } else {
      res.status(400).send('Please enter an email address.');
    }
  }
}

const subscribeRoutes: SubscribeRouter = new SubscribeRouter();
subscribeRoutes.init();

export default subscribeRoutes.router;
