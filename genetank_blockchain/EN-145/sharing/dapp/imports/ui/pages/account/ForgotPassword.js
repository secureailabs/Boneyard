import React, { Component } from 'react';
import {Button} from "react-bootstrap/Button";

const byPropKey = (propertyName, value) => () => ({
  [propertyName]: value,
});

export class ForgotPassword extends Component {
  constructor(props) {
    super(props);
    this.state = {
      email:''
    };
    this.recoverPass = this.recoverPass.bind(this);
  }

  recoverPass(email) {
    Meteor.call('recoverPassword', email, function(error, result) {
      if (error) {
        return console.log("Error");
      } else {
        if (result === undefined){
          alert("No Such Email Address is Registered")
        }
        else
          alert("Password reset link sent. Please check your email.")
      }
    });

  }

  render() {
    const {
      email
    } = this.state;

    return (
      <div className="container-fluid" style={{backgroundColor: "rgba(50, 50, 51, 1.0)", color: "white"}}>
        <div className="row mt-5">
          <div className="col-md-6 ml-auto mr-auto">
              <legend className="border border-top-0 border-left-0 border-right-0">Reset Password</legend>
              <div className="form-group">
                <label htmlFor="username">Email</label>
                <input
                  value={email}
                  type="email"
                  className="form-control"
                  id="username"
                  onChange={event => this.setState(byPropKey('email', event.target.value))}
                  placeholder="Please Enter your email address associated with the account"/>
              </div>
               <div>
                  <button onClick={() => this.recoverPass(email)} className="btn btn-outline-primary">Send Password Reset Link</button>
                </div>
          </div>
        </div>
      </div>
    );
  }
}
