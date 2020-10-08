var db = require('./db');

// Set up Feature class
var Feature = function(feature) {
  var that = Object.create(Feature.prototype);

  that.id           = feature.id;
  that.name         = feature.name;
  that.display_name = feature.display_name;

  return that;
};

// Create a new feature
// callback(err, newFeature)
var createFeature = function(name, display_name, callback) {
  var newFeature = {
    name: name,
    display_name: display_name
  };
  db.query('INSERT INTO features ( name, display_name ) values (?,?)',
    [name, display_name],
    function(err, info) {
      if (err) {
        return callback(err);
      }

      // Successfully created feature
      newFeature.id = info.insertId;
      return callback(null, new Feature(newFeature));
    }
  );
};

// Get the feature by id.
// callback(err, feature)
var getFeatureById = function(id, callback) {
  db.query('SELECT * FROM features WHERE id = ?', [id], function(err, rows) {
    if (err) {
      return callback(err);
    }
    if (!rows.length) {
      return callback(null, null);
    }
    return callback(null, new Feature(rows[0]));
  });
};

// Get the feature by name.
// callback(err, feature)
var getFeatureByName = function(name, callback) {
  db.query('SELECT * FROM features WHERE name = ?', [name], function(err, rows) {
    if (err) {
      return callback(err);
    }
    if (!rows.length) {
      return callback(null, null);
    }
    return callback(null, new Feature(rows[0]));
  });
};

// Get a list of users that have been granted the feature.
// callback(err, users)
var getUsersWithFeature = function(feature_id, callback) {
  db.query('SELECT user_id FROM user_features WHERE feature_id = ?', [feature_id], function(err, rows) {
    if (err) {
      return callback(err);
    }

    if (!rows.length) {
      return callback(null, []);
    }

    // Collect user ids.
    var user_ids = [];
    rows.forEach(function(row) {
      user_ids.push(row.user_id);
    });

    db.query('SELECT id, email FROM users WHERE id IN (?)', [user_ids], function(err, rows) {
      if (err) {
        return callback(err);
      }
      return callback(null, rows);
    });
  });
};

// Grant a feature to a user.
// Does not validate that the given user id or feature id exist.
// callback(err)
var grantFeatureToUser = function(userId, featureId, callback) {
  // Check for an existing entry
  db.query('SELECT * FROM user_features WHERE user_id = ? AND feature_id = ?',
    [userId, featureId],
    function(err, rows) {
    if (err) {
      return callback(err);
    }

    if (rows.length) {
      // An entry granting this feature to this user already exists.
      return callback(null);
    }

    // Grant this feature to this user.
    db.query('INSERT INTO user_features ( user_id, feature_id ) values (?,?)',
      [userId, featureId], callback);
  });
};

// Revoke a feature from a user.
// callback(err)
var revokeFeatureFromUser = function(userId, featureId, callback) {
  // TODO: Differentiate deletes that actually removed entries from no-ops.
  db.query('DELETE FROM user_features WHERE user_id = ? AND feature_id = ?',
    [userId, featureId], callback);
};

// List all features.
// callback(err, features)
var listFeatures = function(callback) {
  db.query('SELECT * FROM features', [], function(err, rows) {
    if (err)
      return callback(err);
     return callback(null, rows);
  });
};

// Update a feature.
// callback(err)
var updateFeature = function(id, display_name, callback) {
  db.query('UPDATE features SET display_name = ? WHERE id = ?', [display_name, id], callback);
};

// Returns true if the feature is enabled for the user.
// callback(err, result)
var isEnabled = function(user_id, feature_id, callback) {
  db.query('SELECT feature_id FROM user_features WHERE user_id = ? AND feature_id = ?', [user_id, feature_id], function(err, rows) {
    if (err) {
      return callback(err);
    }
    if (!rows.length) {
      return callback(null, false);
    }
    return callback(null, true);
  });
};

exports.Feature = Feature;
exports.createFeature = createFeature;
exports.getFeatureById = getFeatureById;
exports.getFeatureByName = getFeatureByName;
exports.getUsersWithFeature = getUsersWithFeature;
exports.grantFeatureToUser = grantFeatureToUser;
exports.revokeFeatureFromUser = revokeFeatureFromUser;
exports.listFeatures = listFeatures;
exports.updateFeature = updateFeature;
exports.isEnabled = isEnabled;
