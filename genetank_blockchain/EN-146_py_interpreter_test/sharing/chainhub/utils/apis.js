// Enum of generic types to augment built-in JavaScript types.
const types = {
  OBJECT_ARRAY: 'OBJECT_ARRAY',
  BOOLEAN_ARRAY: 'BOOLEAN_ARRAY',
  NUMBER_ARRAY: 'NUMBER_ARRAY',
  STRING_ARRAY: 'STRING_ARRAY'
};
const typesValues = Object.values(types);

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

// Returns a list of the specified fields not in the object.
getMissingFields = function (obj, fields) {
  const missingFields = [];
  for (var i = 0; i < fields.length; i++) {
    if (!(fields[i] in obj && obj[fields[i]] !== '')) {
      missingFields.push(fields[i]);
    }
  }
  return missingFields;
}

// Returns a comma-separated string of the specified fields not in the object.
getMissingFieldsAsString = function (obj, fields) {
  const missingFields = getMissingFields(obj, fields);
  return missingFields.join(', ');
}

// Copies the designated fields from one object to another.
copyFields = function (from, to, fields) {
  fields.forEach(function (field) {
    to[field] = from[field];
  });
}

checkArrayTypes = function (field, expectedType) {
  if (!field) {
    return true;
  }
  if (!(field instanceof Array)) {
    return false;
  }
  var typesMatch = true;
  field.forEach(function (element) {
    if (!element && !(typeof element === expectedType)) {
      typesMatch = false;
    }
  });
  return typesMatch;
}

checkTypes = function (object, fields, expectedTypes) {
  for (var i = 0; i < fields.length; i++) {
    if (!(typesValues.includes(expectedTypes[i]))) {
      // Some generic type.
      if (object[fields[i]] &&
        !(typeof object[fields[i]] === expectedTypes[i])) {
        return false;
      }
    } else {
      // A custom type.
      switch (expectedTypes[i]) {
        case types.OBJECT_ARRAY:
          if (!checkArrayTypes(object[fields[i]], 'object')) {
            return false;
          }
          break;
        case types.BOOLEAN_ARRAY:
          if (!checkArrayTypes(object[fields[i]], 'boolean')) {
            return false;
          }
          break;
        case types.NUMBER_ARRAY:
          if (!checkArrayTypes(object[fields[i]], 'number')) {
            return false;
          }
          break;
        case types.STRING_ARRAY:
          if (!checkArrayTypes(object[fields[i]], 'string')) {
            return false;
          }
          break;
      }
    }
  }
  return true;
}

// Scrubs any internal database fields from an object; returns the scrubbed
// result.
scrubDbFields = function (obj) {
  var copy = Object.assign({}, obj);
  delete copy['_id'];
  delete copy['__v'];
  return copy;
}

// Scrubs any private fields from an object; returns the scrubbed result.
scrubHiddenFields = function (obj) {
  var copy = Object.assign({}, obj);
  delete copy['password'];
  return copy;
}

exports.types = types;
exports.emptyString = emptyString;
exports.hasData = hasData;
exports.hasFields = hasFields;
exports.getMissingFields = getMissingFields;
exports.getMissingFieldsAsString = getMissingFieldsAsString;
exports.copyFields = copyFields;
exports.checkTypes = checkTypes;
exports.scrubDbFields = scrubDbFields;
exports.scrubHiddenFields = scrubHiddenFields;