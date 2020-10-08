import React, { Component } from 'react';

import { Form } from 'react-bootstrap'

export class JupyterNotebook extends Component {
  constructor(props) {
    super(props);
  }

  handleInput() {}

  createNotebook() {
    let inputNotebook = this.props.notebook;
    let outputNotebook = [];

    for (let i = 0; i < inputNotebook.length; i++) {
      if (inputNotebook[i].type === 'heading') {
        outputNotebook.push(<h2>{inputNotebook[i].value}</h2>);
      } else if (inputNotebook[i].type === 'code') {
        outputNotebook.push(<Form.Control as="textarea" rows="5" style={{fontFamily: "monospace"}} value={inputNotebook[i].value} onChange={this.handleInput} className="mb-2" />);
      } else if (inputNotebook[i].type === 'output') {
        outputNotebook.push(<Form.Control as="textarea" rows="5" style={{fontFamily: "monospace"}} disabled value={inputNotebook[i].value} onChange={this.handleInput} className="mb-2" />);
      }
    }
    return outputNotebook;
  }

  render() {
    return (
      <div className="mt-3">
        {this.createNotebook()}
      </div>
    )
  }
}
