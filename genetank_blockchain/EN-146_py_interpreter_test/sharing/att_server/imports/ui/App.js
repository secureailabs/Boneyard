import React, { Component } from 'react';
import {DisplayTable} from './index.js';  
// App component - represents the whole app
export default class App extends Component {
 
  render() {
    return (
      <div className="container">
       <h1> Genetank SGX Enclave Attestation Server </h1>
       <DisplayTable />
      </div>
    );
  }
}
