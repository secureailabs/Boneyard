//file runner.jsx Author:sz Version 0.1
import React from 'react'
import { EJSON } from 'meteor/ejson'
import { HTTP } from 'meteor/http'

import { uploadStr, uploadFile, downloadJsonFile, ShowCollectionFiles } from './fsc_tools.js';

import { AccountForm } from "./blockchnop.jsx"
import { modelRepo } from '/imports/startup/both/filescollection.js';
import { contract, contract_info } from '/imports/startup/client/contract.js';

class ShowRegEnclaves extends React.Component {
  constructor(props) {
    super(props);
    const account = web3.eth.defaultAccount;
    this.state = {value: account};

    this.handleSubmit = this.handleSubmit.bind(this);
  }

  handleSubmit(event) {
    const account = web3.eth.defaultAccount;
    this.setState({value: account});
    event.preventDefault();
  }

  render() {
    account = this.state.value;
    var enclave;
    if (account == undefined) {
      enclave = ["No enclave registered",1,0,0];
    } else {
      enclave = contract.getRegEnclVer.call(account,1);
    }
    return(
      <div>
      <form onSubmit={this.handleSubmit}>
        <label>3. My registered enclaves</label>
        <input type="submit" value="check" />
      </form>
      <ul>
        <li>{enclave[0]}</li>
      </ul>
      </div>
    );
  }
}

function ListEnclave(props) {
  return <li> <a href="?download=true" download={props.name}>{props.name}</a> </li>;
}

function ShowEnclaves(props) {
  const enclaves = modelRepo.packageZip.find();
  if (enclaves.count) {
    const listEnclaves = enclaves.map((file) =>
      //<li> <a href="{file.link}?download=true" download="{file.name}">{file.name}</a> </li>
      <ListEnclave key={file.name} name={file.name} />
    );
    return (
      <div>
      <label>Enclaves Uploaded:</label>
      <ul>
        {listEnclaves}
      </ul>
      </div>
    );
  } else {
    return (
      <div>No enclaves uploaded, yet</div>
    );
  }
}

class RegEnclForm extends React.Component {
  constructor(props) {
    super(props);
    this.state = {value: ''};

    this.handleChange = this.handleChange.bind(this);
    this.handleSubmit = this.handleSubmit.bind(this);
  }

  handleChange(event) {
    this.setState({value: event.target.value});
  }

  handleSubmit(event) {
    const txObject = {
      from: "0x12"
    }
    const hash_package = web3.sha3(this.state.value);
    const ret = contract.RegEnclVer.call(this.state.value,1,0,hash_package);
    if (ret>0) {
      alert('Registering AI Model Enclave '+this.state.value+' failed');
    } else {
      alert('Registering AI Model Enclave '+this.state.value+' Succeeded');
    }
    event.preventDefault();
  }

  render() {
    const enclaves = modelRepo.packageZip.find();
    const optEnclaves = enclaves.map((file) =>
      <option value={file.name} key={file.name}>{file.name} </option>
    );
    return (
      <form onSubmit={this.handleSubmit}>
        <label>
          Pick an eclave for registering:
          <select value={this.state.value} onChange={this.handleChange}>
            {optEnclaves}
          </select>
        </label>
        <input type="submit" value="Register" />
      </form>
    );
  }
}

function regRunner(rJson) {
    const getData = contract.registerEnclaveByRunner.getData(
             rJson.enclave_creator,
             rJson.MRENCLAVE,
             rJson.att_report_hash,
             rJson.att_sign_hash);
    const sender = web3.eth.defaultAccount;
    web3.eth.sendTransaction({from:sender, to:contract_info.address,
       data:getData,gas:100000000},(err, res) => {
         if (err) {
           alert('Registering AI Model Enclave failed. Transaction:'+res);
        } else {
          alert('Registering AI Model Enclave Succeeded. Transaction:'+res);
        }
      });
}

class UploadRunner extends React.Component {
  constructor(props) {
    super(props);
    this.state = {values: {"fileJson":null,"fileSrc":null,"fileBin":null }};

    this.handleJsonChange = this.handleJsonChange.bind(this);
    this.handleSubmit = this.handleSubmit.bind(this);
  }

  handleJsonChange(event) {
    var values = this.state.values
    values["fileJson"] = event.target.files[0];
    this.setState({values: values});
  }

  handleSubmit(event) {
    event.preventDefault();
    if (!window.FileReader)
      return;
    // generate a new FileReader object
    let reader = new FileReader();
    let files = this.state.values;
    let file = files["fileJson"];
    console.log(file);

    reader.onload = function(event) {
      let runner_str = reader.result;
      console.log(runner_json);
      let runner_json = EJSON.parse(runner_str);
      if (runner_json.id == "EnclaveRunner") {
        runner_json.att_report_hash=web3.sha3(runner_json.attestation_report);
        runner_json.att_sign_hash=web3.sha3(runner_json.att_sign);
        regRunner(runner_json);
        let filename = runner_json.name+" instance "+ runner_json.runner;
        uploadStr("runnerJson", filename+".json", runner_str,
                     function(err,resp){
                  if ( err )
                    console.log( err.reason );
                  else
                    console.log(resp);
                });
      } else {
        console.log("Bad enclave runner JSON file.");
      }
    }
    // when the file is read it triggers the onload event above.
    reader.readAsText(file, 'UTF-8');
  }

