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

class ForgottenMnemonic extends Component {
  constructor(props) {
    super(props);
    this.state = { recoveredwords:'' };
    this.recoverPass = this.recoverPass.bind(this);
  }

  recoverPass = (event) => {
    const {
      username,
      mnemonic,
    } = this.state;

    const {
      history,
    } = this.props;
    
    self = this;

    Meteor.call('recoverPassword', username, mnemonic, function(error, result) {
      if (error) {
        return console.log("Error");
      } else {
        self.setState({recoveredwords: result});
      }
    });

    event.preventDefault();
  }

  render() {
    const {
      username,
      mnemonic,
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
                <label htmlFor="mnemonic">Insert mnemonic to recover password</label>
                <input
                  value={mnemonic}
                  type="text"
                  className="form-control"
                  id="mnemonic"
                  onChange={event => this.setState(byPropKey('mnemonic', event.target.value))}
                  placeholder="mnemonic"
                />
              </div>
              
               <div>
                  <button onClick={this.recoverPass} className="btn btn-outline-primary">Recover</button>
                </div>

                <label htmlFor="generatewords"> Recovered Password is</label>
                <div>
                  <input
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

export default withRouter(ForgottenMnemonic);