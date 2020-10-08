var bodyParser = require('body-parser');
var cookieParser = require('cookie-parser');
var cors = require('cors');
var express = require('express');
var exphbs = require('express-handlebars');
var favicon = require('serve-favicon');
var flash = require('connect-flash');
var mongoose = require('mongoose');
var logger = require('morgan');
var path = require('path');
var session = require('express-session');

var app = express();

// For running behind a proxy
app.set('trust proxy', true);

// Database setup
try {
  var db = require('./config/db');
  mongoose.connect(process.env.MONGODB_URI || db.url);
} catch (err) {
  mongoose.connect(process.env.MONGODB_URI);
}
var connection = mongoose.connection;
connection.on('error', console.error.bind(console, 'connection error:'));
connection.on('connected', function () {
  console.log('Database connected!');
});

// View engine setup
app.set('views', path.join(__dirname, 'views'));
app.engine('.hbs', exphbs({ extname: '.hbs' }));
app.set('view engine', '.hbs');

// Set up favicon, logging, parsing, static files
app.use(favicon(path.join(__dirname, 'public', 'favicon.ico')));
app.use(logger('dev'));
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: false }));
app.use(cors());
app.options('*', cors());
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));

app.use(session({
  secret: 'projectsecret',
  resave: false,
  saveUninitialized: false
}));
app.use(flash());

// Set up routes
require('./routes/index.js')(app);
require('./routes/entry.js')(app);
require('./routes/file.js')(app);

// Catch 404 and forward to error handler
app.use(function (req, res, next) {
  var err = new Error('Not Found');
  err.status = 404;
  next(err);
});

// Error handlers

// Development error handler
// Will print stacktrace
if (app.get('env') === 'development') {
  app.use(function (err, req, res) {
    res.status(err.status || 500);
    res.render('error', {
      message: err.message,
      error: err
    });
  });
}

// Production error handler
// No stacktraces leaked to user
app.use(function (err, req, res) {
  res.status(err.status || 500);
  res.render('error', {
    message: err.message,
    error: {}
  });
});

module.exports = app;