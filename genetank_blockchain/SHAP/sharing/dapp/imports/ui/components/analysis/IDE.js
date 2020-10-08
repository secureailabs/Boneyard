import React, { Component } from "react";
import { Button, Form, Modal, Tab, Tabs } from "react-bootstrap";
import { Shell } from "/imports/ui/components/analysis/Shell.js";
import { Meteor } from "meteor/meteor";
import { DappFormGroup } from "../../../api/ui/DappFormGroup";
import { ProjectTree } from "./DirectoryTree";
import { MdFormatListBulleted, MdClose } from "react-icons/md";

export class IDE extends Component {
  constructor(props) {
    super(props);
    this.getModels = this.getModels.bind(this);
    //this.getLeafModels = this.getLeafModels.bind(this);
    this.makeModelsList = this.makeModelsList.bind(this);
    this.projectModal = this.projectModal.bind(this);
    this.handleShowNewModel = this.handleShowNewModel.bind(this);
    this.handleCloseNewModel = this.handleCloseNewModel.bind(this);
    this.handleShowUpdateModel = this.handleShowUpdateModel.bind(this);
    this.handleCloseUpdateModel = this.handleCloseUpdateModel.bind(this);
    this.existModel = this.existModel.bind(this);
    this.newModelFile = this.newModelFile.bind(this);
    this.updateModelFile = this.updateModelFile.bind(this);
    this.getModelCode = this.getModelCode.bind(this);
    this.deleteModel = this.deleteModel.bind(this);
    this.formList = this.formList.bind(this);
    this.handleClose = this.handleClose.bind(this);
    this.runJupyter = this.runJupyter.bind(this);

    this.state = {
      models: [],
      showNewModel: false,
      showUpdateModel: false,
      currentModel: { name: "", federated: false, code: {}, enclave: {} },
      //currentEnclaveName: "",
      modelNames: [],
      key: "",
      activeModels: [],
      modified: false
    };
  }

  componentDidMount() {}

  componentWillReceiveProps(nextProps, nextContext) {
    //if (this.props.trainerEthAdd !== "" && nextProps !== this.props.trainerEthAdd) {
    this.getModels();
    //}
  }

  runJupyter(ethAcc) {
    Meteor.call("userRunJupyterlab", ethAcc, (error, result) => {
      if (error) {
        alert("Error: " + error);
      } else if (result.error) {
        alert("Error: " + result.error);
      } else {
        alert(result.result.replace(/\n/g, ","));
      }
    });
  }

  getModels() {
    Meteor.call("getProjectsInfo", (err, res) => {
      if (err) alert(err.toString());
      else {
        this.setState({
          models: res,
          modelNames: this.makeModelsList(res),
          showNewModel: false,
          currentModel: { name: "", federated: false, code: {}, enclave: {} },
          //currentEnclaveName: "",
          key: "",
          activeModels: []
        });
      }
    });
  }

  makeModelsList(raw, result = []) {
    for (var i = 0; i < raw.length; i++) {
      result.push({ name: raw[i].name, federated: raw[i].federated });
    }
    return result;
  }

  getModelCode(modelName) {
    let model = this.state.models.find(model => {
      return (model.name = modelName);
    });
    return model.code;
  }

  deleteModel(name) {
    if (!confirm("Deleting " + name + " !!! press OK to continue")) return;

    this.setState({
      models: this.state.models.filter(model => {
        return !(model.name === name);
      }),
      modelNames: this.state.modelNames.filter(modelName => {
        return !(modelName.name === name);
      }),
      activeModels: []
    });

    Meteor.call("removeProjectInfo", name, (result, error) => {
      if (error) {
        console.log(error.toString());
      } else {
        console.log("model removed");
      }
    });
  }

