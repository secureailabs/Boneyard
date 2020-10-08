import { Meteor } from "meteor/meteor";
import React, { Component } from "react";
import { withRouter } from "react-router-dom";
import { Redirect } from "react-router";
import NavBar from "/imports/ui/components/NavBar";
import {
  Container,
  Button,
  ButtonGroup,
  Row,
  Col,
  Form,
  Modal
} from "react-bootstrap";
import { listEnclaves } from "../../../api/usercfg";
import { roles } from "/imports/api/enum.js";
import { DataList, getLeafDatasets } from "../datasets/View.js";
import { DappFormGroup } from "/imports/api/ui/DappFormGroup.js";

const active_dataset = {
  PUBLIC: "public",
  PERSONAL: "personal"
};

class AccessData extends Component {
  constructor(props) {
    super(props);
    this.state = {
      activeDataset: active_dataset.PUBLIC,
      publicData: [],
      myData: [],
      enclaves: [],
      pubKey: "",
      ethAcc: "", //Current Ethereum account for accessing the enclave
      ethAccounts: [],
      showEditData: false,
      currentDataId: 0,
      dataDescriptions: "",
      dataTags: ""
    };
    this.loadData = this.loadData.bind(this);
    this.getPublicData = this.getPublicData.bind(this);
    this.selectEnclave = this.selectEnclave.bind(this);
    this.setDefaultEnclave = this.setDefaultEnclave.bind(this);
    this.updateData = this.updateData.bind(this);
    this.handleShowDataInfo = this.handleShowDataInfo.bind(this);
    this.handleCloseEditData = this.handleCloseEditData.bind(this);
  }

  componentDidMount() {
    Meteor.call("userGetEnclaveList", (err, result) => {
      if (err) alert(err);
      else if (result.error) alert(result.error);
      else {
        this.setState({ enclaves: result.enclaveList });
        this.getCfg();
      }
    });
  }

  getPublicData() {
    Meteor.call(
      "getPublicDataFromHub",
      this.state.pubKey,
      this.state.ethAcc,
      (err, res) => {
        if (!err) {
          if (!res.error) {
            const pubData = res.response.subEnclaves.map(sub => {
              if (sub.resp.result)
                return {
                  pubKey: sub.pubKey,
                  host: sub.host,
                  name: sub.name,
                  leaves: getLeafDatasets([sub.resp.result])
                };
            });
            const validPubData = pubData.filter(d => undefined !== d);
            this.setState({
              activeDataset: active_dataset.PUBLIC,
              publicData: validPubData
            });
          } else {
            alert("Error connecting to enclave: " + JSON.parse(res.error).errno);
          }
        } else {
          alert("AccessData.js :: Get Public data:: Error");
        }
      }
    );
  }

  loadData(activeData) {
    if (activeData == active_dataset.PUBLIC) {
      if (this.state.pubKey) {
        this.getPublicData();
      }
      this.setState({
        activeDataset: active_dataset.PUBLIC
      });
    } else {
      if (this.state.pubKey) {
        Meteor.call(
          "getEnclaveInfo",
          this.state.pubKey,
          this.state.ethAcc,
          "/data",
          (err, res) => {
            if (err) {
              alert(err.toString());
            } else if (res.error) {
              alert("Error connecting to enclave: " + JSON.parse(res.error).errno);
            } else {
              let personalDataset = getLeafDatasets(res.result.children);
              personalDataset.map((dataset, i) => {
                dataset.id = i.toString();
              });
              this.setState({
                myData: personalDataset
              });
            }
          }
        );
      }
      this.setState({
        activeDataset: active_dataset.PERSONAL
      });
    }
  }

  getCfg = () => {
      Meteor.call("userGetCfg", (err, result) => {
          var ethAccounts = this.state.ethAccounts;
          if (!err) {
              if (result !== null) {
                ethAccounts = result.blockchainAccounts.map((acc) => (acc.account))
              }
          }
          this.setState({ ethAccounts: ethAccounts });
      });
  }

  setDefaultEnclave(pubKey) {
    var ethAcc = this.state.ethAcc;
    const enclave = this.state.enclaves.find(e => e.pubKey === pubKey);
    if (undefined !== enclave) {
      const role = enclave.roleInfos.find(r =>
        r.role === roles.HOST && this.state.ethAccounts.find(account => account === r.ethAddr)
      );
      if (undefined != role) {
        ethAcc = role.ethAddr
      }
    }
    //console.log("Public key updated")
    this.setState(
      {
        ethAcc: ethAcc,
        pubKey: pubKey,
      },
      function() {
        //here.getDatasetForOwner()
        this.loadData(this.state.activeDataset);
      }
    );
  }

  selectEnclave(event) {
    let pubKey = event.target.value;
    //this.setState({pubKey: pubKey})
    if (undefined !== pubKey && "Select" !== pubKey) {
      this.setDefaultEnclave(pubKey);
    }
  }

  handleShowDataInfo(dataId) {
    var dataset;
    dataset = this.state.myData.find(d => d.id === dataId);
    if( dataset !== undefined ) {
      this.setState({
        showEditData: true,
        currentDataId: dataId,
        dataName: dataset.name,
        dataDescriptions: dataset.descriptions,
        dataTags: dataset.tags
      });
    }
  }

  handleCloseEditData() {
    this.setState({ showEditData: false });
  }

