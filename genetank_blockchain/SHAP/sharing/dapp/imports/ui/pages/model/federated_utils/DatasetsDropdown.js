import React, { Component } from 'react';
import {Form} from "react-bootstrap";

export default class DatasetsDropdown extends React.Component {

  constructor(props) {
    super(props);
    this.handleDatasetSelection = this.handleDatasetSelection.bind(this);
    this.state = {
      selectedData: this.props.data[0]
    }
  }


  handleDatasetSelection = (event) => {
    this.setState({
      selectedData: event.target.value
    })
  }

  render() {
    return (
      <div>
        <Form.Control as="select"
                      onChange={this.handleDatasetSelection}
                      value={this.state.selectedData.name}
                      size="sm"
        >
          {this.props.data.map(data => <option value={data} key={data.name}>{data.name}</option>)}
        </Form.Control>
      </div>
    )
  }
}
