var express    = require('express');
var multer     = require('multer');
var nodemailer = require('nodemailer');
var fs         = require('fs');
var path       = require('path');

var upload = multer({ dest: 'uploads/' });

var auth      = require('../utils/auth');
var db        = require('../models/db');
var campaigns = require('../data/campaigns');
var sampleReportData = require('../data/sampleReportData');

module.exports = function(app, passport) {

  app.get('/', function(req, res, next) {

    res.render('index', { user: req.user });

  });

  app.get('/about', function(req, res, next) {

    res.render('about', { user: req.user });

  });

  app.get('/ancestry-report', function(req, res, next) {

    res.render('ancestry-report', { user: req.user });

  });

  app.get('/partner', function(req, res, next) {

    res.render('partner', { user: req.user });

  });

  app.get('/subscribe', function(req, res, next) {

    res.render('subscribe', { user: req.user });

  });

  // Add an email to our subscription list.
  // Expected inputs:
  // req.body: email
  app.post('/subscribe', function(req, res, next) {

    db.query('INSERT INTO subscribe ( email ) values (?)',
      [req.body.email],
      function(err, rows) {
        if (err) {
          return res.render('subscribe', { errorMessage: 'Sorry, something went wrong.' });
        }

        // Successfully added email
        return res.render('subscribe', { successMessage: 'Success! We\'ll be in touch.' });
    });

  });

  app.post('/contact', function(req, res, next) {
    var transporter = nodemailer.createTransport({
      service: 'gmail',
      auth: {
        user: process.env.GENETANK_CONTACT_USER,
        pass: process.env.GENETANK_CONTACT_PASS
      }
    });
    var mailOptions = {
      from: 'GeneTank <genetank.info@gmail.com>',
      to: 'genetank@mit.edu',
      subject: 'GeneTank Contact Us',
      text: req.body.message
    };
    transporter.sendMail(mailOptions, function(err, info) {
      if (err) {
        // Something went wrong
        return res.render('subscribe', { errorMessage: 'Sorry, something went wrong. Your message could not be delivered.' });
      } else {
        // Successfully sent email
        return res.render('subscribe', { successMessage: 'Thanks for your message!' });
      }
    });
  });

  app.get('/campaigns', function(req, res, next) {

    res.render('campaigns', {
      user: req.user,
      campaign: 'about',
      title: campaigns['about']['title'],
      subtitle: campaigns['about']['subtitle'],
      image: campaigns['about']['image'],
      description: campaigns['about']['description']
    });

  });

  app.get('/campaigns-myopia', function(req, res, next) {

    res.render('myopia', {
      user: req.user,
      campaign: 'myopia',
      disease: 'myopia',
      title: campaigns['myopia']['title'],
      subtitle: campaigns['myopia']['subtitle'],
      image: campaigns['myopia']['image'],
      description: campaigns['myopia']['description']
    });

  });

  // TODO: For dynamically serving campaigns in the future.
  // app.get('/campaigns-:campaign', function(req, res, next) {

  //   if (campaigns.hasOwnProperty(req.params.campaign)) {
  //     res.render('campaigns', {
  //       user: req.user,
  //       campaign: req.params.campaign,
  //       disease: req.params.campaign,
  //       title: campaigns[req.params.campaign]['title'],
  //       subtitle: campaigns[req.params.campaign]['subtitle'],
  //       image: campaigns[req.params.campaign]['image'],
  //       description: campaigns[req.params.campaign]['description']
  //     });
  //   } else {
  //     // This campaign doesn't exist
  //     res.render('error', { user: req.user, message: 'This campaign doesn\'t exist, sorry!' });
  //   }

  // });

  // Saves an uploaded DNA file.
  // Expected inputs:
  // req.body: dna (file upload), campaign (text field)
  app.post('/uploadDNA', upload.single('dna'), function(req, res, next) {

    // TODO: Add error handling in case campaign is missing

    // Rename the file to its original name and put it in the relevant campaign folder
    fs.rename(req.file.path, path.join(__dirname, '../uploads/', req.body.campaign, req.file.originalname));
    res.end();

  });

  app.get('/profile', auth.requireLogin, function(req, res, next) {

    res.render('profile', { user: req.user });

  });

  app.get('/survey', auth.requireLogin, function(req, res, next) {

    res.render('survey', { user: req.user });

  });

  // A sample report page to demo future functionality.
  // This is the more "serious" version of the report, in a list format.
  app.get('/report-sample', function(req, res, next) {

    // Shorten report data for the summary page
    var reportDataShort = {};
    for (var card in sampleReportData) {
      if (sampleReportData.hasOwnProperty(card)) {
        reportDataShort[card] = {};
        if (sampleReportData[card]['highlights']) {
          reportDataShort[card]['highlights'] = sampleReportData[card]['highlights'];
        }
        if (sampleReportData[card]['data'].length > 6) {
          reportDataShort[card]['data'] = sampleReportData[card]['data'].slice(0, 6);
        } else {
          reportDataShort[card]['data'] = sampleReportData[card]['data'];
        }
      }
    }

    res.render('reportsample', { user: req.user, data: reportDataShort });

  });

  // // A sample report page to demo future functionality.
  // // This is the more "fun" version of the report, in cards format.
  // app.get('/report-sample', function(req, res, next) {
  // // app.get('/report-sample-cards', function(req, res, next) {

  //   // Shorten report data for the summary page
  //   var reportDataShort = {};
  //   for (var card in sampleReportData) {
  //     if (sampleReportData.hasOwnProperty(card)) {
  //       reportDataShort[card] = {};
  //       if (sampleReportData[card]['data'].length > 6) {
  //         reportDataShort[card]['data'] = sampleReportData[card]['data'].slice(0, 6);
  //       } else {
  //         reportDataShort[card]['data'] = sampleReportData[card]['data'];
  //       }
  //     }
  //   }

  //   res.render('reportsamplecards', { user: req.user, data: reportDataShort });

  // });

  app.get('/report-sample-risks', function(req, res, next) {

    res.render('reportsampledetail', { user: req.user, name: 'Disease Risks', data: sampleReportData.risks });

  });

  app.get('/report-sample-diseases', function(req, res, next) {

    res.render('reportsampledetail', { user: req.user, name: 'Genetic Diseases', data: sampleReportData.diseases });

  });

  app.get('/report-sample-medication', function(req, res, next) {

    res.render('reportsampledetail', { user: req.user, name: 'Medication Information', data: sampleReportData.medication });

  });

  app.get('/report-sample-special', function(req, res, next) {

    res.render('reportsampledetail', { user: req.user, name: 'Special Traits', data: sampleReportData.special });

  });

  app.get('/report-sample-diet', function(req, res, next) {

    res.render('reportsampledetail', { user: req.user, name: 'Diet Recommendations', data: sampleReportData.diet });

  });

  app.get('/report-sample-nutrition', function(req, res, next) {

    res.render('reportsampledetail', { user: req.user, name: 'Nutrition Advice', data: sampleReportData.nutrition });

  });

  app.get('/report-sample-exercise', function(req, res, next) {

    res.render('reportsampledetail', { user: req.user, name: 'Exercise Recommendations', data: sampleReportData.exercise });

  });

  app.get('/testperson', function(req, res, next) {

    res.render('testperson', { user: req.user });

  });

  app.get('/testsurvey', function(req, res, next) {

    res.render('survey', { user: req.user });

  });

};
