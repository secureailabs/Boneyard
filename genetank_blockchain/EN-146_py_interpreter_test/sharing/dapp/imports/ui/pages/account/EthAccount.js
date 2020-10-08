import React, {Component} from 'react';
import ReactTable from 'react-table';
import {Container, Col, Form, Button} from 'react-bootstrap';
import {Link} from "react-router-dom";

export default class EthAccount extends Component {

    render() {


        return(
            <div className="">
                <div>
                    <div className="">
                        <div className="buttoncopy8">
                            <div className="rectangle1">
                            </div>
                        </div>
                        <div className="ownerusernamecom">
                            <span className="span1">EthKey: </span><span className="span2">{this.props.listValue.account}</span>
                        </div>
                        <div className="privacyd12s5dp">
                            <span className="span1"></span><span style={{position:"relative", bottom: "15px", left: "75px"}} className="span2">Set Default : </span>
                        </div>
                        <div className="bitmap">
                            <div className="mask">
                            </div>
                            <div className="group31">
                                <img src="img/share-data-group-3-7@2x.png" className="group32"/>
                            </div>
                        </div>
                        <div className="colorblackcopy5">
                            {this.props.listValue.account ?
                                <i style={{position: "relative", bottom: "10px"}}
                                   className="far fa-trash-alt btn text-white" onClick={() =>
                                    this.props.deleteEthAccount(this.props.listValue.account)}>
                                </i> : null}
                        </div>
                        <div className="datatitled123456">
                            <span className="span1">Account Name : {this.props.listValue.accName}</span>
                        </div>
                        <div>
                            <div className="colorblackcopy6">
                                <div>
                                    {this.props.listValue.account ?
                                        <div>
                                            <input type="radio" value={this.props.listValue.account}
                                                   checked={this.props.listValue.account === this.props.defaultAccount}
                                                   onChange={() => this.props.setDefaultEthAccount(this.props.listValue.account)}>
                                            </input>
                                        </div> : null}
                                </div>
                            </div>
                        </div>
                        <div className="activecopy">
                            <div style={{color: "white"}}>Balance: {this.props.accBalance[this.props.listValue.account]} (in Ethers)</div>
                        </div>
                    </div>
                </div>
            </div>

        );
    }
}
