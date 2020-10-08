// API request handlers and functions relevant to AuditingRecords.

import { Mongo } from 'meteor/mongo';
import '../common/api.js';
import '../common/aws.js';
import { AuditingRecords } from '../common/db.js';
import '../common/eth.js';

//
// Route handlers
//

getAuditingRecord = function (context) {
  const signature = context.params.signature;
  if (emptyString(signature)) {
    return respond(context, 400,
      'Bad request. Please include the signature of an auditing record to ' +
      'search for.');
  }
  // TODO more stuff
}
