import React, {Component} from 'react';
import {withRouter} from 'react-router-dom';
import {EJSON} from 'meteor/ejson'
import {Redirect} from 'react-router'
import {listEnclaves, initRunnerInfoJson} from '/imports/api/usercfg.js'
import {roles} from '/imports/api/enum.js'
import {DappFormGroup} from '/imports/api/ui/DappFormGroup.js'
import NavBar from '/imports/ui/components/NavBar';
//import 'react-table/react-table.css';

import {Container, Row, Col, Button, Form, Modal} from 'react-bootstrap'
import ListItem from "../datasets/ListItem";
import {Meteor} from "meteor/meteor";

class EnclaveTab extends React.Component {
    constructor() {
        super();
        this.state = {
            error: ""
        };
    }

    componentDidMount() {
    }

    componentWillReceiveProps(newProps) {
    }

    render() {

        const {data, hubEnclave} = this.props;
        const infoJson = EJSON.parse(data.infoJson)
        const hubOrSub = (this.props.nodeType === 'hub')?"Hub":"Sub"

        const contents = (type)=>(
          <div class="row">
            <div class="col-sm-2 centerVertical">
              {hubOrSub}
            </div>
              <div class="col-sm-3 centerVertical iconArea">
                {('hub' === type) &&
                  <span>
                    {Roles.userIsInRole(Meteor.userId(), 'admin') ? (
                      <Button
                        className="iconButton"
                        variant="light"
                        onClick={() => this.props.setDefaultEnclave(data.pubKey)}>
                          <i className="fas fa-cog"></i>
                      </Button>
                    ) : ''
                    }
                  </span>
                }

                {('hub' === type) &&
                  <span>
                    <Button
                      className="iconButton"
                      variant="light"
                      onClick={() => this.props.addSubEnclave(data)}>
                        <i className="fas fa-plus"></i>
                    </Button>
                  </span>
                }

                {('hub' === type) &&
                  <span>
                    <Button
                      className="iconButton"
                      variant="light"
                      onClick={() => this.props.deleteEnclave(data.pubKey)}>
                        <i className="fas fa-trash"></i>
                    </Button>
                  </span>
                }
                {('sub' === type) &&
                  <span>
                    <Button
                      className="iconButton"
                      variant="light"
                      onClick={() => this.props.deleteSubEnclave(hubEnclave, data.pubKey)}>
                        <i className="fas fa-trash"></i>
                    </Button>
                  </span>
                }
              </div>
              <div class="col-sm-2 centerVertical">
                {
                  (data.hosts ?
                    data.hosts.map((host, index) =>
                          <span>
                            { host.host }
                            { ( (data.hosts.length - 1) !== index ? "," : null) }
                          </span>)
                    : null)
                }
              </div>
              <div class="col-sm-2 centerVertical">
                {infoJson.name}
              </div>
              <div class="col-sm-2 centerVertical">
                {data.url}
              </div>
            </div>
        )
        return (
          <div>
            {this.props.nodeType === 'sub' &&
            <div className="moduleArea sub">
               {contents(this.props.nodeType)}
            </div>}
            {this.props.nodeType === 'hub' &&
            <div className="moduleArea hub">
               {contents(this.props.nodeType)}
            </div>}
          </div>
        );
    }
}

class ShowEnclaveOp extends Component {
    constructor(props) {
        super(props);
        this.state = {
            training: false,
            attServer: "http://platform.genetank.ai:4200",
            selectModelId: "",
            cloneSource: "",
            trainerAddress: "",
            //geneticFile:"",
            //predict:"",
            //aiModels:[]
        }
        this.handleSelect = this.handleSelect.bind(this)
        this.handleSelectEnclave = this.handleSelectEnclave.bind(this)
        this.handleAttestation = this.handleAttestation.bind(this)
        this.handleInstallModel = this.handleInstallModel.bind(this)
        this.handleRegister = this.handleRegister.bind(this)
        this.handleTraining = this.handleTraining.bind(this)
        this.handleCloning = this.handleCloning.bind(this)
        //this.handlePrediction = this.handlePrediction.bind(this)
    }

