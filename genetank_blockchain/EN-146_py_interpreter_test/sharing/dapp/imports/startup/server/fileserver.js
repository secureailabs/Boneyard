//file: fileserver.js, Author: Shifa, Version:0.1
import { Meteor }          from 'meteor/meteor';
import { FilesCollection } from 'meteor/ostrio:files';
import { packageJsonFiles } from '../both/enclavefile.js';

Meteor.methods({
  fileUpload(data, name) {
    return(packageJsonFiles.write(data,{
      fileName:name
      }));
  }
})
