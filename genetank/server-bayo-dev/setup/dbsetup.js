var mysql = require('mysql');

var dbconfig = require('../src/config/database');

var conn = mysql.createConnection(dbconfig.connection);

conn.query('DROP DATABASE ' + dbconfig.database);
conn.query('CREATE DATABASE ' + dbconfig.database);
// conn.query('USE ' + dbconfig.database);

// Set up database tables

// Users table
conn.query('\
  CREATE TABLE `' + dbconfig.database + '`.`users` ( \
    `id` CHAR(12) NOT NULL PRIMARY KEY, \
    `email` VARCHAR(255) NOT NULL, \
    `username` VARCHAR(255) NOT NULL, \
    `password` CHAR(60) NOT NULL, \
    `gender` VARCHAR(2), \
    `age` VARCHAR(6), \
    `ethnicity` VARCHAR(255), \
    `location_city` VARCHAR(255), \
    `location_province` VARCHAR(255), \
    `location_country` VARCHAR(255), \
    `location_zip` VARCHAR(16), \
    `notifications_email` VARCHAR(8) NOT NULL DEFAULT \'all\' \
  )');

// Email subscription table
conn.query('\
  CREATE TABLE `' + dbconfig.database + '`.`subscribe` ( \
    `id` INT NOT NULL PRIMARY KEY AUTO_INCREMENT, \
    `email` VARCHAR(255) NOT NULL \
  )');

// Report status table
conn.query('\
CREATE TABLE `' + dbconfig.database + '`.`report_status` ( \
  `user_id` CHAR(12) NOT NULL PRIMARY KEY, \
  `genetic` VARCHAR(8) NOT NULL, \
  `ancestry` VARCHAR(8) NOT NULL, \
  `phenotype` VARCHAR(8) NOT NULL \
)');

// Genetic report table
conn.query('\
CREATE TABLE `' + dbconfig.database + '`.`genetic_report` ( \
  `id` INT NOT NULL PRIMARY KEY AUTO_INCREMENT, \
  `user_id` CHAR(12) NOT NULL, \
  `disease_id` INT NOT NULL, \
  `result` VARCHAR(255) NOT NULL \
)');

// Ancestry report table
conn.query('\
CREATE TABLE `' + dbconfig.database + '`.`ancestry_report` ( \
  `id` INT NOT NULL PRIMARY KEY AUTO_INCREMENT, \
  `user_id` CHAR(12) NOT NULL, \
  `population_id` INT NOT NULL, \
  `result` VARCHAR(255) NOT NULL \
)');

// Phenotype report table
conn.query('\
CREATE TABLE `' + dbconfig.database + '`.`phenotype_report` ( \
  `id` INT NOT NULL PRIMARY KEY AUTO_INCREMENT, \
  `user_id` CHAR(12) NOT NULL, \
  `disease_id` INT NOT NULL, \
  `result` VARCHAR(255) NOT NULL \
)');

// Phenotypic analysis diseases table
conn.query('\
CREATE TABLE `' + dbconfig.database + '`.`phenotype_diseases` ( \
  `disease_id` INT NOT NULL PRIMARY KEY \
)');

// Diseases table
conn.query('\
CREATE TABLE `' + dbconfig.database + '`.`diseases` ( \
  `id` INT NOT NULL PRIMARY KEY AUTO_INCREMENT, \
  `code` VARCHAR(255) NOT NULL, \
  `name` VARCHAR(255) NOT NULL, \
  `description` VARCHAR(2048), \
  `image` VARCHAR(255) \
)');

// Symptoms table
conn.query('\
CREATE TABLE `' + dbconfig.database + '`.`symptoms` ( \
  `id` INT NOT NULL PRIMARY KEY AUTO_INCREMENT, \
  `code` VARCHAR(255) NOT NULL, \
  `name` VARCHAR(255) NOT NULL, \
  `description` VARCHAR(2048), \
  `image` VARCHAR(255) \
)');

// Treatments table
conn.query('\
CREATE TABLE `' + dbconfig.database + '`.`treatments` ( \
  `id` INT NOT NULL PRIMARY KEY AUTO_INCREMENT, \
  `disease_id` INT NOT NULL, \
  `code` VARCHAR(255) NOT NULL, \
  `name` VARCHAR(255) NOT NULL \
)');

// Populations table
conn.query('\
CREATE TABLE `' + dbconfig.database + '`.`populations` ( \
  `id` INT NOT NULL PRIMARY KEY AUTO_INCREMENT, \
  `name` VARCHAR(255) NOT NULL, \
  `level` INT, \
  `countries` VARCHAR(255), \
  `description` VARCHAR(2048) \
)');

