var csvParse = require('csv-parse');
var fs = require('fs');

var db = require('./src/models/db');

// Insert diseases into diseases db
db.events.on('connected', function() {
  var parser = csvParse(function (err, data) {
    if (err) {
      console.log('Error parsing csv:', err);
      process.exit(1);
    } else {
      db.query('INSERT INTO diseases (code, name) VALUES ?',
        [data],
        function(err) {
        if (err) {
          console.log('Error inserting diseases into database:', err);
        } else {
          console.log('Successfully inserted diseases into database. Contents:');
          db.query('SELECT * FROM diseases', [], function(err, rows) {
            console.log(rows);
          });
        }
      });
    }
  });
  fs.createReadStream('diseases.csv').pipe(parser);
});