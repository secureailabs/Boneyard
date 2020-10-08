import { Meteor } from "meteor/meteor";
import React, { Component } from "react";
import { withRouter } from "react-router-dom";
import { Redirect } from "react-router";
//import "react-table/react-table.css";
import {
  Container,
  Row,
  Col,
  OverlayTrigger,
  Tooltip,
  Modal,
  Button,
  Form
} from "react-bootstrap";
import { DappFormGroup } from "/imports/api/ui/DappFormGroup.js";
import { listEnclaves } from "../../../api/usercfg";
import ListItem from "./ListItem";
import NavBar from "/imports/ui/components/NavBar";
import { roles } from "/imports/api/enum.js";

//import eu from 'ethereumjs-util'
//var eu = require('ethereumjs-util')

function getEthFromPubkey(pubkey) {
  const newPubkey = "0x" + pubkey;
  //const newPubkey = "04"+pubkey
  //const upk_buf = new Buffer(newPubkey, 'hex')
  //const addr_buf = eu.pubToAddress(upk_buf.slice(1,65))

  //const addr = addr_buf.toString('hex')
  const addr = web3.sha3(newPubkey, { encoding: "hex" });
  const ethAddr = addr.slice(-40);
  return ethAddr;
}

let here;
//const LARGE_FILE_SIZE_THRESHOLD = 4194304;
const LARGE_FILE_SIZE_THRESHOLD = 100 * 1024;
//const LARGE_FILE_SIZE_THRESHOLD = 1*1024;
const MAX_PAGE_SIZE = LARGE_FILE_SIZE_THRESHOLD; //48*1024
const MAX_FILE_SIZE = 500000000;
const ALLOWED_FILE_TYPES = ['text/csv']

export function DataList(props) {
  if (props.value === "") {
    return (
      <ul style={{ listStyleType: "none" }} className="data-list">
      </ul>
    );
  }

  return (
    <ul style={{ listStyleType: "none" }} className="data-list">
      <li className="group2" key={props.pathComment}>
        <div className="buttoncopy8">
          <div className="rectangle1"></div>
        </div>
        <div className="datatitled123456">
          <span className="span1">
            {props.path}({props.pathComment})
          </span>
        </div>
        <div className="bitmap">
          <div className="mask"></div>
          <div className="group31">
            <img
              src="img/share-data-color-black-copy-4@2x.png"
              className="group32"
            />
          </div>
        </div>
      </li>
      {props.data.map((listValue, index) => {
        listValue.id = index.toString();
        return (
          <li
            style={{ position: "relative", left: "40px" }}
            className="group2"
            key={listValue.id}
          >
            <ListItem
              listValue={listValue}
              setSelectedDatabaseInfo={props.setSelectedDatabaseInfo}
              deleteDataset={props.deleteDataset}
              editData={props.editData}
              pubKey={props.pubKey}
              ethAcc={props.ethAcc}
            />
          </li>
        );
      })}
    </ul>
  );
}

function updateDates(info) {
  if (undefined !== info.startTime) {
    info.startDate = new Date(info.startTime).toISOString().slice(0, 10);
  } else info.startDate = new Date().toISOString().slice(0, 10);
  if (undefined !== info.endTime)
    info.endDate = new Date(info.endTime).toISOString().slice(0, 10);
  else info.endDate = new Date().toISOString().slice(0, 10);
}

export function getLeafDatasets(nodes, path="", result = []) {
  for (var i = 0, length = nodes.length; i < length; i++) {
    if (nodes[i].leaf && nodes[i].module !== "data") {
      if (undefined !== nodes[i].info) {
        const dataset = nodes[i].info;
        if (undefined !== dataset.shareInfo)
          dataset.shareInfo.map(info => {
            updateDates(info);
          });
        updateDates(dataset);
        nodes[i].info.name = path+nodes[i].module
        result.push(nodes[i].info);
      } else {
        var data = Object.assign({}, defaultData);
        data.stats = Object.assign({}, defaultData.stats);
        data.name = nodes[i].module;
        if (data.name !== "sharedout" && data.name !== "sharedin")
          result.push(data);
      }
    } else if (nodes[i].children !== undefined) {
      result = getLeafDatasets(nodes[i].children, nodes[i].module+"/", result);
    }
  }
  return result;
}

const defaultData = {
  id: "0",
  name: "default data",
  size: "0",
  type: "csv",
  license: "Open source",
  descriptions: "This is a test dataset.",
  tags: "microbiome, bioinformatics, pharma",
  keywords: "",
  stats: {
    score: "5",
    ratings: "10k",
    comments: "10",
    views: "100k"
  },
  owned: true
};

