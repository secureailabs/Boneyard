import React, { Component } from 'react';
import {Meteor} from "meteor/meteor";
import {MasterDetail} from "./federated_utils/MasterDetail";
import {Nodes} from "./federated_utils/Nodes";
import {Redirect} from "react-router";
import {Button} from "react-bootstrap";
import { saveAs } from 'file-saver';

export default class FederatedLearning extends React.Component {

  constructor() {
    super();
    this.getCfg = this.getCfg.bind(this);
    this.getNodes = this.getNodes.bind(this);
    this.getPersonalDatasets = this.getPersonalDatasets.bind(this);
    this.getLeafDatasets = this.getLeafDatasets.bind(this);
    this.getDatasetForOwner = this.getDatasetForOwner.bind(this);
    this.getDonorAccount = this.getDonorAccount.bind(this);
    this.getModels = this.getModels.bind(this);
    this.getLeafModels = this.getLeafModels.bind(this);
    this.runFlModels = this.runFlModels.bind(this);
    this.downloadOutput = this.downloadOutput.bind(this);
    this.state = {
      nodes:[],
      master: {},
      error: "",
      isRunning: false,
      isFinished: false
  };
  }

  getNodes(enclaves, donorAddress) {
    const nodes = enclaves.map(enclave => {
      const tempNodeInfo = {}
      const infoJson = JSON.parse(enclave.infoJson);
      tempNodeInfo.status = "Not Active"
      tempNodeInfo.name = infoJson.name;
      tempNodeInfo.pubKey = enclave.pubKey;
      tempNodeInfo.model = "model.py"
      tempNodeInfo.trainer = enclave.roleInfos[1].ethAddr;
      return tempNodeInfo
    })
    return nodes
  }

  getDonorAccount = (result) => {
    const tempAllAcc = result.blockchainAccounts.map((acc) => (acc.account))
    const tempNonDonorAcc = result.enclaves[0].roleInfos.map((acc) => (acc.ethAddr))
    const donorEthAccounts = tempAllAcc.filter(( el ) => !tempNonDonorAcc.includes( el ))
    return donorEthAccounts[0]
  }

  getCfg = () => {
    Meteor.call("userGetCfg", (err, result) => {
        if (!err) {
          if (result !== null) {
            var state = this.state;
            if (undefined !== result.enclaves)
              state.enclaves = result.enclaves;
            if (state.enclaves.length > 0){
              const infoJson = JSON.parse(state.enclaves[0].infoJson);
              state.master.name = infoJson.name;
              state.master.pubKey = state.enclaves[0].pubKey;
              state.master.trainer = state.enclaves[0].roleInfos[1].ethAddr;
              state.master.model = "model.py"
              state.master.status = "Active"
              const nodesInfo = state.enclaves.filter(enclave => enclave.pubKey !== state.master.pubKey)
              state.donorAccount = this.getDonorAccount(result)
              state.nodes = this.getNodes(nodesInfo, state.donorAccount)
            }
            this.setState(state, () => {
              this.state.nodes.map(node => this.getDatasetForOwner(node.pubKey, state.donorAccount, this.state.nodes))
              this.state.nodes.map(node => this.getModels(node.pubKey, node.trainer, this.state.nodes))
            })
          }
        }
      }
    )
  };

  componentDidMount() {
    this.getCfg()
  }


  getDatasetForOwner (pubKey, ethAcc, tempNodes) {
    Meteor.call("getEnclaveInfo", pubKey, ethAcc, (err, res)=>{
        if (err)
          console.log(err.toString())
        else if (res.error)
          console.log(res.error)
        else {
          this.getPersonalDatasets(pubKey,res.result.children, tempNodes)
        }
      }
    )
  }

