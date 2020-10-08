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
                    alert(error.message)
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

    render() {

        if (this.state.redirectToPage) {
            return <Redirect push to='/view'/>
        }

        const {
            username,
            password,
        } = this.state;
        return(
            <div className="login" >
                <div className="rectangle">
                </div>
                <div className="group-layout-container">
                    <div className="group">
                        <div className="backgroundcard-layout-container">
                            <div className="backgroundcard">
                                <div className="group1-layout-container">
                                    <div className="group1">
                                        <div className="rectangle1">
                                        </div>
                                    </div>
                                </div>
                            </div>
                        </div>

                        <input value={username}
                               type="text"
                               id="username"
                               onChange={event => this.setState(byPropKey('username', event.target.value))}
                               placeholder="Enter Your Username or Email"
                               className="inputhovered border"
                               style={{color: "white"}}/>

                        <input value={password}
                               type="password"
                               id="passsword"
                               onChange={event => this.setState(byPropKey('password', event.target.value))}
                               placeholder="Enter Your Password"
                               className="inputhoveredcopy border"
                               style={{color: "white"}}/>
                        <div>
                            <Link className="forgotpassword" to="forgotPassword">Forgot Password?</Link>
                        </div>
                        <div className="login1">
                            Login
                        </div>
                        <div>
                            <button onClick={() => this.onSubmit()} className="button btn btn-primary bg-dark">
                                Login
                            </button>
                        </div>
                    </div>
                </div>
            </div>
        );

    }
}


export default withRouter(SignIn);
