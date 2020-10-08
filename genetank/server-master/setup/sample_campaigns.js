var mysql = require('mysql');

var dbconfig = require('../src/config/database');

var conn = mysql.createConnection(dbconfig.connection);

conn.query('USE ' + dbconfig.database);

// Check campaigns
conn.query(
  'SELECT * FROM campaigns',
  [],
  function(err, rows) {
    if (err) {
      console.log('Error:', err);
      process.exit(1);
    } else {
      console.log(rows);
      process.exit(0);
    }
  });

//// Insomnia
//conn.query(
//  'INSERT INTO campaigns (code, name, disease_id, owner, image, description) values (?, ?, ?, ?, ?, ?)',
//  ['insomnia', 'Insomnia', 88, 'xT3sTUs3Ridx',
//    'https://c2.staticflickr.com/4/3397/3628914665_f562a6b722_b.jpg',
//    'Insomnia, also known as sleeplessness, is a sleep disorder where people have trouble sleeping. They may have difficulty falling asleep, or staying asleep as long as desired. Insomnia is typically followed by daytime sleepiness, low energy, irritability, and a depressed mood. It may result in an increased risk of motor vehicle collisions, as well as problems focusing and learning. Insomnia can be short term, lasting for days or weeks, or long term, lasting more than a month.'
//    ],
//  function(err) {
//    if (err) {
//      console.log('Error:', err);
//    }
//  });
//
//// Alzheimer's
//conn.query(
//  'INSERT INTO campaigns (code, name, disease_id, owner, image, description) values (?, ?, ?, ?, ?, ?)',
//  ['alzheimers', 'Alzheimer\'s Disease', 4, 'xT3sTUs3Ridx',
//    'http://www.getmedurgentcare.com/wp-content/uploads/2016/11/alzehimer.jpg',
//    'Alzheimer\'s is a type of dementia that causes problems with memory, thinking and behavior. Symptoms usually develop slowly and get worse over time, becoming severe enough to interfere with daily tasks.'
//    ],
//  function(err) {
//    if (err) {
//      console.log('Error:', err);
//    }
//  });
//
//// Multiple sclerosis
//conn.query(
//  'INSERT INTO campaigns (code, name, disease_id, owner, image, description) values (?, ?, ?, ?, ?, ?)',
//  ['multiple-sclerosis', 'Multiple Sclerosis', 113, 'xT3sTUs3Ridx',
//    'https://jscfeatures.jsc.nasa.gov/images/imagefull-1190.png',
//    'Multiple sclerosis (MS) is an unpredictable, often disabling disease of the central nervous system that disrupts the flow of information within the brain, and between the brain and body. The progress, severity and specific symptoms of MS in any one person cannot yet be predicted. Most people with MS are diagnosed between the ages of 20 and 50, with at least two to three times more women than men being diagnosed with the disease.'
//    ],
//  function(err) {
//    if (err) {
//      console.log('Error:', err);
//    }
//  });
