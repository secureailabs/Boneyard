import { Meteor } from 'meteor/meteor';
import { Mongo } from 'meteor/mongo';

export var userStatus = []
/*
User status entry:
_id: user ID
enclave_session: [
  {pubKey: enclave public key,
   id: string from enclave,
   key: encryption key shared with enclave
  }
]
*/
export function getUserSt(uid){
  var idx = userStatus.findIndex(u=>u._id === uid)
  if (undefined === idx || -1 === idx)
    idx = userStatus.push({_id: uid, enclave_session:[]}) -1
  return idx;
}
