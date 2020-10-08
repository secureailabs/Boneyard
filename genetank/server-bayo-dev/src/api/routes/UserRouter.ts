import { Router, Request, Response, NextFunction } from 'express';
import * as async from 'async';
import * as bcrypt from 'bcrypt-nodejs';
import * as jwt from 'jsonwebtoken';

import * as config from '../../config/secrets';
import * as db from '../../models/db';

interface IUserRequest extends Request {
  user: any;
}

export class UserRouter {
  public router: Router;

  constructor() {
    this.router = Router();
    this.init();
  }

  public init(): void {
    this.router.post('/auth', this.auth);
    this.router.post('/register', this.register);
    this.router.get('/', this.getUserProfile);
    this.router.put('/', this.updateUserProfile);
  }

  private auth = (req: Request, res: Response): void => {
    async.waterfall(
      [
        // Check required parameters
        (callback: Function) => {
          if (req.body.email && req.body.password) {
            callback(null);
          } else {
            callback({ status: 400, message: 'Please enter an email and password.' });
          }
        },
        // Find user
        (callback: Function) => {
          db.query('SELECT id, email, password, username FROM users WHERE email = ?', [req.body.email], (err, rows) => {
            if (err) {
              callback(err);
            } else if (!rows || !rows.length) {
              callback({ status: 400, message: 'Email or password is incorrect.' });
            } else {
              callback(null, rows[0]);
            }
          });
        },
        // Validate password and send user object
        (user: any, callback: Function) => {
          if (this.validPassword(req.body.password, user.password)) {
            res.send({
              email: user.email,
              id: user.id,
              token: jwt.sign({ sub: user.id }, config.jwt),
              username: user.username
            });
          } else {
            callback({ status: 400, message: 'Email or password is incorrect.' });
          }
        }
      ],
      (err) => {
        if (err && err.status && err.message) {
          res.status(err.status).send(err.message);
        } else if (err) {
          res.status(500).send(err);
        }
      });
  }

  private register = (req: Request, res: Response): void => {
    async.waterfall(
      [
        // Check required parameters
        (callback: Function) => {
          if (req.body.email &&
            req.body.password &&
            req.body.username) {
            callback(null);
          } else {
            callback({ status: 400, message: 'Missing registration fields.' });
          }
        },
        // Check for existing users
        (callback: Function) => {
          db.query('SELECT 1 FROM users WHERE email = ?', [req.body.email], (err, rows) => {
            if (err) {
              callback(err);
            } else if (rows && rows.length) {
              callback({ status: 400, message: 'An account with that email address already exists.' });
            } else {
              callback(null);
            }
          });
        },
        // Check for existing username
        (callback: Function) => {
          db.query('SELECT 1 FROM users WHERE username = ?', [req.body.username], (err, rows) => {
            if (err) {
              callback(err);
            } else if (rows && rows.length) {
              callback({ status: 400, message: 'That username is already taken.' });
            } else {
              callback(null);
            }
          });
        },
        // Create user
        (callback: Function) => {
          this.createUser(req.body.email, req.body.password, req.body.username, callback);
        }
      ],
      (err) => {
        if (err && err.status && err.message) {
          res.status(err.status).send(err.message);
        } else if (err) {
          res.status(500).send(err);
        } else {
          // Report success
          res.status(200).send('Account successfully created!');
        }
      });
  }

  private getUserProfile = (req: IUserRequest, res: Response): void => {
    this.getUserById(req.user.sub, (err, user) => {
      if (err && err.status && err.message) {
        res.status(err.status).send(err.message);
      } else if (err) {
        res.status(500).send(err);
      } else {
        res.send(user);
      }
    });
  }

  private updateUserProfile = (req: IUserRequest, res: Response): void => {
    async.waterfall(
      [
        // Check required parameters
        (callback: Function) => {
          if (req.body.username && req.body.email &&
            req.body.gender && req.body.age && req.body.ethnicity &&
            req.body.notifications_email) {
            callback(null);
          } else {
            callback({ status: 400, message: 'Please fill out all required fields.' });
          }
        },
        // Ensure unique username and email
        (callback: Function) => {
          db.query(
            'SELECT id, email FROM users WHERE username = ? OR email = ?',
            [req.body.username, req.body.email],
            (err, rows) => {
              if (err) {
                callback(err);
              } else if (rows && rows.length) {
                rows.forEach((row) => {
                  if (row.id !== req.user.sub) {
                    // Collision with existing user
                    if (row.email === req.body.email) {
                      callback({ status: 400, message: 'That email is already in use.' });
                    } else {
                      callback({ status: 400, message: 'That username is already taken.' });
                    }
                  }
                });
                // Only collides with self, we're okay to continue
                callback(null);
              } else {
                callback(null);
              }
            });
        },
        // Update user
        (callback: Function) => {
          // Populate missing values as empty string
          ['location_city', 'location_province', 'location_country', 'location_zip'].forEach((value) => {
            if (req.body[value] === undefined) {
              req.body[value] = '';
            }
          });
          db.query(
            'UPDATE users \
              SET username = ?, email = ?, \
              gender = ?, age = ?, ethnicity = ?, \
              location_city = ?, location_province = ?, location_country = ?, location_zip = ?, \
              notifications_email = ? \
              WHERE id = ?',
            [req.body.username, req.body.email,
            req.body.gender, req.body.age, req.body.ethnicity,
            req.body.location_city, req.body.location_province, req.body.location_country, req.body.location_zip,
            req.body.notifications_email,
            req.user.sub],
            (err) => {
              callback(err);
            });
        }
      ],
      (err) => {
        if (err && err.status && err.message) {
          res.status(err.status).send(err.message);
        } else if (err) {
          res.status(500).send(err);
        } else {
          res.sendStatus(200);
        }
      });
  }

  // Hash and salt the password with bcrypt
  private hashPassword = (password: string): any => {
    return bcrypt.hashSync(password, bcrypt.genSaltSync(8), null);
  }

  // Check if password is correct
  private validPassword = (password: string, savedPassword: string): boolean => {
    return bcrypt.compareSync(password, savedPassword);
  }

  // Create a new user, to be used as part of an async waterfall
  private createUser = (email: string, password: string, username: string, callback: Function): void => {
    // Populate fields with default values and params
    const newUser: any = {
      email: email,
      id: db.generateId(),
      password: this.hashPassword(password),
      username: username
    };
    db.query(
      'INSERT INTO users (id, email, username, password) values (?,?,?,?)',
      [newUser.id, newUser.email, newUser.username, newUser.password],
      (err) => {
        if (err) {
          callback(err);
        } else {
          // Add entry in report_status for user
          db.query(
            'INSERT INTO report_status (user_id, genetic, ancestry, phenotype) values (?,?,?,?)',
            [newUser.id, 'none', 'none', 'none'],
            (err2) => {
              callback(err2);
            }
          );
        }
      });
  }

  private getUserById = (id: string, callback: Function): any => {
    db.query(
      'SELECT \
      id, email, username, \
      gender, age, ethnicity, \
      location_city, location_province, location_country, location_zip, \
      notifications_email FROM users WHERE id = ?',
      [id], (err, rows) => {
        if (err) {
          callback(err);
        } else if (!rows || !rows.length) {
          callback({ status: 400, message: 'That user does not exist.' });
        } else {
          callback(null, rows[0]);
        }
      });
  }
}

const userRoutes: UserRouter = new UserRouter();
userRoutes.init();

export default userRoutes.router;
