var feature = require('../models/feature');
var user = require('../models/user');
var auth = require('../utils/auth');

// Feature-related routes for app
module.exports = function(app) {

  app.get('/feature/new', auth.requireLogin, auth.requireAdmin, function(req, res, next) {

    return res.render('newfeature', { user: req.user });

  });

  app.post('/feature/new', auth.requireLogin, auth.requireAdmin, function(req, res, next) {

    feature.createFeature(req.body.name, req.body.display_name, function(err, feature) {
      if (err) {
        console.error(err);
        return res.render('newfeature', { user: req.user, message: err, color: 'danger' });
      }
      return res.render('newfeature', { user: req.user, message: 'Feature successfully created.', color: 'success' });
    });

  });

  app.post('/feature/grant/:feature_id', auth.requireLogin, auth.requireAdmin, function(req, res, next) {

    user.getUserByEmail(req.body.user_email, function(err, selectedUser) {
      if (err) {
        req.flash('message', err);
        req.flash('color', 'danger');
        return res.redirect('/admin/features/' + req.params.feature_id);
      }
      if (!selectedUser) {
        req.flash('message', 'No user with email ' + req.body.user_email + ' exists.');
        req.flash('color', 'danger');
        return res.redirect('/admin/features/' + req.params.feature_id);
      }
      feature.grantFeatureToUser(selectedUser.id, req.params.feature_id, function(err) {
        if (err) {
          req.flash('message', err);
          req.flash('color', 'danger');
        } else {
          req.flash('message', 'Successfully granted feature to ' + req.body.user_email + '.');
          req.flash('color', 'success');
        }
        return res.redirect('/feature/' + req.params.feature_id);
      });
    });

  });

  app.get('/feature/revoke/:feature_id/:user_id', auth.requireLogin, auth.requireAdmin, function(req, res, next) {

    feature.revokeFeatureFromUser(req.params.user_id, req.params.feature_id, function(err) {
      if (err) {
        req.flash('message', err);
        req.flash('color', 'danger');
      } else {
        req.flash('message', 'Successfully revoked feature.');
        req.flash('color', 'success');
      }
      return res.redirect('/feature/' + req.params.feature_id);
    });

  });

  app.post('/feature/update/:feature_id', auth.requireLogin, auth.requireAdmin, function(req, res, next) {

    feature.updateFeature(req.params.feature_id, req.body.display_name, function(err, feature) {
      if (err) {
        console.error(err);
        req.flash('message', err);
        req.flash('color', 'danger');
      } else {
        req.flash('message', 'Successfully updated feature.');
        req.flash('color', 'success');
      }
      return res.redirect('/feature/' + req.params.feature_id);
    });

  });

  app.get('/feature/:feature_id', auth.requireLogin, auth.requireAdmin, function(req, res, next) {

    feature.getFeatureById(req.params.feature_id, function(err, selectedFeature) {
      if (err) {
        return res.render('feature', { user: req.user, message: err });
      }
      if (!selectedFeature) {
        return res.render('feature', { user: req.user, message: 'No feature with id ' + req.params.feature_id + ' exists.' });
      }
      feature.getUsersWithFeature(selectedFeature.id, function(err, usersWithFeature) {
        return res.render('feature', {
          user: req.user,
          feature: selectedFeature,
          usersWithFeature: usersWithFeature,
          message: req.flash('message'),
          color: req.flash('color')
        });
      });
    });

  });

  app.get('/features/:user_email', function(req, res, next) {

    user.getUserByEmail(req.params.user_email, function(err, selectedUser) {
      if (err) {
        console.error(err);
        return res.status(500).send(err);
      }
      if (!selectedUser) {
        return res.status(404).send('User not found.');
      }
      user.getFeaturesForUser(selectedUser.id, function(err, features) {
        if (err) {
          console.error(err);
          return res.status(500).send(err);
        }
        return res.status(200).send(features);
      });
    });

  });

  app.get('/enabled/:user_email/:feature_name', function(req, res, next) {

    user.getUserByEmail(req.params.user_email, function(err, selectedUser) {
      if (err) {
        console.error(err);
        return res.status(500).send(err);
      }
      if (!selectedUser) {
        return res.status(404).send('User not found.');
      }
      feature.getFeatureByName(req.params.feature_name, function(err, selectedFeature) {
        if (err) {
          console.error(err);
          return res.status(500).send(err);
        }
        if (!selectedFeature) {
          return res.status(404).send('Feature not found.');
        }
        feature.isEnabled(selectedUser.id, selectedFeature.id, function(err, result) {
          if (err) {
            console.error(err);
            return res.status(500).send(err);
          }
          return res.status(200).send(result);
        });
      });
    });

  });

};
