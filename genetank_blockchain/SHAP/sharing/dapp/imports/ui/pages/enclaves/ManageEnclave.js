import { Meteor } from "meteor/meteor";
import React, { Component } from 'react';
import { withRouter } from 'react-router-dom';
import { uploadFile } from '/imports/api/uploadFile.js'
import { Redirect } from 'react-router'
import { Random } from 'meteor/random'
import { EJSON } from 'meteor/ejson'

import { Button, ButtonGroup, Col, ControlLabel, Form, FormControl, FormGroup, Glyphicon, InputGroup, Nav, NavItem, Pagination, Row, Tab, Tabs } from 'react-bootstrap'

import moment from 'moment';
import { Line } from 'react-chartjs-2';

class ManageEnclave extends Component {
  constructor(props) {
    super(props);
    this.state = {
      data: [],
      stats: {"cpu": {"datasets": []}, "network": {"datasets": []}, "disk": {"datasets": []}},
      graphTimeOptions: this.constructGraphTimeOptions(),
      ethereumAddress: '',
    };
    this.loadData = this.loadData.bind(this);
  }

  componentDidMount() {
    Meteor.call("userGetCfg", (err, result) => {
      if (!err) {
        if (result !== null) {
          this.setState({ethereumAddress:result.defaultEthAccount});
        }
      }
    });
    this.loadData();
  }

  getEnclaveData() {
    return {
      "name": "Cambridge_Enclave_42",
      "owner": "drStrange_BigPharma",
      "type": "Intel Xeon",
      "status": "Running",
      "group": "Bioinformatics",
      "location": "Cambridge",
      "tags": "local/microbiome",
      "os": "Linux",
      "size": "Standard NV6 (6 vcpus, 56 GB memory)",
      "net": "research-vnet/default",
      "hash": "9c56db536e046a5fb84a5c482ce86e6592071dff75dc0e3eb27d701cf2c40508",
    };
  }

  makeSpacedTimePoints(data) {
    var points = [];
    var endDate = new Date();
    for (var i = 0; i < data.length; i++) {
      points.push({"x": moment(endDate).subtract(i, "minutes"), "y": data[i]});
    }
    return points;
  }

  getCpuStats() {
    return this.makeSpacedTimePoints([10, 1, 0, 12, 0, 1, 11, 11, 11, 0, 6, 10, 2, 4, 0, 2, 4, 6, 14, 0, 10, 0, 10]);
  }

  getNetworkStats() {
    return this.makeSpacedTimePoints([10, 1, 0, 12, 0, 1, 11, 11, 11, 0, 6, 10, 2, 4, 0, 2, 4, 6, 14, 0, 10, 0, 10]);
  }

  getDiskStats() {
    return this.makeSpacedTimePoints([10, 1, 0, 12, 0, 1, 11, 11, 11, 0, 6, 10, 2, 4, 0, 2, 4, 6, 14, 0, 10, 0, 10]);
  }

  getEnclaveStats() {
    return {
      "cpu": this.constructGraphData("CPU", this.getCpuStats()),
      "network": this.constructGraphData("Network", this.getNetworkStats()),
      "disk": this.constructGraphData("Disk I/O", this.getDiskStats()),
    };
  }

  constructGraphData(label, dataset) {
    return {
      "datasets": [{
        "label": label,
        "data": dataset,
        "fill": true,
        "lineTension": 0.1,
        "backgroundColor": "rgba(75,192,192,0.4)",
        "borderColor": "rgba(75,192,192,1)",
        "borderCapStyle": "butt",
        "borderDash": [],
        "borderDashOffset": 0.0,
        "borderJoinStyle": "miter",
        "pointBorderColor": "rgba(75,192,192,1)",
        "pointBackgroundColor": "#fff",
        "pointBorderWidth": 1,
        "pointHoverRadius": 5,
        "pointHoverBackgroundColor": "rgba(75,192,192,1)",
        "pointHoverBorderColor": "rgba(220,220,220,1)",
        "pointHoverBorderWidth": 2,
        "pointRadius": 2,
        "pointHitRadius": 10,
      }]
    };
  }

  constructGraphTimeOptions() {
    return {
      "scales": {
        "xAxes": [{
          "type": "time",
          "time": {
            "displayFormats": {
              "day": "MMM D",
              "minute": "h:mm a",
            },
          },
        }],
      },
    };
  }

  loadData(dataset) {
    // TODO: Load real datasets by calling the backend
    this.setState({data: this.getEnclaveData(), stats: this.getEnclaveStats() });
  }

