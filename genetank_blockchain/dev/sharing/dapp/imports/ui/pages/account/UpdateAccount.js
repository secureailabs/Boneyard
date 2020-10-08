import React, {Component} from "react";
import {Row, Col, Container, Button} from 'react-bootstrap';
import {withTracker} from 'meteor/react-meteor-data';
import PropTypes from 'prop-types';
import {MeteorUsers} from "../../../api/adminboard";
import {Meteor} from "meteor/meteor";
import {Link} from "react-router-dom";
import NavBar from '/imports/ui/components/NavBar';
import {DappFormGroup} from "../../../api/ui/DappFormGroup";

const CPW_INIT_STATE = {
    _id: '',
    username: '',
    email: '',
    new_password: '',
    reenter_new_pass: '',
    isAdmin: false
};


const byPropKey = (propertyName, value) => () => ({
    [propertyName]: value,
});

class UpdateAccount extends Component {
    constructor(props) {
        super(props);
        this.state = {...CPW_INIT_STATE};
    }

    componentDidMount() {
    }

    async componentWillReceiveProps(nextProps, nextContext) {
        if (this.props !== nextProps) {
            this.setState({
                _id: nextProps.location.state._id,
                username: nextProps.user.username,
                email: nextProps.user.emails[0].address,
            });

            const meteorCallPromise = (method, ...myParameters) => {
                return new Promise((resolve, reject) => {
                    Meteor.call(method, ...myParameters, (err, res) => {
                        if (err) reject('Something went wrong');
                        resolve(res);
                    });
                });
            }

            this.result = new ReactiveVar();
            this.result.set(false);
            this.result.set(await meteorCallPromise('getRole', nextProps.location.state._id, 'admin'));

            this.setState({
                isAdmin: this.result.curValue
            });
        }
    }

    setUsername = (event) => {
        const {
            _id,
            username,
        } = this.state;

        Meteor.call("updateUserName", _id, username,
            (error, result) => {
                if (result != undefined) {
                    alert("Error: " + result.reason)
                } else {
                    alert("Username Updated Successfully")
                }
            });
        event.preventDefault();
    }

    setEmail = (event) => {
        const {
            _id,
            email,
        } = this.state;

        Meteor.call("updateUserEmail", _id, email,
            (error, result) => {
                if (result != undefined) {
                    alert("Error: " + result.reason)
                } else {
                    alert("Email Updated Successfully")
                }
            });
        event.preventDefault();
    }

    setPassword = (event) => {
        const {
            _id,
            new_password,
            reenter_new_pass,
        } = this.state;

        if (new_password === reenter_new_pass) {
            if (confirm("Are you sure you want to overwrite any existing password for this user?")) {
                Meteor.call('setPassword', _id, new_password, (error, result) => {
                    if (result !== undefined) {
                        alert("Error:" + result.reason)
                    } else {
                        alert("New Password Set Successfully")
                        this.setState(byPropKey('new_password', ''));
                        this.setState(byPropKey('reenter_new_pass', ''));
                    }
                });
            }
        } else {
            alert("Reentered password doesn't match.")
        }
        event.preventDefault();
    }

    makeRemoveAdminUser = (userId, isAdmin) => {
        Meteor.call('makeOrRemoveAdmin', userId, !isAdmin)
    }


    /** If the subscription has been received, render the page, otherwise show loading...  */
    render() {
        return (this.props.ready) ? this.renderPage() : <div>Loading....</div>;
    }


    renderPage() {
        const {
            username,
            email,
            new_password,
            reenter_new_pass,
        } = this.state;

        return (
            <div className="container-fluid">
                <div className="row">
                    <div className="navbar-base">
                        <NavBar/>
                    </div>
                    <div style={{position: "relative", left: "600px", color: "white", width: "1000px"}}
                         className="main-content">
                        <Link to="/admin" className="btn btn-secondary"
                              style={{backgroundColor: "#e5e5e5", color: "black"}}>Go Back</Link>
                        <br/>
                        <br/>
                        <br/>
                        <legend className="border border-top-0 border-left-0 border-right-0">Update Account Details
                        </legend>
                        <Row>
                            <Col>
                                <DappFormGroup onSubmit={this.setUsername}
                                               label="Set/Update Username"
                                               value={username}
                                               type="text"
                                               className="form-control"
                                               id="username"
                                               onChange={event => this.setState(byPropKey('username', event.target.value))}
                                               placeholder="Enter Username"
                                />

                            </Col>
                            <Col>
                                <Button onClick={() => this.setUsername()}
                                        variant="secondary" style={{backgroundColor: "#e5e5e5", color: "black"}}>Set</Button>
                            </Col>
                        </Row>
                        <Row>
                            <Col>
                                <DappFormGroup
                                    label="Set/Update Email Address"
                                    value={email}
                                    type="email"
                                    className="form-control"
                                    id="email"
                                    onChange={event => this.setState(byPropKey('email', event.target.value))}
                                    placeholder="Enter Email"
                                />
                            </Col>
                            <Col>
                                <Button onClick={() => this.setEmail()}
                                        variant="secondary" style={{backgroundColor: "#e5e5e5", color: "black"}}>Set</Button>
                            </Col>
                        </Row>
                        <div>
                            <label>Admin User</label>
                            <br/>
                            <input type="checkbox"
                                   checked={this.state.isAdmin}
                                   onChange={() => this.makeRemoveAdminUser(this.state._id, this.state.isAdmin)}>
                            </input>
                        </div>
                        <legend className="border border-top-0 border-left-0 border-right-0">Update User Password
                        </legend>
                        <DappFormGroup
                            label="Set/Change Password"
                            value={new_password}
                            type="password"
                            className="form-control"
                            id="password"
                            onChange={event => this.setState(byPropKey('new_password', event.target.value))}
                            placeholder="Enter New Password"
                        />
                        <DappFormGroup
                            label="Re-enter Password"
                            value={reenter_new_pass}
                            type="password"
                            className="form-control"
                            id="reenter-password"
                            onChange={event => this.setState(byPropKey('reenter_new_pass', event.target.value))}
                            placeholder="Re-enter Password"
                        />
                        <Button onClick={() => this.setPassword()} variant="secondary" style={{backgroundColor: "#e5e5e5", color: "black"}}>Set/Change
                            Password</Button>
                    </div>
                </div>
            </div>
        );
    }
}

/** A list of required items. */
UpdateAccount.propTypes = {
    user: PropTypes.object,
    ready: PropTypes.bool.isRequired,
};

/** withTracker - react components and data connected. */
export default withTracker((props) => {

    const subscription = Meteor.subscribe('DetailsAdmin');
    return {
        user: MeteorUsers.find({_id: props.location.state._id}).fetch()[0],
        ready: subscription.ready(),
    };
})(UpdateAccount);
