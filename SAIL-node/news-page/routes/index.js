const nodemailer = require('nodemailer');

var email = require('../config/email');

var user = require('../models/user');
var auth = require('../utils/auth');

// Main routes for app
module.exports = function(app) {

  app.get('/', function(req, res, next) {

    res.render('index');

  });

  app.get('/about-us', function(req, res, next) {

    res.render('about-us');

  });

  app.get('/join-us', function(req, res, next) {

    res.render('join-us');

  });

  app.get('/news', function(req, res, next) {

    res.render('news');

  });

  app.get('/news-content/:id', function(req, res, next) {

    res.render('news-content',{id:req.params.id});

  });


  app.get('/join-us/senior-software-architect', function(req, res, next) {

    res.render('senior-software-architect');

  });

  app.get('/thanks.html', function(req, res, next) {

    res.render('thanks');

  });

  app.get('/profile', auth.requireLogin, function(req, res, next) {

    user.getFeaturesForUser(req.user.id, function(err, features) {
      if (err) {
        console.error(err);
        return res.render('profile', { user: req.user, message: 'Something went wrong. Please try again.', color: 'danger' });
      }
      return res.render('profile', { user: req.user, features: features });
    });

  });

  app.post('/beta', function(req, res, next) {

    console.log('Beta signup: ' + req.body.email);
    // Email out the beta request.
    var transporter = nodemailer.createTransport({
      service: 'gmail',
      auth: {
        user: email.user,
        pass: email.pass
      }
    });
    var mailOptions = {
      from: 'GeneTank <genetank.info@gmail.com>',
      to: 'contact@genetank.xyz',
      subject: 'GeneTank Beta Signup',
      text: 'Beta signup received for email address:\n' + req.body.email
    };
    transporter.sendMail(mailOptions, function(err, info) {
      if (err) {
        console.error(err);
        res.sendStatus(500);
        return;
      }
      console.log('Message sent: %s', info.messageId);
      res.sendStatus(200);
    });

  });

};
