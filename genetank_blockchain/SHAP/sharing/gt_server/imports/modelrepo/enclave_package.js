// API request handlers and functions relevant to EnclavePackages.

import async from 'async';
import crypto from 'crypto';
import { Mongo } from 'meteor/mongo';
import '../common/api.js';
import '../common/aws.js';
import { EnclaveImages, EnclavePackages } from '../common/db.js';
import '../common/file.js';
import '../common/web3.js';

//
// Route handlers
//

getEnclaveImage = function (context) {
  const imageHash = context.params.imageHash;
  if (emptyString(imageHash)) {
    return respond(context, 400,
      'Bad request. Please include an enclave image hash to search for.');
  }
  const imageUrl = EnclaveImages.findOne({ hash: imageHash });
  if (imageUrl) {
    respond(context, 200, imageUrl);
  } else {
    respond(context, 404,
      'No enclave image exists for that enclave image hash.');
  }
}

postEnclaveImage = function (context) {
  const imageHash = context.params.imageHash;
  const body = getData(context.request);
  if (emptyString(imageHash)) {
    return respond(context, 400,
      'Bad request. Please include an enclave image hash.');
  }
  validS3Url(body['url'], function (valid) {
    if (!valid) {
      return respond(context, 400, 'Bad request. Please include a valid URL.');
    }
    const imageUrl = EnclaveImages.findOne({ hash: imageHash });
    if (imageUrl) {
      respond(context, 400,
        'An enclave image already exists for that enclave image hash.');
    } else {
      EnclaveImages.insert({ hash: imageHash, url: body['url'] },
        function (err) {
          if (err)
            respond(context, 500, err);
          else
            ok(context);
        });
    }
  });
}

getSource = function (context) {
  const sourceHash = context.params.sourceHash;
  if (emptyString(sourceHash)) {
    return respond(context, 400,
      'Bad request. Please include a source code hash to search for.');
  }
  const source = SourceCodes.findOne({ hash: sourceHash });
  if (source) {
    respond(context, 200, source);
  } else {
    respond(context, 404, 'No source code exists for that source hash.');
  }
}

getEnclavePackage = function (context) {
  const imageHash = context.params.imageHash;
  if (emptyString(imageHash)) {
    return respond(context, 400,
      'Bad request. Please include an enclave image hash to search for.');
  }
  const packageInternal =
    EnclavePackages.findOne({ enclave_image_hash: imageHash });
  if (packageInternal) {
    respond(context, 200, asExternalPackage(packageInternal));
  } else {
    respond(context, 404,
      'No enclave package exists for that enclave image hash.');
  }
}

// Enclave packages are stored as:
// {
//   "creator_address": string,
//   "version": string,
//   "src_zip_file": string (url),
//   "hash_src_zip_file": string, // MD5 hash
//   "bin_zip_file": string (url),
//   "hash_bin_zip_file": string, // MD5 hash
//   "MRENCLAVE": string,
//   "ISVPRODID": number,
//   "ISVSN": number,
//   "hash_signature": string // Keccak-256 SHA3 hash
// }
// The following information will be stored in the database, but will not be
// included in an initial upload.
// TODO: Provide addAuditingRecord, addTrainer, addVerifier methods.
// {
//   "auditing_records": mapping (address => AuditingRecord id),
//   "trainers": mapping (trainer address => EnclaveTrainer id),
//   "verifiers": mapping (verifier address => DataVerifier id)
// }
// The enclave image and source code is stored separately.
postEnclavePackage = function (context) {
  const body = getData(context.request);
  console.log('Received body:');
  console.log(body);

  async.waterfall([
    function validatePackageUpload(callback) {
      validateEnclavePackageUpload(body, function (validation) {
        if (validation.err)
          callback(validation);
        else
          callback(null);
      });
    },
    function createEnclavePackage(callback) {
      EnclavePackages.insert(asInternalPackage(body), function (err, package) {
        if (err)
          callback(internalErrorObject());
        else
          callback(null, package);
      });
    },
    function findEnclaveFamily(package, callback) {
      const family = EnclaveFamilies.findOne(
        { create_address: body['creator_address'] });
      if (!family) {
        callback(internalErrorObject());
      } else if (family['enclave_packages'][body['MRENCLAVE']]) {
        // The package is already part of this family.
        callback(alreadyExistsObject());
      } else {
        callback(null, package, family);
      }
    },
    function addEnclavePackageToFamily(package, family, callback) {
      family['enclave_packages'][body['MRENCLAVE']] = package[0]['_id'];
      EnclaveFamilies.save({ _id: family['_id'] }, family, function (err) {
        if (err)
          callback(internalErrorObject());
        else
          callback(null);
      });
    }
  ], function (err) {
    if (err) {
      console.log(err);
      return respond(context, err.status, err.message);
    } else {
      return ok(context);
    }
  });
}