class View extends Component {
  constructor(props) {
    super(props);
    here = this;
    this.state = {
      data: [],
      selected: new Set(),
      enclaves: [],
      pubKey: "", //Current Enclave's public key
      ethAccounts: [],
      ethAcc: "", //Current Ethereum account for accessing the enclave
      //ethereumAddress: '',
      //selectedDatasetId: '',
      showUploadData: false,
      showEditData: false,
      uploadFile: "",
      dataDescriptions: "",
      dataTags: "",
      anonymized: false,
      currentDataId: 0,
      dataName: "",
      progress: "0",
      isSmallDataset: true,
      isUploadingStarted: false,
      privacyKvalue: 0,
      privacyESdp: "K-Anonomous",
      userAccess: "",
      userAccessEnclavePk: "",
      userAccessStartDate: new Date().toISOString().slice(0, 10),
      userAccessEndDate: new Date().toISOString().slice(0, 10),
      userList: [],
      userEnclaveList: [],
      value: ""
    };
    //this.selectDataset = this.selectDataset.bind(this);
    this.isChecked = this.isChecked.bind(this);
    this.getCfg = this.getCfg.bind(this);
    this.selectEnclave = this.selectEnclave.bind(this);
    //this.selectEthAcc = this.selectEthAcc.bind(this);
    this.getEthAccounts = this.getEthAccounts.bind(this);
    this.getLeafEthAcc = this.getLeafEthAcc.bind(this);
    //this.getLeafDatasets = this.getLeafDatasets.bind(this);
    this.getPersonalDatasets = this.getPersonalDatasets.bind(this);
    this.getDatasetForOwner = this.getDatasetForOwner.bind(this);
    this.handleCloseUploadData = this.handleCloseUploadData.bind(this);
    this.handleCloseEditData = this.handleCloseEditData.bind(this);
    this.handleShowUploadData = this.handleShowUploadData.bind(this);
    this.handleShowEditData = this.handleShowEditData.bind(this);
    this.handleUserAccess = this.handleUserAccess.bind(this);
    this.handleUserEnclave = this.handleUserEnclave.bind(this);
    this.uploadData = this.uploadData.bind(this);
    this.editData = this.editData.bind(this);
    this.existFile = this.existFile.bind(this);
    //this.getDonorAccounts = this.getDonorAccounts.bind(this);
    this.deleteDataset = this.deleteDataset.bind(this);
    this.uploadLargeData = this.uploadLargeData.bind(this);
    this.startPaginationUpload = this.startPaginationUpload.bind(this);
    this.registerDataOnBC = this.registerDataOnBC.bind(this);
    // this.getTempEthAccount = this.getTempEthAccount(this);
  }

  handleUserAccess(event) {
    const user = event.target.value;
    this.updateUserEnclaveList(user);
  }

  updateUserEnclaveList = user => {
    Meteor.call("userGetEnclaveList", user, (err, result) => {
      if (err) alert(err);
      else if (result.error) alert(result.error);
      else {
        var userEnclavePk = this.state.userAccessEnclavePk;
        if (undefined === result.enclaveList) result.enclaveList = [];
        var enclaveList = result.enclaveList.filter(encl => {
          if (encl.hosts) {
            return encl.hosts.find(host =>
              host.host === user
            );
          }
        });
        if (undefined === enclaveList) enclaveList = [];
        const encl = enclaveList.find(encl => encl.pubKey === userEnclavePk);
        if (undefined === encl) {
          if (undefined !== enclaveList[0])
            userEnclavePk = enclaveList[0].pubKey;
        }
        this.setState({
          userAccess: user,
          userEnclaveList: enclaveList,
          userAccessEnclavePk: userEnclavePk
        });
        this.loadShareInfo();
      }
    });
  };

  handleUserEnclave(event) {
    this.setState({ userAccessEnclavePk: event.target.value });
    //this.loadShareInfo()
  }

  componentDidMount() {
    this.getCfg();
  }

  handleOnShow = () => {
    Meteor.call("userGetUserList", (err, result) => {
      if (err) alert(err);
      else if (result.error) alert(result.error);
      else {
        var user = this.state.userAccess;
        if (undefined !== result.userList) {
          user = result.userList.find(u => user === u);
          if (undefined === user) user = result.userList[0];
        }
        this.setState({ userList: result.userList, userAccess: user });
        if (undefined !== user) {
          this.updateUserEnclaveList(user);
        }
      }
    });
  };