    handleAttestation(event) {
        Meteor.call("enclaveAttestation", this.state.attServer,
            this.props.state.pubKey, (err, ret) => {
                if (err)
                    alert(err)
                else {
                    if (undefined !== ret.error)
                        alert("Attestation failed." + ret.error)
                    else
                        alert("Attestation is successful.\nAttestation report:"
                            + EJSON.stringify(ret.result))
                    this.forceUpdate()
                }
            })
    }

    handlePrediction(event) {
        if (undefined === this.state.geneticFile || "" === this.state.geneticFile)
            return

        var reader = new FileReader();
        reader.onload = (fileLoadEvent) => {
            Meteor.call("enclavePrediction", reader.result,
                this.props.state.pubKey, "Add querier eth address here", (err, ret) => {
                    if (err)
                        alert("Call enclavePrediction fail.");
                    else if (ret.error)
                        alert(ret.error)
                    else {
                        alert("Prediction result is: " + ret.result)
                        this.setState({predict: ret.result})
                    }
                });
        };
        reader.readAsBinaryString(this.state.geneticFile);
    }

    handleCloning(event) {
        if ("" === this.state.cloneSource) {
            alert("Enter source enclave public key.")
            return;
        }

        Meteor.call("enclaveCloning", this.state.cloneSource,
            this.props.state.pubKey, (err, ret) => {
                if (err)
                    alert(err)
                else {
                    if (undefined !== ret.error)
                        alert("Cloning failed." + ret.error)
                    else {
                        alert("Cloning is successful.")
                    }
                }
            }
        )
    }

    handleTraining(event) {
        Meteor.call("enclaveTraining", !this.state.training,
            this.props.state.pubKey, (err, ret) => {
                if (err)
                    alert(err)
                else {
                    if (undefined !== ret.error)
                        alert("Training control fail." + ret.error)
                    else {
                        const training = !this.state.training
                        this.setState({training: training})
                    }
                }
            }
        )
    }

    handleRegister(event) {

        Meteor.call('registerEnclave', this.state.trainerAddress, this.props.state.infoJson, this.props.state.pubKey, this.props.state.infoJson.ethereumAddress, (error, result) => {
            if (error) {
                console.log("error:", error)
            } else {
                console.log("Success");
            }
        })
    }

    handleInstallModel(event) {
        if ("" === this.state.selectModelId)
            alert("Select an uploaded AI Model first.")
        Meteor.call("installAiModel", this.state.selectModelId,
            this.props.state.pubKey, (err, result) => {
                if (err)
                    alert("Call installAiModel fail.")
                else if (undefined !== result.error)
                    alert("Install AI Model fail. " + result.error)
                else
                    alert("Install AI Model success. " + result.result)
            })
    }

    handleSelectEnclave(event) {
        this.setState({cloneSource: event.target.value})
    }

    handleSelect(event) {
        this.setState({selectModelId: event.target.value})
    }

    componentDidMount() {
        //this.getCfgAiModel();
    }

    componentWillReceiveProps(newProps) {
        //this.forceUpdate()
        if (undefined !== newProps.state.models[0])
            this.setState({selectModelId: newProps.state.models[0]._id});
    }

