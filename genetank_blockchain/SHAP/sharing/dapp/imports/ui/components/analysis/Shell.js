import React, { Component } from "react";
import { Meteor } from "meteor/meteor";
import { FiDownload } from "react-icons/fi";
import { Button, Form, Accordion, Card } from "react-bootstrap";
import { Editor } from "./Editor";

import "/imports/api/splitpane.css";

export class Shell extends Component {
  constructor(props) {
    super(props);
    this.timer = null;
    this.state = {
      runLoading: false,
      scriptDone: false,
      timer: null,
      subList: [],
      codes: { ...this.props.code },
      modelResult: "",
      tidx: -1,
      modified: this.props.modified,
    };

    this.runScript = this.runScript.bind(this);
    this.handleInput = this.handleInput.bind(this);
    this.createCodePanel = this.createCodePanel.bind(this);
    this.startTimer = this.startTimer.bind(this);
    this.stopTimer = this.stopTimer.bind(this);
    this.checkResult = this.checkResult.bind(this);
    this.addDownload = this.addDownload.bind(this);
    this.handleDownload = this.handleDownload.bind(this);
    this.createCards = this.createCards.bind(this);
    this.getSubs = this.getSubs.bind(this);
    this.handleCodeChange = this.handleCodeChange.bind(this);
  }

  componentDidMount() {
    this.getSubs();
  }

  saveChange = (willUnmount) => {
    const data = {
      name: this.props.name,
      federated: this.props.federated,
      code: this.state.codes,
      enclave: this.props.enclave
    };
    Meteor.call("addProjectInfo", data, (error, result) => {
      if (error) console.log(error.toString());
      else {
        console.log("project updated in the database");
        this.props.updateModels(data);
        if (!willUnmount)
          this.setState({ modified: false });
      }
    });
  };

  componentWillUnmount() {
    if (this.state.modified || this.props.modified)
      if (
        confirm(
          "Do you want to save your changes? (OK to save, Cancel to discard)"
        )
      )
        this.saveChange(true);
  }

  getSubs() {
    Meteor.call(
      "getSublistByHub",
      this.props.enclave.pubKey,
      (error, result) => {
        if (error) alert("Load sub enclave keys failed.");
        else if (result.error) alert("Error: " + result.error);
        else {
          let codes = this.state.codes;
          let modified = this.state.modified;
          if (Object.keys(codes).length === 0) {
            if (this.props.federated)
              result.result.map(sub => {
                codes[sub.name] = "";
              });
            else codes[this.props.enclave.name] = "";
            modified = true;
          }
          this.setState({
            subList: result.result,
            codes: codes,
            modified: modified
          });
        }
      }
    );
  }

  handleInput(event) {
    this.setState({
      modelCode: event.target.value,
      modified: true
    });
  }

  startTimer(interval) {
    this.timer = setInterval(() => {
      this.checkResult();
    }, interval);
  }

  stopTimer() {
    clearInterval(this.timer);
  }

  checkResult() {
    Meteor.call(
      "getTaskStatus",
      this.props.enclave.pubKey,
      this.props.ethAcc,
      this.state.tidx,
      (err, result) => {
        if (err) {
          this.setState({ modelResult: err });
          this.stopTimer();
          this.setState({ runLoading: false, scriptDone: true });
        } else if (result[0].taskStatus === "FINISHED") {
          if (result.error) this.setState({ modelResult: result.error });
          else {
            let retStr = JSON.parse(result[0].output);
            this.setState({ modelResult: retStr.result });
          }
          this.stopTimer();
          this.setState({ runLoading: false, scriptDone: true });
        }
      }
    );
  }

  saveCode2Enclave(pubKey, ethAcc, name) {
    let subEnclaves = [];
    let hubCode = this.state.codes[this.props.enclave.name];
    if (this.props.federated)
      this.state.subList.forEach(item => {
        if (item.name !== this.props.enclave.name) {
          subEnclaves.push({
            key: item.key,
            code: this.state.codes[item.name]
          });
        }
      });
    Meteor.call(
      "userSaveCodes",
      {
        title: name,
        hubContent: hubCode,
        subEnclaves: subEnclaves
      },
      pubKey,
      ethAcc,
      (error, result) => {
        if (error) {
          console.log(error);
        } else console.log(result);
      }
    );
  }

