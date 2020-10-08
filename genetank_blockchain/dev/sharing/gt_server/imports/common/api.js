// Helper functions for implementing HTTP endpoints.

// Extracts the data out of an HTTP request.
getData = function (request) {
  switch (request.method) {
    case 'GET':
      return request.query;
    case 'POST':
    case 'PUT':
    case 'DELETE':
      return request.body;
  }
}

// Checks whether a string is empty.
emptyString = function (string) {
  return !string || !string.length;
}

// Checks whether an object has any contents.
hasData = function (data) {
  return Object.keys(data).length > 0 ? true : false;
}

// Checks whether all the given fields exist (and are non-empty) in an object.
hasFields = function (obj, fields) {
  for (var i = 0; i < fields.length; i++) {
    if (!(fields[i] in obj && obj[fields[i]] !== '')) {
      return false;
    }
  }
  return true;
}

getMissingFields = function (obj, fields) {
  const missingFields = [];
  for (var i = 0; i < fields.length; i++) {
    if (!(fields[i] in obj && obj[fields[i]] !== '')) {
      missingFields.push(fields[i]);
    }
  }
  return missingFields;
}

getMissingFieldsAsString = function (obj, fields) {
  const missingFields = getMissingFields(obj, fields);
  return missingFields.join(', ');
}

// Copies the designated fields from one object to another.
copyFields = function (from, to, fields) {
  fields.forEach((field) => {
    to[field] = from[field];
  });
}

// Scrubs any MongoDB _id fields from an object; returns the scrubbed result.
scrubId = function (obj) {
  var copy = Object.assign({}, obj);
  delete copy['_id'];
  return copy;
}

// Creates and sends an HTTP response.
respond = function (context, status, data) {
  context.response.setHeader('Content-Type', 'application/json');
  context.response.statusCode = status;
  context.response.end(JSON.stringify(data));
}

// Responds with a 200 OK and an empty response.
ok = function (context) {
  respond(context, 200, {});
}

// TODO: These should be static objects instead of methods.
okObject = function () {
  return { status: 200 };
}

// Responds with a 404 Not Found.
notFound = function (context) {
  respond(context, 404,
    'The requested endpoint or resource could not be found.');
}

notFoundObject = function () {
  return {
    status: 404,
    message: 'The requested endpoint or resource could not be found.'
  };
}

// Responds with a 500 Internal Server Error.
internalError = function (context) {
  respond(context, 500, 'An unexpected error occurred. Please try again.');
}

internalErrorObject = function () {
  return {
    status: 500,
    message: 'An unexpected error occurred. Please try again.'
  };
}

// Responds with a 400 Invalid Argument Error and indicates that the resource
// already exists.
alreadyExistsError = function (context) {
  respond(context, 400, 'The resource already exists.');
}

alreadyExistsObject = function () {
  return {
    status: 400,
    message: 'The resource already exists.'
  };
}
