import { Router, Request, Response } from 'express';

import { SQLWrapper } from '../../models/SQLWrapper';
import { IUserRequest, ICampaign, ICampaignBasic } from '../../models/types';

export class CampaignRouter {
  private static db: SQLWrapper = SQLWrapper.getInstance();

  public static createRouter(): Router {
    const router: Router = Router();

    router.get('/', this.listCampaigns);
    router.get('/:code', this.getCampaign);
    router.post('/back/:code', this.backCampaign);
    router.post('/survey/:code', this.answerCampaignSurvey);
    router.post('/survey/:code/complete', this.completeCampaignSurvey);

    return router;
  }

  // Returns a list of basic information for all campaigns in the database. Not sorted.
  // Input parameters: user (optional)
  // Output: [ICampaignBasic]
  private static async listCampaigns(req: IUserRequest, res: Response): Promise<void> {
    try {
      const campaigns: [[ICampaignBasic], any[]] = await CampaignRouter.db.query(
        'SELECT name, code, \
              image, description \
              FROM campaigns',
        []);
      res.json(campaigns[0]);
    } catch (err) {
      CampaignRouter.handleError(err, res);
    }
  }

  // Returns the detailed information about a campaign.
  // Input parameters: user (optional), code: string
  // Output: ICampaign
  private static async getCampaign(req: IUserRequest, res: Response): Promise<void> {
    if (req.user) {
      // This endpoint is public, so we won't necessarily have a user
      const userId: string = req.user.sub;
      // TODO: Check if this user has already backed the campaign
    }

    try {
      const campaign: [[ICampaign], any[]] = await CampaignRouter.db.query(
        'SELECT name, disease_id, \
          owner, image, description, \
          start_time, end_time, \
          num_pledges_data, num_pledges_money, total_money, goal_data, \
          min_money_donation, \
          gwas_report_price, prediction_price \
          FROM campaigns WHERE code = ?',
        [req.params.code]);
      if (campaign[0].length) {
        // Matching campaign exists
        res.json(campaign[0][0]);
      } else {
        CampaignRouter.handleError({ status: 400, message: 'Sorry! That campaign does not exist.' }, res);
      }
    } catch (err) {
      CampaignRouter.handleError(err, res);
    }
  }

  // Adds the user to the list of users backing a campaign, either as a DNA backer or a money backer.
  // Input parameters: user, type: string, amount: int (for type === 'money' only)
  // Output: response status
  private static async backCampaign(req: IUserRequest, res: Response): Promise<void> {
    if (req.body.type === 'dna') {
      return this.backCampaignDNA(req, res);
    } else if (req.body.type === 'money') {
      return this.backCampaignMoney(req, res);
    } else {
      CampaignRouter.handleError({ status: 400, message: 'Please select either DNA backer or money backer.' }, res);
    }
  }

  // Records an answer for a campaign survey question, returning whether the answer was successfully recorded.
  // Input parameters: user, code: string
  // Output: boolean
  private static async answerCampaignSurvey(req: IUserRequest, res: Response): Promise<void> {
    res.send('Success');
  }

  // Records that the user completed the campaign survey, returning whether completion was successfully recorded.
  // Input parameters: user, code: string
  // Output: boolean
  private static async completeCampaignSurvey(req: IUserRequest, res: Response): Promise<void> {
    res.send('Success');
  }

  // Helper method for adding the user as a DNA backer.
  private static async backCampaignDNA(req: IUserRequest, res: Response): Promise<void> {
    try {
      // Get campaign id
      const campaignIdRows: any = await CampaignRouter.db.query('SELECT id FROM campaigns WHERE code = ?', [req.params.code]);
      if (!campaignIdRows[0].length) {
        CampaignRouter.handleError({ status: 400, message: 'Sorry, that campaign does not exist.' }, res);
      } else {
        const campaignId: string = campaignIdRows[0][0];
        // Check if the user has already backed the campaign
        const pledge: any = await CampaignRouter.db.query(
          'SELECT 1 FROM pledges_dna WHERE campaign_id = ? AND user_id = ?',
          [campaignId, req.user.sub]);
        if (pledge[0].length) {
          // User has already backed campaign
          CampaignRouter.handleError({ status: 400, message: 'You have already backed this campaign.' }, res);
        } else {
          // Add user to backers
          await CampaignRouter.db.query(
            'INSERT INTO pledges_dna (campaign_id, user_id) values (?,?)',
            [campaignId, req.user.sub]);

          // Update campaign stats
          const campaignStats: any = await CampaignRouter.db.query('SELECT num_pledges_data WHERE id = ?', [campaignId]);
          const numPledgesData: number = parseInt(campaignStats[0].num_pledges_data, 10) + 1;
          await CampaignRouter.db.query(
            'UPDATE campaigns SET num_pledges_money = ? WHERE id = ?',
            [numPledgesData.toString(), campaignId]);

          // TODO: Add the user's DNA to the AI model
          res.sendStatus(200);
        }
      }
    } catch (err) {
      CampaignRouter.handleError(err, res);
    }
  }

  // Helper method for adding the user as a money backer. If the user has already donated money before, updates their total contribution.
  private static async backCampaignMoney(req: IUserRequest, res: Response): Promise<void> {
    // Validate the amount the user is contributing
    if (req.body.amount) {
      const amount: number = parseInt(req.body.amount, 10);
      if (isNaN(amount) || amount <= 0) {
        CampaignRouter.handleError({ status: 400, message: 'Please enter a positive amount to contribute.' }, res);
      } else {
        try {
          // Get campaign id
          const campaignIdRows: any = await CampaignRouter.db.query('SELECT id FROM campaigns WHERE code = ?', [req.params.code]);
          if (!campaignIdRows[0].length) {
            CampaignRouter.handleError({ status: 400, message: 'Sorry, that campaign does not exist.' }, res);
          } else {
            const campaignId: string = campaignIdRows[0][0];
            // Add user to backers
            // TODO: Record the payment id
            await CampaignRouter.db.query(
              'INSERT INTO pledges_money (campaign_id, user_id, amount) values (?,?,?)',
              [campaignId, req.user.sub, amount]);

            // Update campaign stats
            const campaignStats: any = await CampaignRouter.db.query('SELECT num_pledges_money, total_money WHERE id = ?', [campaignId]);
            const numPledgesMoney: number = parseInt(campaignStats[0].num_pledges_money, 10) + 1;
            const totalMoney: number = parseInt(campaignStats[0].total_money, 10) + amount;
            await CampaignRouter.db.query(
              'UPDATE campaigns SET num_pledges_money = ?, total_money = ? WHERE id = ?',
              [numPledgesMoney.toString(), totalMoney.toString(), campaignId]);

            res.sendStatus(200);
          }
        } catch (err) {
          CampaignRouter.handleError(err, res);
        }
      }
    } else {
      CampaignRouter.handleError({ status: 400, message: 'Please include a contribution amount.' }, res);
    }
  }

  // Helper method for extracting and returning useful error messages and error statuses.
  private static handleError(error: any, response: Response): void {
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

    console.log(error);
    response.status(status).json(message);
  }
}