  handleShowUploadData() {
    this.setState({ showUploadData: true, dataDescriptions: "", dataTags: "" });
  }

  handleShowEditData(dataId) {
    const dataset = this.state.data.find(d => d.id === dataId);
    if (undefined === dataset) return;
    this.setState({
      showEditData: true,
      currentDataId: dataId,
      dataName: dataset.name,
      dataDescriptions: dataset.descriptions,
      dataTags: dataset.tags
    });
    this.handleOnShow();
  }

  registerDataOnBC(file) {
    //  TODO: Maybe add username if needed when user stuff is finalized
    // unable to do that now from front end, along with keeping large data upload stable
    let dataInfo = {
      dataName: file.name,
      ethAddress: this.state.ethAcc,
      enclavePubKey: this.state.pubKey,
      fileSize: file.size,
      timestamp: new Date().getTime()
    };
    Meteor.call(
      "callRegisterDataByOwner",
      this.state.ethAcc,
      dataInfo,
      (error, result) => {
        if (error) {
          alert("error:" + error);
        } else {
          console.log("added");
        }
      }
    );
  }

  handleCloseEditData(uploadSuccess = false) {
    this.setState({
      showEditData: false
    });
  }

  handleCloseUploadData(uploadSuccess = false) {
    if (uploadSuccess) {
      const file = this.state.uploadFile;
      var filename = file.name;
      if (this.state.anonymized)
        filename = "anonymized/"+filename;
      const dsInfo = {
        name: filename,
        descriptions: this.state.dataDescriptions,
        tags: this.state.dataTags,
        size: file.size
      };
      Meteor.call("addDatasetInfo", dsInfo, (error, result) => {
        if (error) {
          alert("error:" + error);
        } else {
          console.log("added");
        }
      });
      //this.registerDataOnBC(file);
    }
    this.setState({
      showUploadData: false,
      dataDescriptions: "",
      dataTags: "",
      progress: "0",
      isSmallDataset: true,
      isUploadingStarted: false
    });
  }

  existFile(filename) {
    var data = this.state.data;
    const dataset = data.find(d => d.name === filename);
    return undefined !== dataset;
  }

  editData() {
    this.handleCloseEditData();
  }

  uploadData(event) {
    const file = this.state.uploadFile;
    if (!file) {
      alert("ERROR: Invalid File.")
      return;
    }
    var data = this.state.data;
    var filename = file.name;
    if (this.state.anonymized)
      filename = "anonymized/"+filename;
    if (this.existFile(filename)) {
      alert("Data file exist.");
      this.handleCloseUploadData();
      return;
    }
    const dataInfo = {
      name: filename,
      size: file.size,
      type: "csv",
      license: "Open source",
      tags: this.state.dataTags,
      keywords: "",
      descriptions: this.state.dataDescriptions,
      stats: { score: "", ratings: "", comments: "", views: "" },
      owned: true
    };
    if (file.size > MAX_FILE_SIZE) {
      alert("ERROR: Max file size exceeded. ");
      return;
    }
    if (file.size > MAX_PAGE_SIZE) {
      //LARGE_FILE_SIZE_THRESHOLD) {
      this.setState({
        isSmallDataset: false,
        isUploadingStarted: true
      });
      this.startPaginationUpload(file, dataInfo);
    } else {
      var reader = new FileReader();
      reader.readAsBinaryString(file);
      reader.onload = fileLoadEvent => {
        var uploadPath = "/";
        Meteor.call(
          "userShareDataFrontEnd",
          this.state.pubKey,
          this.state.ethAcc,
          uploadPath,
          dataInfo,
          btoa(reader.result),
          0,
          (error, result) => {
            if (!error) {
              if (result.receipt === "0x239492347934875349") {
                console.log("Successful");
                dataInfo.receipt = result.receipt;
                data.push(dataInfo);
                this.setState(
                  {
                    data: data,
                    progress: "100"
                  },
                  function() {
                    setTimeout(() => {
                      here.handleCloseUploadData(true);
                    }, 3000);
                  }
                );
              }
            } else {
              console.log(error);
              this.handleCloseUploadData();
            }
          }
        );
      };
    }
  }

