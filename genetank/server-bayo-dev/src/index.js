const debug = require('debug');
const fs = require('fs');
const https = require('https');
const path = require('path');

let app;
let defaultPort;
if (process.argv.length >= 3 && process.argv[2] === 'analysis') {
  console.log('Running analysis app');
  app = require('./AnalysisApp').default;
  defaultPort = 5000;
} else {
  console.log('Running api app');
  app = require('./ApiApp').default;
  defaultPort = 3000;
}

debug('genetank:server');

const port = normalizePort(process.env.PORT || defaultPort);
app.set('port', port);

const sslOptions = {
  cert: fs.readFileSync(path.join(__dirname, 'config/cert.pem')),
  key: fs.readFileSync(path.join(__dirname, 'config/key.pem'))
};
const server = https.createServer(sslOptions, app);
server.listen(port);
server.on('error', onError);
server.on('listening', onListening);

// Normalize a port into a number, string, or false
function normalizePort(val) {
  let parsedPort = (typeof val === 'string') ? parseInt(val, 10) : val;
  if (isNaN(parsedPort)) {
    // Named pipe
    return val;
  } else if (parsedPort >= 0) {
    // Port number
    return parsedPort;
  }
  return false;
}

// Event listener for HTTP server "error" event
function onError(error) {
  if (error.syscall !== 'listen') {
    throw error;
  }
  let bind = (typeof port === 'string') ? 'Pipe ' + port : 'Port ' + port;
  switch (error.code) {
    case 'EACCES':
      console.error(`${bind} requires elevated privileges`);
      process.exit(1);
      break;
    case 'EADDRINUSE':
      console.error(`${bind} is already in use`);
      process.exit(1);
      break;
    default:
      throw error;
  }
}

// Event listener for HTTP server "listening" event
function onListening() {
  let addr = server.address();
  let bind = (typeof addr === 'string') ? `pipe ${addr}` : `port ${addr.port}`;
  debug(`Listening on ${bind}`);
}
