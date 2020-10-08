import React from 'react';
import { Link } from 'react-router-dom';
import { withRouter } from 'react-router-dom';

const Navigation = ({ history }) =>
  <nav className="navbar navbar-expand-lg navbar-light bg-light">
    <a className="navbar-brand" href="#">Genetank</a>
    <button className="navbar-toggler" type="button" data-toggle="collapse" data-target="#navbarSupportedContent" aria-controls="navbarSupportedContent" aria-expanded="false" aria-label="Toggle navigation">
      <span className="navbar-toggler-icon"></span>
    </button>

    <div className="collapse navbar-collapse" id="navbarSupportedContent">
      <ul className="navbar-nav ml-auto">
        <li className="nav-item">
          <Link className="nav-link" to="/">Home <span className="sr-only">(current)</span></Link>
        </li>

        <li className="nav-item">
        {
          Meteor.userId()
          ? <Link className="nav-link" to="/todos">Donor</Link>
          : ''
        }
        </li>

        <li className="nav-item">
        {
          Meteor.userId()
          ? <Link className="nav-link" to="/new-todo">Creator</Link>
          : ''
        }
        </li>

        <li className="nav-item">
        {
          Meteor.userId()
          ? <Link className="nav-link" to="/todos">Verifier</Link>
          : ''
        }
        </li>

        <li className="nav-item">
        {
          Meteor.userId()
          ? <Link className="nav-link" to="/todos">Trainer</Link>
          : ''
        }
        </li>
        
        <li className="nav-item">
        {
          Meteor.userId()
          ? <Link className="nav-link" to="/todos">Runner</Link>
          : ''
        }
        </li>

        <li className="nav-item">
        {
          Meteor.userId()
          ? <a
              className="nav-link"
              onClick={() =>
                Meteor.logout(error => {
                  if (error) {
                    console.log(error);
                  } else {
                    history.push('/');
                  }
                })
              }
              href="#"
            >
              Logout
            </a>
          : <Link className="nav-link" to="/signin">Sign In</Link>
        }
        </li>

        {/* <li className="nav-item dropdown">
          <a className="nav-link dropdown-toggle" href="#" id="navbarDropdown" role="button" data-toggle="dropdown" aria-haspopup="true" aria-expanded="false">
            Dropdown
          </a>
          <div className="dropdown-menu" aria-labelledby="navbarDropdown">
            <a className="dropdown-item" href="#">Action</a>
            <a className="dropdown-item" href="#">Another action</a>
            <div className="dropdown-divider"></div>
            <a className="dropdown-item" href="#">Something else here</a>
          </div>
        </li> */}
      </ul>
    </div>
  </nav>

export default withRouter(Navigation);
