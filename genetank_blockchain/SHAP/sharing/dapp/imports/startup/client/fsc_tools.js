//file: fsc_tools.js Author:sz Version:0.1
import React from 'react'
import { EJSON } from 'meteor/ejson'
import { HTTP } from 'meteor/http'

import { AccountForm, registerPackage } from "./blockchnop.jsx"
import { modelRepo, Images } from '/imports/startup/both/filescollection.js';

export function downloadJsonFile(file_cursor, callback) {
  if (file_cursor === undefined){
    console.error("undefined file cursor");
    return;
  }
  var downloadlink = file_cursor.link();// file_cursor._downloadRoute+'/'+file_cursor._collectionName+'/'+file_cursor._id+'/original/'+file_cursor._id+'?download=true';

  HTTP.call('GET', downloadlink,
    (error, result) => {
      if (!error) {
        callback(JSON.parse(result.content));
      } else {
        console.error("Error retrieving enclave data");
      }
    }
  );
}

export function uploadStr(fsc_id, filename, str, callback){
  const fsc = modelRepo[fsc_id];
  if (fsc.findOne({name:filename}) !== undefined) {
    alert("File "+filename+" exist. Please choose another file.");
  } else {
    Meteor.call ('fileUpload',fsc_id, str, filename,( error, response ) => {
      callback(error, response);
      });
  }
}

export function uploadFile(fsc_id, filename, file, callback) {
  const fsc = modelRepo[fsc_id];
  if (file) {
    if (fsc.findOne({name:filename}) !== undefined) {
      alert("File "+filename+" exist. Please choose another file.");
    } else {
      var uploadInstance = fsc.insert({
            file: file,
            fileName: filename,
            streams: 'dynamic',
            chunkSize: 'dynamic'
      }, false);

      uploadInstance.on('start', function() {
        //set start status
      });

      uploadInstance.on('end', function(error, fileObj) {
        callback(error, EJSON.stringify(fileObj));
      });

      uploadInstance.start();
    } //else
  }
}

export function ShowCollectionFiles(props) {
  //if (props.showCollection)
  //{
    var count=0;
    var files;
    if (props.fileCollection !== undefined) {
      fileCollection = modelRepo[props.fileCollection];
      files = fileCollection.find();
      count = files.count();
    }
    if (count){
      const listFiles = files.map((file) =>
        <li key={file.name}> <a href={file._downloadRoute+"/"+file._collectionName+"/"+file._id+"/original/"+file._id+"?download=true"} download={file.name} target="_parent"> {file.name}</a> </li>
      );
      return (
        <div>
          <label>Files on the model repository of "{props.fileCollection}":</label>
          <ul>
            {listFiles}
          </ul>
        </div>
      );
    } else {
      return (
        <div>No file uploaded in "{props.fileCollection}" yet</div>
      );
    }
  //}({props.showCollection})</div>
}

