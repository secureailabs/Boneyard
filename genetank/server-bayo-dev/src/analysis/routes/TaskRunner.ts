import * as amqp from 'amqplib/callback_api';
import * as path from 'path';
const { spawn } = require('child_process');

import Task from '../models/Task';

export class TaskRunner {
  private q: string;
  private ch: amqp.Channel;

  public attachChannel(q: string, ch: amqp.Channel): void {
    this.q = q;
    this.ch = ch;
  }

  public runTask = (msg: amqp.Message): void => {
    const task: Task = Task.parseTask(msg.content.toString());
    console.log('Received request to run task:', task.name);
    this.runCommand(task.cmd, task.args, () => {
      console.log('Completed task', task.name);
      this.queueNextTask(task);
      this.ch.ack(msg);
    });
  }

  private queueNextTask = (task: Task): void => {
    if (task.nextTask) {
      this.ch.sendToQueue(this.q, new Buffer(JSON.stringify(task.nextTask)), { persistent: true });
    }
  }

  private run(command: string, args: string[], cwd: string = process.env.HOME, callback: Function): void {
    const cmd: any = spawn(command, args, {
      cwd: cwd
    });
    cmd.stdout.on('data', (data) => {
      console.log('stdout:', data.toString());
    });
    cmd.stderr.on('data', (data) => {
      console.log('stderr:', data.toString());
    });
    cmd.on('exit', (code) => {
      console.log('child process exited with code', code);
      callback();
    });
  }

  private runCommand = (command: string, args: string[], callback: Function): void => {
    console.log('Running', command, args);
    this.run(command, args, path.join(path.dirname(require.main.filename), 'analysis/scripts'), callback);
  }

  private runScript = (script: string, args: string[], callback: Function): void => {
    console.log('Running sh', [script].concat(args));
    this.run('sh', [script].concat(args), path.join(path.dirname(require.main.filename), 'analysis/scripts'), callback);
  }

}

const taskRunner: TaskRunner = new TaskRunner();

export default taskRunner;