  startPaginationUpload(file, dataInfo) {
    var pageSize = MAX_PAGE_SIZE;
    var windSize = 1;
    var totalPages = 1;
    var firstMissingPage = 0;
    var streamId;
    var pubKey = this.state.pubKey;
    var ethAcc = this.state.ethAcc;
    var data = this.state.data;
    var currPage = 0;
    var batch = 1;
    var reader = new FileReader();

    const updateStatus = missingPage => {
      if (missingPage) {
        if (firstMissingPage < missingPage) {
          firstMissingPage = missingPage;
          if (firstMissingPage >= totalPages) {
            data.push(dataInfo);
            this.setState({
              data: data,
              progress: "100"
            });
            this.handleCloseUploadData();
          } else {
            const percent = Math.round((firstMissingPage / totalPages) * 100);
            this.setState({
              progress: percent.toString()
            });
          }
        }
      }
    };

    const pushPage = pageNo => {
      var blob = file.slice(pageNo * pageSize, pageNo * pageSize + pageSize);
      reader.readAsBinaryString(blob);
      reader.onload = function(e) {
        if (reader.readyState == FileReader.DONE) {
          console.log("Uploading the packet");
          Meteor.call(
            "userPushPage",
            pubKey,
            ethAcc,
            streamId,
            pageNo,
            btoa(reader.result),
            (error, result) => {
              if (!error && !result.error) {
                updateStatus(result.missingPage);
              } else {
                console.log(error);
              }
              if (firstMissingPage < totalPages) pushPage(firstMissingPage);
            }
          );
        } else {
          console.log("Upload File Failed");
        }
      };
    };

    const uploadPath = "/";
    var contents;
    const startStream = {
      totalSize: file.size,
      maxPageSize: pageSize
    };
    Meteor.call(
      "userShareDataFrontEnd",
      pubKey,
      ethAcc,
      uploadPath,
      dataInfo,
      contents,
      startStream,
      (error, result) => {
        if (!error && !result.error) {
          if (result.streamReady) {
            pageSize = result.streamReady.pageSize;
            windSize = result.streamReady.windSize;
            streamId = result.streamReady.streamId;
            totalPages = Math.ceil(file.size / pageSize);
            pushPage(0);
          }
        } else {
          console.log(error);
          if (result) console.log(result.error);
        }
      }
    );
    this.setState({
      progress: "0"
    });
  }

  uploadLargeData(file) {
    var size = file.size;
    var chunk_size = LARGE_FILE_SIZE_THRESHOLD;
    var chunks = [];
    var offset = 0;
    var bytes = 0;
    var pubKey = this.state.pubKey;
    var ethAcc = this.state.ethAcc;
    const { active, activePath } = this.state;
    var data = this.state.data;
    const self = this;

    /*    function closeFile(){
              console.log("Uploading the packet");
                  Meteor.call("userShareDataFrontEnd",
                  pubKey,
                  ethAcc,
                  activePath.replace("/data","")+'/'+file.name,
                  "",
                  -1,
                  1,(error, result) => {
                    if(!error){
                      console.log('File Upload : success ' + 100 + '%');
                      }
                    else{
                      console.log(error);
                    }
                  })
            }
            */

    const readSlice = file => {
      var reader = new FileReader();
      var blob = file.slice(offset, offset + chunk_size);
      reader.readAsBinaryString(blob);
      reader.onload = function(e) {
        if (reader.readyState == FileReader.DONE) {
          console.log("Uploading the packet");
          Meteor.call(
            "userShareDataFrontEnd",
            pubKey,
            ethAcc,
            file.name,
            btoa(reader.result),
            offset,
            1,
            (error, result) => {
              if (!error) {
                console.log("result " + (offset * 100) / size + "%");
                self.setState({
                  progress:
                    (offset * 100) / size < 5
                      ? "5%"
                      : (offset * 100) / size + "%"
                });
                offset += chunk_size;
                if (
                  offset < size &&
                  result.receipt === "0x239492347934875349"
                ) {
                  readSlice(file);
                } else {
                  data.push({
                    id: (parseInt(data[data.length - 1].id) + 1).toString(),
                    name: file.name,
                    size: file.size,
                    type: "csv",
                    license: "Open source",
                    tags: "microbiome, bioinformatics, pharma",
                    stats: {
                      score: "5",
                      ratings: "10k",
                      comments: "10",
                      views: "100k"
                    },
                    owned: true
                  });
                  self.setState(
                    {
                      data: data
                    },
                    () => self.handleCloseUploadData(true)
                  );
                }
              } else {
                console.log("error");
                console.log(error);
                self.handleCloseUploadData();
              }
            }
          );
        } else {
          console.log("Upload File Failed");
          self.handleCloseUploadData();
        }
      };
    };
    readSlice(file);
  }

