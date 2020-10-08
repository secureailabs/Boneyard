//file:p2paccount.js Author:sz Version:0.1
import { Meteor } from 'meteor/meteor';
import { Accounts } from 'meteor/accounts-base';

//const PeerInfo = require('peer-info')
const PeerId = require('peer-id')

export function initAccount(){
  // code to run on server at startup
  Meteor.users.allow({ update: () => true });

  // Admin 1 created via settings.json file (settings.json file should not get to the git repo and should always be in .gitignore)
  // Admin 2 (Possibiilty to create admin at the start like this)

  // var email = 'javeshmonga@gmail.com'
  // var acc = Accounts.findUserByEmail(email)
  // if (acc === undefined){
  //   var userID = Accounts.createUser({
  //     email: email});
  //   Roles.addUsersToRoles(userID, 'admin');
  //   Accounts.sendEnrollmentEmail(userId, email)
  // }

};

