import React, { Component } from 'react';
import { withRouter, Link, Redirect } from 'react-router-dom';
import { DappFormGroup } from '/imports/api/ui/DappFormGroup.js'
import { Container, Row, Col, Button, Form } from 'react-bootstrap'
import queryString from 'query-string'
import {Meteor} from "meteor/meteor";

const INITIAL_STATE = {
    username: '',
    password: '',
    redirectToPage: false
};

const byPropKey = (propertyName, value) => () => ({
    [propertyName]: value,
});

class SignIn extends Component {
    constructor(props) {
        super(props);
        this.state = { ...INITIAL_STATE };
    }

    login(username, password){
        Meteor.loginWithPassword(
            username,
            password,
            error => {
                if (error) {
                    alert("Error: Login Failed.")
                } else {
                    this.setState({redirectToPage: true})
                }
        });
    }

    componentDidMount() {
      const values = queryString.parse(this.props.location.search)
      if (values.id && values.pw)
        this.setState({username:values.id, password:values.pw})
      console.log(Meteor.userId())
    }

    onSubmit(event) {
        const {
            username,
            password,
        } = this.state;

        this.login(username, password)
        if (undefined !== event)
          event.preventDefault();
    }

    keyPress(event) {
        if(event.keyCode == 13){
            this.onSubmit();
        }
    }

    render() {

        if (this.state.redirectToPage) {
            return <Redirect push to='/view'/>
        }

        const {
            username,
            password,
        } = this.state;
        return(
          <div className="login container">
              <div className="row">
                  <div class="loginLogo">
                    {
                       (Meteor.settings.public.mode === "sales" ?
                         <a href="https://www.secureailabs.com" target="_blank">
                             <img src="https://secureailabs.com/img/logo.png"></img>
                         </a>
                       :
                         <img src="img/kpmg_logo.png" alt="LOGO"/>
                        )
                     }
                  </div>
              </div>
              <div className="row">
                  <input value={username}
                      type="text"
                      id="username"
                      onChange={event => this.setState(byPropKey('username', event.target.value))}
                      placeholder="Username"
                      className="input"
                  />
              </div>
              <div className="row">
                  <input value={password}
                      type="password"
                      id="passsword"
                      onChange={event => this.setState(byPropKey('password', event.target.value))}
                      placeholder="Password"
                      className="input"
                      onKeyDown={() => this.keyPress(event)}
                  />
              </div>
              <div className="row">
                  <button onClick={() => this.onSubmit()} className="sailbutton">
                      Login
                  </button>
              </div>
              {
                (Meteor.settings.public.mode === "production" ?
                  <div className="row">
                    <p className="powered-by">Powered by <a href="https://secureailabs.com/" target="_blank">SAIL</a></p>
                  </div>
                : null)
              }
          </div>
        );

    }
}


export default withRouter(SignIn);
