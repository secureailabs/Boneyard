import * as amqp from 'amqplib/callback_api';
import { Router, Request, Response, NextFunction } from 'express';

import Task from '../models/Task';

export class TaskDispatcher {
  private q: string;
  private ch: amqp.Channel;

  public attachChannel(q: string, ch: amqp.Channel): void {
    this.q = q;
    this.ch = ch;
  }

  // Queue up given task in work queue
  public sendTask(task: Task): void {
    this.ch.sendToQueue(this.q, new Buffer(JSON.stringify(task)), { persistent: true });
    console.log('Sent task', task.name);
  }
}

const taskDispatcher: TaskDispatcher = new TaskDispatcher();

export default taskDispatcher;
