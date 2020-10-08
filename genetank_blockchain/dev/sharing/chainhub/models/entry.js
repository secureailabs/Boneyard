var mongoose = require('mongoose');

var entrySchema = mongoose.Schema({
  key: { type: String, required: true },
  value: { type: String, required: true },
  keywords: { type: [String], default: [] },
  protected: { type: Boolean, default: false },
  password: String,
});
entrySchema.index( { key: "text", value: "text" } );
module.exports = mongoose.model('Entry', entrySchema);