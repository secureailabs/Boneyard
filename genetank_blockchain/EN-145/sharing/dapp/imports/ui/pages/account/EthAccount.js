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
                      <div class="moduleArea">
                        <div class="row tableHeader">
                             <div class="col-md-2">Account Name</div>
                             <div class="col-md-4">SAIL Key</div>
                             <div class="col-md-2">Balance (in ethers)</div>
                             <div class="col-md-2">Set Default</div>
                             <div class="col-md-2">Delete</div>
                         </div>
                      </div>
                      <div className="moduleArea">
                        <div className="row">
                          <div class="col-sm-2 centerVertical">
                            <span className="span1">{this.props.listValue.accName}</span>
                          </div>
                          <div class="col-sm-4 centerVertical">
                            <span className="span2">{this.props.listValue.account}</span>
                          </div>
                          <div class="col-sm-2 centerVertical">
                            <span>{this.props.accBalance[this.props.listValue.account]}</span>
                          </div>
                          <div class="col-sm-2 centerVertical">
                            {this.props.listValue.account ?
                              <div>
                                  <input type="radio" value={this.props.listValue.account}
                                         checked={this.props.listValue.account === this.props.defaultAccount}
                                         onChange={() => this.props.setDefaultEthAccount(this.props.listValue.account)}>
                                  </input>
                              </div> : null}
                          </div>
                          <div class="col-sm-2 centerVertical">
                            {this.props.listValue.account ?
                              <i className="far fa-trash-alt" onClick={() =>
                                  this.props.deleteEthAccount(this.props.listValue.account)}>
                              </i> : null}
                          </div>
                        </div>
                      </div>
                    </div>
                </div>
            </div>

        );
    }
}
