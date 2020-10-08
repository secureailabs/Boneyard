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

class ForgottenPassword extends Component {
  constructor(props) {
    super(props);
    this.state = { recoveredwords:'' };
    this.recoverMnem = this.recoverMnem.bind(this);
  }

  recoverMnem = (event) => {
    const {
      username,
      password,
    } = this.state;

    const {
      history,
    } = this.props;

    var self = this;
    Meteor.call('recoverMnemonic', username, password, function(error, result) {
        if (error) {
          return console.log("Error in recovering");
        } else {
          self.setState({recoveredwords: result});
          console.log("resulting array:", result)
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
              <legend className="border border-top-0 border-left-0 border-right-0">Recover Forgotten Mnemonic</legend>
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
                <label htmlFor="passsword">Insert mnemonic to recover password </label>
                <input
                  value={password}
                  type="password"
                  className="form-control"
                  id="password"
                  onChange={event => this.setState(byPropKey('password', event.target.value))}
                  placeholder="password"
                />
              </div>
              
               
                <div>
                  <button onClick={this.recoverMnem} className="btn btn-outline-primary">Recover</button>
                </div>

                <label htmlFor="generatewords"> Recovered Mnemonic is</label>
                <div>
                  <textarea
                      type="text"
                      className="form-control"
                      value = {this.state.recoveredwords}
                      readOnly
                  />
                </div>
                
               
              { error && <p>{error.reason}</p>}
            </form>
          </div>
        </div>
      </div>
    );
  }
}

export default withRouter(ForgottenPassword);