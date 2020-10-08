import React from 'react';
import {Meteor} from 'meteor/meteor';
import {Roles} from 'meteor/alanning:roles';
import {
  NavDropdown,
  Row,
  Col,
  Glyphicon
} from "react-bootstrap";
import '@trendmicro/react-sidenav/dist/react-sidenav.css';
import Nav from "react-bootstrap/Nav";
import PropTypes from 'prop-types';
import { withTracker } from 'meteor/react-meteor-data';
import {withRouter} from 'react-router-dom';


/** The NavBar appears at the left side of every page. Rendered by the App Layout component. */
class SideNav extends React.Component {
    constructor(props) {
      super(props);
      const location = props.location;
    }

    handleLogout() {
        console.log(Meteor.userId())
        Meteor.logout((err, result) => {
            if (err){
                alert("here")
            }
        })
    }

    render() {
        return (
          <div className="container navigation">
              <div className="row navLogo">
                  <a href="https://secureailabs.com/" target="_blank">
                      <img src="https://secureailabs.com/img/logo.png"/>
                  </a>
              </div>
              <Nav variant="pills" activeKey={"/" + location.pathname.split("/")[1]} className="nav-side-bar flex-column">
              {Roles.userIsInRole(Meteor.userId(), 'admin') ? (
                  <div className="row navItem">
                      <Nav.Link href="/admin">
                        <i className="fas fa-user-cog"></i>
                        <span>&nbsp;&nbsp;Manage Users</span>
                      </Nav.Link>
                  </div>
                  ) : ''
              }
              <div className="row navItem">
                  <Nav.Link href="/analysis">
                    <i className="fas fa-chart-bar"></i>
                    <span>&nbsp;&nbsp;Research</span>
                  </Nav.Link>
              </div>
              <div className="row navItem">
                  <Nav.Link href="/view">
                    <i className="fas fa-share-square"></i>
                    <span>&nbsp;&nbsp;Local Datasets</span>
                  </Nav.Link>
              </div>
              <div className="row navItem">
                  <Nav.Link href="/access">
                    <i className="fas fa-folder-open"></i>
                    <span>&nbsp;&nbsp;Remote Datasets</span>
                  </Nav.Link>
              </div>
              <div className="row navItem">
                  <Nav.Link href="/enclaves">
                    <i className="fas fa-cog"></i>
                    <span>&nbsp;&nbsp;Manage Data Networks</span>
                  </Nav.Link>
              </div>
              <div className="row navItem logout">
                  <Nav.Link href="/" onClick={() => this.handleLogout()}>
                    <i className="fas fa-sign-out-alt"></i>
                    <span>&nbsp;&nbsp;Logout</span>
                  </Nav.Link>
              </div>
            </Nav>
          </div>
        );
    }
}

/** Declare the types of all properties. */
SideNav.propTypes = {
    currentUser: PropTypes.string,
};

/** withTracker connects Meteor data to React components. https://guide.meteor.com/react.html#using-withTracker */
const NavBar = withTracker(() => ({
    currentUser: Meteor.user() ? Meteor.user().username : '',
}))(SideNav);

/** Enable ReactRouter for this component. https://reacttraining.com/react-router/web/api/withRouter */
export default withRouter(NavBar);
