import React, { Component } from 'react';
import { withRouter } from 'react-router-dom';
import { DappFormGroup } from '/imports/api/ui/DappFormGroup.js'
import { Container, Row, Col, Button, Form } from 'react-bootstrap'
import {Link} from 'react-router-dom'
import NavBar from '/imports/ui/components/NavBar';


const INITIAL_STATE = {
  email: '',
  username: '',
  password: '',
  reenter_password: '',
  error: null,
};

const byPropKey = (propertyName, value) => () => ({
  [propertyName]: value,
});

class CreateAccount extends Component {
  constructor(props) {
    super(props);
   
    this.state = { ...INITIAL_STATE };
    //this.state = { words: '' }

    this.onSubmit = this.onSubmit.bind(this);
  }

  onSubmit = (event) => {
    const {
      email,
      username,
      password,
      reenter_password,
    } = this.state;
    
    const {
      history,
    } = this.props;
  
    if (password == reenter_password) {
      Meteor.call('registerUser', email, username, password, (error, result) => {
        if ( typeof result != "string") {
          alert("Error:" + result.reason)
          this.setState(byPropKey('error', error));
        } else {
          this.setState(() => ({ ...INITIAL_STATE }));
            alert("User Created Successfully")
        }
      });
    } else {
      alert("Reentered password doesn't match.")
    }
  }

  render() {
    const {
      email,
      username,
      password,
      reenter_password,
      error,
    } = this.state;

    return (
        <div className="container-fluid">
            <div className="row">
                <div className="navbar-base">
                    <NavBar/>
                </div>
                <div style={{position: "relative", left: "600px", color: "white", width: "500px"}} className="main-content">

                <Container>
          <Link to="/admin" style={{backgroundColor: "#e5e5e5", color: "black"}} className="btn btn-secondary">Go Back</Link>
                    <br/>
                    <br/>
                    <br/>
        <Row>
           <Col>
             <Form>
              <legend className="border border-top-0 border-left-0 border-right-0">Register</legend>
              <DappFormGroup
                label ="Username"
                value={username}
                type="text"
                id="username"
                onChange={event => this.setState(byPropKey('username', event.target.value))}
                placeholder="Enter Username"
              />
              <DappFormGroup
                label ="Email"
                value={email}
                type="email"
                id="email"
                onChange={event => this.setState(byPropKey('email', event.target.value))}
                placeholder="Enter Email Address"
              />
              <DappFormGroup
                label ="Password"
                value={password}
                type="password"
                id="password"
                onChange={event => this.setState(byPropKey('password', event.target.value))}
                placeholder="Enter Password"
              />
              <DappFormGroup
                label ="Re-enter Password"
                value={reenter_password}
                type="password"
                id="reenter_passsword"
                onChange={event => this.setState(byPropKey('reenter_password', event.target.value))}
                placeholder="Re-enter Password"
              />
              <Button variant="secondary" style={{backgroundColor: "#e5e5e5", color: "black"}} onClick={this.onSubmit}>Create</Button>
              { error && <p>{error.reason}</p>}
             </Form>
           </Col>
        </Row>
      </Container>
                </div>
            </div>
        </div>
    );
  }
}

export default withRouter(CreateAccount);
