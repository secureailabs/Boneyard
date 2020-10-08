// Main routes for app
module.exports = function (app) {

  app.get('/', function (req, res, next) {

    res.render('index');

  });

  app.get('/edit', function (req, res, next) {

    res.render('edit');

  });

};