  render() {
    const data = this.state.data;
    const stats = this.state.stats;
    const graphTimeOptions = this.state.graphTimeOptions;

    const usrId = Meteor.userId();
    if (undefined === usrId || null === usrId) {
      return <Redirect push to='/enclaves/login' />
    }

    return (
      <div className="container-flex">
        <div className="row mt-5">
          <div className="col-md-12 offset-md-1">
            <h1>{data.name}</h1>
          </div>
        </div>
        <div className="row mb-5">
          <div className="col-md-2 offset-md-1 sidebar">
            <Form inline onSubmit={() => this.loadData()}>
              <InputGroup className="mr-2">
                <InputGroup.Prepend>
                  <InputGroup.Text id="inputGroupPrepend"><i className="fas fa-search"></i></InputGroup.Text>
                </InputGroup.Prepend>
                <Form.Control type="text" placeholder="Search..." />
              </InputGroup>
            </Form>
            <ul className="list-unstyled">
              <li><a href="#">Overview</a></li>
              <li><a href="#">Activity Log</a></li>
              <li><a href="#">Access Control (IAM)</a></li>
              <li><a href="#">Tags</a></li>
              <li><a href="#">Diagnose</a></li>
              <li>Settings</li>
              <li className="ml-2"><a href="#">Networking</a></li>
              <li className="ml-2"><a href="#">Disks</a></li>
              <li className="ml-2"><a href="#">Size</a></li>
              <li className="ml-2"><a href="#">Security</a></li>
              <li className="ml-2"><a href="#">Extensions</a></li>
              <li className="ml-2"><a href="#">Configuration</a></li>
              <li className="ml-2"><a href="#">Identity</a></li>
              <li className="ml-2"><a href="#">Properties</a></li>
              <li><a href="#">Operations</a></li>
              <li>Monitoring</li>
              <li className="ml-2"><a href="#">Insights</a></li>
              <li className="ml-2"><a href="#">Alerts</a></li>
              <li className="ml-2"><a href="#">Metrics</a></li>
              <li className="ml-2"><a href="#">Diagnostics Settings</a></li>
              <li className="ml-2"><a href="#">Logs</a></li>
              <li className="ml-2"><a href="#">Connection Monitor</a></li>
              <li>Support</li>
              <li className="ml-2"><a href="#">Resources Health</a></li>
              <li className="ml-2"><a href="#">Boot Diagnostics</a></li>
              <li className="ml-2"><a href="#">Redeploy</a></li>
            </ul>
          </div>
          <div className="col-md-8">
            <div className="row mb-3">
	      <div className="col-md-12">
                <ButtonGroup className="mr-2">
                  <Button variant="outline-primary"><i className="fas fa-terminal"></i> Connect</Button>
                  <Button variant="outline-primary"><i className="fas fa-play"></i> Start</Button>
                  <Button variant="outline-primary"><i className="fas fa-sync-alt"></i> Restart</Button>
                  <Button variant="outline-primary"><i className="fas fa-stop"></i> Stop</Button>
                  <Button variant="outline-primary"><i className="fas fa-camera"></i> Capture</Button>
                  <Button variant="outline-primary"><i className="fas fa-trash"></i> Delete</Button>
                  <Button variant="outline-primary"><i className="fas fa-redo-alt"></i> Refresh</Button>
                </ButtonGroup>
              </div>
            </div>
            <div className="row mb-3">
              <div className="col-md-12">
                <ul className="list-unstyled">
                  <li><span className="enclave-label font-weight-bold">Enclave Name:</span> {data.name}</li>
                  <li><span className="enclave-label font-weight-bold">Enclave Location:</span> {data.location}</li>
                  <li><span className="enclave-label font-weight-bold">Enclave Owner:</span> {data.owner}</li>
                  <li><span className="enclave-label font-weight-bold">Type:</span> {data.type}</li>
                  <li><span className="enclave-label font-weight-bold">Status:</span> {data.status}</li>
                  <li><span className="enclave-label font-weight-bold">Resource Group:</span> {data.group}</li>
                  <li><span className="enclave-label font-weight-bold">Tags:</span> {data.tags}</li>
                  <li><span className="enclave-label font-weight-bold">Operation System:</span> {data.os}</li>
                  <li><span className="enclave-label font-weight-bold">Size:</span> {data.size}</li>
                  <li><span className="enclave-label font-weight-bold">Virtual Network/Subnet:</span> {data.net}</li>
                  <li><span className="enclave-label font-weight-bold">Enclave Hash:</span> {data.hash}</li>
                  <li><span className="enclave-label font-weight-bold"><a href={"/enclave/" + data.hash}>Attestation Report</a></span></li>
                </ul>
              </div>
            </div>
            <div className="row mb-3">
	      <div className="col-md-12">
                Show data for last: 
                <ButtonGroup className="ml-2">
                  <Button variant="outline-primary" className="active">1 hr</Button>
                  <Button variant="outline-primary">6 hrs</Button>
                  <Button variant="outline-primary">12 hrs</Button>
                  <Button variant="outline-primary">1 day</Button>
                  <Button variant="outline-primary">7 days</Button>
                  <Button variant="outline-primary">30 days</Button>
                </ButtonGroup>
              </div>
            </div>
            <div className="row">
	      <div className="col-md-4">
                <Line data={stats.cpu} options={graphTimeOptions} />
              </div>
	      <div className="col-md-4">
                <Line data={stats.network} options={graphTimeOptions} />
              </div>
	      <div className="col-md-4">
                <Line data={stats.disk} options={graphTimeOptions} />
              </div>
            </div>
          </div>
        </div>
      </div>
    )
  };
}
export default withRouter(ManageEnclave);
