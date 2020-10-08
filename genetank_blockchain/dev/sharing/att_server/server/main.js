import { Meteor } from 'meteor/meteor'
import { HTTP } from 'meteor/http'

//import '../imports/modelrepo/common_routes.js'
import { att_msg } from './att_server.js'

Meteor.startup(() => {
  // code to run on server at startup
});

Meteor.methods({
	getData(){
		const msg = att_msg.find()
		const result = msg.fetch()
		//result = msg.map((cur) => return (cur.fetch())
		return result
	}
})