    render() {
        const currEnclave = this.props.state
        if (undefined === currEnclave)
            return
        var currModelId = ""
        if (undefined !== this.state.selectModelId)
            currModelId = this.state.selectModelId
        const listModel = currEnclave.models.map((model) => (
            <option value={model._id} key={model._id.toString()}>
                Name:{model.name}, id:{model._id}</option>));
        //show Enclave operations:
        //. attestation
        //. register to blockchain
        //. install AI models
        //. clone running AI model
        //. donate data
        //. start training
        //. stop training
        //. get information: installed AI models, received data, training status...
        return (
            <Container>
                <Row>
                    <Col>
                        <Form>
                            <legend className="border border-top-0 border-left-0 border-right-0">Control my enclave
                            </legend>
                            <DappFormGroup
                                id="att-server"
                                label="Enter the attestation server URL"
                                value={this.state.attServer}
                                type="text"
                                onChange={event => this.setState({attServer: event.target.value})}
                                placeholder="Enter URL (including and port number, e.g. http://...:4200)"
                            />
                            <Button variant="outline-primary" block
                                    onChange={this.handleAttestation}>
                                Start Attestation
                            </Button>
                            <DappFormGroup
                                label="Trainer Address"
                                value={this.state.trainerAddress}
                                type="text"
                                id="trainerAddress"
                                onChange={event => {
                                    var trainerAddress = event.target.value;
                                    this.setState({trainerAddress: trainerAddress})
                                }}
                            />
                            <Button variant="outline-primary" block
                                    onChange={this.handleRegister}>
                                Register to blockchain
                            </Button>
                            <DappFormGroup
                                label="Select AI Model"
                                as="select"
                                value={currModelId}
                                id="select-model"
                                onChange={this.handleSelect}
                                list={listModel}
                            />
                            <Button variant="outline-primary" block
                                    onChange={this.handleInstallModel}>
                                Install AI Model
                            </Button>
                            <DappFormGroup
                                label="Select an enclave to be cloned:"
                                as="select"
                                value={this.state.cloneSource}
                                id="select-enclave"
                                onChange={this.handleSelectEnclave}
                                list={listEnclaves(currEnclave.enclaves)}
                            />
                            <Button variant="outline-success" block
                                    onChange={this.handleCloning}>
                                "Clone from this Enclave
                            </Button>
                            <Button variant="outline-primary" block
                                    onChange={this.handleTraining}>
                                {this.state.training ? "Stop Training" : "Start Training"}
                            </Button>
                        </Form>
                    </Col>
                </Row>
            </Container>
        )
    }
}

const INITIAL_STATE = {
    infoJson: initRunnerInfoJson,
    pubKey: '',
    selectedEnclave: {},
    url: '',
    model_id: '',
    defaultEthAccount: '',
    ethAccounts: [],
    models: [],
    enclaves: [],
    showAction: 0,
    runnerEth: "",
    uniqueHosts: [],
    roleInfos: [],
    currentRole: 100,
    showEnclaveModal: false,
    hosts: [{host:"", hostEth:"", stId:1, hostEthAccounts:[]}],
    users: [],
    showAddSubModal: false,
    freeEnclaves: [],
    hubEnclave: {},
    subEnclaveKey: "",
    error: null,
};

const byPropKey = (propertyName, value) => () => ({
    [propertyName]: value,
});
let here;

class Enclaves extends Component {
    constructor(props) {
        super(props);
        here = this;
        this.state = {...INITIAL_STATE};

        this.onSubmit = this.onSubmit.bind(this);

        this.addSubSubmit = this.addSubSubmit.bind(this);
        this.setDefaultEnclave = this.setDefaultEnclave.bind(this);
        this.deleteEnclave = this.deleteEnclave.bind(this);
        this.deleteSubEnclave = this.deleteSubEnclave.bind(this);
        this.addSubEnclave = this.addSubEnclave.bind(this);
        this.onChangeRunnerEth = this.onChangeRunnerEth.bind(this);
        this.onChangeHostEth = this.onChangeHostEth.bind(this);
        this.onChangeHost = this.onChangeHost.bind(this);
        this.onAddHost = this.onAddHost.bind(this);
        this.onDeleteHost = this.onDeleteHost.bind(this);
        this.onChangeAddingEnclave = this.onChangeAddingEnclave.bind(this);
        this.setEnclaveProperties = this.setEnclaveProperties.bind(this);
        this.addHost = this.addHost.bind(this);
        this.updateHost = this.updateHost.bind(this);
        this.addEnclave = this.addEnclave.bind(this);
      }