  updateData() {
    const guestPubKey = ""
    const dataset = this.state.myData.find(d => d.id === this.state.currentDataId);
    if (undefined === dataset) {
      alert("Dataset not found.");
      return;
    }
    if(dataset.descriptions !== this.state.dataDescriptions ||
      dataset.tags !== this.state.datatags
    ) {
      dataset.descriptions = this.state.dataDescriptions
      dataset.tags = this.state.dataTags
    }
    const updatedDataInfo = Object.assign({}, dataset);
    delete updatedDataInfo.shareInfo;
    const shareInfo = { dataInfo: updatedDataInfo };
    const srcDir = "/";
    const dstDir = "/";
    Meteor.call(
      "userShareData2Guest",
      this.state.pubKey,
      this.state.ethAcc,
      srcDir,
      dstDir,
      this.state.dataName,
      shareInfo,
      guestPubKey.pubKey,
      (err, result) => {
        if (err) { alert(err); }
        else if(result.error) { alert(result.error); }
        else{ this.setState({ showEditData: false }); }
      }
    );
  }

  editDataModal = () => (
    <Modal
      size="lg"
      show={this.state.showEditData}
      onHide={this.handleCloseEditData}
    >
      <Modal.Header
        style={{ backgroundColor: "rgba(50, 50, 51, 1.0)", color: "white" }}
        closeButton
      >
        <Modal.Title>Edit Data</Modal.Title>
      </Modal.Header>
      <Modal.Body
        style={{
          backgroundColor: "rgba(50, 50, 51, 1.0)",
          color: "white",
          fontWeight: "bold"
        }}
      >
        <Container>
          <Row style={{ height: "60px" }}>
            <Col sm={{ span: 4, offset: 3 }}>
              <div>
                {" "}
                <h4>{this.state.dataName}</h4>{" "}
              </div>
            </Col>
          </Row>
          <Row>
            <Col>
              <Form
                style={{
                  backgroundColor: "rgba(56,56,56,1)",
                  color: "white"
                }}
              >
                <DappFormGroup
                  style={{ position: "absolute", top: "60px" }}
                  id="dataDescription"
                  as="textarea"
                  label="Description"
                  value={this.state.dataDescriptions}
                  type="text"
                  onChange={event => {
                    const fn = event.target.value;
                    this.setState({ dataDescriptions: fn });
                  }}
                  placeholder="Provide a brief description of the Dataset"
                  className="inputplaceholder"
                />
                <DappFormGroup
                  id="dataTags"
                  label="Tags"
                  value={this.state.dataTags}
                  type="text"
                  onChange={event => {
                    const fn = event.target.value;
                    this.setState({ dataTags: fn });
                  }}
                  placeholder="Add tags to the Dataset"
                />
              </Form>
            </Col>
          </Row>
        </Container>
      </Modal.Body>
      <Modal.Footer
        style={{ backgroundColor: "rgba(50, 50, 51, 1.0)", color: "white" }}
      >
        <Container>
          <Row>
            <Col>
              <Button
                variant="primary"
                size="lg"
                block
                onClick={() => this.updateData()}
              >
                Update Data
              </Button>
            </Col>
            <Col>
              <Button
                type="button"
                variant="secondary"
                size="lg"
                block
                onClick={this.handleCloseEditData}
              >
                Close
              </Button>
            </Col>
          </Row>
        </Container>
      </Modal.Footer>
    </Modal>
  );

  render() {
    const usrId = Meteor.userId();
    if (undefined === usrId || null === usrId) {
      return <Redirect push to="/login" />;
    }
    const user = Meteor.user()
    var username=""
    if (user)
      username = user.username
    const enclave = this.state.enclaves.find(
      e => e.pubKey === this.state.pubKey
    );
    const url = undefined === enclave ? undefined : enclave.url;

    // unique enclaves
    var uniqueEnclaves = this.state.enclaves.filter((enclave1, index, self) =>
      index === self.findIndex((enclave2) => (enclave2.pubKey === enclave1.pubKey))
    );

    const enclave_list  = uniqueEnclaves.filter(e => {
      if (e.hosts) {
        return e.hosts.find(host =>
          host.host === username
        );
      }
    });

    return (
      <div className="container-fluid">
        <div className="row">
          {this.editDataModal()}
          <div className="navbar-base">
            <NavBar />
          </div>
          <div className="main-content">
            <h1>View Your Access to Remote Data</h1>
            <p>To get started, please select a data collection.</p>
            <div className="form">
              <Form>
                <Form.Row>
                  <Form.Group sm={4} as={Col} controlId="enclaveAcc">
                    <Form.Control
                      as="select"
                      value={this.state.pubKey}
                      onChange={this.selectEnclave}
                    >
                      {listEnclaves(enclave_list)}
                    </Form.Control>
                  </Form.Group>
                </Form.Row>
              </Form>
            </div>
            <div class="moduleArea">
              <div class="row tableHeader">
                <div class="col-md-2">Actions</div>
                <div class="col-md-2">File Name</div>
                <div class="col-md-2">File Size</div>
                <div class="col-md-3">Tags</div>
                {
                  (Meteor.settings.public.mode === "sales" ?
                    <div class="col-md-2">Access Expiration</div>
                  : null)
                }
              </div>
            </div>
            {this.state.activeDataset === active_dataset.PUBLIC &&
              this.state.publicData.map((pd, index) => (
                <DataList
                  key={index.toString()}
                  path={"/data/ "}
                  pathComment={"Enclave Name: " + pd.name}
                  data={undefined === pd.leaves ? [] : pd.leaves}
                />
              ))}
            {this.state.activeDataset !== active_dataset.PUBLIC && (
              <DataList
                path={"/data/ "}
                pathComment={
                  "Enclave:" + this.state.pubKey.substring(0, 19) + "..."
                }
                data={this.state.myData}
                editData={this.handleShowDataInfo}
              />
            )}
          </div>
        </div>
      </div>
    );
  }
}
export default withRouter(AccessData);
