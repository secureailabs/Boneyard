var stripeKeys = require('../config/stripe');
var stripe = require('stripe')(stripeKeys.private);

function getPlanPrice(planName, callback) {
  stripe.plans.retrieve(planName, function(err, plan) {
    if (err) {
      callback(err);
    } else {
      callback(null, plan.amount);
    }
  });
};

function applyCoupon(price, couponCode, callback) {
  if (!couponCode.trim().length) {
    // No coupon was given
    callback(null, false, price, null);
  } else {
    // Look up coupon
    stripe.coupons.retrieve(couponCode, function(err, coupon) {
      if (err) {
        if (err.message.includes('No such coupon')) {
          // The coupon doesn't exist
          callback(null, false, price, 'Sorry, your coupon could not be applied. Please check the spelling and try again.');
        } else {
          // Something went wrong with Stripe
          callback(null, false, price, 'Sorry, something went wrong while applying your coupon. Please try again or contact us for additional help.');
        }
      } else if (!coupon) {
        // The coupon doesn't exist
        callback(null, false, price, 'Sorry, your coupon could not be applied. Please check the spelling and try again.');
      } else if (!coupon.valid) {
        // The coupon is no longer valid
        callback(null, false, price, 'Sorry, this coupon is no longer valid.');
      } else {
        if (coupon.percent_off) { // Percent off
          callback(null, true, price - (price * coupon.percent_off / 100), null);
        } else { // Dollar amount off
          callback(null, true, price - coupon.amount_off, null);
        }
      }
    });
  }
};

function getStripeCustomer(user, stripeEmail, stripeToken, callback) {
  if (user.stripeId) {
    // Retrieve the Stripe customer associated with this user
    stripe.customers.retrieve(user.stripeId, function(err, customer) {
      if (stripeToken) {
        // Change the customer's card
        stripe.customers.update(user.stripeId, {
          source: stripeToken
        }, function(err, customer) {
          callback(err, customer);
        });
      } else {
        callback(err, customer);
      }
    });
  } else {
    // First time purchasing, create a new customer
    if (stripeToken) {
      // We have a card associated with the customer
      stripe.customers.create({ email: stripeEmail, source: stripeToken }, function(err, customer) {
        user.stripeId = customer.id;
        user.save(function(err) {
          if (err) {
            callback(err);
          } else {
            callback(null, customer);
          }
        });
      });
    } else {
      // No card associated with customer
      stripe.customers.create({ email: stripeEmail }, function(err, customer) {
        user.stripeId = customer.id;
        user.save(function(err) {
          if (err) {
            callback(err);
          } else {
            callback(null, customer);
          }
        });
      });
    }
  }
};

function handleOrderPurchase(req, callback) {
  var orderName = req.body.order;
  var coupon = req.body.coupon;

// TODO: Change this to handle orders instead of plans?
  // Subscribe the user to the plan
  stripe.plans.retrieve(planName, function(err, plan) {
    if (err) {
      callback(err);
    } else {
      var email = req.body.stripeEmail ? req.body.stripeEmail : req.user.email;
      var token;
      if (req.body.card === 'new') {
        // We're adding a new card, so we want to update the customer's card info
        token = req.body.stripeToken;
      } else {
        token = null;
      }
      getStripeCustomer(req.user, email, token, function(err, customer) {
        if (err) {
          callback(err);
        } else {
          if (coupon && coupon.trim().length) {
            // Subscribe the user with a coupon
            stripe.subscriptions.create({
              customer: customer.id,
              plan: planName,
              coupon: coupon,
            }, function(err, subscription) {
              if (err) {
                callback(err);
              } else {
                callback(null, { customer: customer, subscription: subscription });
              }
            });
          } else {
            // Subscribe the user
            stripe.subscriptions.create({
              customer: customer.id,
              plan: planName,
            }, function(err, subscription) {
              if (err) {
                callback(err);
              } else {
                callback(null, { customer: customer, subscription: subscription });
              }
            });
          }
        }
      });
    }
  });
};

function handlePlanPurchase(req, callback) {
  var planName = req.body.plan;
  var coupon = req.body.coupon;

  // Subscribe the user to the plan
  stripe.plans.retrieve(planName, function(err, plan) {
    if (err) {
      callback(err);
    } else {
      var email = req.body.stripeEmail ? req.body.stripeEmail : req.user.email;
      var token;
      if (req.body.card === 'new') {
        // We're adding a new card, so we want to update the customer's card info
        token = req.body.stripeToken;
      } else {
        token = null;
      }
      getStripeCustomer(req.user, email, token, function(err, customer) {
        if (err) {
          callback(err);
        } else {
          if (coupon && coupon.trim().length) {
            // Subscribe the user with a coupon
            stripe.subscriptions.create({
              customer: customer.id,
              plan: planName,
              coupon: coupon,
            }, function(err, subscription) {
              if (err) {
                callback(err);
              } else {
                callback(null, { customer: customer, subscription: subscription });
              }
            });
          } else {
            // Subscribe the user
            stripe.subscriptions.create({
              customer: customer.id,
              plan: planName,
            }, function(err, subscription) {
              if (err) {
                callback(err);
              } else {
                callback(null, { customer: customer, subscription: subscription });
              }
            });
          }
        }
      });
    }
  });
};

exports.stripePublicKey = stripeKeys.public;
exports.getPlanPrice = getPlanPrice;
exports.applyCoupon = applyCoupon;
exports.getStripeCustomer = getStripeCustomer;
exports.handleOrderPurchase = handleOrderPurchase;
exports.handlePlanPurchase = handlePlanPurchase;
