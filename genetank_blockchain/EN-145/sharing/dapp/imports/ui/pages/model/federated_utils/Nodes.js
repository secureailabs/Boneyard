import React from 'react';
import { Card,Button, ButtonGroup, Col, ControlLabel, Form, FormControl, FormGroup, Glyphicon, InputGroup, Nav, NavItem, Pagination, Row, Tab, Tabs } from 'react-bootstrap'
import ReactTable from 'react-table';
import DatasetsDropdown from "./DatasetsDropdown";
import ModelDropdown from "./ModelDropdown";

export const Nodes = ({nodes}) => (
  <Card className="container-flex">
    <Card.Header>Node Details</Card.Header>
    <Card.Body>
      <div className="text-center">
        <ReactTable

          data={nodes}
          columns={[
            {
              Header: "Status",
              accessor: "status",
              minWidth: 150,

              Cell: row => (<span className={row.value === 'Active' ? 'text-success': 'text-danger'}>{row.value+" "}{ row.value === 'Active' ? <i className="fas fa-check-circle"/> : <i className="fas fa-times-circle"></i>}</span>)
            },
            {
              Header: "Name",
              accessor: "name",
              minWidth: 200,
            },
            {
              Header: "Dataset",
              accessor: "dataSet",
              minWidth: 200,
              Cell: row => (
                <div className="row">
                  <div className="col-md-12">
                    {row.value !== undefined && row.value.length > 0 ?
                      <DatasetsDropdown data={row.value}/> : <img src="/loading.gif" width="20" height="20" />}
                  </div>
                </div>
              )

            },
            {
              Header: "Models",
              accessor: "models",
              minWidth: 200,
              Cell: row => (
                <div className="row">
                  <div className="col-md-12">
                    {row.value !== undefined && row.value.length > 0 ?
                      <ModelDropdown models={row.value}/> : <img src="/loading.gif" width="20" height="20" />}
                  </div>
                </div>
              )

            },
            {
              Header: "Pub Key",
              accessor: "pubKey",
              minWidth: 200,
            },
            {
              Header: "Trainer Account",
              accessor: "trainer",
              minWidth: 200,
            }
          ]}
          defaultPageSize={5}
          showPagination={false}
          showPageSizeOptions={false}
          className="-striped -highlight"
        />
      </div>
    </Card.Body>
  </Card>
    )
