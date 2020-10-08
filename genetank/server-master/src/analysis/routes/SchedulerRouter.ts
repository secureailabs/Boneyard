import { Router, Request, Response, NextFunction } from 'express';
import * as path from 'path';
const { spawn } = require('child_process');

export class SchedulerRouter {
  public router: Router;
  private tasks: any[];

  constructor() {
    this.router = Router();
    this.tasks = [];
    this.init();
  }

  public init(): void {
    this.router.get('/task', this.runTask);
    this.router.post('/task', this.addTask);
  }

  private run(command: string, args: string[], cwd: string = process.env.HOME): void {
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
    });
  }

  private runScript = (script: string, args: string[]): void => {
    this.run('sh', [script].concat(args), path.join(path.dirname(require.main.filename), 'analysis/scripts'));
  }

  private addTask = (req: Request, res: Response, next: NextFunction): void => {
    const script: string = 'test.sh';
    this.tasks.push({
      args: [],
      script: script
    });
    res.json({
      message: 'Queued a task: ' + script
    });
  }

  private runTask = (req: Request, res: Response, next: NextFunction): void => {
    const task: any = this.tasks.pop();
    this.runScript(task.script, task.args);
    res.json({
      message: 'Ran task: ' + task.script
    });
  }

}

const schedulerRoutes: SchedulerRouter = new SchedulerRouter();
schedulerRoutes.init();

export default schedulerRoutes.router;
