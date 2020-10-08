// API request handlers and functions relevant to EnclaveFamilies.

import { Mongo } from 'meteor/mongo';
import '../common/api.js';
import { EnclaveFamilies } from '../common/db.js';
import '../common/web3.js';

//
// Route handlers
//

getEnclaveFamily = function (context) {
  const creatorAddress = context.params.creatorAddress;
  if (emptyString(creatorAddress)) {
    return respond(context, 400,
      'Bad request. Please include a creator address to search for.');
  }
  const family = EnclaveFamilies.findOne({ creator_address: creatorAddress });
  if (family) {
    respond(context, 200, scrubId(family));
  } else {
    respond(context, 404,
      'No enclave family exists for that creator address.');
  }
}

// Components of an enclave family stored in ModelRepo:
// {
//   "creator_address": eth address,
//   "name": string,
//   "description": string,
//   "public_key": string,
//   "enclave_packages": mapping (enclave hash => EnclavePackage)
// }
postEnclaveFamily = function (context) {
  const body = getData(context.request);
  const validation = validateEnclaveFamilyUpload(body);
  if (validation.err) {
    return respond(context, 400, validation.message);
  }
  EnclaveFamilies.insert(body, function (err) {
    if (err) {
      return internalError(context);
    } else {
      return ok(context);
    }
  });
}

//
// Helpers
//

// Validate a potential enclave family upload.
// We expect an upload object in the form of:
// {
//   "creator_address": eth address,
//   "name": string,
//   "description": string,
//   "public_key": string,
// }
// The following information will be stored in the database, but will not be
// included in an initial upload.
// {
//   "enclave_packages": mapping (MRENCLAVE => EnclavePackage id)  // See enclavePackage method
// }
validateEnclaveFamilyUpload = function (upload) {
  // Check for missing fields.
  const requiredFields = ['creator_address', 'name', 'description',
    'public_key'];
  if (!hasFields(upload, requiredFields)) {
    return {
      err: true, message: 'Missing enclave family fields: ' +
        getMissingFieldsAsString(upload, requiredFields)
    };
  }
  // creator_address must be a valid Ethereum address.
  if (!web3.isAddress(upload['creator_address'])) {
    return { err: true, message: 'Invalid creator address.' };
  }
  // Ensure that the family does not already exist.
  const family = EnclaveFamilies.findOne(
    { creator_address: upload['creator_address'] });
  if (family) {
    return {
      err: true, message:
        'An enclave family with that creator address already exists.'
    };
  }
  return { err: false };
}
