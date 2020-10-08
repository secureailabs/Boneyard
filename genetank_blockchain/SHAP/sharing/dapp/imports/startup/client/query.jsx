// File: query.jsx Author: Larry Tseng and Shifa Zhang Version: 2018.03.16
import React from 'react';
import { HTTP } from 'meteor/http';
import { modelRepo } from '/imports/startup/both/filescollection.js';
import { contract, contract_info } from '/imports/startup/client/contract.js';
import ReactJson from 'react-json-view';

var enclaveList;

// 1. Find an AI Model Enclave
function GetEnclaves() {

  // Grab the collection of available enclaves, in JSON format. Minimize FilesCollection calls
  if (enclaveList == null) {
    enclaveList = modelRepo.runnerJson.find();
  }

  // modelRepo.runnerJson.remove();   // Removes files

  return enclaveList;

}

function ListEnclaves(props) {
  if (props.enclaveJsons.length === 0)
    return (
      <tr> 
        <td>{0}</td>
        <td>{"No enclave available."}</td>
        <td>.</td>
      </tr>);
  const tr_list = props.enclaveJsons.map((e,idx) => 
      <tr key={e.MRENCLAVE+e.name}>
        <td>{idx+1}</td>
        <td>
          <a href="" title={e.name}
            onClick={(x) => {props.onClickEnclave(idx);}}>
            {e.name}</a>
        </td>
        <td>
          <a href="" title={e.other_info}
            onClick={(x) => {props.onClickDetail(idx);}}>
            {e.other_info}</a>
        </td>
      </tr>
  );
  return tr_list;
}

function loadEnclaves(callback) {
  const files = modelRepo.runnerJson.find();
  const cursor_array = files.each();
  for(var i=0;i<cursor_array.length;i++) {
    const cursor = cursor_array[i];
    var link = cursor.link();
    //link = link.substr(link.search("/cdn"),link.length);
    HTTP.get(link,
     (error, result) => {
       if (error) {
         alert("Error retrieving enclave data.");
       } else 
         callback(JSON.parse(result.content));
     }
    );
  }
}

function downloadEnclaveData(enclave, callback) {

  var enclaveURL = enclave._downloadRoute+'/'+enclave._collectionName+'/'+enclave._id+'/original/'+enclave._id+'?download=true';

  HTTP.get(enclaveURL,
    (error, result) => {
      if (!error) {
        callback(JSON.parse(result.content));
      } else {
        console.error("Error retrieving enclave data");
      }
    }
  );

}

function sleep(milliseconds) {
  var start = new Date().getTime();
  for (var i = 0; i < 1e7; i++) {
    if ((new Date().getTime() - start) > milliseconds){
      break;
    }
  }
}

function DisplayEnclaveAuth(props) {
  if (props.retrieving)
    return (
        <div>
          <p>Authenticating enclave and applying access code ...</p>
          <img src="/blockchainintelsgx.gif"
            height={240} width={480}/>
        </div>
      );
  else
    return (<div> </div>);
}

function DisplayEnclaveJson(props) {
  if (props.json_idx === -1)
    return null;
  return (
     <div>
     <p>Detail information of {props.enclaves[props.json_idx].name}:</p>
     <ReactJson src = {props.enclaves[props.json_idx]} collapseStringsAfterLength={40} />
     </div>
  );
}

