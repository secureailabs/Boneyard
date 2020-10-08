//file: trainer.jsx Author:sz Version:0.1
import React from 'react'
import { EJSON } from 'meteor/ejson';
import { Base64 } from 'js-base64';

import { AccountForm, registerPackage } from "./blockchnop.jsx"
import { modelRepo, Images } from '../both/filescollection.js';
import { ShowCollectionFiles, uploadStr, uploadFile, downloadJsonFile } from './fsc_tools.js';
import { contract, contract_info } from './contract.js';

class ShowRegEnclFmly extends React.Component {
  constructor(props) {
    super(props);
    const name = "No enclave package is registed by this account.";
    this.state = {value: web3.eth.defaultAccount,
                  enclave_name: name};


    this.handleAccountChange = this.handleAccountChange.bind(this);
  }

  contrCallback = (error, response) => {
    var name;
    if (error)
      name = "No enclave package is registed by this account.";
    else
      name = response;
    this.setState({enclave_name:name});
    if (this.props.onChange != undefined)
      this.props.onChange(true);
  }

  componentDidMount(){
    const account = web3.eth.defaultAccount;
    if (account !== undefined)
      contract.getEnclaveByCreator.call(account, this.contrCallback);
  }

  handleAccountChange(account) {
    this.setState({value: account});
    contract.getEnclaveByCreator.call(account, this.contrCallback);
  }

  render() {
    return(
      <div>
        <AccountForm onAccountChange={this.handleAccountChange}/>
        {this.state.value &&
          <label>The name of the registered enclave package is: <b>{this.state.enclave_name}</b> </label>
        }
      </div>
    );
  }
}

function toHexString(byteArray) {
  var s = '0x'+ Array.from(byteArray, function(byte) {
    return ('0' + (byte.charCodeAt(0)&0xFF).toString(16)).slice(-2);
  }).join('');
  //byteArray.forEach(function(byte) {
  //  s += ('0' + (byte & 0xFF).toString(16)).slice(-2); });
  return s;
}

function hashFile(file, callback) {
  if (!window.FileReader)
    callback("not supporting window file reader","");
  // generate a new FileReader object
  let reader = new FileReader();

  reader.onload = function(event) {
    let data = reader.result;
    var hexStr = toHexString(data);
    var hash = web3.sha3(hexStr,{encoding: 'hex'});
    callback(0,hash);
  }
  // when the file is read it triggers the onload event above.
  reader.readAsBinaryString(file, 'UTF-8');
}

class RegEnclavePackage extends React.Component {
  constructor(props) {
    super(props);
    this.state = {values: {"fileJson":null,"fileSrc":null,"fileBin":null }};

    this.handleJsonChange = this.handleJsonChange.bind(this);
    this.handleSrcChange = this.handleSrcChange.bind(this);
    this.handleBinChange = this.handleBinChange.bind(this);
    this.handleSubmit = this.handleSubmit.bind(this);
  }

  handleJsonChange(event) {
    var values = this.state.values
    values["fileJson"] = event.target.files[0];
    this.setState({values: values});
  }

  handleSrcChange(event) {
    var values = this.state.values
    values["fileSrc"] = event.target.files[0];
    this.setState({values: values});
  }

  handleBinChange(event) {
    var values = this.state.values
    values["fileBin"] = event.target.files[0];
    this.setState({values: values});
  }

  handle_file_err = (err, resp) => {
    if (err)
      alert(err);
    else
      console.log(resp);
  }

  fs_op_callback  = (err, resp) => {
    if (err) alert(err);
    else console.log(resp);
    if (this.props.onChange != undefined)
      this.props.onChange(true);
  }

