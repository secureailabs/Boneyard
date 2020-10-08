import React, { Component } from 'react';

import { Button, Form, InputGroup } from 'react-bootstrap'

import { SampleDataTable } from '/imports/ui/components/SampleDataTable.js'
import {DappFormGroup} from "../../../api/ui/DappFormGroup";

export class Data extends Component {
  constructor(props) {
    super(props);
    this.state = {
      selected: new Set(['linked', 'sas']),
      description: props.data.description
    };
    this.handleCheck = this.handleCheck.bind(this);
    this.isChecked = this.isChecked.bind(this);
    this.handleDescription = this.handleDescription.bind(this);
  }

  componentWillReceiveProps(nextProps, nextContext) {
    if (nextProps.data.description !== undefined){
      this.setState({
        description: nextProps.data.description
      })
    }
  }

  handleCheck(e) {
    let selectedAnalysis = this.state.selected;
    if (e.target.checked) {
      selectedAnalysis.add(e.target.getAttribute('data-analysis'));
    } else {
      selectedAnalysis.delete(e.target.getAttribute('data-analysis'));
    }
    this.setState({selected: selectedAnalysis});
  }

  isChecked(analysis) {
    return this.state.selected.has(analysis);
  }

  handleDescription(event) {
    this.setState({description: event.target.value})
  }

  render() {

    const optionList = [
        <option>k-Anonymity</option>,
      <option>t-Closeness</option>,
      <option>l-Diversity</option>,
      <option>(c-l)-Diversity</option>
    ]
    return (
      <div>
        <DappFormGroup
            label="Description"
            as="textarea"
            rows="4"
            value={this.state.description} onChange={this.handleDescription}
        />
              <DappFormGroup
                  label="Tags"
                  type="text"
                  value={this.props.data.tags}
              />
            <h5>Size:</h5>
            <h5>{this.props.data.size}</h5>
        <Form.Group>
          <div className="row">
            <div className="col-md-2">
              <h2>Data Usage</h2>
              <a href="/contract">
                <Button variant="outline-primary"><i className="fas fa-scroll"></i> Review Smart Contract</Button>
              </a>
            </div>
            <div className="col-md-10">
              <div className="row mb-3">
                <div className="col-md-10">
                    <DappFormGroup
                        label="Usage Expiration"
                        type="text"
                        value="2019-04-22" />
                </div>
              </div>
              <div className="row mb-3">
                <div className="col-md-2">
                  <Form.Label>Location Restrictions</Form.Label>
                </div>
                <div className="col-md-10">
                  <Button variant="outline-primary"><i className="fas fa-globe-americas"></i> Select locations</Button>
                </div>
              </div>
              <div className="row mb-3">
                <div className="col-md-2">
                </div>
                <div className="col-md-10">
                  <DappFormGroup
                      label="Privacy"
                      as="select"
                      list={optionList}
                      className="mb-3"/>
                  <DappFormGroup
                      as="textarea"
                      rows="2"
                      value="k=50" />
                </div>
              </div>
              <div className="row mb-3">
                <div className="col-md-2">
                  <Form.Label>Analysis</Form.Label>
                </div>
                <div className="col-md-10">
                  <Form.Check inline data-analysis="linked" checked={this.isChecked("linked")} onChange={this.handleCheck} /> Data can be linked<br />
                  <Form.Check inline data-analysis="sas" checked={this.isChecked("sas")} onChange={this.handleCheck} /> SAS only<br />
                  <Form.Check inline data-analysis="basicml" checked={this.isChecked("basicml")} onChange={this.handleCheck} /> Basic ML (SVM, Random Forest, Rank Sum, etc.)<br />
                  <Form.Check inline data-analysis="deepml" checked={this.isChecked("deepml")} onChange={this.handleCheck} /> Deep ML (CNN, RNN, GANN, etc.)
                </div>
              </div>
            </div>
          </div>
        </Form.Group>
        <div className="row">
          <div className="col-md-12">
            <h2>Data Sample</h2>
            <SampleDataTable sampleData = {this.props.data.sampleData}/>
          </div>
        </div>
      </div>
    )
  }
}