  render() {
    return(
      <div>
        <form onSubmit={this.handleSubmit}>
          <ul><li>
          <label>Choose an AI Model enclave runner JSON file :</label>
            <input type="file" id="enclave_runner_json"
              onChange={this.handleJsonChange} accept=".json" />
          </li></ul>
          <label>Click on this button to upload enclave runner:</label>
          <input type="submit" value="Upload and Register" />
        </form>
      </div>
    );
  }
}

class SelectEnclavePackage extends React.Component {
  constructor(props) {
    super(props);
    //const files = modelRepo.packageJson.find();
    //const file = files.first();
    let state_value = "";
    //if (file != undefined)
    //  state_value=file._id;
    //this.state = { value:state_value };
    if (props.default_id !== undefined)
      state_value = props.default_id;
    this.state = { value:state_value };
    this.handleSelect = this.handleSelect.bind(this);
  }

  ldFileCallback = (pkgJson) => {
      this.props.onPkgChange(this.state.value, pkgJson)
  }

  handleSelect(event) {
    this.setState({value: event.target.value});
    var file = modelRepo.packageJson.findOne({_id:event.target.value});
    downloadJsonFile(file, this.ldFileCallback);
    //function(pkgJson){
    //  this.props.onPkgChange(pkgJson)
    //});
  }

  render() {
    const files = modelRepo.packageJson.find();
    const listFiles = files.map((file) =>
      <option value={file._id} key={file._id}>{file.name}</option>
    );
    return (
      <div className="form-group">
        <label htmlFor="enclave-package">Select the package of the new enclave:</label>
        <select value={this.state.value} id="enclave-package" onChange={this.handleSelect}>
          {listFiles}
        </select>
      </div>
    );
  }
}

/*
    *"name": pkgJson.name+ " version " + pkgJson.version
    *"enclave_creator":pkgJson.enclave_creator
    *"MRENCLAVE": pkgJson.MRENCLAVE //verify with report_json.MRENCLAVE
    "runner":defaultAccount
    report=report_json.attestationResult;
    "attestation_report":report.attestation_report
    "att_report_hash":sha3
    "att_sign":report.X-IASReport-Signature
    "att_sign_hash":sha3
    "att_sign_cert":report.X-IASReport-Signing-Certificate
    "enclave_pubkey":report_json.enclave_public_key
    "model": "" //update in another command
    "URL_port": runner input
    "hardware_info": runner input
    "other_info": runner input
*/
function registerRunner(report_json, info_json) {
  let report = report_json.attestation_report;
  let runner_json={"id":"EnclaveRunner",
            "name": info_json.pkg_name,
            "enclave_creator":info_json.enclave_creator,
            "MRENCLAVE":info_json.mrenclave,
            "runner": web3.eth.defaultAccount,
            "attestation_report": report_json.attestation_report,
            "att_sign":report["X-IASReport-Signature"],
            "att_sign_cert": report["X-IASReport-Signing-Certificate"],
            "enclave_pubkey": report_json.enclave_public_key,
            "model": info_json.model_id,
            "URL_port": info_json.URL_port,
            "hardware_info": info_json.hardware_info,
            "other_info": info_json.other_info};
  runner_json.att_report_hash = web3.sha3(EJSON.stringify(report.attestation_report));
  runner_json.att_sign_hash = web3.sha3(runner_json.att_sign);
  let filename = runner_json.name+" instance "+ runner_json.runner;
  uploadStr("runnerJson", filename+".json", EJSON.stringify(runner_json),
             function(err,resp){
               if ( err )
                 console.log( err.reason );
               else
                 console.log(resp);
             });
  regRunner(runner_json);
}

class AttRegRunner extends React.Component {
  constructor(props) {
    super(props);
    let state_value = "";
    const file = modelRepo.packageJson.findOne();
    if (file != undefined)
      state_value=file.get('_id');
    this.state = {
      enclave_url:'http://108.26.187.228:4500/enclave',
      server_url:'http://localhost:4200',
      hardware_info: "Processor: Intel(R) Core(TM) i5-6200U CPU @ 2.30GHz 2.40GHz",
      other_info: "Enclave for Genetank AI Alzheimers Disease prediction",
      pkg_id:state_value,
      model_id:"0",
      pkg_name:"",
      enclave_creator:"",
      mrenclave:"",
      spid:""
    }

    this.handleInputChange = this.handleInputChange.bind(this);
    this.handlePkgChange = this.handlePkgChange.bind(this);
    this.handleSubmit = this.handleSubmit.bind(this);
  }

  ldFileCallback = (pkgJson) => {
      this.setState({
        pkg_name: pkgJson.name+ " version " + pkgJson.version,
        enclave_creator:pkgJson.enclave_creator,
        mrenclave:pkgJson.MRENCLAVE,
        spid:pkgJson.spid,
        //other_info:pkgJson.discription
      });
  }