  projectModal = newModal => {
    var title = "Enter the name of the new project";
    var show = this.state.showNewModel;
    var onHide = this.handleCloseNewModel;
    if (!newModal) {
      title = "Properties of the project";
      show = this.state.showUpdateModel;
      onHide = this.handleCloseUpdateModel;
    }
    return (
      <Modal show={show} onHide={onHide}>
        <div
          style={{
            borderStyle: "solid",
            borderWidth: "1px",
            borderColor: "white"
          }}
        >
          <Modal.Header
            style={{ backgroundColor: "rgba(50, 50, 51, 1.0)", color: "white" }}
            closeButton
          >
            <Modal.Title>{title}</Modal.Title>
          </Modal.Header>
          <Modal.Body
            style={{ backgroundColor: "rgba(50, 50, 51, 1.0)", color: "white" }}
          >
            <Form>
              <DappFormGroup
                id="modelName"
                label="Project Name"
                value={this.state.currentModel.name}
                type="text"
                onChange={event => {
                  if (!newModal) return;
                  let currentModel = this.state.currentModel;
                  currentModel.name = event.target.value;
                  this.setState({ currentModel: currentModel });
                }}
                placeholder="Name of the project"
              />
              <DappFormGroup
                id="federated"
                label="Is a federated learning model"
                value={this.state.currentModel.federated}
                type="checkbox"
                onChange={event => {
                  let currentModel = this.state.currentModel;
                  currentModel.federated = event.target.checked;
                  this.setState({ currentModel: currentModel });
                }}
              />
              <DappFormGroup
                id="enclave"
                label="Select an enclave"
                value={this.state.currentModel.enclave.name}
                list={this.formList()}
                as="select"
                onChange={event => {
                  const fn = event.target.value;
                  //let newstate = Object.assign({}, this.state);
                  //newstate.currentEnclaveName = fn;
                  let currentModel = this.state.currentModel;
                  currentModel.enclave = this.props.enclaveInfo.find(item => {
                    return item.name === fn;
                  });
                  this.setState({ currentModel: currentModel });
                }}
              />
            </Form>
          </Modal.Body>
          <Modal.Footer
            style={{ backgroundColor: "rgba(50, 50, 51, 1.0)", color: "white" }}
          >
            {newModal && (
              <Button variant="success" onClick={this.newModelFile}>
                Create a New Project
              </Button>
            )}
            {!newModal && (
              <Button variant="success" onClick={this.updateModelFile}>
                Update Project
              </Button>
            )}
          </Modal.Footer>
        </div>
      </Modal>
    );
  };

  formList() {
    let options = [<option key="dummy">Please Select</option>];
    this.props.enclaveInfo.forEach(item => {
      options.push(<option key={item.name}>{item.name}</option>);
    });
    return options;
  }

  newModelFile(event) {
    const currentModel = this.state.currentModel;
    if (this.existModel(currentModel.name)) {
      alert("Model exist");
      return;
    }
    if (!currentModel.name.trim()) {
      alert("Model name can not be blank.");
      return;
    }

    let newModelNames = [
      ...this.state.modelNames,
      { name: currentModel.name, federated: currentModel.federated }
    ];
    let newModels = [...this.state.models, currentModel];

    let openedModels = [this.state.currentModel];
    this.setState(
      {
        activeModels: openedModels,
        key: openedModels.length > 0 ? this.state.currentModel.name : "Add",
        modified: true,
        modelNames: newModelNames,
        models: newModels
      },
      this.handleCloseNewModel
    );
  }

  updateModelFile(event) {
    var currentModel = this.state.currentModel;
    if (!this.existModel(currentModel.name)) {
      alert("Model doesn't exist");
      return;
    }
    if (!currentModel.name.trim()) {
      alert("Model name can not be blank.");
      return;
    }
    currentModel.enclave = this.props.enclaveInfo.find(
      encl => encl.name === currentModel.enclave.name
    );

    let obj = this.state.models.find(item => {
      return item.name === currentModel.name;
    });
    if (!obj) obj = this.state.activeModels[0];
    Object.assign(obj, currentModel);
    this.setState(
      {
        activeModels: [obj],
        modified: true
      },
      this.handleCloseUpdateModel
    );
  }

  existModel(filename) {
    const data = this.state.modelNames;
    return undefined !== data.find(x => x.name === filename);
  }

  handleCloseNewModel() {
    this.setState({
      showNewModel: false,
      currentModel: { name: "", federated: false, code: {}, enclave: {} },
      currentModelName: ""
    });
  }

  handleCloseUpdateModel() {
    this.setState({
      showUpdateModel: false,
      currentModel: { name: "", federated: false, code: {}, enclave: {} },
      currentModelName: ""
    });
  }

  handleShowNewModel() {
    this.setState({ showNewModel: true });
  }

  handleShowUpdateModel() {
    this.setState({
      showUpdateModel: true,
      currentModel: { ...this.state.activeModels[0] }
    });
  }

