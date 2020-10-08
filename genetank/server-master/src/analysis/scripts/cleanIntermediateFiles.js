#!/usr/bin/env node
// cleanIntermediateFiles.js

var fs = require('fs');
var path = require('path');

if (process.argv.length !== 4) {
  console.log('Usage: node cleanIntermediateFiles.js <patient id> <output dir>');
  process.exit(1);
}
// Parse passed-in parameters
const patientId = process.argv[2];
const outputDir = process.argv[3];
const patientDir = path.join(outputDir, 'Patient_' + patientId);

// Remove raw DNA file
fs.unlinkSync(patientId + '.zip');

// Remove VCF
fs.unlinkSync(path.join(patientDir, patientId + '.vcf'));

// Remove genotype-to-phenotype prediction results
fs.unlinkSync(path.join(patientDir, 'GeneticPhenotypeReport_Patient_' + patientId + '.csv'));
fs.unlinkSync(path.join(patientDir, patientId + '.sites_not_in_reference.txt'));

// Remove patient directory
fs.rmdirSync(patientDir);