    onChangeAddingEnclave(event){
      this.setState({subEnclaveKey:event.target.value})
    }

    addSubEnclave(enclave){
        const enclaves = this.state.enclaves
/*
        var found = false
        var idx
        for (idx in enclaves)
            if (enclaves[idx].pubKey === pubKey) {
                found = true
                break
            }
        if (found) {
            const enclave = enclaves[idx]
*/
            var freeEnclaves = enclaves.map((encl)=>{
              if (undefined === enclave.subEnclaves ||
                  enclave.subEnclaves.length === 0)
                return(encl.pubKey)
              const key = enclave.subEnclaves.find((k)=>{
                if (k === encl.pubKey)
                return (k)
              })
              if (undefined === key)
                return(encl.pubKey)
            })
            freeEnclaves = freeEnclaves.filter((e)=>(
              e !== undefined && e !== enclave.pubKey))
            this.setState({
                freeEnclaves: freeEnclaves,
                hubEnclave: enclave,
                showAddSubModal: true
            })
//        }
    }

    deleteSubEnclave(hubEnclave, subPubKey){
        //var subEnclaves = hubEnclave.subEnclaves.filter(e=>e !== subPubKey)
        if (confirm("Are you sure you want to delete this Sub Enclave permanently?"))
        Meteor.call('adminDelSubEnclave',
            hubEnclave.pubKey,
            subPubKey,
            (error, result) => {
                if (error) {
                    alert("error:" + error)
                    this.setState(byPropKey('error', error));
                } else {
                    alert("Sub Enclave updated.")
                    this.getCfg()
                }
            });

    }

    onChangeRunnerEth(event) {
        const ethAddr = event.target.value;
        var infoJson = this.state.infoJson;
        console.log(infoJson);
        infoJson.ethereumAddress = ethAddr
        this.setState({runnerEth: ethAddr, infoJson: infoJson})
    }

    getHostEthAcc = (host, i)=> {
      Meteor.call("userGetEthInfo", host, (err, result) => {
        if (err)
          return;
        if (undefined === result.error) {
          let hosts = [...this.state.hosts]
          hosts[i]["hostEthAccounts"] = [result.defaultEthAccount]
          this.setState({
            hosts: hosts
          })
        }
      })
    }

    onChangeHost(index, event) {
        const host = event.target.value;
        let hosts = [...this.state.hosts];
        hosts[index][event.target.id] = host;
        this.setState( {
          hosts: hosts,
        });
        this.getHostEthAcc(host, index);
    }

    onChangeHostEth(index, event) {
        const ethAddr = event.target.value
        let hosts = [...this.state.hosts];
        hosts[index][event.target.id] = ethAddr;
        this.setState({hosts: hosts});
    }

    onAddHost(event) {
      // console.log(this.state.hosts.filter((value, index) => this.state.hosts.indexOf(value) === index))
      const length = this.state.hosts.length;
      let newId = 1;
      if(length) {
        newId = this.state.hosts[length - 1].stId + 1;
      }
      if (length <= 3) {
        this.setState((prevState) => ({
          hosts: [...prevState.hosts, {host:"", hostEth:"", stId:newId , hostEthAccounts:[]}],
        }));
      }
    }

    onDeleteHost(index, stId) {
      let hosts = this.state.hosts;
      let deletedHost = hosts[index];
      deletedHost.hostEthAccounts = [...""]
      hosts.splice(index, 1);
      this.setState({
          hosts: hosts,
      });
      if(this.state.selectedEnclave.hosts.findIndex(
          host => JSON.stringify(host) === JSON.stringify(deletedHost))
        != -1) {
        Meteor.call(
          "adminDelHost",
          this.state.pubKey,
          deletedHost,
          stId,
          this.state.runnerEth,
          (error, result) => {
          if(error) {
            alert(error);
          } else if(result.error) {
            alert(result.error);
          }
        });
        this.setEnclaveProperties();
        this.addEnclave(this.state.infoJson,
                        this.state.pubKey,
                        this.state.roleInfos,
                        this.state.uniqueHosts,
                        this.state.url,
                        true);
      }
    }

