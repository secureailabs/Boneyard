import { Meteor } from "meteor/meteor";
import React, { Component } from "react";
import { withRouter } from "react-router-dom";
import { Redirect } from "react-router";
import NavBar from "/imports/ui/components/NavBar";
import {
  Button,
  ButtonGroup,
  Col,
  Form,
} from "react-bootstrap";
import { listEnclaves } from "../../../api/usercfg";
import { roles } from "/imports/api/enum.js";
import { DataList, getLeafDatasets } from "../datasets/View.js";

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
      ethAcc: "" //Current Ethereum account for accessing the enclave
    };
    this.loadData = this.loadData.bind(this);
    this.getPublicData = this.getPublicData.bind(this);
    this.selectEnclave = this.selectEnclave.bind(this);
    this.setDefaultEnclave = this.setDefaultEnclave.bind(this);
  }

  componentDidMount() {
    Meteor.call("userGetEnclaveList", (err, result) => {
      if (err) alert(err);
      else if (result.error) alert(result.error);
      else {
        this.setState({ enclaves: result.enclaveList });
        if (undefined != result.enclaveList)
          if (undefined != result.enclaveList[0])
            this.setDefaultEnclave(result.enclaveList[0].pubKey);
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
            if (
              validPubData.length > 0 &&
              !(
                validPubData.length === 1 &&
                validPubData[0].leaves.length === 1 &&
                validPubData[0].leaves[0].name === "sharedin"
              )
            )
              this.setState({
                activeDataset: active_dataset.PUBLIC,
                publicData: validPubData
              });
          } else {
            alert(res.error);
          }
        } else {
          alert("View.js :: Get Public data:: Error");
        }
      }
    );
  }

  loadData(activeData) {
    if (activeData == active_dataset.PUBLIC) {
      if (this.state.publicData.length === 0) {
        this.getPublicData();
      } else {
        this.setState({
          activeDataset: active_dataset.PUBLIC
        });
      }
    } else {
      Meteor.call(
        "getEnclaveInfo",
        this.state.pubKey,
        this.state.ethAcc,
        "/data",
        (err, res) => {
          if (err) alert(err.toString());
          else if (res.error) alert(res.error);
          else {
            let personalDataset = getLeafDatasets(res.result.children);
            personalDataset.map((dataset, i) => {
              dataset.id = i.toString();
            });
            this.setState({
              activeDataset: active_dataset.PERSONAL,
              myData: personalDataset
            });
          }
        }
      );
    }
  }

  setDefaultEnclave(pubKey) {
    var ethAcc = this.state.ethAcc;
    const enclave = this.state.enclaves.find(e => e.pubKey === pubKey);
    if (undefined !== enclave) {
      const role = enclave.roleInfos.find(r => r.role === roles.HOST);
      if (undefined !== role) ethAcc = role.ethAddr;
    }
    //console.log("Public key updated")
    this.setState(
      {
        pubKey: pubKey,
        ethAcc: ethAcc
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
    if (undefined !== pubKey && "Select" !== pubKey)
      this.setDefaultEnclave(pubKey);
  }

  handleShowDataInfo() {}

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

    return (
      <div className="container-fluid">
        <div className="row">
          <div className="navbar-base">
            <NavBar />
          </div>
          <div className="main-content">
            <div className="activetab">
              <ButtonGroup className="mr-2">
                <Button
                  variant="outline-light"
                  className={
                    this.state.activeDataset === active_dataset.PUBLIC
                      ? "active"
                      : ""
                  }
                  onClick={() => this.loadData(active_dataset.PUBLIC)}
                >
                  Public Dataset
                </Button>
                <Button
                  variant="outline-light"
                  className={
                    this.state.activeDataset === active_dataset.PERSONAL
                      ? "active"
                      : ""
                  }
                  onClick={() => this.loadData(active_dataset.PERSONAL)}
                >
                  My Dataset
                </Button>
              </ButtonGroup>
              <div className="divider"> </div>
            </div>
            <div className="form">
              <Form>
                <Form.Row>
                  <Form.Group sm={4} as={Col} controlId="enclaveAcc">
                    <Form.Label style={{ color: "white" }}>
                      Select a Hub Enclave
                    </Form.Label>
                    <Form.Control
                      as="select"
                      value={this.state.pubKey}
                      onChange={this.selectEnclave}
                      style={{ backgroundColor: "black", color: "white" }}
                    >
                      {listEnclaves(this.state.enclaves.filter(encl=>encl.host===username))}
                    </Form.Control>
                  </Form.Group>
                  <Form.Group sm={8} as={Col} controlId="EnclaveInfo">
                    <Form.Label style={{ color: "white" }}>
                      <span>
                        Public key:{this.state.pubKey.substring(0, 19) + "..."}
                      </span>
                    </Form.Label>
                    <Form.Label style={{ color: "white" }}>
                      <span>URL:{url}</span>
                    </Form.Label>
                  </Form.Group>
                </Form.Row>
              </Form>
            </div>
            {this.state.activeDataset === active_dataset.PUBLIC &&
              this.state.publicData.map((pd, index) => (
                <DataList
                  key={index.toString()}
                  path={"/data/ "}
                  pathComment={"Enclave Name: " + pd.name}
                  data={undefined === pd.leaves ? [] : pd.leaves}
                  editData={this.handleShowDataInfo}
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
