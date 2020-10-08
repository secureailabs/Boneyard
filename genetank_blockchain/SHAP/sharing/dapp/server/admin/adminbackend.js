import { Meteor } from 'meteor/meteor';
import { Roles } from 'meteor/alanning:roles';
import { MeteorUsers } from '../../imports/api/adminboard';


/** If user is admin then publish */

Meteor.publish('DetailsAdmin', function publish() {
  
  if (this.userId && Roles.userIsInRole(this.userId, 'admin')) {
   
    return MeteorUsers.find({}, {
      fields: {
          username: 1,
          emails: 1,
          verified: 1,
          roles: 1
      }
    });
  }
  return this.ready();
});
