import React, { Component } from 'react';
import ReactTable from 'react-table';
import { withRouter } from 'react-router-dom';
import { listEnclaves } from '/imports/api/usercfg.js'
import { Redirect } from 'react-router'
import { Random } from 'meteor/random'
import { EJSON } from 'meteor/ejson'
import { Form, Container, Row, Col, Button, Tabs, Tab } from 'react-bootstrap'


export class TaskManager extends Component {
	constructor(props) {
		super(props);
		this.state = {
			panes:[],
			activeKey: 0,
			resultR: '',
			enclaves: [],
			pubKey: '',
			error: '',
			ethAdd:'',
			statusDetails:[],
			tid2Kill: '',
		};
		this.selectEnclave = this.selectEnclave.bind(this)
		this.onClickGetStatus = this.onClickGetStatus.bind(this)
		this.onClickClearTaskList = this.onClickClearTaskList.bind(this)
		this.selectTid2Kill = this.selectTid2Kill.bind(this)
		this.onClickKillTask = this.onClickKillTask.bind(this)
		this.onClickRunCycleTest = this.onClickRunCycleTest.bind(this)
	}

	selectEnclave(event) {
		var pubKey = event.target.value;
		this.setState({ pubKey: pubKey })
	}

	selectTid2Kill(event) {
		var state = this.state
		var tid2Kill = event.target.value;
		state.tid2Kill = tid2Kill
		this.setState(state)
	}


	getCfg = () => {
		Meteor.call("userGetCfg", (err, result) => {
			if (!err) {
				if (result !== null) {
					var state = this.state;
					state.enclaves = result.enclaves;
					state.ethAdd = result.defaultEthAccount;
					this.setState(state)
				}
			}
		})
	}

	componentDidMount() {
		this.getCfg();
	}

	listTid = (data) =>{
		if(data !== undefined){
			var dataFilter = data.filter((taskDet) => {
				console.log(typeof(taskDet.taskStatus))
				return taskDet.taskStatus !== "FINISHED"
			})
			return(dataFilter.map(
				(taskDet) => {
					var tid = taskDet.tid
					return(<option value={tid} key={tid.toString()}>
						TID:{tid}</option>)
					}
					))
				}
				}

	clearTaskList(pubKey,ethAdd){
		if(!(pubKey =='' ||  pubKey == 'Select')){
			Meteor.call("clearTaskList", pubKey, ethAdd, (err,result) => {
				if (err) {
					alert(err);
				} else if (result.error)
				alert(result.error)
				else {
					var state = this.state;
					state.statusDetails = [];
					this.setState(state);
					alert("Task List Cleared")
				}
			})
		}
	}

	killTask(pubKey,ethAdd){
		if(!(pubKey =='' ||  pubKey == 'Select')){
			Meteor.call("killTask", pubKey, ethAdd, this.state.tid2Kill, (err,result) => {
				if (err) {
					alert(err);
				} else if (result.error)
				alert(result.error)
				else {
					this.getTaskStatus(pubKey,ethAdd)
					//alert("Task Killed")
					}
				})
		}
	}

	getTaskStatus(pubKey,ethAdd){
	  if(!(pubKey =='' ||  pubKey == 'Select')){
	    Meteor.call("getTaskStatus", pubKey, ethAdd, (err,result) => {
	      if (err) {
		alert(err);
	      } else if (result.error)
		alert(result.error)
	      else {
		var state = this.state;
		state.statusDetails = result;
		this.setState(state);
	      }
	    })
	  }
	}


	onClickGetStatus(event){
	  const pubKey =this.state.pubKey
	  const ethAdd = this.state.ethAdd
	  if ( pubKey === '') {
      		alert("Please select an enclave public key.")
      		return;
  	  }
  	  this.getTaskStatus(pubKey,ethAdd);
	}

  onClickRunCycleTest(event){
    const pubKey =this.state.pubKey
    const ethAdd = this.state.ethAdd
    if(pubKey =='' ||  pubKey == 'Select'){
      		alert("Please select an enclave public key.")
      		return;
    }
      Meteor.call("runSubCycleTest", pubKey, ethAdd, (err,result) => {
        if (err) {
	  alert(err);
	} else if (result.error)
	  alert(result.error)
	else {
          alert("Run sub code successful.")
	}
      })
  }

  onClickClearTaskList(event){
    const pubKey =this.state.pubKey
    const ethAdd = this.state.ethAdd
    if ( pubKey === '') {
      alert("Please select an enclave public key.") //throw alert if no public key is selected
      return;
    }
    this.clearTaskList(pubKey,ethAdd);
  }

