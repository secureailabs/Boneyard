import { Meteor } from 'meteor/meteor'

export function uploadFile(file,subDir,prefix){
   if (undefined === file || "" === file)
     return
   var reader = new FileReader();
   //const filename=file.name;
   reader.onload = (fileLoadEvent)=>{
      Meteor.call('userFileUpload', file.name, subDir, prefix, reader.result, (err, ret)=>{
        if (err)
          alert("upload file fail.");
        else
          console.log("File:"+file.name+" upload successed.")
      });
   };
   reader.readAsBinaryString(file);
}


export function uploadFileWithHash(file,subDir,prefix){
    if (undefined === file || "" === file)
        return
    var reader = new FileReader();
    reader.readAsBinaryString(file);

    reader.onload = (fileLoadEvent) => {
        Meteor.call('userFileUploadWithHash', file.name, subDir, prefix, reader.result, (error, result)=>{
            if (error)
                alert("upload file fail.");
            else {
                console.log("File:" + file.name + " upload successed."+ result.hash)
                return result.hash;
            }

        });
    };

}