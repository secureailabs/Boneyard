import { Mongo } from 'meteor/mongo';

/** Get a collection for the list of user details. */
const MeteorUsers = Meteor.users;

/** Make the above available to others. */
export { MeteorUsers };