class DisplayEnclaves extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      enclave_json: [],
      enclave_idx:0,
      json_idx:-1,
      retrieving_code:false,
      uploading:false
    };

    this.handleClick = this.handleClick.bind(this);
    this.handleClickDetail = this.handleClickDetail.bind(this);

    // TODO: REMOVE
    // Only for testing function
      storeDataToFS("hi");
  }

  add_state_json = (json) => {
    let temp_json = this.state.enclave_json;
    json.access_code = null;
    json.paid = false;
    json.prediction = null;
    temp_json.push(json);
    this.setState({enclave_json:temp_json});
  }

  componentDidMount(){
    loadEnclaves(this.add_state_json);
  }

  timeout_setstate = (idx,code) =>{
    var temp_json = this.state.enclave_json;
    temp_json[idx].access_code = code;
    temp_json[idx].paid = false;
    this.setState({
      retrieving_code:false,
      //paid: false;
      enclave_idx: idx,
      //access_code: code
      enclave_json: temp_json
    });
  }

  handleClickDetail(idx) {
    if (idx === this.state.json_idx)
      idx = -1;
    this.setState({json_idx:idx});
  }

  handleClick(idx) {
    if (this.state.uploading) {
      alert("Getting prediction, please wait for a while to start another query.");
      return;
    }
    this.setState({retrieving_code:true, access_code:null});
    let setstate = this.timeout_setstate;
    retrieveEnclaveAccessCode(this.state.enclave_json[idx], (error,code) => {
      var tmp_idx, tmp_code;
      if(error){
        tmp_idx = this.state.enclave_idx;
        tmp_code = this.state.access_code;
      }
      else{
        tmp_idx = idx;
        tmp_code = code.access_code;
      }
      //sleep(10000);
      setTimeout(function(){
        setstate(tmp_idx,tmp_code);
      }, 5000);
    });
  }

  setStatePrediction = (error,response) => {
    var result;
    if (error)
      result="Error, no result.";
    else
      result = JSON.parse(response.content).prediction;

    const enclave=this.state.enclave_json[this.state.enclave_idx];
    enclave.prediction = result;
    this.setState({
      //prediction: result,
      uploading:false
    });
  }

  handleGetPrediction = (file) => {
    const enclave=this.state.enclave_json[this.state.enclave_idx];
    if (enclave.prediction !== null) {
      alert("Prediction is ready. Don't repeat geting prediction.")
      return;
    }
      
    if (!enclave.paid) {
      alert("You must pay the query fee deposit first.")
      return;
    }

    if (this.state.uploading) {
      alert("It is working on the prediction. Don't repeat geting prediction.")
      return;
    }
      
    this.setState({uploading:true});

    var reader = new FileReader();

    var access_code = enclave.access_code;
    var model_id = enclave.model;
    var prediction = this.setStatePrediction;

    reader.onload = function(e) {
      //console.log(reader.result);
      uploadUserDNAData(enclave, access_code, reader.result, model_id, prediction);
    }
    reader.readAsText(file);
  }

  render() {
    return (
      <div>
        <fieldset>
          <legend>Select an AI Model enclave to query.</legend>
          <table>
            <thead>
              <tr>
                <th>No.</th>
                <th>Name (Click to start query)</th>
                <th>Description (Click to see details)</th>
              </tr>
            </thead>
            <tbody>
              <ListEnclaves enclaveJsons={this.state.enclave_json}
                 onClickEnclave={this.handleClick}
                 onClickDetail = {this.handleClickDetail} />
            </tbody>
          </table>
          <DisplayEnclaveAuth retrieving={this.state.retrieving_code} />
        </fieldset>
        <br />
        <DisplayAccessCode
           retrieving={this.state.retreiving_code}
           onGetPrediction = {this.handleGetPrediction}
           uploading = {this.state.uploading}
           enclave={this.state.enclave_json[this.state.enclave_idx]}
        />
        <DisplayEnclaveJson enclaves = {this.state.enclave_json}
             json_idx = {this.state.json_idx} />
      </div>
    );
  }
}

function retrieveEnclaveAccessCode(enclave, callback) {

  // TODO: Validate enclave

  const account = web3.eth.defaultAccount;

  const headers = new Headers({
    "Content-Type": "application/json"
  })

  const requestData = {
    "command": "access_code",
    "parameters": {
      "user_address": account,
      "access_code": "",
      "data_hash": "",
      "data": "",
      "model_id":""
    }
  }

  console.log(enclave.URL_port);

  HTTP.post(enclave.URL_port,
    { data: requestData },
    (error, result) => {
      if (!error) {
        console.log(result.content);
        callback(error, JSON.parse(result.content));
      } else {
        alert("Error return from the enclave. Request access code fail.");
        console.error(error);
        callback(error, JSON.parse("{}"));
        // Display something to user!
      }
    }
  );

}

// TODO: remove console.log statements

function payForAccessCode(enclave, code, callback) {

  console.log(enclave.enclave_creator);
  console.log(enclave.runner);
  console.log(enclave.MRENCLAVE);
  console.log(String(code));

  const getData = contract.sendDepositAccessCode.getData(
    enclave.enclave_creator,
    enclave.runner,
    "true",
    enclave.MRENCLAVE,
    String(code)
  );

  console.log(getData);

  const account = web3.eth.defaultAccount;

  // callback?
  web3.eth.sendTransaction(
    {
      from: account,
      to: contract_info.address,
      gas: 100000000,
      data: getData
    }, (error, result) => {
      if (!error) {
        console.log("Successful access code deposit");
        console.log(result);
        callback(true);
      } else {
        console.error("Error paying deposit" + error);
      }
    }
  );
}

function DisplayUpload(props) {
  if (props.paid !== undefined) {
    if(props.paid) {
      if (false)//(props.uploading || props.prediction !== null)
        return null;
      else
        return(
          <form onSubmit={props.handleSubmit}>
            <div className="form-group">
              <label htmlFor="user_dna">Upload a DNA file:</label>
              <input type="file" id="user_dna" onChange={props.handleUserDNAChange} accept='.json' />
            </div>
            <div className="form-group">
              <input type="submit" value="Get prediction for DNA file" />
            </div>
          </form>
        );
    }
  }
  return (<label>Please pay for the access code to continue.</label>);
}

function DispalyCode(props) {
  if (props.paid)
    return <label>{props.code}</label>
  return (
    <a href='' onClick={(x) => props.onClick(x)}>
       {props.code}
    </a>
  )
}

