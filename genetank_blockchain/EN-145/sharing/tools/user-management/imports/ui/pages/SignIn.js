import React, { Component } from 'react';
import { withRouter, Link } from 'react-router-dom';

const INITIAL_STATE = {
  username: '',
  password: '',
  error: null,
};

const byPropKey = (propertyName, value) => () => ({
  [propertyName]: value,
});

class SignIn extends Component {
  constructor(props) {
    super(props);

    this.state = { ...INITIAL_STATE };
  }

  onSubmit = (event) => {
    const {
      username,
      password,
    } = this.state;

    const {
      history,
    } = this.props;

    Meteor.loginWithPassword(
      username,
      password,
      error => {
      if (error) {
        this.setState(byPropKey('error', error));
      } else {
        this.setState(() => ({ ...INITIAL_STATE }));
        history.push('/password');
      }
    });

    event.preventDefault();
  }

  render() {
    const {
      username,
      password,
      error,
    } = this.state;

    return (
      <div className="container">
        <div className="row mt-5">
          <div className="col-md-6 ml-auto mr-auto">
            <form onSubmit={this.onSubmit}>
              <legend className="border border-top-0 border-left-0 border-right-0">Login</legend>
              <div className="form-group">
                <label htmlFor="username">Username</label>
                <input
                  value={username}
                  type="text"
                  className="form-control"
                  id="username"
                  onChange={event => this.setState(byPropKey('username', event.target.value))}
                  placeholder="Username"
                />
              </div>
              <div className="form-group">
                <label htmlFor="passsword">Password</label>
                <input
                  value={password}
                  type="password"
                  className="form-control"
                  id="passsword"
                  onChange={event => this.setState(byPropKey('password', event.target.value))}
                  placeholder="Password"
                />
              </div>
              
                <div className="form-group">
                    <button type="submit" className="btn btn-outline-primary">Login</button>
                    <Link to="/register" className="float-right">Not Registered?</Link>
                </div>
                <div className="form-group">
                    <Link to="/password">Forgotten Password?</Link>
                  </div>
                <div className="form-group">
                    <Link to="/mnemonic">Forgotten Mnemonic?</Link>
                </div>
                

              { error && <p>{error.reason}</p>}
            </form>
          </div>
        </div>
      </div>
    );
  }
}

export default withRouter(SignIn);