  getCfg = () => {
    Meteor.call("userGetCfg", (err, result) => {
      if (!err) {
        if (result !== null) {
          var state = this.state;
          if (undefined !== result.enclaves && Array.isArray(result.enclaves)) {
            state.enclaves = result.enclaves;
            if (state.enclaves.length > 0) {
              state.pubKey = state.enclaves[0].pubKey;
              const enclave = state.enclaves[0];
              if (undefined !== enclave) {
                if (undefined !== enclave.roleInfos) {
                  const role = enclave.roleInfos.find(
                    r => r.role === roles.HOST &&
                         result.blockchainAccounts.find(account => account.account === r.ethAddr)
                  );
                  if (undefined !== role) state.ethAcc = role.ethAddr;
                }
              }
            }
          }
          state.ethAccounts = result.blockchainAccounts.map((acc) => (acc.account));
          this.setState(state, () => this.getDatasetForOwner());
        }
      }
    });
  };

  getDatasetForOwner() {
    if (this.state.value === "") {
      return;
    }
    const enclave = this.state.enclaves.find(
      e => e.pubKey === this.state.pubKey
    );
    if (undefined === enclave) {
      alert("Enclave not found.");
      return;
    }
    var role;
    if (Array.isArray(enclave.roleInfos)) {
      role = enclave.roleInfos.find(
        r => r.role === roles.HOST &&
             this.state.ethAccounts.find(account => account === r.ethAddr)
      );
      // role = enclave.roleInfos.find(r => r.role === roles.HOST);
    }
    if (undefined === role) {
      alert("Not a Host role user for the enclave.");
      return;
    }
    Meteor.call(
      "getEnclaveInfo",
      this.state.pubKey,
      role.ethAddr,
      "/data",
      (err, res) => {
        if (err) {
          console.log("err:", err)
          alert(err.toString());
        }
        else if (res.error) {
          alert("Error connecting to enclave: " + JSON.parse(res.error).errno);
        }
        else {
          //var tree = renderTree(this.state.enclaves, res.result)
          this.getPersonalDatasets(res.result.children);
        }
      }
    );
  }

  getPersonalDatasets(tree) {
    let personalDataset = tree === undefined ? [] : getLeafDatasets(tree);
    var i = 1;
    personalDataset.forEach(dataset => {
      (dataset.id = i.toString()), (i = i + 1);
    });
    this.setState({
      data: personalDataset
    });
  }

  setSelectedDatabaseInfo = (pubkey, ethAcc, datasetId) => {
    var selectedData = this.state.data.filter(x => x.id === datasetId);
    if (selectedData.length === 0) {
      alert("Please select a dataset for analysis");
      return;
    }
    var selectedDatabaseInfo = {
      pubKey: pubkey,
      ethAcc: ethAcc,
      name: selectedData[0].name
    };
    Meteor.call(
      "setSelectedDatabase",
      selectedDatabaseInfo,
      (error, result) => {
        if (error) {
          alert("error:" + error);
        } else {
          console.log("added");
        }
      }
    );
  };

  isChecked(e) {
    return this.state.selected.has(e.row.id);
  }

  getLeafEthAcc(nodes, result = []) {
    for (var i = 0, length = nodes.length; i < length; i++) {
      if (nodes[i].leaf) {
        result.push(nodes[i].module);
      } else if (
        nodes[i].module === "stakeholders" ||
        nodes[i].module === "Data donors"
      ) {
        result = this.getLeafEthAcc(nodes[i].children, result);
      }
    }
    return result;
  }

  getEthAccounts(tree) {
    return this.getLeafEthAcc(tree);
  }

  selectEnclave(event) {
    let pubKey = event.target.value;
    this.state.value = pubKey;
    if (undefined !== pubKey && "" !== pubKey) {
      var ethAcc = this.state.ethAcc;
      const enclave = this.state.enclaves.find(e => e.pubKey === pubKey);
      if (undefined !== enclave) {
        // const role = enclave.roleInfos.find(r => r.role === roles.HOST);
        const role = enclave.roleInfos.find(
          r => r.role === roles.HOST &&
               this.state.ethAccounts.find(account => account === r.ethAddr)
        );
        if (undefined !== role) ethAcc = role.ethAddr;
      }
      this.setState(
        {
          pubKey: pubKey,
          ethAcc: ethAcc
        },
        function() {
          here.getDatasetForOwner();
        }
      );
    }else {
      this.setState(
        {
          pubKey: "",
          ethAcc: ""
        },
        function() {
          here.getDatasetForOwner();
        }
      );
    }
  }

