var EventEmitter = require('events');
var crypto = require('crypto');
var mysql = require('mysql');

var dbconfig = require('../config/database');

var emitter = new EventEmitter();

// Database setup
var pool = mysql.createPool(dbconfig.connection);
pool.getConnection(function(err, conn) {
  conn.query('USE ' + dbconfig.database, function(err, rows) {
    conn.release();
    emitter.emit('connected');
  });
});

// Set up a keepalive heartbeat
// Heartbeat function to keep the connection to the database up
var keepAlive = function() {
  pool.getConnection(function(err, conn) {
    if (err)
      return;

    conn.ping();
    conn.release();
  });
};
setInterval(keepAlive, 30000);

// Returns a connection to the db
var getConnection = function(callback) {
  pool.getConnection(function(err, conn) {
    callback(err, conn);
  });
};

// Helper function for querying the db; releases the db connection
// callback(err, rows)
var query = function(queryString, params, callback) {
  getConnection(function(err, conn) {
    conn.query(queryString, params, function(err, rows) {
      conn.release();

      if (err)
        return callback(err);

      return callback(err, rows);
    });
  });
};

var generateId = function() {
  const length = 12;
  return crypto.randomBytes(Math.ceil(length / 2)).toString('hex').slice(0, length);
}

exports.events = emitter;
exports.generateId = generateId;
exports.query = query;