// Helper functions for interacting with AWS.

import { AWS } from 'meteor/peerlibrary:aws-sdk';

// Returns whether the given url is a valid AWS S3 url.
validS3Url = function (url, callback) {
  console.log('Validating url:', url);
  if (!url || !url.length) {
    return callback(false);
  }
  const parts = extractPartsFromS3Url(url);
  if (parts.err) {
    return callback(false);
  }
  // We don't use any credentials. The S3 bucket must be publicly accessible.
  AWS.config.update({ region: parts.region });
  const s3 = new AWS.S3({ params: { Bucket: parts.bucket, Key: parts.key } });
  // The S3 bucket must allow HEAD requests in its CORS configuration.
  // https://forums.aws.amazon.com/message.jspa?messageID=571491#571491
  // Meteor.bindEnvironment is needed because Meteor runs everything in Fibers:
  // https://stackoverflow.com/questions/44333548/error-cant-wait-without-a-fiber
  s3.headObject(Meteor.bindEnvironment(function (err) {
    if (err) {
      console.log(err);
      return callback(false);
    }
    return callback(true);
  }));
}

// Extracts the bucket, region, and key from an AWS S3 url. Returns an error
// if the url is invalid.
extractPartsFromS3Url = function (url) {
  var bucket, region, key;
  // This matches URLs in the form http://bucket.s3-region.amazonaws.com/key
  const s3matcher1 =
    /^https?:\/\/(.*)\.s3(\.dualstack)?[\.\-]?(.*)\.amazonaws\.com\/(.*)$/g;
  // Matches should be [bucket, dualstack, region, key], starting at index 1.
  // https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/RegExp/exec
  const match1 = s3matcher1.exec(url);
  if (match1) {
    console.log('S3 url match:', match1);
    bucket = match1[1];
    region = match1[3];
    key = match1[4];
  } else {
    // This matches URLs in the form http://s3-region.amazonaws.com/bucket/key
    const s3matcher2 =
      /^https?:\/\/s3(\.dualstack)?[\.\-]?(.*)\.amazonaws\.com\/([^\/]+)\/(.*)$/g;
    // Matches should be [dualstack, region, bucket, key], starting at index 1.
    // https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/RegExp/exec
    const match2 = s3matcher2.exec(url);
    if (match2) {
      console.log('S3 url match:', match2);
      bucket = match2[3];
      region = match2[2];
      key = match2[4];
    } else {
      // Neither regex matched the URL.
      return { err: true };
    }
  }

  if (!bucket || !key) {
    return { err: true };
  }
  if (!region) {
    region = 'us-east-1';
  }
  return { bucket: bucket, region: region, key: key };
}
