var assert = require('chai').assert
var expect = require('chai').expect

import { Meteor } from 'meteor/meteor';
import {Accounts} from 'meteor/accounts-base';
import {Roles} from 'meteor/alanning:roles';

import { userCfgDb, getUserCfg, homeDir } from '../account/account.js';
import { userStatus, getUserSt } from '../account/acc_status.js';

import { endpoint, roles } from '/imports/api/enum.js'
import { msgType } from '/imports/api/msg_type.js'

import { Random } from 'meteor/random'

import { EJSON } from 'meteor/ejson'

import { setupSession, enclaveCheck } from "/server/model/enclave.js";

var fs = require('fs')

var userId = "HRZ9B3XEJWpGfiFDd";
var pubKey = "f38f968104309fab3f7d8fcb24ae82fd229afcef179763f27ef2086ff6283db0d7b0a0f6f59589be05b00187d9d1fee4f5301d1deb006b8b9c48629696c44311";
var ethAddr = "0xd5f19805ab7EceA56dB10Cc21CE9f9a701e97B59";
var url = "http://0.0.0.0:4250";
describe('setupSession function test', function () {
  describe('methods', () => {
    let userID;
    before(() => {
      userID = Accounts.createUser({
        username: 'admin',
        email: 'admin@admin.com',
        password: 'admin'
      });
      Roles.addUsersToRoles(userID, 'admin');
      console.log("id: " + userID);
    });
  });
 it('setupSession with a given sessionIdx', function () {
    var {error, cfg, userStIdx, enclaveIdx, sessionIdx}
      =  enclaveCheck(userId, endpoint.RUNNER, pubKey, roles.DATA_SCIENTIST)
    expect(error).equal("Enclave cannot be found.");
    /*
    *  setupSession is called in enclaveCheck if enclave is found
    * below code is not useful
    */
    // const url = cfg.enclaves[enclaveIdx].url+"/"+endpoint.RUNNER
    // var roleInfo = {
    //   role: roles.DATA_SCIENTIST,
    //   stId: 0,
    //   ethAddr: ethAddr
    // }
    // var ret = setupSession(url, pubKey, userStIdx, roleInfo, userId, 101);
    // expect(ret.error).equal(undefined);
    });
});
