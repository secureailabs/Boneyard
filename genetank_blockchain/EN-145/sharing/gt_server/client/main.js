import { Meteor } from 'meteor/meteor';
import { render } from 'react-dom';

import { Template } from 'meteor/templating';
import { ReactiveVar } from 'meteor/reactive-var';
import '/imports/startup/both/usracntfr.jsx';

import './main.html';

import '/imports/startup/client/routes.jsx';

//BlazeLayout.setRoot('body');

FlowRouter.go('/');

//Meteor.startup(() => {
//  render(renderRoutes(), document.getElementById('app'));
//});

