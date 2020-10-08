#!/usr/bin/env node
// uploadRawAndVCF.js

var async = require('async');
var AWS = require('aws-sdk');
var fs = require('fs');
var path = require('path');

if (process.argv.length !== 6) {
  console.log('Usage: node uploadRawAndVCF.js <bucket name> <object key> <raw uploads bucket name> <vcf directory>');
  process.exit(1);
}
// Parse passed-in parameters
const bucket = process.argv[2]; // This is the bucket to upload files to
const key = process.argv[3];
const uploadsBucket = process.argv[4]; // This should be the bucket where the raw file was originally uploaded
const vcfDir = process.argv[5];
const vcfFilePath = path.join(vcfDir, 'Patient_' + key, key + '.vcf');

const s3 = new AWS.S3();

async.parallel([
  function (callback) {
    // Copy the raw .zip from the uploads bucket to the destination bucket
    const options = {
      Bucket: bucket,
      CopySource: uploadsBucket + '/' + key,
      Key: 'raw/' + key + '.zip'
    };
    s3.copyObject(options, callback);
  },
  function (callback) {
    // Upload the VCF file to S3
    const options = {
      Bucket: bucket,
      Key: 'vcf/' + key + '.vcf',
      Body: fs.createReadStream(vcfFilePath)
    };
    s3.upload(options, callback);
  }
], function(err, results) {
  if (err) {
    console.log('Error uploading to S3:', err);
    process.exit(1);
  } else {
    process.exit(0);
  }
});