  onClickKillTask(event){
	const pubKey = this.state.pubKey
	const ethAdd = this.state.ethAdd
	if ( pubKey === '') {
			alert("Please select an enclave public key.") //throw alert if no public key is selected
			return;
	}
	this.killTask(pubKey,ethAdd)
  }

jsonString = (str) => {
	if(str === undefined)
		return ""
	else return JSON.stringify(str)
}

render() {
      //if (Meteor.user() === null)
      //  return ( <Login /> )
      const { panes, enclaves, error, } = this.state;

      const usrId = Meteor.userId();
      if (undefined === usrId || null === usrId) {
      	return <Redirect push to='/data/login' />
      }
      const data_table = this.state.statusDetails

/*
[{"taskType":906,"result":"\nOUTPUT:\nabcd\nERRORMSG:","msgType":907,"startTime":38324978,"taskStatus":4},{"taskType":906,"result":"\nOUTPUT:\nabcd\nERRORMSG:","msgType":907,"startTime":38324978,"taskStatus":4}]

   const data_table = [{
     	taskType: '1',
     	result: 26,
      msgType: 123,
			startTime: 'hello',
			taskStatus:
    }];
		const dummy = {"session_id":1, "taskDetails" : [ {"tid":2885678848,"taskType":906,"result":"\nOUTPUT:\nabc\n\nERRORMSG:","msgType":907,"startTime":38488137,"taskStatus":"FINISHED"},{"tid":2877286144,"taskType":906,"result":"\nOUTPUT:\nabcd\n\nERRORMSG:","msgType":907,"startTime":38488145,"taskStatus":"FINISHED"}], "msgType":1201}
		const dummy1 = dummy["taskDetails"]
{"session_id":1, "taskDetails" : [ {"tid":2885678848,"taskType":906,"result":"\nOUTPUT:\nabc\n\nERRORMSG:","msgType":907,"startTime":38488137,"taskStatus":"FINISHED"},{"tid":2877286144,"taskType":906,"result":"\nOUTPUT:\nabcd\n\nERRORMSG:","msgType":907,"startTime":38488145,"taskStatus":"FINISHED"}], "msgType":1201}
*/


const columns_table = [
	{
		Header: 'Task ID',
	  accessor: 'tid'
	},{
		Header: 'Task Type',
		accessor: 'taskType' // String-based value accessors!
  },{
      Header: 'Start Time', // Required because our accessor is not a string
      accessor: 'startTime',
  },{
      Header: 'Task Status', // Custom header components!
      accessor: 'taskStatus'
  },{
  	Header: 'Task Output',
  	accessor: 'output',
  	Cell: props => this.jsonString(props.value)
  }, {
      Header: 'Task Output Status', // Required because our accessor is not a string
      accessor: 'msgType',
  }, {
  	Header: 'Error',
  	accessor: 'error',}
  	];

  	return(
  		<div>
		    <div className='welcome-message'>
		        <div>
		            <h4>Welcome to <strong>SAIL</strong></h4>
		        </div>
		    </div>
		    <div className='login-message'>
		        Select the enclave.
		        <br/>
		        <div className="container">
		            <div className="row mt-6">
		                <div className="col-md-12 ml-auto mr-auto">
		                    <select value={this.state.pubKey} className="form-control" id="enclaveAcc" onChange={this.selectEnclave}>
					<option defaultValue="">Select</option>
					{listEnclaves(enclaves)}
				    </select>
				</div>
		            </div>
		            <br/>
		            <div className='row'>
		                <div className="col-md-2 ml-auto mr-auto">
		                    <button className="btn btn-success" id="getTaskStatus" onClick={this.onClickGetStatus}>Get Task Status</button>
		                </div>
		                <div className="col-md-2 ml-auto mr-auto">
		                    <button className="btn btn-primary"
                                      id="Run Cycle Test"
                                      onClick={this.onClickRunCycleTest}>
                                      Run Cycle Test</button>
		                </div>
		                <div className="col-md-2 ml-auto mr-auto">
		                    <button className="btn btn-danger" id="clearTask" onClick={this.onClickClearTaskList}>Clear Task</button>
		                </div>

				<div className="col-md-2 ml-auto mr-auto">
					<select value={this.state.tid2Kill} className="form-control" id="tidAcc" onChange={this.selectTid2Kill}>
					<option defaultValue="">Select tid to kill</option>
					{this.listTid(data_table)}
					</select>
				</div>
				<div className="col-md-2 ml-auto mr-auto">
					<button className="btn btn-danger" id="killTask" onClick={this.onClickKillTask}>Kill Task</button>
				</div>
		            </div>
		        </div>
		        <br/>
		    </div>
		    <div className="container">
		        <ReactTable data={data_table} columns={columns_table} />
		    </div>
		</div>
  		)
  }
}
