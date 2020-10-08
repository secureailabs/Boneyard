import { Meteor } from 'meteor/meteor';
import { Mongo } from 'meteor/mongo';
import { Roles } from 'meteor/alanning:roles';
var find = require('find');
import {Accounts} from "meteor/accounts-base";
import {getEnclavesByHost} from "../model/enclave.js"
import { createAccount, importRawKey } from "../account/EthAccountService.js";

var fs = require('fs')

export const userCfgDb = new Mongo.Collection("usercfg");

export const homeDir = getDataHomeDir() + "/data";

const crypto = Npm.require('crypto');

function getDataHomeDir(){
  var dir = process.cwd()
  const label = ".meteor/local"
  const pos = dir.indexOf(label)
  return(dir.substring(0,pos+7))
}

// function to create the first default admin user
function createNewUser(username, email, password, role) {
    const userID = Accounts.createUser({
        username: username,
        email: email,
        password: password,
    });
    if (role === 'admin') {
        Roles.addUsersToRoles(userID, 'admin');
    }
}

export function initSysAccount() {
    const admin = Meteor.users.findOne({username:"admin"});
    if (undefined === admin || 0 === Meteor.users.find().count()) {
        if (Meteor.settings.defaultAccount) {
            console.log('Creating Default admin user(s)');
            Meteor.settings.defaultAccount.map(({username, email, password, role }) => createNewUser(username, email, password, role));
        } else {
            console.log('You Need To Have a settings.json File');
        }
    }
    else{
        console.log('Default admin User Already Exists');
    }
}

export function getUserCfg(userId){
    if (undefined === userId)
        return;
    var cfg = userCfgDb.findOne({_id:userId});
    if (cfg === undefined) {
        userCfgDb.insert({_id:userId,
            blockchainAccounts:[],
            messageAccounts:[],
            data:[],
            enclavePackages:[],
            models:[],
            enclaves:[],
            roleInfos:[],
            defaultEthAccount : ""
        });
        cfg = userCfgDb.findOne({_id:userId});
    }
    if (!validateUserRole(userId,"admin")) {
      cfg.enclaves = getEnclavesByHost(userId);
    }
    return cfg
}

export function validateUserRole(userId, roleStr){
    if (undefined === userId || null === userId)
        return false;
    const user = Meteor.users.findOne({_id:userId});
    if (undefined === user || null === user)
        return false;
    if (undefined === user.roles || null === user.roles)
        return false;
    const role = user.roles.find((element)=>{
        if (roleStr === element)
            return (element);
    });
    if (undefined === role || null === role)
        return false;
    if (undefined !== role[roleStr]) { //to be finished
        var today = new Date();
        var validTo = new Date(role.validTo);
        if (today > validTo)
            return false
    }
    return true;
}