  componentDidMount(){
    const file = modelRepo.packageJson.findOne({'_id':this.state.pkg_id});
    if (file != undefined) {
      //const state_value=file.get('_id');
      //this.setState({pkg_id:state_value});
      downloadJsonFile(file, this.ldFileCallback);
    }
  }

  handlePkgChange(file_id, pkgJson) {
    //var file = modelRepo.packageJson.findOne({_id:file_id});
    //let pkgJson=undefined;
    //downloadJsonFile(file, function(ret_pkgJson){
    //  pkgJson = ret_pkgJson;
    //});
    if (pkgJson !== undefined){
      this.setState({
        pkg_id: file_id,
        pkg_name: pkgJson.name+ " version " + pkgJson.version,
        enclave_creator:pkgJson.enclave_creator,
        mrenclave:pkgJson.MRENCLAVE,
        spid:pkgJson.spid
      });
    }
  }

  handleInputChange(event) {
    const target = event.target;
    //const value = target.type === 'checkbox' ? target.checked : target.value;
    const value = target.value;
    const name = target.name;

    this.setState({
      [name]: value
    });
  }

  httpAttCallback = (error,response)  => {
        if ( error ) {
          alert( "Attestation failed. Error: " + error );
        } else {
          console.log( response );
          report_json = EJSON.parse(response.content);
          if (this.state.mrenclave != report_json.MRENCLAVE){
            console.log("The MRENCLAVE doesn't match. Runner registrated failed.");
            //return;
          }
          var info_json = {
            "URL_port": this.state.enclave_url,
            "hardware_info": this.state.hardware_info,
            "other_info": this.state.other_info,
            "model_id": this.state.model_id,
            "pkg_file_id": this.state.pkg_id,
            "pkg_name": this.state.pkg_name,
            "enclave_creator":this.state.enclave_creator,
            "mrenclave":this.state.mrenclave,
            "spid":this.state.spid
          }
          registerRunner(report_json, info_json);
        }
  }

  handleSubmit(event) {
    HTTP.call('POST',this.state.enclave_url,
      {data: {
        "command":"attestation",
        "server_url":this.state.server_url,
        "runner_address":web3.eth.defaultAccount,
        "spid":this.state.spid
      }}, this.httpAttCallback);
    event.preventDefault();
  }

  render() {
    return(
      <div>
        <form onSubmit={this.handleSubmit}>
          <SelectEnclavePackage default_id={this.state.pkg_id}
                  onPkgChange={this.handlePkgChange}/>
          <ul className="info-list">
            <li>Enclave creator: <b>{this.state.enclave_creator}</b></li>
            <li>Enclave image hash (MRENCLAVE): <b>{this.state.mrenclave}</b></li>
            <li>Service provider (for IAS) ID: <b>{this.state.spid}</b></li>
          </ul>

          <div className="form-group">
            <label htmlFor="enclave_url">Enclave URL:</label>
            <input type="text" name="enclave_url" id="enclave_url"
                onChange={this.handleInputChange}
                value={this.state.enclave_url} />
          </div>
          <div className="form-group">
            <label htmlFor="server_url">Attestation server URL:</label>
            <input type="text" name="server_url" id="server_url"
                onChange={this.handleInputChange}
                value={this.state.server_url} />
          </div>
          <div className="form-group">
            <label htmlFor="hardware_info">Enclave hardware information:</label>
            <input type="text" name="hardware_info" id="hardware_info"
              onChange={this.handleInputChange}
              value={this.state.hardware_info} />
          </div>
          <div className="form-group">
            <label htmlFor="other_info">Other enclave registration information:</label>
            <input type="text" name="other_info" id="other_info"
              onChange={this.handleInputChange}
              value={this.state.other_info} />
          </div>
          <div className="form-group">
            <label htmlFor="model_id">AI Model ID:</label>
            <input type="number" name="model_id" id="model_id"
              onChange={this.handleInputChange}
              value={this.state.model_id} />
          </div>
          <div className="form-group">
            <input type="submit" value="Attest and Register Enclave" />
          </div>
        </form>
      </div>
    );
  }
}

//export const Runner = () => (
export class Runner extends React.Component{
  constructor(props) {
    super(props);
    this.state = { value: true };

    this.handleChange = this.handleChange.bind(this);
  }

  handleChange(newValue) {
    this.setState({value: true});
  }

  render() {
  return (
    <div>
      <h2 className="page-header">Runner Operations</h2>
      <fieldset>
        <legend>Runner information </legend>
        <AccountForm onAccountChange={this.handleChange}/>
        <label>The registered enclaves include: </label>
        <ShowCollectionFiles showCollection={this.state.value} fileCollection={"runnerJson"} />
      </fieldset>
      <p />
      <fieldset>
        <legend>Download an enclave image file and install it </legend>
        <ShowCollectionFiles showCollection={this.state.value} fileCollection={"packageJson"}/>
        <ShowCollectionFiles showCollection={this.state.value} fileCollection={"packageZip"}/>
      </fieldset>
      <p />
      <fieldset>
        <legend>Enter the information of a new enclave </legend>
        <AttRegRunner />
      </fieldset>
    </div>
  )};
}
