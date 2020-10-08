import {Meteor} from 'meteor/meteor';
import React from "react";
//import namor from "namor";

export function getData(){
	const syncCall = Meteor.wrapAsync(Meteor.call)
	const ret = syncCall("getData")
	return ret
}