Meteor.methods({

    async getRole (userId, roles) {
        query = {_id: userId, roles: roles};
        found = Meteor.users.findOne(query, {fields: {_id: 1}});
        return found ? true : false;
    },

    userGetUserList(){
        if (undefined === this.userId)
            return ({error:"Login first."});
        const users = Meteor.users.find()
        const result = {userList:users.map((user)=>(user.username))}
        result.userList = result.userList.filter((u)=>(u !== 'admin'))
        return result
    },

    adminGetUserList(){
        if (!validateUserRole(this.userId,"admin"))
            return ({error:"Must login as an administrator."});
        const users = Meteor.users.find()
        const result = {userList:users.map((user)=>(user.username))}
        return result
    },

    adminCreateAccount(email,username,password){
        if (!validateUserRole(this.userId,"admin"))
            return ("Must login as an administrator.");
        var uid;
        try {
            uid = Accounts.createUser({
                username: username,
                password: password,
                email: email});
        } catch (e) {
            console.log(e)
            return e;
        }
        Meteor.users.update({_id:uid}, {$push:{roles:{modelOwner:null}}});
        Meteor.users.update({_id:uid}, {$push:{roles:{dataOwner:null}}});
        return uid;
    },

    deleteUser(id) {
        if (!validateUserRole(this.userId,"admin"))
          return ("Must login as an administrator.");
        try {
            Meteor.users.remove(id)
        } catch (err) {
            //console.log(err)
            return err;
        }
    },

    sendSignUpEmail(username, email){
        if (!validateUserRole(this.userId,"admin"))
          return ("Must login as an administrator.");
        try {
            const user = {username: username, email: email};
            userId = Accounts.createUser(user);
            if (typeof userId !== "string"){
                console.log(userId.result)
                return userId.result
            }
            Accounts.emailTemplates.siteName = "SAIL: The Healthcare Data Network";
            Accounts.emailTemplates.from = "SAIL Admin <admin@secureailabs.com>";
            Accounts.emailTemplates.enrollAccount.subject = (user) => {
              return "Welcome to SAIL's Platform";
            };
            Accounts.emailTemplates.enrollAccount.text = (user, url) => {
              return "Hello " + user.username + ",\n\nTo start using the service, simply click the link below.\n\n"
                + url + "\n\nThanks.\n"
            };
            result = Accounts.sendEnrollmentEmail(userId, email);
        } catch (err) {
            return err;
        }
    },

    recoverPassword(email){
        try {
            var user = Accounts.findUserByEmail(email)
            if (user === undefined){
                return user
            }
            userId = user._id
            return Accounts.sendResetPasswordEmail(userId, email)
        } catch (err) {
            return err;
        }
    },

    registerUser(email, username, password, role) {
        try {
            userId = Accounts.createUser({
                username: username,
                email: email,
                password: password });
            if (role === 'admin') {
                Roles.addUsersToRoles(userId, 'admin');
            }
            return userId
        } catch (err) {
            return err;
        }
    },

    makeOrRemoveAdmin(userId, makeRemoveAdmin) {
        try {
            if (makeRemoveAdmin) {
                Roles.addUsersToRoles(userId, 'admin');
            }
            else{
                Roles.removeUsersFromRoles(userId, 'admin')
            }
        } catch (err) {
            return err;
        }
    },


    updateUserName(id, username) {
        try {
            result = Accounts.setUsername(id, username)
            return result
        }
         catch (err) {
            return err;
        }
    },

    updateUserEmail(id, email) {
        try {
            var emailToRemove = Meteor.users.findOne({_id: id}).emails[0].address
            result = Accounts.addEmail(id, email)
            if (result != undefined){
                return result.reason
            }
            var numOfEmails = Meteor.users.findOne({_id: id}).emails.length
            if (numOfEmails > 1){
                result = Accounts.removeEmail(id, emailToRemove)
                if (result != undefined){
                    return result.reason
                }
            }
            return result
        }
        catch (err) {
            return err;
        }
    },

    setPassword(id, password) {
        try {
            result = Accounts.setPassword(id, password)
            console.log(result)
            if (result !== undefined){
                return result.reason
            }
            return result
        }
        catch (err) {
            return err;
        }
    },

    adminAddRole(username, role, validToDate){
        if (validateUserRole(this.userId,"admin"))
            return ("Must login as an administrator.");
        const user = Meteor.users.findOne({username:username});
    },

    userAddEthAcc(ethAccName, ethPassword, username) {
        if (undefined === this.userId)
            return ("Login required.");
        var uId = this.userId
        if (undefined !== username && "" !== username) {
           if (!validateUserRole(this.userId,"admin"))
             return { error: "Only Admin can add other user's SAIL account."}
          const user = Meteor.users.findOne( { username: username })
          if (!user)
            return ({ error: "User not found."})
          uId = user._id
        }
        let cfg = userCfgDb.findOne({_id:uId});
        if (undefined === cfg){
            cfg = getUserCfg(uId);
        }

        let newAccount = createAccount(ethPassword);

        // [EN-169] temporary fix
        // let Future = Npm.require('fibers/future');
        // let etherFut = new Future();
        // Meteor.call('seedEther', newAccount, (err, res) => {
        //     console.log("ether seeded successfully");
        //     etherFut.return(null)
        // });
        // etherFut.wait();

        const hash = crypto.createHash('sha256');
        hash.update(uId);
        let key = hash.digest();

        const hashIV = crypto.createHash('md5');
        hashIV.update(newAccount);
        let IV = hashIV.digest();

        let encryptor = crypto.createCipheriv('aes256', key, IV);
        let encrPassword = encryptor.update(ethPassword, "utf8", "hex");
        encrPassword += encryptor.final("hex");

        cfg.blockchainAccounts.push({accName: ethAccName, account: newAccount, password:encrPassword});

        userCfgDb.update({_id:uId},{$push: {blockchainAccounts:
                    {accName: ethAccName,account: newAccount, password:encrPassword}}});

        if (cfg.defaultEthAccount === ""){
            userCfgDb.update({_id:uId},{$set: {defaultEthAccount: newAccount}});
        }

        cfg = getUserCfg(uId);
        return cfg;
    },

    userDeleteEthAcc(ethAccount, username) {
        if (undefined === this.userId)
            return ("Login required.")
        var uId = this.userId
        if (undefined !== username && "" !== username) {
           if (!validateUserRole(this.userId,"admin"))
             return { error: "Only Admin can delete other user's SAIL account."}
          const user = Meteor.users.findOne( { username: username })
          if (!user)
            return ({ error: "User not found."})
          uId = user._id
        }
        var cfg = userCfgDb.findOne({_id:uId})
        if (undefined === cfg){
            return ("Can't find user configuration.");
        }

        userCfgDb.update({_id:uId},{$pull: {blockchainAccounts:
                            {account:ethAccount}}});

        // new cfg after update
        cfg = userCfgDb.findOne({_id:uId})
        if (cfg.defaultEthAccount === ethAccount){
            if (cfg.blockchainAccounts.length !== 0){
                userCfgDb.update({_id:uId},{$set: {defaultEthAccount: cfg.blockchainAccounts[0].account}});
            } else{
                userCfgDb.update({_id:uId, defaultEthAccount:ethAccount},{$set: {defaultEthAccount: ""}});
            }
        }

        cfg = getUserCfg(uId);
        return cfg;

    },

    setDefaultEthAcc(ethAccount) {
        if (undefined === this.userId)
            return ("Login required.")
        var cfg = userCfgDb.findOne({_id:this.userId})
        if (undefined === cfg){
            cfg = getUserCfg(this.userId);
        }
        userCfgDb.update({_id:this.userId},{$set: {defaultEthAccount:ethAccount}});
    },

    setSelectedDatabase(selectedDatabaseInfo) {
        if (undefined === this.userId)
            return ("Login required.")
        var cfg = userCfgDb.findOne({_id:this.userId})
        if (undefined === cfg){
            cfg = getUserCfg(this.userId);
        }
        userCfgDb.update({_id:this.userId}, {$set: {selectedDatabaseInfo: selectedDatabaseInfo}})
    },

    addDatasetInfo(dsInfo) {
        if (undefined === this.userId)
            return ("Login required.")
        var cfg = userCfgDb.findOne({_id:this.userId})
        if (undefined === cfg){
            cfg = getUserCfg(this.userId);
        }
        var tempDatasetsInfo = []
        if (cfg.datasetsInfo === undefined) {
            tempDatasetsInfo.push(dsInfo)
        }else {
            tempDatasetsInfo = cfg.datasetsInfo
            tempDatasetsInfo.push(dsInfo)
        }
        console.log(tempDatasetsInfo)
        userCfgDb.update({_id:this.userId}, {$set: {datasetsInfo: tempDatasetsInfo}})
    },

    removeDatasetInfo(dsName) {
        if (undefined === this.userId)
            return ("Login required.")
        var cfg = userCfgDb.findOne({_id:this.userId})
        if (undefined === cfg){
            cfg = getUserCfg(this.userId);
            //return ("Can't find user configuration.");
        }
        if (undefined === cfg.datasetsInfo)
          return;
        const tempDatasetsInfo = cfg.datasetsInfo.filter(ds => ds.name !== dsName)
        userCfgDb.update({_id:this.userId}, {$set: {datasetsInfo: tempDatasetsInfo}})
    },

    removeProjectInfo(proj){
        if(undefined === this.userId)
            return ({error:"Login required."});
        var cfg = userCfgDb.findOne({_id:this.userId})
        if (undefined === cfg){
            cfg = getUserCfg(this.userId);
        }
        userCfgDb.update({_id:this.userId}, {$pull: {models: {name : proj}}});
    },

    userGetCfg(){
        if (undefined === this.userId)
            return (null);
        return getUserCfg(this.userId)
    },

    getUserName(){
        if (undefined === this.userId)
            return (null)
        const user = Meteor.user()
        if (undefined === user || user == null)
            return (null)
        else
            return user.username
    },

    userFileUpload(file,subDir,prefix,data) {
        if (undefined === this.userId)
            return ({error:"Login required."})
        var path = homeDir
        if (!fs.existsSync(path))
            fs.mkdirSync(path)
        path = path +"/"+this.userId
        if (!fs.existsSync(path))
          fs.mkdirSync(path)
        path = path + "/" + subDir
        if (!fs.existsSync(path))
          fs.mkdirSync(path)
        const filename = prefix+file
        fs.writeFile(path+"/"+filename, data, (err)=>{
          if (err)
            console.log("Write file:"+filename+" fail.")
        });
        return ({result:"Write file success."});
    },

    userFileUploadWithHash(file,subDir,prefix,data) {
        if (undefined === this.userId)
            return ({error:"Login required."})
        var path = homeDir
        if (!fs.existsSync(path))
            fs.mkdirSync(path)
        path = path +"/"+this.userId
        if (!fs.existsSync(path))
            fs.mkdirSync(path)
        path = path + "/" + subDir
        if (!fs.existsSync(path))
            fs.mkdirSync(path)
        const filename = prefix+file
        fs.writeFile(path+"/"+filename, data, (err)=>{
            if (err)
                console.log("Write file:"+filename+" fail.")
        });
        const hash = crypto.createHash('sha256');
        hash.update(data);
        var fileHash = hash.digest('hex');
        return ({result:"Write file success.", "hash":fileHash});
    },

    importAccByPrivateKey(ethAccName, private_key, ethPassword) {
        if (undefined === this.userId)
            return ("Login required.")
        var cfg = userCfgDb.findOne({_id:this.userId})
        if (undefined === cfg){
            cfg = getUserCfg(this.userId);
        }

        var newAccount = importRawKey(private_key, ethPassword);

        if(newAccount == null)
            return null;

        const hash = crypto.createHash('sha256');
        hash.update(this.userId);
        var key = hash.digest();

        const hashIV = crypto.createHash('md5');
        hashIV.update(newAccount);
        var IV = hashIV.digest();

        var encryptor = crypto.createCipheriv('aes256', key, IV);
        var encrPassword = encryptor.update(ethPassword, "utf8", "hex");
        encrPassword += encryptor.final("hex");

        cfg.blockchainAccounts.push({accName: ethAccName, account: newAccount, password:encrPassword})
        userCfgDb.update({_id:this.userId},{$push: {blockchainAccounts:
                    {accName: ethAccName,account: newAccount, password:encrPassword}}});


        cfg = getUserCfg(this.userId);
        return cfg;
    }
})
