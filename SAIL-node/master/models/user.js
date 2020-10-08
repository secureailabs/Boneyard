var bcrypt = require('bcrypt-nodejs');
var uuidV4 = require('uuid/v4');

var db      = require('./db');
var feature = require('./feature');

// Set up User class
var User = function(user) {
  var that = Object.create(User.prototype);

  that.id       = user.id;
  that.email    = user.email;
  that.password = user.password;

  return that;
};

// Gets a random id for this user
var generateUserId = function() {
  return uuidV4();
};

// Hash and salt the password with bcrypt
var hashPassword = function(password) {
  return bcrypt.hashSync(password, bcrypt.genSaltSync(8), null);
};

// Check if password is correct
var validPassword = function(password, savedPassword) {
  return bcrypt.compareSync(password, savedPassword);
};

// Create a new user
// callback(err, newUser)
var createUser = function(email, password, callback) {
  var newUser = {
    id: generateUserId(),
    email: email,
    password: hashPassword(password)
  };
  db.query('INSERT INTO users ( id, email, password ) values (?,?,?)',
    [newUser.id, email, newUser.password],
    function(err) {
      if (err) {
        if (err.code === 'ER_DUP_ENTRY') {
          // Determine whether the duplicate was due to the email or a duplicate user id.
          db.query('SELECT * FROM users WHERE email = ?', [email], function(err, rows) {
            if (err || rows.length) {
              // An error occurred, or the email was a duplicate.
              return callback(err);
            }

            // We somehow generated a duplicate user id; try again.
            return createUser(email, password, callback);
          });
        }
        return callback(err);
      }

      // Successfully created user
      return callback(null, new User(newUser));
    }
  );
};

// Check if a user exists and create them if they do not
// callback(err, newUser)
var signup = function(req, email, password, callback) {
  // Check if there's already a user with that email
  db.query('SELECT * FROM users WHERE email = ?', [email], function(err, rows) {
    if (err) {
      return callback(err);
    }

    if (rows.length) {
      return callback(null, false, req.flash('signupMessage', 'An account with that email address already exists.'));
    } else {
      // No user exists, create the user
      return createUser(email, password, callback);
    }
  });
};

// Log in a user
// callback(err, user)
var login = function(req, email, password, callback) {
  // Check that the user logging in exists
  db.query('SELECT * FROM users WHERE email = ?', [email], function(err, rows) {
    if (err) {
      return callback(err);
    }

    if (!rows.length) {
      return callback(null, false, req.flash('loginMessage', 'No user found.'));
    }

    if (!validPassword(password, rows[0].password)) {
      return callback(null, false, req.flash('loginMessage', 'Wrong password.'));
    }

    // User successfully logged in, return user
    return callback(null, new User(rows[0]));
  });
};

// Get a user by their email address.
// callback(err, user)
var getUserByEmail = function(email, callback) {
  db.query('SELECT * FROM users WHERE email = ?', [email], function(err, rows) {
    if (err)
      return callback(err);
    if (!rows.length)
      return callback(null, null);
    return callback(null, new User(rows[0]));
  });
};

// List all users
// callback(err, users)
var listUsers = function(callback) {
  db.query('SELECT * FROM users', [], function(err, rows) {
    if (err)
      return callback(err);
     return callback(null, rows);
  });
};

// Delete a user
// callback(err)
var deleteUser = function(id, callback) {
  db.query('DELETE FROM users WHERE id = ?', [id], callback);
};

// Get the features granted to a user.
// callback(err, features)
var getFeaturesForUser = function(id, callback) {
  db.query('SELECT feature_id FROM user_features WHERE user_id = ?', [id], function(err, rows) {
    if (err) {
      return callback(err);
    }

    if (!rows.length) {
      return callback(null, []);
    }

    // Collect feature ids.
    var feature_ids = [];
    rows.forEach(function(row) {
      feature_ids.push(row.feature_id);
    });

    db.query('SELECT name, display_name FROM features WHERE id IN (?) ORDER BY display_name', [feature_ids], function(err, rows) {
      if (err) {
        return callback(err);
      }

      // Sanitize the db rows into Features since this will be used for externally-facing APIs.
      var features = [];
      rows.forEach(function(row) {
        features.push(new feature.Feature(row));
      });
      return callback(null, features);
    });
  });
};

exports.User = User;
exports.signup = signup;
exports.login = login;
exports.getUserByEmail = getUserByEmail;
exports.listUsers = listUsers;
exports.deleteUser = deleteUser;
exports.getFeaturesForUser = getFeaturesForUser;
