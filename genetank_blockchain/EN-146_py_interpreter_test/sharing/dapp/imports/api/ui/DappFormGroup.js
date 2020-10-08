import React, { Component } from "react";
import { Form, Col } from "react-bootstrap";
import ProgressBar from "react-bootstrap/ProgressBar";

export class DappFormGroup extends Component {
  constructor(props) {
    super(props);
  }
  render() {
    var {
      as,
      id,
      label,
      value,
      type,
      onChange,
      placeholder,
      list,
      accept
    } = this.props;
    if (undefined === as) as = "input";
    if (undefined === accept) accept = "*";
    var control;
    if ("select" === as) {
      control = () => (
        <Form.Control
          as="select"
          value={value}
          onChange={onChange}
          style={{ backgroundColor: "black", color: "white" }}
        >
          {list}
        </Form.Control>
      );
    } else if (type === "file")
      control = () => (
        <Form.Control
          value={value}
          type={type}
          accept={accept}
          onChange={onChange}
          placeholder={placeholder}
        />
      );
    else if (type === "checkbox")
      control = () => (
        <Form.Control
          value={value}
          checked={value}
          type={type}
          onChange={onChange}
        />
      );
    else if ("input" === as || undefined === as)
      control = () => (
        <Form.Control
          value={value}
          type={type}
          onChange={onChange}
          placeholder={placeholder}
          style={{ backgroundColor: "black", color: "white" }}
        />
      );
    else if (as === "progressbar") {
      control = () => (
        <ProgressBar animated now={parseInt(value)} label={parseInt(value)} />
      );
    } else
      control = () => (
        <Form.Control
          as={as}
          value={value}
          onChange={onChange}
          placeholder={placeholder}
          type={type}
          style={{ backgroundColor: "black", color: "white" }}
        />
      );

    return (
      <Form.Group as={Col} controlId={id}>
        <Form.Label>{label}</Form.Label>
        {control()}
      </Form.Group>
    );
  }
}
