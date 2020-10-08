import React, { Component } from 'react';
import {Form} from "react-bootstrap";

export default class ModelDropdown extends Component {

  constructor(props) {
    super(props);
    this.handleModelSelection = this.handleModelSelection.bind(this);
    this.state = {
      selectedModel: this.props.models[0]
    }
  }


  handleModelSelection = (event) => {
    this.setState({
      selectedModel: event.target.value
    })
  }

  render() {
    return (
      <div>
        <Form.Control as="select"
                      onChange={this.handleModelSelection}
                      value={this.state.selectedModel.name}
                      size="sm"
        >
          {this.props.models.map(model => <option value={model} key={model.name}>{model.name}</option>)}
        </Form.Control>
      </div>
    )
  }
}
