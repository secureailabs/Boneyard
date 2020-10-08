var express = require('express');

var auth = require('../utils/auth');

module.exports = function(app, passport) {

  app.get('/signup', auth.alreadyLoggedIn, function(req, res, next) {

    res.render('signup', { message: req.flash('signupMessage') });

  });

  app.post('/signup', passport.authenticate('local-signup', {
    successRedirect: '/profile',
    failureRedirect: '/signup',
    failureFlash: true // Allow flash messages
  }));

  app.get('/login', auth.alreadyLoggedIn, function(req, res, next) {

    res.render('login', { message: req.flash('loginMessage') });

  });

  app.post('/login', passport.authenticate('local-login', {
    successRedirect: '/profile',
    failureRedirect: '/login',
    failureFlash: true // Allow flash messages
  }));

  app.get('/logout', function(req, res, next) {

    req.logout();
    res.redirect('/');

  });

};