// 2. Applied access codes. (Choose one to pay query fee deposit)
class DisplayAccessCode extends React.Component {
  constructor(props) {
    super(props);
    const enclave = this.props.enclave;
    var paid;
    if (enclave === undefined)
      paid = false;
    else
      paid = enclave.paid;
    this.state = {
      paid: paid,
      //access_code: this.props.access_code,
      dna_file: null,
      //prediction: null,
      //uploading:false
    };

    this.handleSubmit = this.handleSubmit.bind(this);
    this.handleClick = this.handleClick.bind(this);
    this.handleUserDNAChange = this.handleUserDNAChange.bind(this);

  }

  handleClick() {
    if (this.props.enclave.paid) {
      alert("The access code has been paid.")
      return;
    }
    payForAccessCode(this.props.enclave,
      this.props.enclave.access_code, (paid) => {
        if (paid) {
          this.props.enclave.paid=true;
          this.props.enclave.prediction=null;
          this.setState({
            //prediction: null,
            paid: true
          });
        //this.props.onPaid(this.props.idx);
        } else {
          alert("Access code not yet paid for! Error with payment.");
        }
    });
  }

  handleUserDNAChange(event) {
    this.setState({
      dna_file: event.target.files[0]
    });
  }

  handleSubmit(event) {
    event.preventDefault();

    var file = this.state.dna_file;
    if (file === null) {
      alert("Please choose a genetic data file.")
      return;
    }
    this.setState({paid:true});
    this.props.onGetPrediction(file);
  }

  render() {

    if (this.props.enclave==undefined || this.props.retrieving)
      return null;

    let props_enclave = this.props.enclave;
    if (props_enclave.access_code === null)
      return null;

    let props_access_code = this.props.enclave.access_code;

    return (
      <div>
        <fieldset>
          <legend>Apply an access code to the selected enclave.</legend>
          <p title={this.props.enclave.name}>Selected enclave: <b>{this.props.enclave.name}</b></p>
          <label>Enclave access code ({(this.props.enclave.paid)? "paid":"Unpaid, click to pay"}): </label>
          <DispalyCode code = {props_enclave.access_code}
             onClick={this.handleClick}
             paid = {this.props.enclave.paid}
          />
          <p />
          <DisplayUpload paid={this.props.enclave.paid}
             handleUserDNAChange = {this.handleUserDNAChange}
             prediction = {this.props.enclave.prediction}
             uploadling = {this.props.uploading}
             handleSubmit = {this.handleSubmit}/>
          <QueryResults uploading={this.props.uploading}
            enclave={this.props.enclave}
            access_code={this.props.enclave.access_code}
            paid ={this.props.enclave.paid}
            prediction={this.props.enclave.prediction}/>
        </fieldset>
      </div>
    );
  }
}

// Need callback?
function uploadUserDNAData(enclave, access_code, dna_file, model_id, callback) {

  // File Hash
  var data_hash = web3.sha3(dna_file);
  let data_encoded = Base64.encode(dna_file);

  // TODO: File Verification

  // File Storage and Encrypt
  storeDataToFS(dna_file);

  // Server call
  const account = web3.eth.defaultAccount;

  const headers = new Headers({
    "Content-Type": "application/json"
  });

  const requestData = {
    "command": "get_prediction",
    "parameters": {
      "user_address": account,
      "access_code": access_code,
      "data_hash": data_hash,
      "data": data_encoded,
      "model_id": model_id
    }
  };

  HTTP.post(enclave.URL_port,
    { data: requestData },
    (error, result) => {
      if (error) {
        alert("Error in requesting prediction from the enclave.");
      }
      callback(error,result);
    }
  );

}

// A list of query results
class QueryResults extends React.Component {
  constructor(props) {
    super(props);
  }

  // Function: Query stuff, show spinning circle or similar?

  render() {

    if (this.props.uploading) {
      return (
        <div>
          <p>Encrypting DNA data... Uploading DNA file... Analyzing data...</p>
          <img src="/gt_uploadingyourDNAanimation-3.gif"
            height={240} width={480}/>
        </div>
      )
    }

    if (!this.props.enclave) {
      return null;
    }

    if (!this.props.prediction || !this.props.paid) {
      return null;
    }

    return (
      <div>
        <fieldset>
          <legend>Query Report</legend>
          <table>
            <thead>
              <tr>
                <th>Access Code</th>
                <th>Enclave</th>
                <th>Description</th>
              </tr>
            </thead>
            <tbody>
              <tr>
                <td>{this.props.access_code}</td>
                <td title={this.props.enclave.name}>{this.props.enclave.name}</td>
                <td>{this.props.prediction}</td>
              </tr>
            </tbody>
          </table>
        </fieldset>
      </div>
    );
  }
}

function storeDataToFS(data) {

  // TODO: Make it work with any file system [pathToDapp + /.meteor...]
    const path = "/Users/Larry/git/genetank_blockchain/sharing/dapp/.meteor/local/user_data/hi.txt";

    Meteor.call("save_data", path, data);

    // Wait
    sleep(1000);

    let read = Meteor.call("read_data", path);
    console.log(read);

    // Meteor.call("read_data", path, function (error, result) {
    //     console.log("returned data is: " + result);
    // });

}

export const Query = () => (
  <div>
    <h2 className="page-header">Query Operations</h2>
    <DisplayEnclaves />
  </div>
);