    getCfg = () => {
        Meteor.call("userGetCfg", (err, result) => {
            var state = {...INITIAL_STATE};
            if (!err) {
                if (result !== null) {
                    state.showEnclaveModal = false;
                    state.defaultEthAccount = result.defaultEthAccount
                    state.ethAccounts = [result.defaultEthAccount]
                    state.enclaves = result.enclaves.map((enclave) => {
                        const report = enclave.infoJson.attestationReport
                        if ("object" === typeof (report))
                            enclave.infoJson.attestationReport = EJSON.stringify(report)
                        return (enclave)
                    })
                    state.models = result.models;
                }
            }
            this.setState(state);
        });
    }

    componentDidMount() {
        this.getCfg();
    }

    setDefaultEnclave(pubKey) {
        var found = false
        var idx
        for (idx in this.state.enclaves)
            if (this.state.enclaves[idx].pubKey === pubKey) {
                found = true
                break
            }
        if (found) {
            const enclave = this.state.enclaves[idx];
            this.state.selectedEnclave = JSON.parse(JSON.stringify(enclave));
            const infoJson = EJSON.parse(enclave.infoJson)
            const roleInfos = enclave.roleInfos
            var runnerEth = "", hostEth = []
            if (undefined !== roleInfos) {
                runnerEth = roleInfos[0].ethAddr
                for(var i = 1; i < roleInfos.length; i++) {
                  hostEth[(i - 1)]= roleInfos[i].ethAddr
                }
            }
            Meteor.call("adminGetUserList", (err, result) => {
                if (err)
                  return;
                if (result !== null) {
                  this.state.users = result.userList
                }
            })
            let hostList = []
            if(typeof enclave.hosts === 'undefined'){
              hostList = this.state.hosts;
            } else {
              this.state.hosts = enclave.hosts
              hostList = enclave.hosts;
            }
            console.log(this.state.hosts)
            hostList.forEach((host, i) => {
              this.getHostEthAcc(host.host, i)
            });
            this.setState({
                pubKey: pubKey, infoJson: infoJson,
                url: enclave.url,
                model_id: enclave.model_id,
                runnerEth: runnerEth,
                showEnclaveModal: true,
            });

        }
    }

    addHost(pubKey, host, stId, runnerEth) {
      console.log("In add host: ");
      console.log(host);
      Meteor.call(
        "adminAddHost",
        pubKey,
        host,
        stId,
        runnerEth,
        (error, result) => {
        if(error) {
          alert(error);
        } else if(result.error) {
          alert(result.error);
        }
      });
    }

    updateHost(pubKey, host, hostEth, runnerEth) {
      console.log("In update host.");
      console.log(host);
      Meteor.call(
        "adminUpdateHost",
        pubKey,
        host,
        hostEth,
        runnerEth,
        (error, result) => {
        if(error) {
          alert(error);
        } else if(result.error) {
          alert(result.error);
        }
      });
    }

    addEnclave(infoJson, pubKey, roleInfos, uniqueHosts, url, deleteHost) {
      console.log("In adding enclave");
      console.log(uniqueHosts);
      Meteor.call('adminAddEnclave',
          infoJson,
          pubKey,
          roleInfos,
          uniqueHosts,
          url,
          (error, result) => {
              if (error) {
                  alert("error:" + error)
                  this.setState(byPropKey('error', error));
              } else if (!deleteHost) {
                  alert("Enclave added.")
                  this.getCfg()
              } else {
                  this.getCfg();
              }
        });
    }

    deleteEnclave(pubKey) {
        if (confirm("Are you sure you want to delete this Enclave permanently?"))
        Meteor.call("adminDelEnclave", pubKey, (error, result) => {

            if (!error && undefined == result.error)
                this.getCfg()
            else if (error)
              alert(error)
            else
              alert(result.error)
        })
    }

