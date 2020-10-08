// Routes related to key-value entries.

var async = require('async');

var Entry = require('../models/entry');
var apis = require('../utils/apis');
var errors = require('../utils/errors');

module.exports = function (app) {

  app.get('/entries', listEntries);
  app.get('/entries/:keyword', listSearchEntries);
  app.get('/entry/:key', getEntry);
  app.post('/entry/insert', insertEntry);
  app.post('/entry/update', updateEntry);
  app.post('/entry/delete', deleteEntry);

};

function listEntries(req, res, next) {
  async.waterfall([
    function findLatestEntries(callback) {
      Entry.find({}).sort({ _id: -1 }).exec(callback);
    },
    function returnExistingEntries(entries, callback) {
      if (!entries) {
        callback(errors.NOT_FOUND);
        return;
      }
      for (var i = 0; i < entries.length; i++) {
        entries[i] = entries[i]._doc;
        // Strip any password or internal database fields.
        entries[i] = apis.scrubHiddenFields(entries[i]);
        entries[i] = apis.scrubDbFields(entries[i]);
      }
      callback(null, entries);
    }
  ], function (err, entries) {
    if (errors.handleError(err, res)) {
      return;
    }
    res.send(entries);
  });
}

function listSearchEntries(req, res, next) {
  async.waterfall([
    function findLatestEntries(callback) {
      Entry.find({$text: { $search: "\""+req.params.keyword+"\""}}).sort({ _id: -1 }).exec(callback);
    },
    function returnExistingEntries(entries, callback) {
      if (!entries) {
        callback(errors.NOT_FOUND);
        return;
      }
      for (var i = 0; i < entries.length; i++) {
        entries[i] = entries[i]._doc;
        // Strip any password or internal database fields.
        entries[i] = apis.scrubHiddenFields(entries[i]);
        entries[i] = apis.scrubDbFields(entries[i]);
      }
      callback(null, entries);
    }
  ], function (err, entries) {
    if (errors.handleError(err, res)) {
      return;
    }
    res.send(entries);
  });
}


function getEntry(req, res, next) {
  async.waterfall([
    function findExistingEntry(callback) {
      Entry.findOne({ key: req.params.key }, callback);
    },
    function returnExistingEntry(entry, callback) {
      if (!entry) {
        callback(errors.NOT_FOUND);
        return;
      }
      entry = entry._doc;
      // Strip any password or internal database fields.
      callback(null, apis.scrubDbFields(apis.scrubHiddenFields(entry)));
    }
  ], function (err, entry) {
    if (errors.handleError(err, res)) {
      return;
    }
    res.send(entry);
  });
}

function insertEntry(req, res, next) {
  const requiredFields = ['key', 'value'];
  const allFields = ['key', 'value', 'keywords', 'protected'];
  var request = {};
  apis.copyFields(req.body, request, allFields);

  async.waterfall([
    function validateRequest(callback) {
      if (!apis.hasFields(request, requiredFields)) {
        callback(
          errors.missingFields(apis.getMissingFieldsAsString(requiredFields)));
        return;
      }
      if (!apis.checkTypes(request, allFields,
        ['string', 'object', apis.types.STRING_ARRAY, 'boolean'])) {
        callback(errors.INVALID_REQUEST);
        return;
      }
      callback(null);
    },
    function findExistingEntry(callback) {
      Entry.findOne({ key: request.key }, callback);
    },
    function insertEntry(entry, callback) {
      if (entry) {
        callback(errors.ALREADY_EXISTS);
        return;
      }
      // TODO: Generate a password if the entry is protected.
      request = {
        key : request.key,
        value: JSON.stringify(request.value),
        keywords: request.keywords,
        protected : request.protected
      };
      var newEntry = new Entry(request);
      newEntry.save(callback);
    }
  ], function (err) {
    if (errors.handleError(err, res)) {
      return;
    }
    res.sendStatus(200);
  });
}

function updateEntry(req, res, next) {
  const requiredFields = ['key', 'value'];
  const allFields = ['key', 'value', 'keywords', 'protected'];
  var request = {};
  apis.copyFields(req.body, request, allFields);

  async.waterfall([
    function validateRequest(callback) {
      if (!apis.hasFields(request, requiredFields)) {
        callback(
          errors.missingFields(apis.getMissingFieldsAsString(requiredFields)));
        return;
      }
      if (!apis.checkTypes(request, allFields,
        ['string', 'object', apis.types.STRING_ARRAY, 'boolean'])) {
        callback(errors.INVALID_REQUEST);
        return;
      }
      callback(null);
    },
    function findExistingEntry(callback) {
      Entry.findOne({ key: request.key }, callback);
    },
    function checkPassword(entry, callback) {
      if (!entry) {
        callback(errors.NOT_FOUND);
        return;
      }
      // TODO: Add password checking.
      callback(null, entry);
    },
    function updateEntry(entry, callback) {
      apis.copyFields(
        request, entry, ['key', 'value', 'keywords', 'protected']);
      entry.save(callback);
    }
  ], function (err) {
    if (errors.handleError(err, res)) {
      return;
    }
    res.sendStatus(200);
  });
}

function deleteEntry(req, res, next) {
  if (!req.body.key) {
    errors.handleError(errors.missingFields('key'), res);
    return;
  }
  async.waterfall([
    function findExistingEntry(callback) {
      Entry.findOne({ key: req.body.key }, callback);
    },
    function deleteExistingEntry(entry, callback) {
      if (!entry) {
        callback(errors.NOT_FOUND);
        return;
      }
      Entry.findOneAndDelete({ key: req.body.key }, callback);
    }
  ], function (err) {
    if (errors.handleError(err, res)) {
      return;
    }
    res.sendStatus(200);
  });
}