  handleSubmit(event) {
    event.preventDefault();
    if (!window.FileReader)
      return;
    // generate a new FileReader object
    let reader = new FileReader();
    let files = this.state.values;
    let file = files["fileJson"];
    //let this_props_onChange = this.props.onChange;
    let fs_op_callback = this.fs_op_callback;
    console.log(file);

    reader.onload = function(event) {
      let package_str = reader.result;
      console.log(package_json);
      let package_json = EJSON.parse(package_str);
      package_json.enclave_creator = web3.eth.defaultAccount;
      let src_file = files["fileSrc"];
      hashFile(src_file, function(err, result) {
        if (err)
          console.log(err);
        else {
          package_json.hash_src_file=result;
          let bin_file = files["fileBin"];
          hashFile(bin_file, function(err, result) {
            if (err)
              console.log(err);
            else {
              package_json.hash_bin_file=result;
              if (package_json.id == "EnclavePackage")
              {
                package_json.hash_signature=web3.sha3(package_json.signature);
                package_json.hash_public_key=web3.sha3(package_json.public_key);
                let filename = package_json.name+" version "+ package_json.version;
                registerPackage(package_json);
                uploadStr("packageJson", filename+".json", 
                  EJSON.stringify(package_json),
                  fs_op_callback);
                uploadFile("packageZip",
                  filename+".src"+package_json.src_file_type, src_file,
                  fs_op_callback);
                uploadFile("packageZip",
                  filename+".bin"+package_json.bin_file_type, bin_file,
                  fs_op_callback);
              } else {
                console.log("Bad enclave package JSON file.");
              }
            }
          });
        }
      });
    }
    // when the file is read it triggers the onload event above.
    reader.readAsText(file, 'UTF-8');
  }

  render() {
    return(
      <div>
        <form onSubmit={this.handleSubmit}>
          <div className="form-group">
            <label htmlFor="enclave_package_json">AI Model enclave package JSON file:</label>
            <input type="file" id="enclave_package_json"
              onChange={this.handleJsonChange} accept=".json" />
          </div>
          <div className="form-group">
            <label htmlFor="enclave_package_src">AI Model enclave package source ZIP/RAR file:</label>
            <input type="file" id="enclave_package_src"
              onChange={this.handleSrcChange} accept=".zip, .rar" />
          </div>
          <div className="form-group">
            <label htmlFor="enclave_package_bin">AI Model enclave package binary ZIP/RAR file:</label>
            <input type="file" id="enclave_package_bin"
              onChange={this.handleBinChange} accept=".zip, .rar" />
          </div>
          <div className="form-group">
            <input type="submit" value="Register Enclave Package" />
          </div>
        </form>
      </div>
    );
  }
}

//export const Trainer = () => (
export class Trainer extends React.Component{
  constructor(props) {
    super(props);
    this.state = {
      value: 1,
      packageJson:"packageJson",
      packageZip:"packageZip",
      modelParam:"modelParam",
    };

    this.handleChange = this.handleChange.bind(this);
    this.handleClick = this.handleClick.bind(this);
  }

  handleChange(newValue) {
    var val = this.state.value+1;
    this.setState({value: val});
  }

  removeCallback = (error) => {
    var val = this.state.value+1;
    this.setState({value:val});
  }

  handleClick(event) {
    const repo = prompt('Enter the repository name to be clear', "packageZip");
    if (repo !== null)
      modelRepo[repo].remove({}, this.removeCallback);
  }


  render() {
  return (
    <div>
      <h2 className="page-header">Trainer Operations</h2>
      <fieldset>
        <legend>1. Your registered AI Models</legend>
        <ShowRegEnclFmly onChange={this.handleChange} />
      </fieldset>
      <p />
      <fieldset>
        <legend>2. Register an enclave for an AI Model</legend>
        <RegEnclavePackage onChange={this.handleChange}/>
      </fieldset>
      <p />
      <fieldset>
        <legend>3. Information of registered AI Model enclaves </legend>
        <ShowCollectionFiles showCollection = {this.state.value}
                     fileCollection={this.state.packageJson} />
        <ShowCollectionFiles showCollection = {this.state.value}
                     fileCollection={this.state.packageZip} />
        <ShowCollectionFiles showCollection = {this.state.value}
                     fileCollection={this.state.modelParam} />
        <br />
        <div className="form-group">
          <input type = "button" id="clear_file" value="Clear ModelRepo" onClick={this.handleClick} />
        </div>
      </fieldset>
    </div>
  )};
}
