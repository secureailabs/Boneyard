#!/usr/bin/env node
// storeGenotypeToPhenotypeResults.js

var async = require('async');
var csvParse = require('csv-parse');
var fs = require('fs');
var path = require('path');

var db = require('../../../src/models/db');

if (process.argv.length !== 4) {
  console.log('Usage: node storeGenotypeToPhenotypeResults.js <patient id> <output dir>');
  process.exit(1);
}
// Parse passed-in parameters
const patientId = process.argv[2];
const outputDir = process.argv[3];
const resultsFilePath = path.join(outputDir, 'Patient_' + patientId, 'GeneticPhenotypeReport_Patient_' + patientId + '.csv');

db.events.on('connected', function () {
  async.waterfall([
    // Make sure user exists
    function(callback) {
      db.query('SELECT * FROM users WHERE id = ?', [patientId], function(err, rows) {
        if (err) {
          callback(err);
        } else if (!rows || !rows.length) {
          callback('No user with id ' + patientId + ' exists.');
        } else {
          callback(null);
        }
      });
    },
    // Read in results CSV
    function(callback) {
      var parser = csvParse(function(err, data) {
        // Remove header line
        data.shift();
        callback(err, data);
      });
      fs.createReadStream(resultsFilePath).pipe(parser);
    },
    // Load diseases into memory
    function(data, callback) {
      var codes = data.map(function(row) {
        return row[1];
      });
      db.query('SELECT id, code FROM diseases WHERE code IN (?)', [codes], function(err, rows) {
        callback(err, data, rows);
      });
    },
    // Correspond disease codes and ids, bulk insert into genetic_report database
    function(data, rows, callback) {
      var diseases = {};
      rows.forEach(function(row) {
        diseases[row.code] = row.id;
      });

      // Construct (user_id, disease_id, result) entries
      data.forEach(function(line) {
        line[0] = patientId;
        line[1] = diseases[line[1]];
      });

      // Bulk insert results into database
      db.query('INSERT INTO genetic_report (user_id, disease_id, result) VALUES ?', [data], function(err) {
        callback(err);
      });
    },
    // Mark genetic report as done
    function(callback) {
      db.query('UPDATE report_status SET genetic = ? WHERE user_id = ?',
        ['complete', patientId],
        function(err) {
          callback(err);
        }
      );
    }
  ], function(err, result) {
    if (err) {
      console.log(err);
      process.exit(1);
    } else {
      console.log('Successfully recorded genotype to phenotype results for', patientId);
      process.exit(0);
    }
  });
});
