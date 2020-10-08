#!/usr/bin/env node
// pullDownUpload.js

var AWS = require('aws-sdk');
var fs = require('fs');

if (process.argv.length !== 4) {
  console.log('Usage: node pullDownUpload.js <bucket name> <object key>');
  process.exit(1);
}
// Parse passed-in parameters
const bucket = process.argv[2];
const key = process.argv[3];

const s3 = new AWS.S3();
const options = {
  Bucket: bucket,
  Key: key
};

// Download and save the file from S3
var file = fs.createWriteStream(key + '.zip');
s3.getObject(options).createReadStream().pipe(file);
