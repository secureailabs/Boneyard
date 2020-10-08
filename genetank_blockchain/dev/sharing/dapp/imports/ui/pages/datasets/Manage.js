import { Meteor } from "meteor/meteor";
import React, { Component } from 'react';
import { withRouter } from 'react-router-dom';
import { uploadFile } from '/imports/api/uploadFile.js'
import { Redirect } from 'react-router'
import { Random } from 'meteor/random'
import { EJSON } from 'meteor/ejson'
import NavBar from '/imports/ui/components/NavBar';


import { Button, ButtonGroup, Col, ControlLabel, Form, FormControl, FormGroup, Glyphicon, InputGroup, Nav, NavItem, Pagination, Row, Tab, Tabs } from 'react-bootstrap'

import { SampleDataTable } from '/imports/ui/components/SampleDataTable.js'
import { Data } from '/imports/ui/components/datasets/Data.js'
import { Activity } from '/imports/ui/components/datasets/Activity.js'

class Manage extends Component {
  constructor(props) {
    super(props);
    this.state = {
      data: {},
      activity: [],
      ethereumAddress: '',
      selectedDatabaseInfo: {},
      dsInfo : {
        name: "Not Available",
        size: "Not Available",
        description: "Not Available",
        tags: "Not Available"
      }
    };
    this.loadData = this.loadData.bind(this);
    this.getFormattedDataset = this.getFormattedDataset.bind(this);
  }

  componentDidMount() {
    Meteor.call("userGetCfg", (err, result) => {
      if (!err) {
        if (result !== null) {
          this.setState({ethereumAddress:result.defaultEthAccount,
            selectedDatabaseInfo: result.selectedDatabaseInfo,
            dsInfo: result.datasetsInfo.filter(x => x.name === result.selectedDatabaseInfo.name)[0]});
                    Meteor.call("userLoadSingleDataView", result.selectedDatabaseInfo.name,
            result.selectedDatabaseInfo.pubKey, result.selectedDatabaseInfo.ethAcc,
            (error, result) => {
              if (error) {
                if ('object' === typeof(error))
                  error = JSON.stringify(error)
                alert("Error:"+ error)
              }
              else if (result.error)
                console.log("Error:"+ result.error)
              else{
                this.loadData(this.getFormattedDataset(result.result));
              }
            });
        }
      }
    });
  }

  getFormattedDataset(json) {
    if (json === "") {
      return json
    }

    var tempData = []
    var headers = json[1].split(",");
    for (var key in json) {
      var temp = {}
      if (json.hasOwnProperty(key) && key !== "1") {
        var i;
        var values = json[key].split(",");
        for (i = 0; i < headers.length; i++) {
          temp[headers[i]] = values[i];
        }
      }
      tempData.push(temp)
    }
    var columns = []
    headers.forEach((head) => {
      var col = {
        Header: head,
        accessor: head,
      }
      columns.push(col)
    })
    const sampleData = {
      columns: columns,
      data: tempData
    }
    return sampleData
  }

  formatBytes(bytes, decimals = 2) {
    if (bytes === 0) return '0 Bytes';

    const k = 1024;
    const dm = decimals < 0 ? 0 : decimals;
    const sizes = ['Bytes', 'KB', 'MB', 'GB', 'TB', 'PB', 'EB', 'ZB', 'YB'];

    const i = Math.floor(Math.log(bytes) / Math.log(k));

    return parseFloat((bytes / Math.pow(k, i)).toFixed(dm)) + ' ' + sizes[i];
  }

  getDataset(sampleData) {
    return {
      "name": "Pan Genome 201903",
      "description": this.state.dsInfo === undefined || this.state.dsInfo.description === '' ? "Description not available":this.state.dsInfo.description,
      "license": "Open source",
      "tags": this.state.dsInfo === undefined || this.state.dsInfo.tags === ''? "Tags not available": this.state.dsInfo.tags,
      "size": this.state.dsInfo === undefined || this.state.dsInfo.size === ''? "Size not available": this.formatBytes(this.state.dsInfo.size),
      "usage": {
        "analysis": {
          "linked": 1,
          "sasOnly": 1,
          "basicMl": 0,
          "deepMl": 0
        },
        "privacy": {
          "type": "k-Anonymity",
          "k": "50"
        },
        "expiration": "2019-04-22",
        "location": ""
      },
      "sampleData": sampleData
    };
  }

  getActivity() {
    return [{
      "timestamp": "2019-04-22 12:33:44",
      "from": "drZoidberg_SmallPharma",
      "to": "drStrange_BigPharma",
      "txhash": "0x860e02b18975b",
      "computation": "1 GB",
      "elapsed": "00:05:02",
    }, {
      "timestamp": "2019-04-22 11:59:27",
      "from": "anne_SAIL",
      "to": "drStrange_BigPharma",
      "txhash": "0xd35899173975b",
      "computation": "4 GB",
      "elapsed": "00:20:19",
    }, {
      "timestamp": "2019-04-22 11:25:54",
      "from": "drZoidberg_SmallPharma",
      "to": "drStrange_BigPharma",
      "txhash": "0x860e8381a5990",
      "computation": "2 MB",
      "elapsed": "00:01:19",
    }, {
      "timestamp": "2019-04-22 10:00:22",
      "from": "drZoidberg_SmallPharma",
      "to": "drStrange_BigPharma",
      "txhash": "0x860fffffb15b343",
      "computation": "500 MB",
      "elapsed": "00:03:44",
    }];
  }

  loadData(dataset) {
    // TODO: Load real datasets by calling the backend
    this.setState({data: this.getDataset(dataset), activity: this.getActivity()});
  }

  render() {
    const data = this.state.data;

    const usrId = Meteor.userId();
    if (undefined === usrId || null === usrId) {
      return <Redirect push to='/login' />
    }

    return (
        <div className="container-fluid">
          <div className="row">
            <div className="navbar-base">
              <NavBar/>
            </div>
            <div style={{position: "relative", left: "300px", color: "white"}} className="main-content">
            <Form>
              <div className="row mb-3">
                <div className="col-md-12">
                  <h1 style={{display: "inline-block"}}>{this.state.selectedDatabaseInfo.name !== undefined ? this.state.selectedDatabaseInfo.name : this.state.data.name}</h1>
                </div>
              </div>
              {/*<Tabs>*/}
              {/*  <Tab eventKey="data" title="Data">*/}
                  <Data data={this.state.data} />
                {/*</Tab>*/}
                {/*<Tab eventKey="activity" title="Activity">*/}
                {/*  <Activity activity={this.state.activity} />*/}
                {/*</Tab>*/}
                {/*<Tab eventKey="discussion" title="Discussion">*/}
                {/*</Tab>*/}
                {/*<Tab eventKey="metadata" title="Metadata">*/}
                {/*</Tab>*/}
                {/*<Tab eventKey="settings" title="Settings">*/}
                {/*</Tab>*/}
              {/*</Tabs>*/}
            </Form>
          </div>
        </div>
      </div>
    )
  };
}
export default withRouter(Manage);