  runScript(pubKey, ethAcc, name) {
    if (this.state.modified) {
      this.saveCode2Enclave(
        this.props.enclave.pubKey,
        this.props.ethAcc,
        this.props.name
      );
      this.saveChange();
      this.setState({ modified: false });
    }
    this.setState({ runLoading: true });
    if (!this.props.federated) {
      const name = this.props.enclave.name;
      Meteor.call(
        "userRunCode",
        { title: name, content: this.state.codes[name] },
        pubKey,
        ethAcc,
        this.props.dataName,
        (error, result) => {
          if (error) {
            console.log("error:", error);
            if ("object" === typeof error) error = JSON.stringify(error);
            this.setState({ modelResult: error });
            this.setState({ runLoading: false, scriptDone: true });
          } else if (result.error) {
            this.setState({ modelResult: result.error });
            this.setState({ tidx: result.tidx });
          } else {
            console.log("Run model returned.");
            this.setState({ modelResult: result.result });
            this.setState({ runLoading: false, scriptDone: true });
            this.setState({ tidx: result.tidx });
          }
        }
      );
    } else {
      let subKeys = [];
      let subCodes = [];
      let hubCode = this.state.codes[this.props.enclave.name];
      this.state.subList.forEach(item => {
        if (item.name !== this.props.enclave.name) {
          subKeys.push(item.key);
          subCodes.push(this.state.codes[item.name]);
        }
      });
      Meteor.call(
        "userFederated",
        {
          title: name,
          subContent: subCodes,
          sKeys: subKeys,
          hubContent: hubCode
        },
        pubKey,
        ethAcc,
        this.props.dataName,
        (error, result) => {
          if (error) {
            console.log("error:", error);
            if ("object" === typeof error) error = JSON.stringify(error);
            this.setState({ modelResult: error });
            this.setState({ runLoading: false, scriptDone: true });
          } else if (result.error) {
            this.setState({ modelResult: result.error });
            this.setState({ tidx: result.tidx }, () => {
              this.startTimer(3000);
            });
          } else {
            console.log("Run model returned.");
            this.setState({ tidx: result.tidx }, () => {
              this.startTimer(3000);
            });
          }
        }
      );
    }
  }

  handleCodeChange(code, name) {
    let newcodes = this.state.codes;
    newcodes[name] = code;
    this.setState({ codes: newcodes, modified: true });
  }

  createCodePanel(subName) {
    return (
      <Editor
        name={subName}
        call={this.handleCodeChange}
        code={this.state.codes[subName]}
      />
    );
  }

  addDownload() {
    if (!this.state.runLoading && this.state.scriptDone)
      return (
        <Button variant="success" className="mr-2">
          <FiDownload />
          Download
        </Button>
      );
    else return <></>;
  }

  handleDownload() {
    //TODO
  }

  createCards() {
    return (
      <Accordion defaultActiveKey="0">
        {Object.keys(this.state.codes).map(item => {
          return (
            <Card
              key={item}
              style={{
                backgroundColor: "#263238",
                color: "white",
                border: "1px solid #ced4da"
              }}
            >
              <Card.Header>
                <Accordion.Toggle
                  as={Card.Header}
                  variant="secondary"
                  eventKey={item}
                >
                  {item}
                </Accordion.Toggle>
              </Card.Header>
              <Accordion.Collapse eventKey={item}>
                <Card.Body>{this.createCodePanel(item)}</Card.Body>
              </Accordion.Collapse>
            </Card>
          );
        })}
      </Accordion>
    );
  }

  render() {
    return (
      <div style={{ marginTop: "10px", marginBottom: "10px" }}>
        {this.createCards()}

        <Form.Control
          as="textarea"
          rows="10"
          style={{ backgroundColor: "rgba(64, 64, 65, 1.0)", color: "white" }}
          disabled
          value={this.state.scriptDone ? this.state.modelResult : ""}
        />

        <Button
          variant="primary"
          className="mr-2"
          onClick={() =>
            this.runScript(
              this.props.enclave.pubKey,
              this.props.ethAcc,
              this.props.name
            )
          }
        >
          <i className="fas fa-play"></i> Run
        </Button>
        <Button
          variant="secondary"
          className="mr-2"
          onClick={() => {
            this.saveCode2Enclave(
              this.props.enclave.pubKey,
              this.props.ethAcc,
              this.props.name
            );
            this.saveChange();
          }}
        >
          <i className="far fa-save"></i> Save
        </Button>
        <Button
          variant="danger"
          className="mr-2"
          onClick={() => this.props.deleteModel(this.props.name)}
        >
          <i className="far fa-trash-alt"></i> Delete
        </Button>
        {this.state.runLoading ? (
          <span>
            <img src="/loading.gif" width="20" height="20" />
          </span>
        ) : null}
        {this.addDownload()}
      </div>
    );
  }
}
