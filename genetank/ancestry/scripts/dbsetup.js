var mysql    = require('mysql');
var dbconfig = require('../config/database');

var conn = mysql.createConnection(dbconfig.connection);

conn.query('CREATE DATABASE ' + dbconfig.database);

// Set up users table
conn.query('\
  CREATE TABLE `' + dbconfig.database + '`.`users` ( \
    `id` CHAR(12) NOT NULL PRIMARY KEY, \
    `email` VARCHAR(255) NOT NULL, \
    `password` CHAR(60) NOT NULL \
  )');

// Set up email subscription table
conn.query('\
  CREATE TABLE `' + dbconfig.database + '`.`subscribe` ( \
    `email` VARCHAR(255) NOT NULL \
  )');

console.log('Success! Database created.');
conn.end();