// Campaigns table
conn.query('\
CREATE TABLE `' + dbconfig.database + '`.`campaigns` ( \
  `id` INT NOT NULL PRIMARY KEY AUTO_INCREMENT, \
  `code` VARCHAR(255) NOT NULL, \
  `name` VARCHAR(255) NOT NULL, \
  `disease_id` INT NOT NULL, \
  `owner` CHAR(12) NOT NULL, \
  `image` VARCHAR(255) NOT NULL, \
  `description` VARCHAR(2048), \
  `start_time` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP, \
  `end_time` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,\
  `num_data_pledges` INT  NOT NULL DEFAULT 0,\
  `sucess_criteria_num_data` INT  NOT NULL DEFAULT 0,\
  `minimum_money_donation` INT  NOT NULL DEFAULT 20,\
  `GWAS_report_price` INT  NOT NULL DEFAULT 20000,\
  `prediction_price` DECIMAL  NOT NULL DEFAULT 1,\
  `num_money_pledges` INT   NOT NULL DEFAULT 0,\
  `total_money_amount` INT   NOT NULL DEFAULT 0\
)');

// DNA pledges table
conn.query('\
CREATE TABLE `' + dbconfig.database + '`.`pledges_dna` ( \
  `id` INT NOT NULL PRIMARY KEY AUTO_INCREMENT, \
  `campaign_id` INT NOT NULL, \
  `user_id` CHAR(12) NOT NULL, \
  `timestamp` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP \
)');

// Money pledges table
conn.query('\
CREATE TABLE `' + dbconfig.database + '`.`pledges_money` ( \
  `id` INT NOT NULL PRIMARY KEY AUTO_INCREMENT, \
  `campaign_id` INT NOT NULL, \
  `user_id` CHAR(12) NOT NULL, \
  `amount` INT NOT NULL, \
  `timestamp` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP \
)');

// Campaign survey answers tables
conn.query('\
CREATE TABLE `' + dbconfig.database + '`.`campaign_answers_disease` ( \
  `id` INT NOT NULL PRIMARY KEY AUTO_INCREMENT, \
  `campaign_id` INT NOT NULL, \
  `user_id` CHAR(12) NOT NULL, \
  `response` VARCHAR(255) NOT NULL \
)');

conn.query('\
CREATE TABLE `' + dbconfig.database + '`.`campaign_answers_symptom` ( \
  `id` INT NOT NULL PRIMARY KEY AUTO_INCREMENT, \
  `campaign_id` INT NOT NULL, \
  `user_id` CHAR(12) NOT NULL, \
  `symptom_id` INT NOT NULL, \
  `response` VARCHAR(255) NOT NULL \
)');

conn.query('\
CREATE TABLE `' + dbconfig.database + '`.`campaign_answers_treatment_tried` ( \
  `id` INT NOT NULL PRIMARY KEY AUTO_INCREMENT, \
  `campaign_id` INT NOT NULL, \
  `user_id` CHAR(12) NOT NULL, \
  `treatment_id` INT NOT NULL, \
  `response` VARCHAR(255) NOT NULL \
)');

conn.query('\
CREATE TABLE `' + dbconfig.database + '`.`campaign_answers_treatment_success` ( \
  `id` INT NOT NULL PRIMARY KEY AUTO_INCREMENT, \
  `campaign_id` INT NOT NULL, \
  `user_id` CHAR(12) NOT NULL, \
  `treatment_id` INT NOT NULL, \
  `response` VARCHAR(255) NOT NULL \
)');

// Campaign survey parents answers tables
conn.query('\
CREATE TABLE `' + dbconfig.database + '`.`campaign_answers_parent_disease` ( \
  `id` INT NOT NULL PRIMARY KEY AUTO_INCREMENT, \
  `campaign_id` INT NOT NULL, \
  `user_id` CHAR(12) NOT NULL, \
  `response` VARCHAR(255) NOT NULL \
)');

conn.query('\
CREATE TABLE `' + dbconfig.database + '`.`campaign_answers_parent_symptom` ( \
  `id` INT NOT NULL PRIMARY KEY AUTO_INCREMENT, \
  `campaign_id` INT NOT NULL, \
  `user_id` CHAR(12) NOT NULL, \
  `symptom_id` INT NOT NULL, \
  `response` VARCHAR(255) NOT NULL \
)');

conn.query('\
CREATE TABLE `' + dbconfig.database + '`.`campaign_answers_parent_treatment_tried` ( \
  `id` INT NOT NULL PRIMARY KEY AUTO_INCREMENT, \
  `campaign_id` INT NOT NULL, \
  `user_id` CHAR(12) NOT NULL, \
  `treatment_id` INT NOT NULL, \
  `response` VARCHAR(255) NOT NULL \
)');

conn.query('\
CREATE TABLE `' + dbconfig.database + '`.`campaign_answers_parent_treatment_success` ( \
  `id` INT NOT NULL PRIMARY KEY AUTO_INCREMENT, \
  `campaign_id` INT NOT NULL, \
  `user_id` CHAR(12) NOT NULL, \
  `treatment_id` INT NOT NULL, \
  `response` VARCHAR(255) NOT NULL \
)');

// Phenotypic report survey answers tables
conn.query('\
CREATE TABLE `' + dbconfig.database + '`.`phenotype_answers_disease` ( \
  `id` INT NOT NULL PRIMARY KEY AUTO_INCREMENT, \
  `user_id` CHAR(12) NOT NULL, \
  `disease_id` INT NOT NULL \
)');

console.log('Success! Database created.');
conn.end();
