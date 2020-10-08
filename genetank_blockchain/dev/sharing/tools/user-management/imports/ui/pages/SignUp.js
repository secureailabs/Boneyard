import React, { Component } from 'react';
import { withRouter } from 'react-router-dom';
//let bip39 = require('bip39');


const INITIAL_STATE = {
  email: '',
  username: '',
  password: '',
  generatewords: '',
  error: null,
};

const byPropKey = (propertyName, value) => () => ({
  [propertyName]: value,
});

class SignUp extends Component {
  constructor(props) {
    super(props);
   
  //  this.state = { ...INITIAL_STATE };
    this.state = {
      words: ''
    }

    this.onSubmit = this.onSubmit.bind(this);
    this.genenerateWords = this.genenerateWords.bind(this);
  }

  genenerateWords(event) {
   
    console.log('Button pressed');

    var self = this;
    Meteor.call('generateWords', function(error, result) {
      if (error) {
        return console.log("Error");
      } else {
        console.log("result is:", result);
        self.setState({words: result});
      }
    });

    event.preventDefault();
  }

  onSubmit = (event) => {
    const {
      email,
      username,
      generatewords,
      password,
      confirm_password,
      data
    } = this.state;

    const {
      history,
    } = this.props;

    console.log('generatewords:', this.state.words)
    console.log('username:', username)
    console.log('password:', password)
    console.log('confirm password:', confirm_password)

   // var that = this;
    Meteor.call('createKeys', this.state.words, username, password, confirm_password, function(error, result) {
      if (error) {
        return console.log("Error:", error)
      } else {
        console.log("result is:", result)
      }
    });
   
    console.log("Create the accounts")
    Accounts.createUser({
     // email,
      username,
      password
    }, error => {
      if (error) {
        console.log("error:", error)
        this.setState(byPropKey('error', error));
      } else {
        this.setState(() => ({ ...INITIAL_STATE }));
        console.log('redirect home')
        history.push('/password');
      }
    });

    event.preventDefault();
  }

  render() {
    const {
      email,
      username,
      password,
      confirm_password,
      generatewords,
      error,
    } = this.state;

    return (
      <div className="container">
        <div className="row mt-5">
          <div className="col-md-6 ml-auto mr-auto">
            <form onSubmit={this.onSubmit}>
              <legend className="border border-top-0 border-left-0 border-right-0">Register</legend>
              <div className="form-group">

                <label htmlFor="generatewords">Generate 12 words</label>
                <div>
                  <button onClick={this.genenerateWords} className="btn btn-outline-primary">Generate</button>
                </div>
                <small id="emailHelp" className="form-text text-muted">Save these words in a secure place</small>
                <div>
                  <input
                      type="text"
                      className="form-control"
                      value = {this.state.words}
                      readOnly
                  />
                </div>
               
              </div>
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
                <label htmlFor="passsword">Confirm Password</label>
                <input
                  value={confirm_password}
                  type="password"
                  className="form-control"
                  id="passsword"
                  onChange={event => this.setState(byPropKey('confirm_password', event.target.value))}
                  placeholder="Confirm Password"
                />
              </div>
              <button type="submit" className="btn btn-outline-primary">Submit</button>


              { error && <p>{error.reason}</p>}
            </form>
          </div>
        </div>
      </div>
    );
  }
}

export default withRouter(SignUp);