  shareData(op) {
    var dataset = this.state.data.find(d => d.id === this.state.currentDataId);
    if (undefined === dataset) {
      alert("Data id not found.");
      return;
    }
    if (
      dataset.descriptions !== this.state.dataDescriptions ||
      dataset.tags !== this.state.dataTags
    ) {
      dataset.descriptions = this.state.dataDescriptions;
      dataset.tags = this.state.dataTags;
    }
    var guestEnclave = "";
    if ("Share" === op) {
      //share rather than save
      guestEnclave = this.state.userEnclaveList.find(
        e => e.pubKey === this.state.userAccessEnclavePk
      );
      if (undefined === guestEnclave) {
        alert("Select a valid user Enclave please.");
        return;
      }
    }
    const updatedDataInfo = Object.assign({}, dataset);
    delete updatedDataInfo.shareInfo;
    const shareInfo = {
      startTime: new Date(this.state.userAccessStartDate).getTime(),
      endTime: new Date(this.state.userAccessEndDate).getTime(),
      dataInfo: updatedDataInfo
    };
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
      guestEnclave.pubKey,
      (err, result) => {
        if (err) alert(err);
        else if (result.error) alert(result.error);
        else {
          this.setState({ showEditData: false });
        }
      }
    );
  }

  saveDataInfo(event) {
    this.shareData();
  }

  loadShareInfo() {
    const guestEnclavePk = this.state.userAccessEnclavePk;
    if (undefined === guestEnclavePk) return;
    const dataset = this.state.data[this.state.currentDataId];
    if (undefined === dataset) return;
    const ethAddr = getEthFromPubkey(this.state.userAccessEnclavePk);
    var shareInfo;
    if (undefined !== dataset.shareInfo)
      shareInfo = dataset.shareInfo.find(info => info.ethAddr === ethAddr);
    if (undefined !== shareInfo) {
      this.setState({
        userAccessStartDate: new Date(shareInfo.startTime).toISOString().slice(0, 10),
        userAccessEndDate: new Date(shareInfo.endTime).toISOString().slice(0, 10),
      });
    } else {
      this.setState({
        userAccessStartDate: new Date().toISOString().slice(0, 10),
        userAccessEndDate: new Date().toISOString().slice(0, 10)
      });
    }
  }

  getUserEnclaveList = () => {
    const enclaves = this.state.userEnclaveList;
    if (undefined === enclaves) return;
    return enclaves.map((item, index) => (
      <option value={item.pubKey} key={index}>
        {item.infoJson.name}
      </option>
    ));
  };

  editDataModal = () => (
    <Modal
      size="lg"
      show={this.state.showEditData}
      onHide={this.handleCloseEditData}
      //onShow={this.handleOnShow}
      //style={{backgroundColor: "rgba(56,56,56,1)", color: "white"}}
    >
      <Modal.Header
        style={{ backgroundColor: "rgba(50, 50, 51, 1.0)", color: "white" }}
        closeButton
      >
        <Modal.Title>Edit and Share Data</Modal.Title>
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
            <a
              href="#1"
              style={{ position: "absolute", right: "10px", top: "10px" }}
            >
              <img
                src="./img/share-data-settings-color-black-copy-7@2x.png"
                style={{ height: "16px", width: "8px" }}
              />
            </a>
            <a
              href="#2"
              style={{ position: "absolute", left: "10px", top: "10px" }}
            >
              <img
                src="./img/smart-contract-color-black-copy-6-1@2x.png"
                style={{ height: "16px", width: "8px" }}
              />
            </a>
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
                  /*position:"relative", top:"50px"*/
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
              <Form
                style={{ backgroundColor: "rgba(56,56,56,1)", color: "white" }}
              >
                <Container>
                  <Row>
                    <Col sm={5}>
                      <DappFormGroup
                        id="privacysetting"
                        as="select"
                        label="Privacy E-S dp"
                        value={this.state.privacyESdp}
                        type="text"
                        onChange={event => {
                          this.setState({ privacyESdp: event.target.value });
                        }}
                        list={["K-Anonomous"].map(item => (
                          <option value={item} key={item}>
                            {item}
                          </option>
                        ))}
                      />
                    </Col>
                    <Col sm={5}>
                      <DappFormGroup
                        id="privacyparameter"
                        label="Parameter (k)"
                        value={this.state.privacyKvalue}
                        type="number"
                        onChange={event => {
                          this.setState({ privacyKvalue: event.target.value });
                        }}
                        placeholder="Enter the k value"
                      />
                    </Col>
                    <Col sm={1}>
                      <OverlayTrigger
                        placement="top"
                        overlay={
                          <Tooltip id={`tooltip-top`}>
                            A dataset is k-anonymous if every combination of
                            identify-revealing characteristics occurs in at
                            least k different rows of the data set.
                          </Tooltip>
                        }
                      >
                        <img
                          src="./img/styleguide-4---map-layout-icon-left-5@2x.png"
                          style={{ height: "16px", width: "16px" }}
                        />
                      </OverlayTrigger>
                    </Col>
                  </Row>
                  <Row>
                    <Col sm={5}>
                      <DappFormGroup
                        id="useraccess"
                        as="select"
                        label="User Access"
                        value={this.state.userAccess}
                        type="text"
                        onChange={this.handleUserAccess}
                        list={this.state.userList.map((item, index) => {
                          //const user=item.name+"@"+item.home
                          return (
                            <option value={item} key={index}>
                              {item}
                            </option>
                          );
                        })}
                      />
                    </Col>
                    <Col sm={5}>
                      <DappFormGroup
                        id="useraccessenclavePk"
                        as="select"
                        label="User Enclave"
                        value={this.state.userAccessEnclavePk}
                        type="text"
                        onChange={this.handleUserEnclave}
                        list={this.getUserEnclaveList()}
                      />
                    </Col>
                  </Row>
                  <Row>
                    <Col sm={5}>
                      <DappFormGroup
                        id="useraccessdatestart"
                        label="Access Dates Start:"
                        value={this.state.userAccessStartDate}
                        type="date"
                        onChange={event => {
                          this.setState({
                            userAccessStartDate: event.target.value
                          });
                        }}
                      />
                    </Col>
                    <Col sm={5}>
                      <DappFormGroup
                        id="useraccessdateend"
                        label="End:"
                        value={this.state.userAccessEndDate}
                        type="date"
                        onChange={event => {
                          this.setState({
                            userAccessEndDate: event.target.value
                          });
                        }}
                      />
                    </Col>
                  </Row>
                </Container>
              </Form>
            </Col>
          </Row>
          <Row>
            <Col>
              <Button
                variant="secondary"
                size="lg"
                block
                style={{ backgroundColor: "#e5e5e5", color: "black" }}
                onClick={() => this.shareData("Save")}
              >
                Save
              </Button>
            </Col>
            <Col>
              <Button
                variant="primary"
                size="lg"
                block
                onClick={() => this.shareData("Share")}
              >
                Share
              </Button>
            </Col>
          </Row>
        </Container>
      </Modal.Footer>
    </Modal>
  );

  uploadDataModal = () => (
    <Modal show={this.state.showUploadData} onHide={this.handleCloseUploadData}>
      <div
        style={{
          borderStyle: "solid",
          borderWidth: "1px",
          borderColor: "white"
        }}
        className=""
      >
        <Modal.Header
          style={{ backgroundColor: "rgba(50, 50, 51, 1.0)", color: "white" }}
          closeButton
        >
          <Modal.Title>Choose a Data File to Upload</Modal.Title>
        </Modal.Header>
        <Modal.Body
          style={{
            backgroundColor: "rgba(50, 50, 51, 1.0)",
            color: "white",
            fontWeight: "bold"
          }}
        >
          <Form>
            <DappFormGroup
              style={{ backgroundColor: "rgba(36,36,36,1.0)", color: "white" }}
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
              classname="inputplaceholder"
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
            <DappFormGroup
              id="anonymized"
              label="Is it an anonymized dataset? Check for yes"
              value={this.state.anonymized}
              type="checkbox"
              onChange={event => {
                this.setState({ anonymized: event.target.checked });
              }}
            />
            {!this.state.isUploadingStarted && (
              <DappFormGroup
                id="dataUpload"
                type="file"
                accept={ALLOWED_FILE_TYPES}
                onChange={event => {
                  var fn = event.target.files[0];
                  if(ALLOWED_FILE_TYPES.includes(fn.type)) {
                    this.setState({ uploadFile: fn });
                  } else {this.setState({ uploadFile: "" });}
                }}
              />
            )}
            <DappFormGroup
              id="progress"
              label="Upload Progress"
              as="progressbar"
              value={this.state.progress}
              type="text"
            />
          </Form>
        </Modal.Body>
        <Modal.Footer
          style={{ backgroundColor: "rgba(50, 50, 51, 1.0)", color: "white" }}
        >
          {!this.state.isUploadingStarted && (
            <Button
              variant="secondary"
              style={{ backgroundColor: "#e5e5e5", color: "black" }}
              className="upload-btn"
              onClick={() => this.uploadData()}
            >
              <b>Upload</b>
            </Button>
          )}
        </Modal.Footer>
      </div>
    </Modal>
  );

  deleteDataset(pubkey, ethAcc, datasetId) {
    console.log("delete selected dataset");
    const selectedData = this.state.data.filter(x => x.id === datasetId);
    const pathPrefix = "/data/";
    if (
      !confirm("Deleting " + selectedData[0].name + " !!! press OK to continue")
    )
      return;

    Meteor.call(
      "userDelDataFrontEnd",
      pubkey,
      ethAcc,
      pathPrefix + selectedData[0].name,
      (error, result) => {
        if (error) {
          console.log("error:", error);
          if ("object" === typeof error) error = JSON.stringify(error);
          alert(error);
        } else if (result.error) alert(result.error);
        else {
          Meteor.call(
            "removeDatasetInfo",
            selectedData[0].name,
            (error, result) => {
              console.log("delete data returned.");
              this.setState({
                data: this.state.data.filter(
                  d => d.name !== selectedData[0].name
                )
              });
            }
          );
        }
      }
    );
  }

  render() {
    const data = this.state.data;
    const { enclaves, ethAccounts } = this.state;

    const usrId = Meteor.userId();
    if (undefined === usrId || null === usrId) {
      return <Redirect push to="/login" />;
    }
    const user = Meteor.user();
    var username = "";
    if (user) username = user.username;
    //const listEthAcc = ethAccounts.map((acc) => (
    //    <option value={acc.account}
    //            key={acc.accName === "" ? acc.account : acc.accName}>{acc.accName === undefined || acc.accName === '' ? acc.account : acc.accName}</option>));

    const enclave = enclaves.find(e => e.pubKey === this.state.pubKey);
    const url = (undefined === enclave) || (this.state.value == '') ? undefined : enclave.url;

    // unique enclaves
    var uniqueEnclaves = enclaves.filter((enclave1, index, self) =>
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
            {/*<div className="group5">*/}
            {this.uploadDataModal()}
            <div className="activetab">
              <Button
                variant="secondary"
                style={{ backgroundColor: "#e5e5e5", color: "black" }}
                className="upload-btn"
                disabled={this.state.pubkey === ""}
                onClick={() => this.handleShowUploadData()}
              >
                <i className="fas fa-upload"></i>
                <b>Upload Dataset</b>
              </Button>
              <div className="divider"> </div>
            </div>
            <div className="form">
              <Form>
                <Form.Row>
                  <Form.Group sm={4} as={Col} controlId="enclaveAcc">
                    <Form.Label style={{ color: "white" }}>
                      Select Enclave
                    </Form.Label>
                    <Form.Control
                      as="select"
                      value={this.state.value}
                      onChange={this.selectEnclave}
                      style={{ backgroundColor: "black", color: "white" }}
                    >
                      {listEnclaves(enclave_list)}
                    </Form.Control>
                  </Form.Group>
                  <Form.Group sm={8} as={Col} controlId="EnclaveInfo">
                    <Form.Label style={{ color: "white" }}>
                      <span>
                        Public key: {this.state.value.substring(0, 19) + "..."}&nbsp;&nbsp;
                      </span>
                    </Form.Label>
                    <Form.Label style={{ color: "white" }}>
                      <span>URL: {url}</span>
                    </Form.Label>
                  </Form.Group>
                </Form.Row>
              </Form>
            </div>
            {/*</div>*/}
            <DataList
              path={"/data/ "}
              pathComment={
                "Enclave:" + here.state.value.substring(0, 19) + "..."
              }
              data={data}
              setSelectedDatabaseInfo={here.setSelectedDatabaseInfo}
              deleteDataset={here.deleteDataset}
              editData={here.handleShowEditData}
              pubKey={here.state.pubKey}
              value={here.state.value}
              ethAcc={here.state.ethAcc}
            />
          </div>
        </div>
      </div>
    );
  }
}
export default withRouter(View);
