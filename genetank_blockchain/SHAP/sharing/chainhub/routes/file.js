// Routes related to file uploads and management.

var async = require('async');
var crypto = require('crypto');
var fs = require('fs');
var multer = require('multer');
var path = require('path');

var upload = multer({ dest: 'public/file/' });

var apis = require('../utils/apis');
var errors = require('../utils/errors');

module.exports = function (app) {

  app.post('/file/upload', upload.single('file'), uploadFile);
  app.post('/file/delete', deleteFile);

};

function uploadFile(req, res, next) {
  console.log('in uploadFile');
  console.log(req.file);
  if (!req.file || !req.file.path) {
    errors.handleError(errors.INVALID_REQUEST, res);
    return;
  }
  // multer gives the file a random name for us.
  res.send({
    'key': req.file.filename,
    'path': '/file/' + req.file.filename
  });
}

function deleteFile(req, res, next) {
  if (!req.body.key) {
    errors.handleError(errors.missingFields('key'), res);
    return;
  }
  const filePath = path.join(__dirname, '../public/file/', req.body.key);
  fs.unlink(filePath, function (err) {
    if (err) {
      if (err.code === 'ENOENT') {
        errors.handleError(errors.NOT_FOUND, res);
        return;
      }
      errors.handleError(err, res);
      return;
    }
    res.sendStatus(200);
  });
}
