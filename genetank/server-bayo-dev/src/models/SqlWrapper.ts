import { EventEmitter } from 'events';
import { randomBytes } from 'crypto';
import {
    Pool,
    OkPacket,
    createPool,
    FieldPacket,
    RowDataPacket,
    PoolConnection
} from 'mysql2/promise';

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
        if (!this._isInitialized) {
            // Create connection pool instance we will use
            // for the duration of the app
            this._pool = createPool(dbconfig.connection);
            // Test connection to make sure we are ok
            const connection: PoolConnection = await this._pool.getConnection();
            await connection.query('USE ' + dbconfig.database);
            connection.release();
            this._isInitialized = true;
        }
        else {
            // Already initialized
        }
    }

    // Helper function for querying the db; releases the db connection
    // callback(err, rows)
    public async query(queryString: string, params: string[]): Promise<[RowDataPacket[] | RowDataPacket[][] | OkPacket | OkPacket[], FieldPacket[]]> {
        const connection: PoolConnection = await this._pool.getConnection();
        const rows = await connection.query(queryString, params);
        connection.release();
        return rows;
    }

    public generateId(): string {
        const length = 12;
        return randomBytes(Math.ceil(length / 2)).toString('hex').slice(0, length);
    }
}
