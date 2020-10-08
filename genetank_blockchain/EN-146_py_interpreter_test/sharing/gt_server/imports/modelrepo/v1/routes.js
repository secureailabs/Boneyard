// Routes for the v1 ModelRepo API.

import '../enclave_family.js';
import '../enclave_package.js';

const API_PREFIX = '/api/v1';

//
// Enclave image routes
//

Router.route(API_PREFIX + '/enclaveImage/:imageHash', function () {
  switch (this.request.method) {
    case 'GET':
      return getEnclaveImage(this);
    case 'POST':
      return postEnclaveImage(this);
    default:
      return notFound(this);
  }
}, { where: 'server' });

//
// Enclave source code routes
//

Router.route(API_PREFIX + '/source/:sourceHash', function () {
  switch (this.request.method) {
    case 'GET':
      return getSource(this);
    default:
      return notFound(this);
  }
}, { where: 'server' });

//
// Enclave package routes
//

Router.route(API_PREFIX + '/enclavePackage/:imageHash', function () {
  switch (this.request.method) {
    case 'GET':
      return getEnclavePackage(this);
    default:
      return notFound(this);
  }
}, { where: 'server' });

Router.route(API_PREFIX + '/enclavePackage', function () {
  this.response.setHeader('Access-Control-Allow-Origin', '*');

  if (this.request.method === 'OPTIONS') {
    this.response.setHeader('Access-Control-Allow-Headers', 'Origin, X-Requested-With, Content-Type, Accept');
    this.response.setHeader('Access-Control-Allow-Methods', 'POST, PUT, GET, DELETE, OPTIONS');
    this.response.end('Set OPTIONS.');
  } else {
    switch (this.request.method) {
      case 'POST':
        return postEnclavePackage(this);
      default:
        return notFound(this);
    }
  }
}, { where: 'server' });

//
// Enclave family routes
//

Router.route(API_PREFIX + '/enclaveFamily/:creatorAddress', function () {
  switch (this.request.method) {
    case 'GET':
      return getEnclaveFamily(this);
    default:
      return notFound(this);
  }
}, { where: 'server' });

Router.route(API_PREFIX + '/enclaveFamily', function () {
  this.response.setHeader('Access-Control-Allow-Origin', '*');

  if (this.request.method === 'OPTIONS') {
    this.response.setHeader('Access-Control-Allow-Headers', 'Origin, X-Requested-With, Content-Type, Accept');
    this.response.setHeader('Access-Control-Allow-Methods', 'POST, PUT, GET, DELETE, OPTIONS');
    this.response.end('Set OPTIONS.');
  } else {
    switch (this.request.method) {
      case 'POST':
        return postEnclaveFamily(this);
      default:
        return notFound(this);
    }
  }
}, { where: 'server' });

//
// Audit package routes
//

// TODO

// FOR TESTING PURPOSES ONLY. THIS MUST BE REMOVED BEFORE PRODUCTIONALIZATION.
Router.route(API_PREFIX + '/clearDb', function () {
  function clearDb() {
    removeSync = Async.wrap(EnclaveImages, 'remove');
    var removeResult = removeSync({});
    console.log(removeResult);
    return removeResult;
  }
  switch (this.request.method) {
    case 'GET':
      return clearDb(this);
    default:
      return notFound(this);
  }
}, { where: 'server' });
