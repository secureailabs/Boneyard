import React, { Component } from 'react';

import { Button, Form } from 'react-bootstrap'

import { SampleDataTable } from '/imports/ui/components/SampleDataTable.js'

export class PreviewDatasets extends Component {
  constructor(props) {
    super(props);
  }

  handleSqlInput() {}

  render() {
    return (
      <div>
        <Form.Group className="mt-3">
          <div className="row">
            <div className="col-md-2">
              <h2>Description</h2>
            </div>
            <div className="col-md-10">
              <p>{this.props.data.description}</p>
              <p><b>Tags: </b>{this.props.data.tags}</p>
            </div>
          </div>
          <div className="row">
            <div className="col-md-2">
              <h2>Size</h2>
            </div>
            <div className="col-md-10">
              <p>{this.props.data.size}</p>
            </div>
          </div>
        </Form.Group>
        <Form.Group>
          <div className="row">
            <div className="col-md-2">
              <h2>Data Usage</h2>
            </div>
            <div className="col-md-10">
              <div className="row mb-3">
                <div className="col-md-12">
                  <a href="/datasets/contract">
                    <Button variant="outline-primary"><i className="fas fa-scroll"></i> Review Smart Contract</Button>
                  </a>
                </div>
              </div>
              <div className="row mb-3">
                <div className="col-md-12">
                  Data can be linked, SAS only
                </div>
              </div>
            </div>
          </div>
        </Form.Group>
        <div className="row mb-3">
          <div className="col-md-12">
            <h2>Data Sample</h2>
            <SampleDataTable sampleData = {this.props.data.sampleData}/>
          </div>
        </div>

      </div>
    )
  }
}
