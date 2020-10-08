import React from 'react';
import {Meteor} from 'meteor/meteor';
import {Roles} from 'meteor/alanning:roles';
import {NavDropdown} from "react-bootstrap";
import '@trendmicro/react-sidenav/dist/react-sidenav.css';
import Nav from "react-bootstrap/Nav";
import PropTypes from 'prop-types';
import { withTracker } from 'meteor/react-meteor-data';
import {withRouter} from 'react-router-dom';


/** The NavBar appears at the left side of every page. Rendered by the App Layout component. */
class SideNav extends React.Component {

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
            <ul style={{listStyleType: "none", marginTop: "40px", lineHeight: "0.5rem"}}>
                <li>
                    <div>
                        <img src="/user_icon.svg" className="bitmap1"/>
                    </div>
                    <div className="down-arrow">
                        <NavDropdown id="basic-nav-dropdown" title={this.props.currentUser}>
                            {Roles.userIsInRole(Meteor.userId(), 'admin') ? (
                                <NavDropdown.Item href={"/admin"}> Admin </NavDropdown.Item>) : ''
                            }
                            <NavDropdown.Item href={"/wallet"}>
                                Ethereum Accounts</NavDropdown.Item>
                            <NavDropdown.Item href={"/login"} onClick={() => this.handleLogout()} href="/">
                                Logout
                            </NavDropdown.Item>
                        </NavDropdown>
                    </div>
                </li>
                <li>
                    <span>
                        <img src="/img/enclave-icon-left-1@2x.png" className="iconleft2"/>
                    </span>
                    <Nav.Link href="/view" className="nav-item">
                        Share Data
                    </Nav.Link>
                </li>
                <li>
                    <span>
                        <img src="/img/analyze-ide-icon-left@2x.png" className="iconleft2"/>
                    </span>
                    <Nav.Link href="/access" className="nav-item">
                        Access Data
                    </Nav.Link>
                </li>
                <li>
                    <span>
                        <img src="/img/settings-users-icon-left-2@2x.png" className="iconleft1"/>
                    </span>
                    <Nav.Link href="/analysis" className="nav-item">
                        Analyze Data
                    </Nav.Link>
                </li>
                <li>
                    <span>
                        <img src="/img/settings-org-edit-group-7@2x.png" className="iconleft1"/>
                    </span>
                    <Nav.Link href="/" className="nav-item">
                        Audit Data
                    </Nav.Link>
                </li>
                <li>
                    <span>
                        <img src="/img/access-data-saved-icon-left-copy@2x.png" className="iconleft1"/>
                    </span>
                    <Nav.Link href="/enclaves/manage" className="nav-item">
                        Enclaves
                    </Nav.Link>
                </li>
                <li>
                    <span>
                        <img src="/img/access-data-my-data-icon-left-3@2x.png" className="iconleft1"/>
                    </span>
                    <Nav.Link  href="/" className="nav-item">
                        Settings
                    </Nav.Link>
                </li>

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
