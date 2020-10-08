// Helper functions for files.

import crypto from 'crypto';
import fs from 'fs';
import request from 'request';

const FILE_HASH_ALGORITHM = 'md5';

urlExists = function (url, callback) {
  request({ url: url, method: 'HEAD' }, function (err, res) {
    if (err) {
      return callback(err, false);
    }
    callback(null, /4\d\d/.test(res.statusCode) === false);
  });
}

// sourceUrl: a URL.
// destinationPath: a valid file path.
// callback: a function to run when the download is complete.
// TODO: Make this synchronous?
// TODO: If the file doesn't exist (ex. if it was deleted in between checking its existence and downloading it), the resulting file will contain "Not Found". Find some way to handle this (read in the file contents?).
downloadFile = function (sourceUrl, destinationPath, callback) {
  var r = request(sourceUrl);
  r.on('response', function (res) {
    var file = fs.createWriteStream(destinationPath);
    res.pipe(file);

    file.on('finish', function () {
      file.close(callback);
    });

    file.on('error', function (err) {
      console.log(err);
      fs.unlink(destinationPath, callback);  // Delete the file asynchronously.
    });
  });
}

// TODO: Create async and sync versions of this method.
// TODO: Document this function's args and return values.
verifyFileHash = function (filename, expectedHash, callback) {
  var hash = crypto.createHash(FILE_HASH_ALGORITHM);
  // Stream file into hash.
  var s = fs.ReadStream(filename);
  s.on('data', (data) => {
    hash.update(data);
  });
  s.on('end', () => {
    var digest = hash.digest('hex');
    if (digest === expectedHash) {
      callback(true);
    } else {
      callback(false);
    }
  });
}
