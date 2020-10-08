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
            <ul className="nav-ul">
                <li>
                    <div>
                        <img src="/user_icon.svg" className="bitmap1"/>
                    </div>
                    <div className="down-arrow">
                        <NavDropdown id="basic-nav-dropdown" title={this.props.currentUser}>
                            {Roles.userIsInRole(Meteor.userId(), 'admin') ? (
                                <NavDropdown.Item href={"/admin"}> Admin </NavDropdown.Item>) : ''
                            }
                            {Roles.userIsInRole(Meteor.userId(), 'admin') ? (
                                <NavDropdown.Item href={"/wallet"}>
                                  SAIL Accounts </NavDropdown.Item>) : ''
                            }
                            <NavDropdown.Item href={"/login"} onClick={() => this.handleLogout()}>
                                Logout
                            </NavDropdown.Item>
                        </NavDropdown>
                    </div>
                </li>
                <Nav variant="pills" activeKey={location.pathname} className="nav-side-bar flex-column">
                <li>
                  <Nav.Link href="/view" className="nav-item">
                    <i className="fas fa-share-square"></i>
                    <span>
                        &nbsp;&nbsp;Share Data
                    </span>
                  </Nav.Link>
                </li>
                <li>
                    <Nav.Link href="/access" className="nav-item">
                      <i className="fas fa-folder-open"></i>
                      <span>
                          &nbsp;&nbsp;Access Data
                      </span>
                    </Nav.Link>
                </li>
                <li>
                    <Nav.Link href="/analysis" className="nav-item">
                      <i className="fas fa-chart-bar"></i>
                      <span>
                          &nbsp; &nbsp;Analyze Data
                      </span>
                    </Nav.Link>
                </li>
                {
                  (Meteor.settings.public.mode === "sales" ?
                    <li>
                        <Nav.Link href="/about" className="nav-item">
                          <i className="fas fa-eye"></i>
                          <span>
                              &nbsp; &nbsp;Audit Data
                          </span>
                        </Nav.Link>
                    </li>
                  : null)
                }
                <li>
                    <Nav.Link href="/enclaves/manage" className="nav-item">
                      <i className="fas fa-cog"></i>
                      <span>
                          &nbsp; &nbsp;Enclaves
                      </span>
                    </Nav.Link>
                </li>
                {
                  (Meteor.settings.public.mode === "sales" ?
                    <li>
                      <Nav.Link  href="/about" className="nav-item">
                        <i className="fas fa-user-cog"></i>
                        <span>
                            &nbsp; &nbsp;Settings
                        </span>
                      </Nav.Link>
                    </li>
                  : null)
                }
              </Nav>
            </ul>
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
