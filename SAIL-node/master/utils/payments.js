var stripeKeys = require('../config/stripe');
var stripe = require('stripe')(stripeKeys.private);

// Creates a Stripe PaymentIntent object. paymentAmount is in cents.
// callback(err, paymentIntent)
function createPaymentIntent(paymentAmount, callback) {
  stripe.paymentIntents.create({
    amount: paymentAmount,
    currency: 'usd',
  }, callback);
};

// Retrieves a Stripe PaymentIntent object by client secret.
// callback(err, paymentIntent)
function getPaymentIntent(clientSecret, callback) {
  stripe.paymentIntents.retrieve(clientSecret, callback);
};

// Retrieves whether a Stripe payment succeeded by payment id.
// callback(err, success)
function paymentSucceeded(id, callback) {
  getPaymentIntent(id, function(err, paymentIntent) {
    if (err) {
      return callback(err);
    }
    return callback(null, paymentIntent.status === 'succeeded');
  });
};

exports.stripePublicKey = stripeKeys.public;
exports.createPaymentIntent = createPaymentIntent;
exports.getPaymentIntent = getPaymentIntent;
exports.paymentSucceeded = paymentSucceeded;