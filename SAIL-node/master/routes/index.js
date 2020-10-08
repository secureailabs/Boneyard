const nodemailer = require('nodemailer');

var email = require('../config/email');

var db = require('../models/db');
var user = require('../models/user');
var auth = require('../utils/auth');
var payments = require('../utils/payments');

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

  app.get('/demo', function(req, res, next) {

    res.render('demo');

  });

  app.post('/demo/pay', function(req, res, next) {

    var demoPrice = 100 * 100;  // $100
    payments.createPaymentIntent(demoPrice, function(err, paymentIntent) {
      if (err) {
        console.error(err);
        return res.redirect('/demo', { err: 'Sorry, something went wrong. Please try again.' });
      }

      return res.render('demo-pay', {
        price_dollars: 100,  // $100
        stripe_pk: payments.stripePublicKey,
        stripe_payment_id: paymentIntent.id,
        stripe_client_secret: paymentIntent.client_secret
      });
    });

  });

  app.get('/demo/code/:stripe_payment_id', function(req, res, next) {

    payments.paymentSucceeded(req.params.stripe_payment_id, function(err, success) {
      if (err) {
        console.error(err);
        return res.redirect('/demo');
      }

      if (!success) {
        console.error('Payment ' + req.params.stripe_payment_id + ' did not succeed.');
        return res.redirect('/demo');
      }

      var code = 'a1b2c3';
      // TODO: Call the code-generating API to get a real access code.
      return res.render('demo-code', {
        code: code
      });
    });

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

  app.post('/alpha', function(req, res, next) {

    console.log('Alpha signup: ' + req.body.email);
    // Save the alpha request to the database.
    db.query('INSERT INTO alpha_signup ( email ) values (?)', [req.body.email], function(err, info) {
      if (err) {
        console.error(err);
        res.sendStatus(500);
        return;
      }
      res.sendStatus(200);
    });
//    var transporter = nodemailer.createTransport({
//      service: 'gmail',
//      auth: {
//        user: email.user,
//        pass: email.pass
//      }
//    });
//    var mailOptions = {
//      from: 'SAIL Website <genetank.info@gmail.com>',
//      to: ['anne@secureailabs.com', 'lahuang4@gmail.com'],
//      subject: 'SAIL Alpha Signup',
//      text: 'Alpha signup received for email address:\n' + req.body.email
//    };
//    transporter.sendMail(mailOptions, function(err, info) {
//      if (err) {
//        console.error(err);
//        res.sendStatus(500);
//        return;
//      }
//      console.log('Message sent: %s', info.messageId);
//      res.sendStatus(200);
//    });

  });

  app.get('/view-alpha-signups', function(req, res, next) {

    // Check password.
    if (req.query.password !== 'alpha') {
      res.sendStatus(404);
      return;
    }
    db.query('SELECT * FROM alpha_signup', [], function(err, rows) {
      if (err) {
        console.error(err);
        res.sendStatus(500);
        return;
      }
      res.send(rows);
    });

  });

};
