//file: enclavefile.js, Author: Shifa, Version:0.1
import { Meteor }          from 'meteor/meteor';
import { FilesCollection } from 'meteor/ostrio:files';

//const fs = require('fs');

export const Images = new FilesCollection({
  debug: true,
  collectionName: 'Images'
});

// To have sample image in DB we will upload it on server startup:
if (Meteor.isServer) {
  Images.denyClient();
  //Images.collection.attachSchema(Images.schema);

  Meteor.startup(function () {
    if (!Images.find().count()) {
      Images.load('https://raw.githubusercontent.com/VeliovGroup/Meteor-Files/master/logo.png', {
        fileName: 'logo.png',
        meta: {}
      });
    }
  });

  Meteor.publish('files.images.all', function () {
    return Images.find().cursor;
  });

} else {
  Meteor.subscribe('files.images.all');
}


const packageJsonFiles = new FilesCollection({
  debug: true,
  collectionName: 'PackageJsonFiles',
  allowClientCode: true, // Allow remove files from Client
  onBeforeUpload: function (file) {
    // Allow upload files under 1MB, and only in json or txt formats
    if (file.size <= 1024 * 1024 * 1 && /json/i.test(file.extension)) {
      return true;
    }
    return 'Please upload enclave package JSON file, with size equal or less than 1MB';
    //return true;
  }
});

const packageZipFiles = new FilesCollection({
  debug: true,
  collectionName: 'PackageZipFiles',
  allowClientCode: true, // Allow remove files from Client
  onBeforeUpload: function (file) {
    // Allow upload files under 10MB, and only in zip formats
    if (file.size <= 1024 * 1024 * 10 && /zip/i.test(file.extension)) {
      return true;
    }
    return 'Please upload enclave zip file, with size equal or less than 10MB';
  }
});

const modelParamFiles = new FilesCollection({
  debug: true,
  collectionName: 'ModelParamFiles',
  allowClientCode: true, // Allow remove files from Client
  onBeforeUpload: function (file) {
    // Allow upload files under 10MB, and only in zip formats
    if (file.size <= 1024 * 1024 * 10 && /zip|json/i.test(file.extension)) {
      return true;
    }
    return 'Please upload model parameter file (zip/json), with size equal or less than 10MB';
  }
});

const runnerJsonFiles = new FilesCollection({
  debug: true,
  collectionName: 'RunnerJsonFiles',
  allowClientCode: true, // Allow remove files from Client
  onBeforeUpload: function (file) {
    // Allow upload files under 1MB, and only in zip formats
    if (file.size <= 1024 * 1024 * 1 && /zip|json/i.test(file.extension)) {
      return true;
    }
    return 'Please upload runner JSON file, with size equal or less than 1MB';
  }
});
/*
if (Meteor.isServer) {
  data = "test data";
  enclaveFiles.write(data, {
      fileName: 'sample.json',
      fielId: 'abc123myId', //optional
      type: 'image/json'
  }, function (error, fileRef) {
      if (error) {
        throw error;
      } else {
        console.log(fileRef.name + ' is successfully saved to FS. _id: ' + fileRef._id);
      }
  });
}
*/

function configFsc(fsc,pub_str) {
  if (Meteor.isServer) {
    fsc.denyClient();
    //fsc.collection.attachSchema(packageJsonFiles.schema);
    Meteor.publish(pub_str, function () {
      return fsc.find().cursor;
    });
  } else {
    Meteor.subscribe(pub_str);
  }
}

configFsc(packageJsonFiles,'files.packagejson.all');
configFsc(packageZipFiles,'files.packagezip.all');
configFsc(modelParamFiles,'files.modelparam.all');
configFsc(runnerJsonFiles,'files.runner.all');

export const modelRepo = {
  "packageJson": packageJsonFiles,
  "packageZip": packageZipFiles,
  "modelParam": modelParamFiles,
  "runnerJson": runnerJsonFiles
}

if (Meteor.isServer) {
  Meteor.methods({
    fileUpload(fsc_id, data, name) {
      const fsc = modelRepo[fsc_id]; 
      var files = fsc.write(data,{ fileName:name },
      function (error, fileRef) {
        if (error) {
          console.log("write file error");
        } else {
          console.log(fileRef.name + ' is successfully saved to FS. _id: ' + fileRef._id);
        }
      });
      return (fsc_id+name);
    }
  });
  Meteor.methods({
    ias_get_sigrl() {
      Meteor.http.call('GET','https://test-as.sgx.trustedservices.intel.com'+
        ':443/attestation/sgx/v2/sigrl/E30A0000',{data:{}},
      function( error, response ) {
        if ( error ) {
          console.log( error );
        } else {
          console.log( response );
        }
      });
    }
  });
}

