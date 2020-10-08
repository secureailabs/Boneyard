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

  public static getInstance(): SQLWrapper {
    if (this._SQLWrapperInstance === null) {
      this._SQLWrapperInstance = new SQLWrapper();
    } else {
      // Already initialized, nothing more to do here
    }

    return this._SQLWrapperInstance;
  }

  constructor() {
    this._isInitialized = false;
  }

  // Create a connection to the database, which will be reused among calls
  public async initialize(): Promise<void> {
    if (!this._isInitialized) {
      this._pool = createPool(dbconfig.connection);
      const connection: PoolConnection = await this._pool.getConnection();
      await connection.query('USE ' + dbconfig.database);
      connection.release();
      this._isInitialized = true;
      // Start heartbeating to keep database connection up
      const dbWrapper: any = this;
      setInterval(function (): void { dbWrapper._keepAlive(); }, 30000);
    } else {
      // Already initialized
    }
  }

  // Helper function for querying the db; releases the db connection
  public async query(queryString: string, params: string[]):
    Promise<[RowDataPacket[] | RowDataPacket[][] | OkPacket | OkPacket[], FieldPacket[]]> {
    const connection: PoolConnection = await this._pool.getConnection();
    const rows: any = await connection.query(queryString, params);
    connection.release();
    return rows;
  }

  public generateId(): string {
    const length: number = 12;
    return randomBytes(Math.ceil(length / 2)).toString('hex').slice(0, length);
  }

  // Set up a keepalive heartbeat
  // Heartbeat function to keep the connection to the database up
  private async _keepAlive(): Promise<void> {
    const connection: PoolConnection = await this._pool.getConnection();
    connection.ping();
    connection.release();
  }
}