    setEnclaveProperties() {
      const {
          pubKey,
          url,
          infoJson,
          runnerEth,
          hosts
      } = this.state;

      const {
          history,
      } = this.props;

      var role_id = 0

      // remove empty hosts
      var nonEmptyHosts = hosts.filter((host) =>
        host.host != "" && host.hostEth != ""
      );
      // remove duplicate hosts
      this.state.uniqueHosts = nonEmptyHosts.filter((host1, index, self) =>
        index === self.findIndex((host2) => (host2.hostEth === host1.hostEth))
      );
      console.log("unique hosts:")
      console.log(this.state.uniqueHosts);
      this.state.uniqueHosts.forEach((item, i) => {
        item.hostEthAccounts = [...""]
      });

      this.state.roleInfos[role_id] = {role: roles.RUNNER, ethAddr: runnerEth, stId:role_id++}
      for(var i = 0; i < this.state.uniqueHosts.length; i++) {
        this.state.roleInfos[role_id++] = {
          role: roles.HOST, ethAddr: this.state.uniqueHosts[i].hostEth, stId:this.state.uniqueHosts[i].stId
        }
      }
    }

    addSubSubmit = (event) => {
        const {
            hubEnclave,
            subEnclaveKey,
        } = this.state;

        const {
            history,
        } = this.props;

        var subEnclaves = hubEnclave.subEnclaves
        if (undefined === subEnclaves)
          subEnclaves = [subEnclaveKey]
        else
          subEnclaves.push(subEnclaveKey)

        Meteor.call('adminAddSubEnclave',
            hubEnclave.pubKey,
            subEnclaveKey,
            (error, result) => {
                if (error) {
                    alert("error:" + error)
                    this.setState(byPropKey('error', error));
                } else if (undefined == result.error) {
                    alert("Sub Enclave updated");
                    this.getCfg()
                } else {
                    alert(result.error)
                }
            });

        event.preventDefault();
    }


    onSubmit = (event) => {
        this.setEnclaveProperties();
        const {
            pubKey,
            url,
            infoJson,
            runnerEth,
            roleInfos,
            uniqueHosts
        } = this.state;

        const {
            history,
        } = this.props;

        // add and update hosts
        if(this.state.selectedEnclave.hasOwnProperty('hosts')) {
          for(var idx in uniqueHosts) {
            if(this.state.selectedEnclave.hosts[idx]) {
              console.log(this.state.selectedEnclave.hosts[idx]);
              console.log(uniqueHosts[idx]);
              if(this.state.selectedEnclave.hosts[idx].hostEth != uniqueHosts[idx].hostEth) {
                this.updateHost(pubKey,
                                uniqueHosts[idx],
                                this.state.selectedEnclave.hosts[idx].hostEth,
                                runnerEth);
              }
            }
            else {
              this.addHost(pubKey, uniqueHosts[idx], uniqueHosts[idx].stId, runnerEth);
            }
          }
        }

        this.addEnclave(infoJson, pubKey, roleInfos, uniqueHosts, url, false);

        event.preventDefault();
    }

    handleShowUploadData() {
        this.setState({showUploadData: true});
    }


    addSubModal = (listOption) => (
        <Modal
            show={this.state.showAddSubModal}
            onHide={() => this.setState({showAddSubModal: false})}
        >
            <div style={{
                borderStyle: "solid",
                borderWidth: "1px",
                borderColor: "white",
            }}
                 className="">
                <Modal.Header style={{backgroundColor: "rgba(50, 50, 51, 1.0)", color: "white"}} closeButton>
                    <Modal.Title>Adding a Sub Enclave</Modal.Title>
                </Modal.Header>
                <Modal.Body style={{backgroundColor: "rgba(50, 50, 51, 1.0)", color: "white", fontWeight: "bold"}}>
                    <Form>
                        <DappFormGroup
                            label="Enclaves Available"
                            as="select"
                            value={this.state.subEnclaveKey}
                            id="adding-enclave"
                            onChange={this.onChangeAddingEnclave}
                            list={listOption(this.state.freeEnclaves)}
                        />
                    </Form>
                </Modal.Body>
                <Modal.Footer style={{backgroundColor: "rgba(50, 50, 51, 1.0)", color: "white", fontWeight: "bold"}}>
                    <Button className="btn btn-secindary float-right"
                            onClick={(event) => this.addSubSubmit(event)}>Add</Button>
                    {this.state.error && <p>{this.state.error.reason}</p>}
                </Modal.Footer>
            </div>
        </Modal>
    )