  handleTabSelect(key) {
    if (key === "Add") {
      this.handleShowNewModel();
    } else if (key === "Properties") {
      this.handleShowUpdateModel();
    } else {
      this.setState({
        key: key,
        currentModel: this.state.activeModels.find(i => i.name === key)
      });
    }
  }

  handleClose(e, key) {
    e.nativeEvent.stopImmediatePropagation();
    let index = this.state.activeModels.findIndex(i => i.name === key);
    let old = this.state.activeModels[index].name;
    let newCurrentModel, newKey;
    if (index === this.state.activeModels.length - 1 && index === 0) {
      newKey = "Add";
      newCurrentModel = { name: "", federated: false, code: {}, enclave: {} };
    } else if (index === this.state.activeModels.length - 1) {
      newKey = this.state.activeModels[index - 1].name;
      newCurrentModel = this.state.activeModels[index - 1];
    } else {
      newKey = this.state.activeModels[index + 1].name;
      newCurrentModel = this.state.activeModels[index + 1];
    }
    let newActiveModels = Object.assign([], this.state.activeModels);
    newActiveModels = newActiveModels.filter(i => i.name !== old);

    this.setState(
      {
        currentModel: newCurrentModel,
        activeModels: newActiveModels
      },
      () => {
        this.setState({
          key: newKey
        });
      }
    );
  }

  handleCursorMove = data => {
    let obj = this.state.models.find(item => {
      return item.name === data.name;
    });
    let openedModels = this.state.activeModels;
    if (openedModels && openedModels[0] && openedModels[0].name === data.name)
      return;
    openedModels = [obj];
    this.setState({
      activeModels: openedModels,
      key: data.name,
      modified: false
    });
  };

  updateModelsAfterSave = data => {
    let models = this.state.models;
    const model = models.find(m => m.name === data.name);
    model.code = data.code;
    this.setState({ models: models, modified: false });
  };

  render() {
    return (
      <div style={{ display: "flex" }}>
        <div
          className="col-2"
          style={{
            marginTop: "10px",
            marginBottom: "10px",
            marginRight: "40px",
            marginLeft: "-40px"
          }}
        >
          <h3 style={{ color: "grey" }}>PROJECTS</h3>
          <ProjectTree
            projects={this.state.modelNames}
            call={this.handleCursorMove}
          />
        </div>
        <div className="col-8">
          <Button
            onClick={() => this.handleTabSelect("Add")}
            variant="secondary"
            style={{ backgroundColor: "#e5e5e5", color: "black" }}
            className="upload-btn"
          >
            <b>New Project </b>
            <i className="fas fa-plus"></i>
          </Button>
          <Button
            onClick={() => this.handleTabSelect("Properties")}
            variant="secondary"
            style={{ backgroundColor: "#e5e5e5", color: "black" }}
            className="upload-btn"
          >
            <b>Project Properties</b>
          </Button>
          <Button
            onClick={() =>
              this.runJupyter(this.props.trainerEthAdd)
            }
            variant="secondary"
            style={{ backgroundColor: "#e5e5e5", color: "black" }}
            className="mr-2"
          >
            <b>Run Jupyter</b>
          </Button>

          <Tabs
            className="data-list tab-color"
            style={{ borderBottom: "0px" }}
            onSelect={key => this.handleTabSelect(key)}
            activeKey={this.state.key}
          >
            {this.state.activeModels.map(model => (
              <Tab
                key={model.name}
                eventKey={model.name}
                title={
                  <>
                    {model.name}
                    <Button
                      style={{
                        backgroundColor: "transparent",
                        border: "0px",
                        marginLeft: "40px",
                        display: "inline-block"
                      }}
                      size="sm"
                      onClick={(e, key = model.name) => {
                        this.handleClose(e, key);
                      }}
                    >
                      <MdClose />
                    </Button>
                  </>
                }
              >
                <Shell
                  dataName={this.props.enclaveInfo[0].name}
                  name={model.name}
                  code={model.code}
                  federated={model.federated}
                  enclave={model.enclave}
                  ethAcc={this.props.trainerEthAdd}
                  deleteModel={this.deleteModel}
                  updateModels={this.updateModelsAfterSave}
                  modified={this.state.modified}
                />
              </Tab>
            ))}
          </Tabs>
        </div>
        {this.projectModal(true)}
        {this.projectModal(false)}
      </div>
    );
  }
}
