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
            username:"",
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

    sendSignUpInvitation(username, email) {
        if (!username || !email) {
          alert("Error: " + (!username ? "User name " : "Email ") + "cannot be empty.");
          return;
        }
        if (confirm("Please confirm that you are inviting " + email)) {
            Meteor.call('sendSignUpEmail', username, email, (error, result) => {
                if (result !== undefined) {
                    alert("Error: " + result.reason)
                } else {
                    alert("Invitation Sent")
                    this.setState({
                      username: "",
                      invitationEmail: ""
                    });
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
                      <div class="row">
                          <div class="col-md-8">
                          <h1>Manage User Access</h1>
                          </div>
                          <div class="col-md-4 alignbutton">
                              <a href="/admin/register" className="upload-btn">Create New User</a>
                          </div>
                      </div>
                      <br /><p className="headline">Create, update, and remove users from the SAIL app.</p>
                      <div>
                          <div class="moduleArea">
                          <div class="row tableHeader">
                              <div class="col-md-1"></div>
                              <div class="col-md-2">Account Name</div>
                              <div class="col-md-3">Email Address</div>
                              <div class="col-md-2">Account Verified</div>
                              <div class="col-md-2">Role</div>
                              <div class="col-md-2">Delete</div>
                          </div>
                          </div>
                          <div className="data-list">
                              {this.props.userLists.map(function (userlist) {
                                  return (
                                      <div className="group2" key={userlist._id}>
                                          <AdminInfo key={userlist._id}
                                                  userlist={userlist}
                                                  deleteUser={here.deleteUser}/>
                                      </div>
                                  );
                              })}
                          </div>
                          <div class="alignbutton"><a href="/admin/wallet" className="upload-btn">Manage SAIL Accounts</a></div>
                      </div>
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