    enclaveSettingsModal = (listOption) => (
        <Modal
            show={this.state.showEnclaveModal}
            onHide={() => this.setState({showEnclaveModal: false})}
        >
            <div style={{
                borderStyle: "solid",
                borderWidth: "1px",
                borderColor: "white",
            }}
                 className="">
                <Modal.Header style={{backgroundColor: "rgba(50, 50, 51, 1.0)", color: "white"}} closeButton>
                    <Modal.Title>Enclave Settings</Modal.Title>
                </Modal.Header>
                <Modal.Body style={{backgroundColor: "rgba(50, 50, 51, 1.0)", color: "white", fontWeight: "bold"}}>
                    <Form>
                        <DappFormGroup
                            label="Name of the Enclave"
                            value={this.state.infoJson.name}
                            type="text"
                            id="enclave_name"
                            onChange={event => {
                                const infoJson = this.state.infoJson
                                infoJson.name = event.target.value
                                this.setState({infoJson: infoJson})
                            }}
                            placeholder="Enter enclave name"
                        />
                        <DappFormGroup
                            label="Public key of the Enclave"
                            value={this.state.pubKey}
                            type="text"
                            id="pubKey"
                            onChange={event => this.setState(byPropKey('pubKey', event.target.value))}
                            placeholder="Enter public key (hex string without 0x)"
                        />
                        <DappFormGroup
                            label="Runner SAIL Account"
                            as="select"
                            value={this.state.runnerEth}
                            id="runner-account"
                            onChange={this.onChangeRunnerEth}
                            list={listOption(this.state.ethAccounts)}
                        />
                        {
                          (this.state.hosts ?
                            this.state.hosts.map((val, idx)=> {
                              let hostId = 'host-${idx}', hostEthId = 'hostEth-${idx}'
                              return (
                                <>
                                <input
                                    type="hidden"
                                    name="stId"
                                    value={this.state.hosts[idx].stId}
                                />
                                <DappFormGroup
                                    label="Select a User as the Host"
                                    value={this.state.hosts[idx].host}
                                    as="select"
                                    id="host"
                                    onChange={event => this.onChangeHost(idx, event)}
                                    list={listOption(this.state.users)}
                                />
                                <DappFormGroup
                                    label="Enclave Host's SAIL Account"
                                    value={this.state.hosts[idx].hostEth}
                                    as="select"
                                    id="hostEth"
                                    onChange={event => this.onChangeHostEth(idx, event)}
                                    list={listOption(this.state.hosts[idx].hostEthAccounts)}
                                />
                                <Form.Group as={Col}>
                                  <Button className="btn btn-danger" onClick={() => this.onDeleteHost(idx, this.state.hosts[idx].stId)}>
                                    Remove Host
                                  </Button>
                                </Form.Group>
                                </>
                              )
                            })
                          : null
                        )
                        }
                        <Form.Group as={Col}>
                          <Button className="btn btn-primary" onClick={this.onAddHost}>
                            Add Host
                          </Button>
                        </Form.Group>
                        <DappFormGroup
                            label="URL of the Enclave"
                            value={this.state.url}
                            type="text"
                            id="enclaveURL"
                            onChange={event => this.setState(byPropKey('url', event.target.value))}
                            placeholder="Enter the url as: http(s)://domain(or IP):port"
                        />
                        <DappFormGroup
                            label="Hardware information of the enclave"
                            value={this.state.infoJson.hardwareInfo}
                            type="text"
                            id="hardwareInfo"
                            onChange={event => {
                                const infoJson = this.state.infoJson
                                infoJson.hardwareInfo = event.target.value
                                this.setState({infoJson: infoJson})
                            }}
                            placeholder="Hardware information of the enclave"
                        />
                    </Form>
                </Modal.Body>
                <Modal.Footer style={{backgroundColor: "rgba(50, 50, 51, 1.0)", color: "white", fontWeight: "bold"}}>
                    <Button className="btn btn-secindary float-right"
                            onClick={(event) => this.onSubmit(event)}>Save</Button>
                    {this.state.error && <p>{this.state.error.reason}</p>}
                </Modal.Footer>
            </div>
        </Modal>
    )


