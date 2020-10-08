import React from 'react';
import { Card,Button, ButtonGroup, Col, ControlLabel, Form, FormControl, FormGroup, Glyphicon, InputGroup, Nav, NavItem, Pagination, Row, Tab, Tabs } from 'react-bootstrap'
import ListGroup from "react-bootstrap/ListGroup";
import ListGroupItem from "react-bootstrap/ListGroupItem";

export const MasterDetail = ({master}) => (
  <Card>
    <Card.Header>Hub Detail</Card.Header>
    <Card.Body>
      <ListGroup className="flush">
        <ListGroupItem>

            <span className='col-3'>
              Name :
            </span>
            <span className='col-9 float-right'>
              {master.name}
            </span>

        </ListGroupItem>
        <ListGroupItem>

            <span className='col-3'>
              Public Key :
            </span>
            <span className='col-9 float-right'>
              {master.pubKey}
            </span>

        </ListGroupItem>
        <ListGroupItem>

            <span className='col-3'>
              Runner Address :
            </span>
            <span className='col-9 float-right'>
              {master.trainer}
            </span>

        </ListGroupItem>
        <ListGroupItem>

            <span className='col-3'>
              Status:
            </span>
            <span className='col-9 float-right'>
              <span className='text-success'>Active <i className="fas fa-check-circle"/></span>
            </span>

        </ListGroupItem>
      </ListGroup>
    </Card.Body>
  </Card>
);