//
// Helpers
//

// Get a copy of a package ready to return to an external user.
asExternalPackage = function (packageInternal) {
  var package = { 'package': {} };
  copyFields(packageInternal, package, ['registration']);
  copyFields(packageInternal, package['package'],
    ['name', 'source_hash', 'MRENCLAVE', 'ISVPRODID', 'ISVSN', 'signature',
      'public_key']);
  package['package']['enclave_image_hash'] =
    packageInternal['enclave_image_hash'];
  return package;
}

// Get a copy of a package ready for storage into a database.
asInternalPackage = function (packageExternal) {
  var package = { 'package': {} };
  copyFields(packageExternal, package, ['registration']);
  copyFields(packageExternal, package['package'],
    ['name', 'source_hash', 'MRENCLAVE', 'ISVPRODID', 'ISVSN', 'signature',
      'public_key']);
  package['enclave_image_hash'] =
    packageExternal['package']['enclave_image_hash'];
  return package;
}

// Validate a potential enclave package upload.
// We expect an upload object in the form of:
// {
//   "creator_address": string,
//   "version": string,
//   "src_zip_file": string (url),
//   "hash_src_zip_file": string, // MD5 hash
//   "bin_zip_file": string (url),
//   "hash_bin_zip_file": string, // MD5 hash
//   "MRENCLAVE": string,
//   "ISVPRODID": number,
//   "ISVSN": number,
//   "hash_signature": string // Keccak-256 SHA3 hash
// }
// The following information will be stored in the database, but will not be
// included in an initial upload.
// TODO: Provide addAuditingRecord, addTrainer, addVerifier methods.
// {
//   "auditing_records": mapping (address => AuditingRecord id),
//   "trainers": mapping (trainer address => EnclaveTrainer id),
//   "verifiers": mapping (verifier address => DataVerifier id)
// }
validateEnclavePackageUpload = function (upload, cb) {
  async.waterfall([
    function checkRequiredFields(callback) {
      const requiredFields = ['creator_address', 'version', 'src_zip_file',
        'hash_src_zip_file', 'bin_zip_file', 'hash_bin_zip_file', 'MRENCLAVE',
        'ISVPRODID', 'ISVSN', 'hash_signature'];
      if (!hasFields(upload, requiredFields)) {
        callback({
          status: 400, message: 'Missing package fields: ' +
            getMissingFieldsAsString(upload, requiredFields)
        });
      }
    },
    function checkExistingEnclavePackage(callback) {
      const findResult = EnclavePackages.findOne(
        { MRENCLAVE: upload['package']['MRENCLAVE'] });
      if (findResult)
        callback({
          status: 400, message: 'A package with MRENCLAVE ' +
            upload['package']['MRENCLAVE'] + ' already exists.'
        });
      else
        callback(null);
    },
    function findEnclaveFamily(callback) {
      const family = EnclaveFamilies.findOne(
        { creator_address: upload['creator_address'] });
      if (!family)
        callback({
          status: 400,
          message: 'No enclave family is associated with that creator ' +
            'address. Create an enclave family first.'
        });
      else
        callback(null, family);
    },
    // The enclave creator's public key is currently stored in the
    // EnclaveFamily object, which can be looked up with the creator address.
    function validateSignature(family, callback) {
      const data = upload['MRENCLAVE'] + ',' + upload['ISVPRODID'] + ',' +
        upload['ISVSN'];
      const public_key = family['public_key'];
      // TODO: Verify the signature.
      // web3.eth.accounts.recover doesn't seem to be working as intended, so
      // I'm skipping this for now. We may want to consider using a standard
      // SHA-256 hash. There seem to be a lot of inconsistencies with using the
      // web3 sign/recover functions.
      // https://github.com/ethereum/web3.js/issues/1663
      callback(null);
    },
    function checkSrcZipFile(callback) {
      urlExists(upload['src_zip_file'], function (err, exists) {
        if (err) {
          callback(internalErrorObject());
        } else if (!exists) {
          callback({
            status: 400, message: 'That src_zip_file does not exist.'
          });
        } else {
          callback(null);
        }
      });
    },
    function checkBinZipFile(callback) {
      urlExists(upload['bin_zip_file'], function (err, exists) {
        if (err) {
          callback(internalErrorObject());
        } else if (!exists) {
          callback({
            status: 400, message: 'That bin_zip_file does not exist.'
          });
        } else {
          callback(null);
        }
      });
    },
    function downloadSrcZipFile(callback) {
      const srcFilePath = temporaryFilePath() + generateRandomFileName('zip');
      // const srcFilePath = 'C:\\Users\\Lauren\\Desktop\\download_src_zip.jpg';
      // TODO: Probably just randomly generate a file path name to avoid conflicts.
      downloadFile(upload['src_zip_file'], srcFilePath, function (err) {
        if (err)
          callback(internalErrorObject());
        else
          callback(null, srcFilePath);
      });
    },
    function verifySrcZipHash(srcFilePath, callback) {
      verifyFileHash(filePath, upload['hash_src_zip_file'], function (valid) {
        if (!valid)
          callback({
            status: 400, message: 'Unexpected hash_src_zip_file.'
          });
        else
          callback(null, srcFilePath);
      });
    },
    function downloadBinZipFile(srcFilePath, callback) {
      const binFilePath = 'C:\\Users\\Lauren\\Desktop\\download_bin_zip.jpg';
      // TODO: Probably just randomly generate a file path name to avoid conflicts.
      downloadFile(upload['bin_zip_file'], binFilePath, function (err) {
        if (err)
          callback(internalErrorObject());
        else
          callback(null, srcFilePath, binFilePath);
      });
    },
    function verifyBinZipHash(srcFilePath, binFilePath, callback) {
      verifyFileHash(filePath, upload['hash_bin_zip_file'], function (valid) {
        if (!valid)
          callback({
            status: 400, message: 'Unexpected hash_bin_zip_file.'
          });
        else
          callback(null, srcFilePath, binFilePath);
      });
    }
  ], function (err, srcFilePath, binFilePath) {
    if (err) {
      err['err'] = true;
      cb(err);
    } else {
      cb({ err: false, srcFilePath: srcFilePath, binFilePath: binFilePath });
    }
  });
}

//
// Enclave image handling
// This is not currently functional. Users should instead upload their enclave images to S3 and link them here.
//

Images = new FS.Collection('images', {
  stores: [new FS.Store.FileSystem('images', { path: '~/testuploads' })]
});

getEnclaveImageFS = function () {
  const image = EnclaveImages.findOne(this.urlParams.imageHash);
  if (image) {
    return image;
  } else {
    return {
      statusCode: 404,
      body: 'No enclave image exists for that hash.'
    };
  }
}

uploadEnclaveImageFS = function () {
  console.log(this.request.body);
  var uploadedFile = new FS.File(this.request.body.photo);
  Images.insert(uploadedFile, function (err, fileObj) {
    if (err)
      console.log(err);
    else
      console.log(fileObj);
  });
  // TODO: This doesn't actually return anything yet, you need to wait for the insert to complete first.
  return {
    status: 'success'
  };
}
