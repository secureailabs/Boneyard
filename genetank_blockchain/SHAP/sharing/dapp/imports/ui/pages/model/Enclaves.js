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
          <div>
              <div className="buttoncopy8">
                {this.props.nodeType === 'sub' &&
                <div className="rectangle1" style={{width: '864px'}}>
                </div>}
                {this.props.nodeType === 'hub' &&
                <div className="rectangle1" style={{width: '904px'}}>
                </div>}
              </div>
              <div className="ownerusernamecom">
                <span className="span1">Owner: </span>
                <span className="span2">{data.host}</span>
              </div>
              <div className="expiration083019">
                <span className="span1">Enclave Name:</span>
                <span className="span2">{infoJson.name}</span>
              </div>
              <div className="categorygenomics">
                <span className="span1">Master/Sub: </span>
                <span className="span2">{hubOrSub}</span>
              </div>
              <div className="privacyd12s5dp">
                <span className="span1">URL: </span>
                <span className="span2">{data.url}</span>
              </div>
              <div className="bitmap">
                <div className="mask">
                </div>
                <div className="group31">
                  <img src="/img/enclave-iconmessage-5@2x.png"
                    className="group32"/>
                </div>
              </div>
              {('hub' === type) &&
              <div className="colorblackcopy7">
                <i onClick={() =>
                  this.props.addSubEnclave(data)}>
                  <img src="/img/share-data-color-black-copy-5-1@2x.png"
                     className="colorblackcopy71"/>
                </i>
              </div>}
              {('hub' === type) &&
              <div className="colorblackcopy6">
                <i onClick={() =>
                  this.props.setDefaultEnclave(data.pubKey)}>
                  <img src="/img/share-data-import-color-black-copy-6@2x.png"
                     className="colorblackcopy61"/>
                </i>
              </div>}
              {('hub' === type) &&
              <div className="colorblackcopy5">
                <i style={{position: "relative", bottom: "10px"}}
                  className="far fa-trash-alt btn text-white" onClick={() =>
                    this.props.deleteEnclave(data.pubKey)}>
                </i>
              </div>}
              {('sub' === type) &&
              <div className="colorblackcopy6">
                <i style={{position: "relative", bottom: "10px"}}
                  className="far fa-trash-alt btn text-white" onClick={() =>
                    this.props.deleteSubEnclave(hubEnclave, data.pubKey)}>
                </i>
              </div>}
              <div className="datatitled123456">
                <span className="span1">Pub Key :
                   {data.pubKey.substring(0, 19) + "..."}</span>
                <span className="span2"></span>
              </div>
          </div>
        )
        return (
          <div>
            {this.props.nodeType === 'sub' &&
            <div style={{position: 'relative', left: '40px'}}>
               {contents(this.props.nodeType)}
            </div>}
            {this.props.nodeType === 'hub' &&
            <div style={{position: 'relative', left: '0px'}}>
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
    url: '',
    model_id: '',
    defaultEthAccount: '',
    ethAccounts: [],
    models: [],
    enclaves: [],
    showAction: 0,
    runnerEth: "",
    hostEth: "",
    currentRole: 100,
    showEnclaveModal: false,
    host: "",
    hostEthAccounts: [],
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
        this.onChangeAddingEnclave = this.onChangeAddingEnclave.bind(this);
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

    getHostEthAcc = (host)=>{
        Meteor.call("userGetEthInfo", host, (err, result) => {
            if (err)
              return;
            if (undefined === result.error) {
              this.setState({hostEthAccounts: 
                result.blockchainAccounts.map((e)=>(e.account))})
            }
        })
    }

    onChangeHost(event) {
        const host = event.target.value
        this.setState({host: host})
        this.getHostEthAcc(host);
    }

    onChangeHostEth(event) {
        const ethAddr = event.target.value
        this.setState({hostEth: ethAddr})
    }

    getCfg = () => {
        Meteor.call("userGetCfg", (err, result) => {
            var state = {...INITIAL_STATE};
            if (!err) {
                if (result !== null) {
                    state.showEnclaveModal = false;
                    state.defaultEthAccount = result.defaultEthAccount
                    state.ethAccounts =
                        result.blockchainAccounts.map((acc) => (acc.account))
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

            const enclave = this.state.enclaves[idx]
            const infoJson = EJSON.parse(enclave.infoJson)
            const roleInfos = enclave.roleInfos
            var runnerEth = "", hostEth = ""
            if (undefined !== roleInfos) {
                runnerEth = roleInfos[0].ethAddr
                hostEth = roleInfos[1].ethAddr
            }
            Meteor.call("adminGetUserList", (err, result) => {
                if (err)
                  return;
                if (result !== null) {
                  this.setState({users: result.userList})
                }
            })
            this.getHostEthAcc(enclave.host);
            this.setState({
                pubKey: pubKey, infoJson: infoJson,
                url: enclave.url,
                model_id: enclave.model_id,
                host: enclave.host,
                runnerEth: runnerEth,
                hostEth: hostEth,
                showEnclaveModal: true
            })
        }
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


    addSubSubmit = (event) => {
        const {
            hubEnclave,
            subEnclaveKey,
        } = this.state;

        const {
            history,
        } = this.props;

        //var subEnclaves = hubEnclave.subEnclaves
        //if (undefined === subEnclaves)
        //  subEnclaves = [subEnclaveKey]
        //else
        //  subEnclaves.push(subEnclaveKey)
        Meteor.call('adminAddSubEnclave',
            hubEnclave.pubKey, 
            subEnclaveKey,
            (error, result) => {
                if (error) {
                    alert("error:" + error)
                    this.setState(byPropKey('error', error));
                } else {
                    alert("Sub Enclave updated.")
                    this.getCfg()
                }
            });

        event.preventDefault();
    }


    onSubmit = (event) => {
        const {
            pubKey,
            url,
            infoJson,
            runnerEth,
            hostEth,
            host
        } = this.state;

        const {
            history,
        } = this.props;

        var roleInfos = []

        roleInfos[0] = {role: roles.RUNNER, ethAddr: runnerEth, stId:0}
        roleInfos[1] = {role: roles.HOST, ethAddr: hostEth, stId:1}

        Meteor.call('adminAddEnclave',
            infoJson,
            pubKey, //model_id,
            roleInfos,
            host,

            url,
            (error, result) => {
                if (error) {
                    alert("error:" + error)
                    this.setState(byPropKey('error', error));
                } else {
                    alert("Enclave added.")
                    this.getCfg()
                }
            });

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
                            label="Runner Ethereum Account"
                            as="select"
                            value={this.state.runnerEth}
                            id="runner-account"
                            onChange={this.onChangeRunnerEth}
                            list={listOption(this.state.ethAccounts)}
                        />
                        <DappFormGroup
                            label="Select a User as the Host"
                            value={this.state.host}
                            as="select"
                            id="trainer-account"
                            onChange={this.onChangeHost}
                            list={listOption(this.state.users)}
                        />
                        <DappFormGroup
                            label="Enclave Host's Ethereum Account"
                            value={this.state.hostEth}
                            as="select"
                            id="host-account"
                            onChange={this.onChangeHostEth}
                            list={listOption(this.state.hostEthAccounts)}
                        />
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
            <option value={item} key={index}>{item}</option>))
            var opList = ["Select"].map((i)=>(
            <option defaultValue="" key={"select"}>Select</option>))
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
                    <li className="group2" key={key}>
                      <EnclaveTab deleteSubEnclave={here.deleteSubEnclave}
                         data={subEncl}
                         hubEnclave={enclave}
                         nodeType={'sub'}/>
                    </li>
                )
            }
          })
          return list
        }

        const data = here.state.enclaves;
        return (
            <div className="container-fluid">
                <div className="row">
                    <div className="navbar-base">
                        <NavBar/>
                    </div>
                    <div className="main-content">
                      <div className="activetab">
                         Enclave List
                      </div>
                      {this.enclaveSettingsModal(listOption)}
                      {this.addSubModal(listOption)}
                      <ul style={{listStyleType: "none"}} className="data-list">
                        {data.map(function (listValue) {
                          return (
                            <div>
                            {/*Add nodeType as hub or sub for respective enclaves*/}
                              <li className="group2" key={listValue.pubKey}>
                                <EnclaveTab setDefaultEnclave={here.setDefaultEnclave}
                                 deleteEnclave={here.deleteEnclave}
                                 addSubEnclave={here.addSubEnclave}
                                 defaultEthAccount={here.state.defaultEthAccount}
                                 data={listValue}
                                 nodeType={'hub'}/>
                              </li>
                              {listSubEnclaves(listValue)}
                             </div>
                          );
                        })}
                      </ul>
                    </div>
                </div>
            </div>
        );

    }

}

export default withRouter(Enclaves);
