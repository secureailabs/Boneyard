import React, { Component } from 'react';

import { Button, Form, InputGroup } from 'react-bootstrap'

import ReactTable from 'react-table';
//import 'react-table/react-table.css';

export class Activity extends Component {
  constructor(props) {
    super(props);
  }

  render() {
    return (
      <div className="mt-3">
        <div className="row">
          <div className="col-md-12">
            <h2 className="mr-3" style={{display: "inline-block"}}>Operations</h2> 
            <Form inline className="float-right">
              <InputGroup>
                <InputGroup.Prepend>
                  <InputGroup.Text id="inputGroupPrepend">🔎</InputGroup.Text>
                </InputGroup.Prepend>
                <Form.Control type="text" placeholder="Search..." />
              </InputGroup>
            </Form>
          </div>
        </div>
        <div className="row">
          <div className="col-md-12">
            <ReactTable
              data={this.props.activity}
              columns={[
                {
                  Header: "Timestamp",
                  accessor: "timestamp",
                },
                {
                  Header: "From",
                  accessor: "from",
                },
                {
                  Header: "To",
                  accessor: "to",
                },
                {
                  Header: "TxHash",
                  accessor: "txhash",
                },
                {
                  Header: "Computation",
                  accessor: "computation",
                },
                {
                  Header: "Time Elapsed",
                  accessor: "elapsed",
                },
              ]}
              defaultPageSize={10}
              className="-striped -highlight"
            />
          </div>
        </div>
      </div>
    )
  }
}