  getModels(pubKey, ethAcc, temp) {

    Meteor.call("getEnclaveInfo", pubKey, ethAcc, (err, res)=>{
      if (err)
        console.log(err.toString())
      else if (res.error)
        console.log(res.error)
      else {
        console.log("Result recieved")
        let models = []
        let i = 1
        this.getLeafModels(res.result.children).forEach((model) => {
          let mod = {
            "pubKey": pubKey,
            "id": i.toString(),
            "name": "Pan Genome 201903",
          }
          mod.name = model
          models.push(mod)
          i = i + 1
        })


        const tempNodes = temp

        tempNodes.forEach(n => {
          if (n.pubKey === pubKey) {
            n.models = models
          }
        })

        this.setState({
          nodes: tempNodes
        })
      }
    })
  }

  getLeafModels(nodes, result = []){
    for(var i = 0, length = nodes.length; i < length; i++){
      if(nodes[i].leaf){
        if (nodes[i].module !== "model") {
          result.push(nodes[i].module);
        }
      }else if (nodes[i].module === "model" || nodes[i].module === "AI Model Trainer" ) {
        result = this.getLeafModels(nodes[i].children, result);
      }
    }
    return result;
  }

  getPersonalDatasets(pubKey,tree, temp) {
    let personalDataset = []
    let datasetNames = this.getLeafDatasets(tree);
    var i = 1
    datasetNames.forEach((datasetName) => {
      var dataset = {
        "pubKey": pubKey,
        "id": i.toString(),
        "name": "Pan Genome 201903",
      }
      dataset.name = datasetName
      personalDataset.push(dataset)
      i = i + 1
    })

    console.log(personalDataset)
    console.log(pubKey)


    const tempNodes = temp

    tempNodes.forEach(n => {
      if (n.pubKey === pubKey) {
        n.dataSet = personalDataset
        n.status = personalDataset.length > 0 ? "Active" : "Not Active"
      }
    })

    console.log(tempNodes)
    this.setState({
        nodes: tempNodes
    })

  }

  getLeafDatasets(nodes, result = []){
    for(var i = 0, length = nodes.length; i < length; i++){
      if(nodes[i].leaf && nodes[i].module !== 'data'){
        result.push(nodes[i].module);
      }else if (nodes[i].children !== undefined) {
        result = this.getLeafDatasets(nodes[i].children, result);
      }
    }
    return result;
  }

  runFlModels = () => {
    console.log("runFlModels")
    let tempNodes = this.state.nodes
    this.setState({
      isRunning: true
    })
    Meteor.call("federated_granular", tempNodes, this.state.master,this.state.donorAccount, (err, res)=>{
            if(!err){
              this.setState({
                isRunning: false,
                isFinished: true
              })
            }
            else{
              console.log(err)
            }
      })
  }

  downloadOutput = () => {
    console.log("Downloading Model")
    Meteor.call("downloadFinalResult", this.state.master,this.state.donorAccount, (err, res)=>{
      if(!err){
        var text = res.result.replace(/\$/g,'\n')
        var blob = new Blob([text], {type: "text/plain;charset=utf-8"});
        saveAs(blob, "Result.csv")
      } else {
        console.log(err)
      }
    })
  }
  render() {

    const usrId = Meteor.userId();
    if (undefined === usrId || null === usrId) {
      return <Redirect push to='/federated/login' />
    }

    return(
      <div className='container'>
        <div className="row mt-5 mb-5">
          <div className="col-md-10 offset-md-1">
            <div className="row m-2 justify-content-between align-items-end">
              <div className="col-md-4">
                <h5>Federated Learning</h5>
              </div>
              <div className="col-md-8 text-right">
                <Button variant="outline-primary" disabled = {this.state.isRunning} onClick={this.runFlModels}><i className="far fa-play-circle"></i> Run Model</Button>
                { this.state.isRunning ? <span className='mr-3 ml-3'><img src="/loading.gif" width="25" height="25" /></span> : null }
                <Button className="ml-2" variant="outline-success" disabled = {!this.state.isFinished} onClick={this.downloadOutput}>Download Result</Button>
              </div>
            </div>
            <div className='row m-2 mt-5'>
              <div className='col-12'>
                <MasterDetail master={this.state.master}/>
              </div>

            </div>
            <div className='row m-2 mt-5'>
              <div className='col-12'>
                <Nodes nodes={this.state.nodes}/>
              </div>
            </div>
          </div>
        </div>
      </div>
    )
  }
}
