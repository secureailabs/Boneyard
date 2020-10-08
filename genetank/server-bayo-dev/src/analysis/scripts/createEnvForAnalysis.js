#!/usr/bin/env node
// createEnvForAnalysis.js

var fs = require('fs');
var path = require('path');

if (process.argv.length !== 4) {
  console.log('Usage: node createEnvForAnalysis.js <patient id> <output dir>');
  process.exit(1);
}
// Parse passed-in parameters
const patientId = process.argv[2];
const outputDir = process.argv[3];

fs.mkdirSync(path.join(outputDir, 'Patient_' + patientId));