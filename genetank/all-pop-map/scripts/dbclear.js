var mysql    = require('mysql');
var dbconfig = require('../config/database');

var conn = mysql.createConnection(dbconfig.connection);

conn.query('DROP DATABASE ' + dbconfig.database);

console.log('Success! Database cleared.');
conn.end();