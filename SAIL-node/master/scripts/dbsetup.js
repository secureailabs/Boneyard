var mysql    = require('mysql');

var dbconfig = require('../config/database');

// Script for setting up database and tables
var conn = mysql.createConnection(dbconfig.connection);

conn.query('CREATE DATABASE ' + dbconfig.database);

// Set up users table
conn.query('\
  CREATE TABLE `' + dbconfig.database + '`.`users` ( \
    `id` CHAR(36) NOT NULL PRIMARY KEY, \
    `email` VARCHAR(255) NOT NULL UNIQUE, \
    `password` CHAR(60) NOT NULL, \
    `created` TIMESTAMP DEFAULT CURRENT_TIMESTAMP, \
    `last_modified` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP, \
    `last_login` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP, \
    `admin` BOOLEAN NOT NULL DEFAULT 0 \
  )');

// Set up features table
conn.query('\
  CREATE TABLE `' + dbconfig.database + '`.`features` ( \
    `id` INT NOT NULL PRIMARY KEY AUTO_INCREMENT, \
    `name` VARCHAR(255) NOT NULL UNIQUE, \
    `display_name` VARCHAR(255) NOT NULL \
  )');

// Set up user_features table
conn.query('\
  CREATE TABLE `' + dbconfig.database + '`.`user_features` ( \
    `user_id` CHAR(36) NOT NULL, \
    `feature_id` INT NOT NULL \
  )');

console.log('Success! Database created.');
conn.end();
