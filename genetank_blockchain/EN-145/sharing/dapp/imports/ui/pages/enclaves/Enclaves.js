import { Meteor } from "meteor/meteor";
import React, { Component } from 'react';
import { withRouter } from 'react-router-dom';
import { Redirect } from 'react-router'

import ReactTable from 'react-table';
//import 'react-table/react-table.css';
import { Button, ButtonGroup, Form, InputGroup } from 'react-bootstrap'

class Enclaves extends Component {
  constructor(props) {
    super(props);
    this.state = {
      data: [],
      selected: new Set(),
      ethereumAddress: '',
    };
    this.loadData = this.loadData.bind(this);
    this.selectEnclave = this.selectEnclave.bind(this);
    this.isChecked = this.isChecked.bind(this);
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

  getEnclaves() {
    return [{
      "id": "1",
      "name": "Cambridge_Enclave_42",
      "type": "Intel Xeon",
      "status": "Running",
      "group": "Bioinformatics",
      "location": "Cambridge",
      "tags": "local/microbiome",
      "maintenance": "-"
    }, {
      "id": "2",
      "name": "IBM_Enclave_400",
      "type": "Intel Pentium",
      "status": "Running",
      "group": "Bioinformatics",
      "location": "Seattle",
      "tags": "cloud/ibm",
      "maintenance": "-"
    }, {
      "id": "3",
      "name": "MGH_Enclave_314",
      "type": "Intel Celeron",
      "status": "Stopped",
      "group": "Radiology",
      "location": "Boston",
      "tags": "remote/mgh",
      "maintenance": "-"
    }, {
      "id": "4",
      "name": "Paris_Enclave_007",
      "type": "Intel Core i7",
      "status": "Running",
      "group": "Chemistry",
      "location": "Paris",
      "tags": "local/chemistry",
      "maintenance": "-"
    }];
  }

  loadData() {
    // TODO: Load real datasets by calling the backend
    this.setState({data: this.getEnclaves()});
  }

  selectEnclave(e) {
    let selectedEnclaves = this.state.selected;
    if (e.target.checked) {
      selectedEnclaves.add(e.target.getAttribute('data-enclaveid'));
    } else {
      selectedEnclaves.delete(e.target.getAttribute('data-enclaveid'));
    }
    console.log('Selected enclaves:');
    console.log(selectedEnclaves);
    this.setState({selected: selectedEnclaves});
  }

  isChecked(e) {
    return this.state.selected.has(e.row.id);
  }

  render() {
    const data = this.state.data;
    const selected = this.state.selected;

    const usrId = Meteor.userId();
    if (undefined === usrId || null === usrId) {
      return <Redirect push to='/login' />
    }

    return (
      <div className="container-flex">
        <div className="row mt-5 mb-5">
          <div className="col-md-10 offset-md-1">
            <div className="row mb-3">
	      <div className="col-md-12">
                <ButtonGroup className="mr-2">
                  <Button variant="outline-primary"><i className="fas fa-plus"></i> Add</Button>
                  <Button variant="outline-primary"><i className="fas fa-clock"></i> Reservations</Button>
                  <Button variant="outline-primary"><i className="fas fa-sync-alt"></i> Refresh</Button>
                  <Button variant="outline-primary"><i className="fas fa-tools"></i> Services</Button>
                </ButtonGroup>
                <ButtonGroup className="mr-2">
                  <Button variant="outline-primary" disabled={selected.size === 0}><i className="fas fa-play"></i> Start</Button>
                  <Button variant="outline-primary" disabled={selected.size === 0}><i className="fas fa-stop"></i> Stop</Button>
                  <Button variant="outline-primary" disabled={selected.size === 0}><i className="fas fa-trash"></i> Delete</Button>
                </ButtonGroup>
              </div>
            </div>
            <div className="row mb-3">
              <div className="col-md-12">
                <Form inline onSubmit={() => this.loadData()}>
                  <InputGroup className="mr-2">
                    <InputGroup.Prepend>
                      <InputGroup.Text id="inputGroupPrepend">🔎</InputGroup.Text>
                    </InputGroup.Prepend>
                    <Form.Control type="text" placeholder="Filter by name" />
                  </InputGroup>
                  <Form.Control as="select" className="mr-2">
                    <option>All Resource Groups</option>
                  </Form.Control>
                  <Form.Control as="select" className="mr-2">
                    <option>All Types</option>
                  </Form.Control>
                  <Form.Control as="select" className="mr-2">
                    <option>All Locations</option>
                  </Form.Control>
                  <Form.Control as="select" className="mr-2">
                    <option>All Tags</option>
                  </Form.Control>
                  <Button variant="outline-primary"><i className="fas fa-undo-alt"></i> Reset Filters</Button>
                </Form>
              </div>
            </div>
            <div className="row">
	      <div className="col-md-12">
                <ReactTable
                  data={data}
                  columns={[
                    {
                      Header: "",
                      accessor: "id",
                      maxWidth: 30,
                      Cell: row => (
                        <div className="text-center">
                          <input type="checkbox" checked={this.isChecked(row)} data-enclaveid={row.value} onChange={this.selectEnclave} />
                        </div>
                      )
                    },
                    {
                      Header: "Name",
                      accessor: "name",
                      Cell: row => (
                        <a href="/enclaves">{row.value}</a>
                      )
                    },
                    {
                      Header: "Type",
                      accessor: "type",
                      minWidth: 50,
                    },
                    {
                      Header: "Status",
                      accessor: "status",
                      minWidth: 50,
                    },
                    {
                      Header: "Resource Group",
                      accessor: "group",
                    },
                    {
                      Header: "Location",
                      accessor: "location",
                    },
                    {
                      Header: "Tags",
                      accessor: "tags",
                    },
                    {
                      Header: "Maintenance",
                      accessor: "maintenance",
                    },
                  ]}
                  defaultPageSize={10}
                  className="-striped -highlight"
                />
              </div>
            </div>
          </div>
        </div>
      </div>
    )
  };
}
export default withRouter(Enclaves);
