import { Router, Request, Response, NextFunction } from 'express';
import * as async from 'async';
import * as bcrypt from 'bcrypt-nodejs';
import * as jwt from 'jsonwebtoken';

import * as config from '../../config/secrets';
import * as db from '../../models/db';

interface IUserRequest extends Request {
  user: any;
}

export class CampaignRouter {
  public router: Router;

  constructor() {
    this.router = Router();
    this.init();
  }

  public init(): void {
   // this.router.post('/auth', this.auth);
    this.router.post('/supportData', this.supportData);
    this.router.get('/detail', this.getCampaignProfile);
    this.router.put('/suppertMoney', this.suppertMoney);
     this.router.get('/', this.listCampaigns);
  }




private listCampaigns = (req: IUserRequest, res: Response): void => {

}

private supportData = (req: IUserRequest, res: Response): void => {
    // res.json({
    //             message: 'Hello World!Campaign'
    //         });
}

private suppertMoney = (req: IUserRequest, res: Response): void => {

}


 private getCampaignProfile = (req: IUserRequest, res: Response): void => {
   const userId: string = req.user.sub;
   const campgId: string = req.body.id;
   
   this.getCampaignById(campgId, (err, campaign) => {
      if (err && err.status && err.message) {
        res.status(err.status).send(err.message);
      } else if (err) {
        res.status(500).send(err);
      } else {
        res.send(campaign);
      }
    });
  }
  private getCampaignById = (id: string, callback: Function): any => {
    db.query(
      'SELECT \
      id, name,disease, \
      owner, image, description, \
      start_time,end_time \
       FROM users WHERE id = ?',
      [id], (err, rows) => {
        if (err) {
          callback(err);
        } else if (!rows || !rows.length) {
          callback({ status: 400, message: 'That campaign does not exist.' });
        } else {
          callback(null, rows[0]);
        }
      });
  }
}


const campaignRoutes: CampaignRouter = new CampaignRouter();
campaignRoutes.init();

export default campaignRoutes.router;
