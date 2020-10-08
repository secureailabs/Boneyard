import * as amqp from 'amqplib/callback_api';
import * as bodyParser from 'body-parser';
import * as cors from 'cors';
import * as express from 'express';
import * as expressJwt from 'express-jwt';
import * as logger from 'morgan';
import * as path from 'path';

import AWSRouter from './api/routes/AWSRouter';
import DNARouter from './api/routes/DNARouter';
import ReportRouter from './api/routes/ReportRouter';
import SubscribeRouter from './api/routes/SubscribeRouter';
import UserRouter from './api/routes/UserRouter';
import CampaignRouter from './api/routes/CampaignRouter';
import TaskDispatcher from './analysis/routes/TaskDispatcher';
import * as config from './config/secrets';

// Showcasing the internal module based router
// import { DNARouterINT } from './api/routes/DNARouterINT';

class ApiApp {
  public app: express.Application;
  private BASEURL: string = '/api/v1';

  constructor() {
    this.app = express();
    this.middleware();
    this.routes();
    this.messaging();
  }

  // Set up Express middleware
  private middleware(): void {
    this.app.use(logger('dev'));
    this.app.use(bodyParser.json());
    this.app.use(bodyParser.urlencoded({ extended: false }));
    this.app.use(cors());
    this.app.options('*', cors());
    this.app.use(expressJwt({
      getToken: function (req: any): any {
        if (req.headers.authorization && req.headers.authorization.split(' ')[0] === 'Bearer') {
          return req.headers.authorization.split(' ')[1];
        } else if (req.query && req.query.token) {
          return req.query.token;
        }
        return null;
      },
      secret: config.jwt
    }).unless({
      // Add endpoints which do not require login here
      path: [
        this.BASEURL + '/users/auth',
        this.BASEURL + '/users/register',
        this.BASEURL + '/subscribe'
      ]
    }));
  }

  // Set up routing
  private routes(): void {
    const router: express.Router = express.Router();
    router.get('/', (req, res, next) => {
      res.json({
        message: 'Hello World!'
      });
    });
    this.app.use('/', router);
    this.app.use(this.BASEURL + '/aws', AWSRouter);
    this.app.use(this.BASEURL + '/dna', DNARouter);
    // this.app.use(this.BASEURL + '/dna', DNARouterINT.createRouter());
    this.app.use(this.BASEURL + '/reports', ReportRouter);
    this.app.use(this.BASEURL + '/subscribe', SubscribeRouter);
    this.app.use(this.BASEURL + '/users', UserRouter);
    this.app.use(this.BASEURL + '/campaigns', CampaignRouter);
  }

  // Set up messaging through RabbitMQ
  private messaging(): void {
    amqp.connect('amqp://localhost', (err, conn) => {
      if (err) {
        console.log('Cannot connect to RabbitMQ on amqp://localhost. Exiting');
        process.exit(1);
      }
      conn.createChannel((err2, ch) => {
        if (err2) {
          console.log('Cannot create RabbitMQ channel. Exiting');
          process.exit(1);
        }
        const q: string = 'dna_analysis_work_queue';

        ch.assertQueue(q, { durable: true });
        console.log('Sending messages in', q);
        TaskDispatcher.attachChannel(q, ch);
      });
    });
  }
}

export default new ApiApp().app;
