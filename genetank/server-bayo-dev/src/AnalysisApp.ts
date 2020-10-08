import * as amqp from 'amqplib/callback_api';
import * as bodyParser from 'body-parser';
import * as express from 'express';
import * as logger from 'morgan';
import * as path from 'path';
import * as session from 'express-session';

import TaskRunner from './analysis/routes/TaskRunner';

class AnalysisApp {
  public app: express.Application;

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
    this.app.use(session({
      resave: false,
      saveUninitialized: false,
      secret: 'genetank'
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
        ch.prefetch(1);
        console.log('Waiting for messages in', q);
        TaskRunner.attachChannel(q, ch);
        // Pass the message along to TaskRunner
        ch.consume(q, TaskRunner.runTask, { noAck: false });
      });
    });
  }
}

export default new AnalysisApp().app;
