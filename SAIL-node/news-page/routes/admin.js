var feature = require('../models/feature');
var user = require('../models/user');
var auth = require('../utils/auth');

// Admin-related routes for app
module.exports = function(app) {

  app.get('/admin/users', auth.requireLogin, auth.requireAdmin, function(req, res, next) {

    user.listUsers(function(err, rows) {
      var users = [];
      if (!err) {
        rows.forEach(function(row) {
          users.push({ id: row.id, email: row.email, admin: row.admin });
        });
      }
      res.render('adminusers', {
        user: req.user,
        users: users,
        message: req.flash('message'),
        color: req.flash('color')
      });
    });

  });

  app.get('/delete/user/:id', auth.requireLogin, auth.requireAdmin, function(req, res, next) {

    if (req.user.id === req.params.id) {
      // If the user is trying to delete their own account, log them out first
      req.logout();
    }
    user.deleteUser(req.params.id, function(err) {
      if (err) {
        console.error(err);
        req.flash('message', err);
        req.flash('color', 'danger');
      } else {
        req.flash('message', 'Successfully deleted user.');
        req.flash('color', 'success');
      }
      res.redirect('/admin/users');
    });

  });

  app.get('/admin/features', auth.requireLogin, auth.requireAdmin, function(req, res, next) {

    feature.listFeatures(function(err, rows) {
      var features = [];
      if (!err) {
        rows.forEach(function(row) {
          features.push({ id: row.id, name: row.name, display_name: row.display_name });
        });
      }
       res.render('adminfeatures', { user: req.user, features: features });
    });

  });

};
