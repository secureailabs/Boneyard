import { EventEmitter } from 'events';
import { randomBytes } from 'crypto';
import {
    Pool,
    OkPacket,
    createPool,
    RowDataPacket,
    PoolConnection
} from 'mysql2';

const dbconfig: any = require('../config/database');

export class SQLWrapper {

    private static _SQLWrapperInstance: SQLWrapper = null;

    private _isInitialized: boolean;

    private _pool: Pool;

    // Run configuration method on SQL instance
    constructor() {
        this._isInitialized = false;
    }

    public static getInstance(): SQLWrapper {
        if (null === this._SQLWrapperInstance) {
            this._SQLWrapperInstance = new SQLWrapper();
        } else {
            // Already initialized, nothing more to do here
        }

        return this._SQLWrapperInstance;
    }

    // Sql needs to be initialized before it can
    // be useful
    // Also making this a promise so that the init code
    // can be signaled on success/failure
    public async initialize(): Promise<void> {
        return new Promise<void>(
            (resolve, reject) => {
                if (!this._isInitialized) {
                    // Create connection pool instance we will use
                    // for the duration of the app
                    this._pool = createPool(dbconfig.connection);
                    // Test connection to make sure we are ok
                    this._pool.getConnection(
                        (error, connection) => {
                            // In callbacks, it is recommended to check
                            // for errors first
                            if (error) {
                                // if an error occurs, reject the promise
                                // This signals to the caller of this function
                                // that the init failed
                                reject();
                            } else {
                                // Eveything seems good, continue
                            }

                            // Connect to the database
                            connection.query('USE ' + dbconfig.database, (err, rows) => {
                                if (error) {
                                    // Fail promise
                                    reject();
                                } else {
                                    // Eveything seems good, continue
                                }

                                connection.release();
                                this._isInitialized = true;
                                // Promise is successful
                                resolve();
                            });
                        });
                }
                else {
                    // Already initialized
                    // Mark as resolved as everything should be all ok
                    resolve();
                }
            }
        );
    }

    // Returns a connection to the db
    public getConnection(): Promise<PoolConnection> {
        return new Promise<PoolConnection>(
            (resolve, reject) => {
                this._pool.getConnection(
                    (error, connection) => {
                        if (error) {
                            // Fail promise
                            reject();
                        } else {
                            // Eveything seems good, return connection
                            resolve(connection);
                        }
                    });
            });
    }

    // Helper function for querying the db
    // releases the db connection
    // callback(err, rows)
    public query(queryString: string, params: string[]): Promise<RowDataPacket[] | RowDataPacket[][] | OkPacket | OkPacket[]> {
        return new Promise<RowDataPacket[] | RowDataPacket[][] | OkPacket | OkPacket[]>(
            async (resolve, reject) => {
                const connection: PoolConnection = await this.getConnection();
                connection.query(queryString, params, (error, rows) => {
                    connection.release();

                    if (error) {
                        // Fail promise
                        reject();
                    } else {
                        // Eveything seems good, return connection
                        resolve(rows);
                    }
                });
            });
    }

    public generateId(): string {
        const length = 12;
        return randomBytes(Math.ceil(length / 2)).toString('hex').slice(0, length);
    }

}