    render() {
        const usrId = Meteor.userId();
        if (undefined === usrId || null === usrId) {
            return <Redirect push to='/login'/>
        }

        const listOption = (list) =>{
              //<option defaultValue="">Select</option>
            const res = list.map((item, index) => (
            <option
              value={item}
              key={index}
              disabled={(this.state.hosts.filter(host => host.host === item).length > 0)
                          ? 'disabled' : null}
            >
              {item}
            </option>))
            var opList = ["Select"].map((i)=>(
            <option defaultValue="" value="" key={"select"}>Select</option>))
            var merged = opList.concat(res)
            return merged
        }

        const listSubEnclaves = (enclave) => {
          const subEnclaves = enclave.subEnclaves
          if (undefined === subEnclaves || 0 === subEnclaves.length)
            return
          const list = subEnclaves.map((pubKey,index)=>{
            const subEncl = this.state.enclaves.find((subEnclave)=>(
              subEnclave.pubKey === pubKey))
            if (undefined !== subEncl){
                const key = enclave.pubKey+subEncl.pubKey
                return(
                    <span className="sub" key={key}>
                      <EnclaveTab deleteSubEnclave={here.deleteSubEnclave}
                         data={subEncl}
                         hubEnclave={enclave}
                         nodeType={'sub'}/>
                    </span>
                )
            }
          })
          return list
        }
        // unique enclaves
        var uniqueEnclaves = here.state.enclaves.filter((enclave1, index, self) =>
          index === self.findIndex((enclave2) => (enclave2.pubKey === enclave1.pubKey))
        );
        const data = uniqueEnclaves;
        return (
            <div className="container-fluid">
                <div className="row">
                    <div className="navbar-base">
                        <NavBar/>
                    </div>
                    <div className="main-content">
                      <h1>Manage Enclaves</h1>
                      <br /><p className="headline">View enclave setups. If you have administrative privileges, you can edit as well.</p>
                      {this.enclaveSettingsModal(listOption)}
                      {this.addSubModal(listOption)}
                      <div class="moduleArea">
                        <div class="row tableHeader">
                          <div class="col-md-2">Type</div>
                          <div class="col-md-3">Actions</div>
                          <div class="col-md-2">Enclave Name</div>
                          <div class="col-md-2">Owner</div>
                          <div class="col-md-2">URL</div>
                        </div>
                      </div>
                      <div>
                        {data.map(function (listValue) {
                          return (
                            <div>
                            {/*Add nodeType as hub or sub for respective enclaves*/}
                              <span key={listValue.pubKey}>
                                <EnclaveTab setDefaultEnclave={here.setDefaultEnclave}
                                 deleteEnclave={here.deleteEnclave}
                                 addSubEnclave={here.addSubEnclave}
                                 defaultEthAccount={here.state.defaultEthAccount}
                                 data={listValue}
                                 nodeType={'hub'}/>
                              </span>
                              {listSubEnclaves(listValue)}
                             </div>
                          );
                        })}
                      </div>
                    </div>
                </div>
            </div>
        );

    }

}

export default withRouter(Enclaves);
