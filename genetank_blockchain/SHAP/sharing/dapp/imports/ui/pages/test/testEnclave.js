import React, { Component } from 'react';
import { withRouter } from 'react-router-dom';
import { listEnclaves } from '/imports/api/usercfg.js'
import { Redirect } from 'react-router'
import { Random } from 'meteor/random'
import { EJSON } from 'meteor/ejson'
import { Form, Container, Row, Col, Button, Tabs, Tab } from 'react-bootstrap'


export class TestEnclave extends Component {
  constructor(props) {
    super(props);
    this.state = {
      panes:[],
      activeKey: 0,
      resultR: '',
      enclaves: [],
      pubKey: '',
      error: '',
    };
    this.selectEnclave = this.selectEnclave.bind(this)
    this.runTestSession = this.runTestSession.bind(this)
    this.onSubmit = this.onSubmit.bind(this)
  }

  selectEnclave(event) {
    var pubKey = event.target.value;
    this.setState({ pubKey: pubKey })
  }

  getCfg = () => {
    Meteor.call("userGetCfg", (err, result) => {
      if (!err) {
        if (result !== null) {
          var state = this.state;
          state.enclaves = result.enclaves;
          this.setState(state)
        }
      }
    })
  }

  componentDidMount() {
    this.getCfg();
  }


  runTestSession(pubKey,testCode){
    if(!(pubKey =='' ||  pubKey == 'Select')){
      Meteor.call("runTestOnServerSession", pubKey, testCode, (err,result) => {
        if (err) {
          alert(err);
        } else if (result.error)
    	    alert(result.error)
        else {
          document.getElementById("result").innerHTML = "Running Test..."

        }
      })
    }
  }


  onSubmit(event){
    const pubKey =this.state.pubKey
    if ( pubKey === '') {
      alert("Please select an enclave public key.") //throw alert if no public key is selected
      return;
    }
		var testCode = 0
    var testList = document.getElementsByName('testChkBx')
		
		for(var i = 0; i < testList.length; i++){
			if(testList[i].checked){
				switch(testList[i].value){
				case 'sessions' :
					testCode = testCode + 1
					break;
				case 'pyVM':
					testCode = testCode + 2
					break;
				case 'dataUpload' : 
					testCode = testCode + 4
					break;
				case 'getInfo' :
					testCode = testCode + 8
					break;
				default:
					break;
			}
		}
	}
		
		if(testCode == 0){
			alert('Please select a test')
      return;
		}
		if(testCode != 0 && testCode % 2 == 0){
			testCode = testCode + 1
   }
	this.runTestSession(pubKey,testCode)
}


    render() {
      //if (Meteor.user() === null)
      //  return ( <Login /> )
      const { panes, enclaves, error, } = this.state;

      const usrId = Meteor.userId();
      if (undefined === usrId || null === usrId) {
        return <Redirect push to='/data/login' />
      }
      return (
        <div>
        	<div className='welcome-message'>
        		<div>
        			<h4>Welcome to</h4>
        		</div>
        		<div>
        			<h1>GeneTank</h1>
        		</div>
        	</div>
        	<div className='login-message'>
                    Select the enclave and run the test.


        		<br/>
        		<div className="container">
        			<div className="row mt-6">
        				<div className="col-md-12 ml-auto mr-auto">
        					<select  value={this.state.pubKey}
                            className="form-control"
                            id="enclaveAcc"
                            onChange={this.selectEnclave}
                            >
        						<option defaultValue="">Select</option>
                            {listEnclaves(enclaves)}


        					</select>
        				</div>
        			</div>
        		</div>
        		<br/>

        		<div name="checkBox-frame" className="col-md-2 ml-auto mr-auto" align='left'> 
        			<input text-align='left' type="checkbox" name="testChkBx" value="sessions"/> Session Setup
        			<br/>
        			<input text-align='left' type="checkbox" name="testChkBx" value="pyVM"/> pyVM
        			<br/>
        			<input text-align='left' type="checkbox" name="testChkBx" value="dataUpload"/> Data Upload
        			<br/>
        			<input align='left' type="checkbox" name="testChkBx" value="getInfo"/> Get info
        			<br/>
        			<button className="btn btn-success" id="runTest" onClick={this.onSubmit}>Run Test</button>
        		</div>
        			<br/>
        		<div id="result"></div>
        		<div hidden id='hide-show' class="spinner-border" role="status">
        			<span class="sr-only">Loading...</span>
        		</div>
        	</div>
        	<br/>
        	<div className='welcome-message'>
        		<img src='/fitness/icons8-heart-rate.png' title="Heart Rate"/>{' '}


        	</div>
        </div>
      )
    }
  }

