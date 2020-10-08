import { Meteor } from "meteor/meteor";
import React, { Component } from 'react';
import { withRouter } from 'react-router-dom';
import { Redirect } from 'react-router'
import NavBar from '/imports/ui/components/NavBar';

class Contract extends Component {
  constructor(props) {
    super(props);
    this.state = {
      data: [],
      ethereumAddress: '',
    };
    this.loadData = this.loadData.bind(this);
  }

  componentDidMount() {
    Meteor.call("userGetCfg", (err, result) => {
      if (!err) {
        if (result !== null) {
          this.setState({ethereumAddress:result.defaultEthAccount});
        }
      }
    });
    this.loadData();
  }

  getContract() {
    return {
      "name": "Pan Genome 201903",
      "contract":
        "pragma solidity 0.4.25;\n" +
        "/**\n" +
        "  @title Data Usage Agreement for Sample 201903\n" +
        "  @author Dr. Strange\n" +
        "*/\n" +
        "\n" +
        "struct Usage {\n" +
        "  string user;  // ID of the user\n" +
        "  Use use;  // Defined class of microbiome analyses classes\n" +
        "  uint256 expiration;  // When data usage will expire\n" +
        "  while (time < expiration) {\n" +
        "     verify(user)\n"+
        "     checkUse(current_task,use)\n"+
        "  }}\n"
    };
  }

  loadData(dataset) {
    // TODO: Load real datasets by calling the backend
    this.setState({data: this.getContract()});
  }

  render() {
    const data = this.state.data;

    const usrId = Meteor.userId();
    if (undefined === usrId || null === usrId) {
      return <Redirect push to='/login' />
    }

    return (
        <div className="container-fluid">
          <div className="row">
            <div className="navbar-base">
              <NavBar/>
            </div>
            <div style={{position: "relative", left: "600px", backgroundColor: "white", width: "500px"}} className="main-content">
                <h1>{data.name}</h1>
                <pre style={{fontFamily: "monospace"}}>
                  {data.contract}
                </pre>
              </div>
            </div>
          </div>
    )
  };
}
export default withRouter(Contract);
