const INVALID_REQUEST = {
  code: 400,
  message: 'Invalid request.'
};
const ALREADY_EXISTS = {
  code: 400,
  message: 'The resource already exists.'
};
const NOT_FOUND = {
  code: 404,
  message: 'The requested endpoint or resource could not be found.'
};
const INTERNAL_ERROR = {
  code: 500,
  message: 'An unexpected error occurred. Please try again.'
};

function handleError(err, res) {
  if (err) {
    console.log(err);
    if (!err.code) {
      // Scrub internal error messages.
      err = INTERNAL_ERROR;
    }
    res.status(err.code).send(err.message);
    return true;
  }
  return false;
}

function missingFields(fieldsAsString) {
  return {
    code: 400,
    message: 'Request is missing the following fields: ' + fieldsAsString
  };
}

exports.INVALID_REQUEST = INVALID_REQUEST;
exports.ALREADY_EXISTS = ALREADY_EXISTS;
exports.NOT_FOUND = NOT_FOUND;
exports.INTERNAL_ERROR = INTERNAL_ERROR;
exports.handleError = handleError;
exports.missingFields = missingFields;