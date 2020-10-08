import React from 'react';
import {Meteor} from 'meteor/meteor';
import AdminInfo from '/imports/ui/components/AdminInfo';
import {withTracker} from 'meteor/react-meteor-data';
import PropTypes from 'prop-types';
import {Form, Table} from 'react-bootstrap';
import {MeteorUsers} from '../../../api/adminboard';
import {Link} from 'react-router-dom';
import {DappFormGroup} from "../../../api/ui/DappFormGroup";
import Button from "react-bootstrap/Button";
import NavBar from '/imports/ui/components/NavBar';

let here;
/** Renders a list of users to the admin. */
class ListUsers extends React.Component {

    constructor(props) {
        super(props);
        here = this;
        this.state = {
            invitationEmail: ""
        }

    }

    deleteUser(id) {
        if (confirm("Are you sure you want to delete this user permanently?")) {
            Meteor.call('deleteUser', id, (error, result) => {
                if (error) {
                    alert("error:", error)
                    this.setState(byPropKey('error', error));
                } else {
                    alert("User Deleted Successfully")
                    // history.push('/')
                }
            });
        }
    }

    sendSignUpInvitation(email) {
        if (confirm("Please confirm that you are inviting " + email)) {
            Meteor.call('sendSignUpEmail', email, (error, result) => {
                if (result !== undefined) {
                    alert("Error: " + result.reason)
                } else {
                    alert("Invitation Sent")
                }
            });
        }
    }

    /** If the subscription has been received, render the page, otherwise show loading...  */
    render() {
        return (this.props.ready) ? this.renderPage() : <div>Loading....</div>;
    }

    /** So subscription worked, render the page. */
    renderPage() {
        const usrId = Meteor.userId();
        if (undefined === usrId || null === usrId) {
          return <Redirect push to="/login" />;
        }
        
        return (
            <div className="container-fluid">
                <div className="row">
                    <div className="navbar-base">
                        <NavBar/>
                    </div>
                    <div className="main-content">
                        <Form>
                            <DappFormGroup
                                label="Email"
                                value={this.state.invitationEmail}
                                type="email"
                                id="invitationEmail"
                                onChange={event => this.setState({invitationEmail: event.target.value})}
                                placeholder="Enter Email Address"/>
                            <Link to="/register" className="float-right btn btn-link">OR Create a New User</Link>
                            <Button onClick={() => this.sendSignUpInvitation(this.state.invitationEmail)}
                                    className="float-right btn btn-primary">Send Invite Link</Button>
                        </Form>
                        <br/>
                        <br/>
                        <ul style={{listStyleType: "none"}} className="data-list">
                            {this.props.userLists.map(function (userlist) {
                                return (
                                    <li className="group2" key={userlist._id}>
                                        <AdminInfo key={userlist._id}
                                                   userlist={userlist}
                                                   deleteUser={here.deleteUser}/>)}
                                    </li>
                                );
                            })}
                        </ul>
                    </div>
                </div>
            </div>
        );
    }
}

/** A list of required items. */
ListUsers.propTypes = {
    userLists: PropTypes.array.isRequired,
    ready: PropTypes.bool.isRequired,
};

/** withTracker - react components and data connected. */
export default withTracker(() => {

    const subscription = Meteor.subscribe('DetailsAdmin');
    return {
        userLists: MeteorUsers.find({}).fetch(),
        ready: subscription.ready(),
    };
})(ListUsers);
