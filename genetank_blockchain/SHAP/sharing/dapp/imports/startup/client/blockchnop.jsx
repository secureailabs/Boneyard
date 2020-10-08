//file blockchnop.jsx Author:sz Version:0.1
import React from 'react'
import { EJSON } from 'meteor/ejson'
import {contract, contract_info, setContractInfo} from './contract.js';
import ReactJson from 'react-json-view'

export function registerPackage(pkgJson){

    const sender = web3.eth.defaultAccount;

    //const ret;
    //contract.getEnclavePackage.call(sender,pkgJson.MRENCLAVE,
    //  function(error, response){
    //    if (error)
    //      alert('AI Model Enclave '+pkgJson.name+
    //        ' version:' + pkgJson.version +
    //        ' enclave hash (MRENCLAVE):'+pkgJson.MRENCLAVE+
    //        ' exist. Please change to other enclave and try again.');
    //    else
    //      ret = respose;
    //   });
    const getData = contract.registerEnclaveByTrainer.getData(
               pkgJson.MRENCLAVE,
               pkgJson.version,
               pkgJson.hash_bin_file,
               pkgJson.hash_src_file,
               pkgJson.name,
               pkgJson.hash_signature,
               pkgJson.hash_public_key);
    web3.eth.sendTransaction({from:sender, to:contract_info.address,
       data:getData,gas:100000000},(err, res) => {
         if (err) {
           alert('Registering AI Model Enclave '+pkgJson.name+' failed. Transaction:'+err);
        } else {
          alert('Registering AI Model Enclave '+pkgJson.name+' Succeeded. Transaction:'+res);
        }
      }
    );
}

export class SmartContract extends React.Component {
  constructor(props) {
    super(props);
    this.state = {value: 'No JSON file selected'};

    this.handleChange = this.handleChange.bind(this);
  }

  handleChange(event) {
    this.setState({value: event.target.value});
    if (window.FileReader) {
      // FileReader is supported.
      // generate a new FileReader object
      let reader = new FileReader();
      var file = event.target.files[0];
      //console.log(file);
   
      reader.onload = function(event) {
        let sc_str = reader.result;
        //console.log(sc_json);
        var sc_json = EJSON.parse(sc_str);
        if (sc_json.name == "GeneTankAI")
        {
          setContractInfo(sc_json);
        } else {
          console.log("Bad smart contract JSON file.");
        }
      }
      // when the file is read it triggers the onload event above.
      reader.readAsText(file, 'UTF-8');
    } else {
      console.log("FileReader is not supported.");
    }
  }

  render() {
    return (
      <form>
        <div>Current smart contract: <b>{contract_info.name}</b></div>
        <ul className="info-list">
          <li>Date: <b>{contract_info.date}</b></li>
          <li>Address: <b>{contract_info.address}</b></li>
        </ul>
        <div className="form-group">
          <label htmlFor="scjson">Select the description file (.json) of the smart contract you want to switch to:</label>
          <input type="file" id="scjson" name={this.value} accept=".json" onChange={this.handleChange}/>
        </div>
      </form>
    );
  }
}

export class AccountForm extends React.Component {
  constructor(props) {
    super(props);
    account = web3.eth.defaultAccount;
    if (account === undefined)
      alert("Default account is unavailable. Please sign into your wallet.");
    this.state = {account: web3.eth.defaultAccount};

    this.handleChange = this.handleChange.bind(this);
  }

  handleChange(event) {
    web3.eth.defaultAccount=event.target.value;
    this.setState({value: event.target.value});
    if (this.props.onAccountChange != undefined)
      this.props.onAccountChange(event.target.value);
  }

  render() {
    const accounts = web3.eth.accounts;
    const listAccounts = accounts.map((acnt) => {
         return(<option value={acnt} key={acnt.toString()}>{acnt}</option>);
    });
    return (
      <form>
        <div className="form-group">
          <label htmlFor="select-account">Current account:</label>
          <select id="select-account" value={web3.eth.defaultAccount} onChange={this.handleChange}>
            {listAccounts}
          </select>
        </div>
      </form>
    );
  }
}

function ShowBlock(props) {
  const num = props.num;
  const style = {
    whiteSpace: 'pre-line',
    color: 'black'
  };
  //return <p style={style}> {JSON.stringify(web3.eth.getBlock(num),null,2)} </p>;
  return <p>Some block data here</p>
}
    
export class BlockNumForm extends React.Component {
  constructor(props) {
    super(props);
    //const blk_num = web3.eth.blockNumber;
    this.state = {value:100000, max_block:0, block:{}};

    this.handleChange = this.handleChange.bind(this);
  }

  getBlockCallback = (error, result) => {
    var data;
    if (error)
      data = {"error":"Get block error."};
    else
      data = result;
    this.setState({block:data});
  }

  getBlockNumCallback = (error, result) => {
    var maxnum;
    if (error)
      maxnum = 0;
    else
      maxnum = result;
    var num = this.state.value;
    if (num>maxnum)
      num = maxnum;
    this.setState({value:num, max_block:maxnum});
    web3.eth.getBlock(num, this.getBlockCallback);
  }

  componentDidMount(){
    web3.eth.getBlockNumber(this.getBlockNumCallback);
  }

  handleChange(event) {
    web3.eth.getBlockNumber(this.getBlockNumCallback);
    const num = event.target.value;
    if (num<0)
      num = 0;
    this.setState({value: num});
  }

  render() {
    return (
      <div>
      <form className="blockchain-browser-input">
        <label htmlFor="block-number">
          Block number: 
          <input type="number" className="inline-input" value={this.state.value} id="block-number" onChange={this.handleChange} />
        </label>
      </form>
      <ReactJson src = {this.state.block} collapseStringsAfterLength={40} />
      </div>
    );
  }
}
