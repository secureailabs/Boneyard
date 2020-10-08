import { Meteor } from 'meteor/meteor';
import React from 'react';
import { render } from 'react-dom';
import { renderRoutes } from './routes';

Meteor.startup(function() {
  render(renderRoutes(), document.getElementById('app'));
});
