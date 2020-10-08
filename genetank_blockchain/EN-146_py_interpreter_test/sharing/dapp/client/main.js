import React from 'react';
import { Meteor } from 'meteor/meteor';
import { render } from 'react-dom';
import { GtDapp } from '/imports/ui/App.jsx'

Meteor.startup(() => {
  render(<GtDapp />, document.getElementById('app'));
});
