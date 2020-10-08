import { Router, Request, Response } from 'express';
import * as AWS from 'aws-sdk';

import { IUserRequest } from '../../models/types';

export class AWSRouter {
  public router: Router;

  constructor() {
    this.router = Router();
    this.init();
  }

  public init(): void {
    this.router.post('/s3', this.createPresignedS3URL);
  }

  // Generates a presigned AWS S3 URL for doing GeneTank file uploads.
  // Input parameters: user, filename: string, type: string
  // Output: url: string
  private createPresignedS3URL(req: IUserRequest, res: Response): void {
    const filename: string = req.body.filename;
    const type: string = req.body.type;
    if (filename) {
      const s3: AWS.S3 = new AWS.S3();
      const signedUrlExpireSeconds: number = 60 * 10;
      let params: any = {
        Bucket: 'genetank-uploads',
        ContentType: type,
        Expires: signedUrlExpireSeconds,
        Key: filename
      };
      s3.getSignedUrl('putObject', params, (err, url) => {
        if (err) {
          console.log('Error while generating presigned url:', err);
          res.status(500).send({
            message: err
          });
        } else {
          console.log('Generated presigned url:', url);
          res.json({ url: url });
        }
      });
    } else {
      res.status(400).send({
        message: 'Invalid params'
      });
    }
  }

}

const awsRoutes: AWSRouter = new AWSRouter();
awsRoutes.init();

export default awsRoutes.router;
