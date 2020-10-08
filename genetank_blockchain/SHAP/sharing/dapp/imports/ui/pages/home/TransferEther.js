import React, { Component } from 'react'
import { Button, Form } from 'react-bootstrap'

export default class TransferEther extends Component {
    constructor(props) {
        super(props);
        this.state = {
            toEthAccount: "",
            transferBalance: 0,
        }
    }

    setToAccount = (account) => {
        this.setState({toEthAccount: account});
    }

    initiateTransfer(fromAcc, toAcc, amount){
        this.props.transferEther(
            fromAcc,
            toAcc,
            amount)
        this.setState({
            toEthAccount: "",
            transferBalance: 0,
        })
    }


    render() {

        let accounts = this.props.state.ethAccountList;
        let optionItems = accounts
            .filter(a => a.account != this.props.state.defaultAccount)
            .map((acc) =>
            <option key={acc.account} value={acc.account}>{acc.accName}</option>
        );

        return(
          <div>
              <br/>
            <Form>
               <legend className="border border-top-0 border-left-0 border-right-0">Transfer Ether between accounts</legend>
                <br/>
               <Form.Group controlId="transAccount">
                 <Form.Label>From Default Account: <b>{this.props.state.ethAccountList
                     .map(acc => {
                         if (acc.account === this.props.state.defaultAccount)
                              return (acc.accName);
                     })}</b>
                   <br/>
                   To Account:
                 </Form.Label>
                 <Form.Control as="select"
                    value={this.state.toEthAccount}
                    onChange={(event) => this.setToAccount(event.target.value)}
                 >
                    <option key="" defaultValue></option>
                    {optionItems}
                 </Form.Control>
               </Form.Group>
               <Form.Group controlId="transAmount">
                 <Form.Label>Amount (in Wei)</Form.Label>
                 <Form.Control
                    type="text"
                    value={this.state.transferBalance}
                    onChange={event =>
                        this.setState({transferBalance: event.target.value})}
                    placeholder="1000"/>
               </Form.Group>
               <Button onClick={() => this.initiateTransfer(
                   this.props.state.defaultAccount,
                   this.state.toEthAccount,
                   this.state.transferBalance)}
                       variant="success" block>
                  Transfer
               </Button>
            </Form>
          </div>
        )
    }
}
