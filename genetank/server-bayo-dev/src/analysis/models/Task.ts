export default class Task {
  public name: string;
  public cmd: string;
  public args: string[];

  public nextTask: Task | void;

  public static parseTask(taskString: string): Task {
    let currentTaskObject: any = JSON.parse(taskString);
    let newTask: Task = new Task(currentTaskObject);
    let currentTask: Task = newTask;
    while (currentTaskObject.nextTask) {
      currentTaskObject = currentTaskObject.nextTask;
      currentTask.nextTask = new Task(currentTaskObject);
      currentTask = currentTask.nextTask;
    }
    return newTask;
  }

  constructor(taskInput: any) {
    // Parse string if necessary
    let taskObject: any;
    if (typeof taskInput === 'string') {
      taskObject = JSON.parse(taskInput);
    } else {
      taskObject = taskInput;
    }

    if (taskObject.name) {
      this.name = taskObject.name;
    }
    if (taskObject.cmd) {
      this.cmd = taskObject.cmd;
    }
    if (taskObject.args) {
      this.args = taskObject.args;
    }
  }

  // Creates and returns a new task to be run after this one
  public addNextTask(taskInput: any): Task {
    const nextTask: Task = new Task(taskInput);
    this.nextTask = nextTask;
    return nextTask;
  }

  public toString(): string {
    return JSON.stringify({
      args: this.args,
      cmd: this.cmd,
      name: this.name
    });
  }
}
