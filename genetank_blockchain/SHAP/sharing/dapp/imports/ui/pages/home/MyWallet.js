import React, {Component} from 'react';
import EthAccount from '../account/EthAccount';
import TransferEther from './TransferEther';
import {Modal, Button, Col, Form, Tab, Tabs} from 'react-bootstrap';
import {Redirect} from "react-router";
import {Meteor} from "meteor/meteor";
import ImportEthAcc from "../account/ImportEthAcc";
import NavBar from '/imports/ui/components/NavBar';
import {DappFormGroup} from "../../../api/ui/DappFormGroup";
import ListItem from "../datasets/ListItem";

let here;

export class MyWallet extends Component {

    constructor(props) {
        super(props);
        here = this;
        this.state = {
            ethAccountList: [],
            defaultAccount: "",
            accBalance: {},
            ethAccName: '',
            ethPassword: '',
            ethReenterPassword: '',
            showCreateEthModal: false,
            toEthAccount: "",
            transferBalance: 0,
            showTransferEtherModal: false,
            importEthAccName: '',
            importEthPrivateKey: '',
            importEthPassword: '',
            importEthReEnterPassword: '',
            showImportEthModal: false,
            user: "",
            userList: [],
        }
        this.selectUser = this.selectUser.bind(this)
    }

    selectUser(event) {
      const user = event.target.value
      this.setState({user:user})
      this.getEthAccounts(user);
    }

    componentDidMount() {
        this.getEthAccounts();

        Meteor.call("adminGetUserList", (err, result) => {
            if (err)
              return;
            if (result !== null) {
              if (undefined !== result.userList)
                this.setState({userList: result.userList})
            }
        })

    }

    handleCloseCreateEthModal() {
        here.setState({showCreateEthModal: false})
    }

    handleCloseTransferEtherModal() {
        here.setState({showTransferEtherModal: false})
    }

    handleCloseImportEthModal() {
        here.setState({showImportEthModal: false})
    }


    setToAccount = (account) => {
        this.setState({toEthAccount: account});
    }

    initiateTransfer(fromAcc, toAcc, amount) {
        this.transferEther(
            fromAcc,
            toAcc,
            amount)
        this.setState({
            toEthAccount: "",
            transferBalance: 0,
            showTransferEthModal: false
        })
    }

    importEthModal() {
        const {
            importEthAccName,
            importEthPrivateKey,
            importEthPassword,
            importEthReEnterPassword,
            error,
        } = this.state;


        return (
            <Modal
                show={this.state.showImportEthModal}
                onHide={this.handleCloseImportEthModal}
            >
                <div
                    style={{
                        borderStyle: "solid",
                        borderWidth: "1px",
                        borderColor: "white",
                    }}
                    className="">
                    <Modal.Header style={{backgroundColor: "rgba(50, 50, 51, 1.0)", color: "white"}} closeButton>
                        <Modal.Title>Import Ethereum Account</Modal.Title>
                    </Modal.Header>
                    <Modal.Body style={{backgroundColor: "rgba(50, 50, 51, 1.0)", color: "white", fontWeight: "bold"}}>

                        <Form>
                            <DappFormGroup
                                label="Account Name"
                                value={importEthAccName}
                                type="text"
                                onChange={event =>
                                    this.setState({'importEthAccName': event.target.value})}
                                placeholder="Please Enter a Name For This Account"
                            />
                            <DappFormGroup
                                label="Private Key (without 0x)"
                                value={importEthPrivateKey}
                                type="text"
                                onChange={event =>
                                    this.setState({'importEthPrivateKey': event.target.value})}
                                placeholder="Please a private key to import"
                            />
                            <DappFormGroup
                                label="Passphrase"
                                value={importEthPassword}
                                type="password"
                                onChange={event =>
                                    this.setState({'importEthPassword': event.target.value})}
                                placeholder="Please Enter a Passphrase"
                            />
                            <DappFormGroup
                                label="Re-enter Passphrase"
                                value={importEthReEnterPassword}
                                type="password"
                                onChange={event =>
                                    this.setState({'importEthReEnterPassword': event.target.value})}
                                placeholder="Re-enter Passphrase To Confirm"
                            />

                            <Button variant="secondary"
                                    style={{backgroundColor: "#e5e5e5", color: "black", fontWeight: "bold"}}
                                    onClick={() => this.importAccount(importEthAccName, importEthPrivateKey, importEthPassword, importEthReEnterPassword)}>
                                Submit</Button>
                            <br/>
                            {error && <p>{error.reason}</p>}
                            <br/>
                        </Form>
                    </Modal.Body>
                </div>
            </Modal>
        )
    }

    transferEtherModal() {
        let accounts = this.state.ethAccountList;
        let optionItems = accounts
            .filter(a => a.account != this.state.defaultAccount)
            .map((acc) =>
                <option key={acc.account} value={acc.account}>{acc.accName}</option>
            );

        return (

            <Modal
                show={this.state.showTransferEtherModal}
                onHide={this.handleCloseTransferEtherModal}
            >
                <div
                    style={{
                        borderStyle: "solid",
                        borderWidth: "1px",
                        borderColor: "white",
                    }}
                    className="">
                    <Modal.Header style={{backgroundColor: "rgba(50, 50, 51, 1.0)", color: "white"}} closeButton>
                        <Modal.Title>Transfer Ethers Between Accounts</Modal.Title>
                    </Modal.Header>
                    <Modal.Body style={{backgroundColor: "rgba(50, 50, 51, 1.0)", color: "white", fontWeight: "bold"}}>

                        <Form>
                            <Form.Label style={{color: "white"}}>
                                From Default Account: <b>{this.state.ethAccountList
                                .map(acc => {
                                    if (acc.account === this.state.defaultAccount)
                                        return (acc.accName);
                                })}</b>
                                <br/>
                            </Form.Label>
                            <DappFormGroup
                                as="input"
                                type="text"
                                label="To Account"
                                value={this.state.toEthAccount}
                                onChange={(event) => this.setState(
                                  {toEthAccount: event.target.value})}
                                placeholder="0x..."
                            />
                            <DappFormGroup
                                label="Amount (in Wei)"
                                value={this.state.transferBalance}
                                onChange={event =>
                                    this.setState({transferBalance: event.target.value})}
                                placeholder="1000"/>

                            <Button onClick={() => this.initiateTransfer(
                                this.state.defaultAccount,
                                this.state.toEthAccount,
                                this.state.transferBalance)}
                                    variant="secondary"
                                    style={{backgroundColor: "#e5e5e5", color: "black", fontWeight: "bold"}}>
                                Initiate Transfer
                            </Button>
                        </Form>
                    </Modal.Body>
                </div>
            </Modal>
        )
    }

    createEthAccModal(ethAccName, ethPassword, ethReenterPassword) {
        return (
            <Modal
                show={this.state.showCreateEthModal}
                onHide={this.handleCloseCreateEthModal}
            >
                <div
                    style={{
                        borderStyle: "solid",
                        borderWidth: "1px",
                        borderColor: "white",
                    }}
                    className="">
                    <Modal.Header style={{backgroundColor: "rgba(50, 50, 51, 1.0)", color: "white"}} closeButton>
                        <Modal.Title>Create Ethereum Account</Modal.Title>
                    </Modal.Header>
                    <Modal.Body style={{backgroundColor: "rgba(50, 50, 51, 1.0)", color: "white", fontWeight: "bold"}}>

                        <Form>
                            <DappFormGroup
                                label="Account Name"
                                value={ethAccName}
                                type="text"
                                onChange={event =>
                                    this.setState({ethAccName: event.target.value})}
                                placeholder="Please Enter A Name For This Account"
                            />
                            <DappFormGroup
                                label="Passphrase"
                                value={ethPassword}
                                type="password"
                                onChange={event =>
                                    this.setState({ethPassword: event.target.value})}
                                placeholder="Please Enter a Passphrase To Generate Your Account's Address"
                            />
                            <DappFormGroup
                                label="Re-enter Passphrase"
                                value={ethReenterPassword}
                                type="password"
                                onChange={event =>
                                    this.setState({ethReenterPassword: event.target.value})}
                                placeholder="Re-enter Passphrase To Confirm"
                            />
                            <Button variant="secondary"
                                    style={{backgroundColor: "#e5e5e5", color: "black", fontWeight: "bold"}}
                                    onClick={() => this.createAccount(ethAccName, ethPassword, ethReenterPassword)}>
                                Submit</Button>
                        </Form>
                    </Modal.Body>
                </div>
            </Modal>
        )
    }

    getEthAccounts = (user) => {
        if (undefined === user)
           user = this.state.user
        Meteor.call("userGetEthInfo", user, (err, result) => {
            if (err)
              return;
            if (result !== null) {
              this.setState({ethAccountList: result.blockchainAccounts})
              this.setState({defaultAccount: result.defaultEthAccount});
              if (this.state.user === "" || this.state.user === "admin")
                result.blockchainAccounts.forEach(function (acc) {
                  Meteor.call('getBalance', acc.account, (error, resultBalance) => {
                    if (error) {
                      console.log("error:" + error)
                    } else if (undefined !== resultBalance.error)
                      console.log(resultBalance.error)
                    else {
                      here.state.accBalance[acc.account] = resultBalance.balance
                      here.forceUpdate();
                    }
                  })
                })
            }
        });
    }

    setDefaultEthAccount = (ethAcc) => {
      if (this.state.user === "" || this.state.user === "admin")
        Meteor.call('setDefaultEthAcc', ethAcc, (error, result) => {
            if (error) {
                alert("error:" + error)
            } else {
                console.log("Default Account Set Successfully");
                this.setState({defaultAccount: ethAcc});
            }
      })
    }

    deleteEthAccount = (ethAcc) => {
        // console.log(ethAcc)
        if (confirm("Are you sure you want to delete this Ethereum Account permanently?"))
        Meteor.call('userDeleteEthAcc', ethAcc, this.state.user,
          (error, result) => {
            if (error) {
                alert("error:" + error)
            } else {
                console.log(result.blockchainAccounts)
                console.log(this.state.ethAccountList)
                if (result.blockchainAccounts !== this.state.ethAccountList) {
                    this.getEthAccounts()
                    alert("Account Deleted Successfully")
                }
            }
        })
    }

    onSubmit = (ethAccName, ethPassword, ethReEnterPassword) => {

        if (ethPassword === ethReEnterPassword)
            Meteor.call('userAddEthAcc', ethAccName, ethPassword, 
              this.state.user, (error, result) => {
                if (error) {
                    alert("error:" + error)
                } else {
                    if (result.blockchainAccounts !== this.state.ethAccountList) {
                        this.getEthAccounts();
                        alert("Account Created Successfully")
                    }
                }
            });
        else
            alert("Reentered password doesn't match.");
    }

    transferEther = (fromEth, toEth, balance) => {
        Meteor.call('transferEther', fromEth, toEth, balance, (error, result) => {
            if (error) {
                alert("error:" + error)
            } else {
                console.log('transfer ether call is successful: ');
                this.getEthAccounts()
                alert("Transfer Initiated Successfully, " +
                    "Please be advised that it may take a while to complete. " +
                    "Your account balance will be updated after the transfer has been completed successfully")
            }
        })
    }

    importEthAccByKey = (accName, private_key, password) => {
        Meteor.call('importAccByPrivateKey', accName, private_key, password, (error, result) => {
            if (error) {
                alert("error:" + error)
            } else {
                // console.log(result)
                if (result == null) {
                    alert("Account already exists! Cannot import duplicate Account!")
                } else {
                    this.getEthAccounts()
                    alert("Account imported Successfully")
                }
            }
        })
    }


    createAccount(ethAccName, ethPassword, ethReenterPassword) {
        this.onSubmit(ethAccName, ethPassword, ethReenterPassword)
        this.setState({
            ethAccName: '',
            ethPassword: '',
            ethReenterPassword: '',
            showCreateEthModal: false
        })
    }

    importAccount(importEthAccName, importEthPrivateKey, importEthPassword, importEthReEnterPassword) {
        if (importEthPassword === importEthReEnterPassword) {
            this.importEthAccByKey(importEthAccName, importEthPrivateKey, importEthPassword)
            this.setState({
                importEthAccName: '',
                importEthPrivateKey: '',
                importEthPassword: '',
                importEthReEnterPassword: '',
            })
        } else
            alert("Passwords don't match")
    }


    render() {
        const usrId = Meteor.userId();
        if (undefined === usrId || null === usrId) {
            return <Redirect push to='/login'/>
        }
        return (
            <div className="container-fluid">
                <div className="row">
                    <div className="navbar-base">
                        <NavBar/>
                    </div>
                    <div className="main-content">
                        {this.createEthAccModal(this.state.ethAccName, this.state.ethPassword, this.state.ethReenterPassword)}
                        {this.transferEtherModal()}
                        {this.importEthModal()}

                        <Tabs>
                            {/*<Tab eventKey="accounts" title="Manage Accounts">*/}
                            {/*    <EthAccount setDefaultEthAccount={this.setDefaultEthAccount}*/}
                            {/*                deleteEthAccount={this.deleteEthAccount}*/}
                            {/*                onSubmit={this.onSubmit}*/}
                            {/*                state={this.state}/>*/}
                            {/*</Tab>*/}
                        </Tabs>
                        <Button variant="secondary"
                                style={{backgroundColor: "#e5e5e5", color: "black", fontWeight: "bold"}}
                                onClick={() => this.setState({showCreateEthModal: true})}>
                            Create New Eth Account
                        </Button>
                        <Button variant="secondary"
                                style={{backgroundColor: "#e5e5e5", color: "black", fontWeight: "bold"}}
                                onClick={() => this.setState({showTransferEtherModal: true})}>
                            Transfer Ethers
                        </Button>
                        <Button variant="secondary"
                                style={{backgroundColor: "#e5e5e5", color: "black", fontWeight: "bold"}}
                                onClick={() => this.setState({showImportEthModal: true})}>
                            Import Account
                        </Button>
                        <Form> <Form.Row>
                        <Form.Label style={{color: "white"}}>Select a User</Form.Label>
                        <Form.Control as="select"
                           value={this.state.user}
                           onChange={this.selectUser}
                           style={{backgroundColor: "black", color: "white"}}
                        >
                          {this.state.userList.map((usr)=>(
                          <option value={usr} key={usr}> {usr} </option>
                          ))}
                        </Form.Control>
                        </Form.Row> </Form>
                        {/*{console.log(this.state.ethAccountList)}*/}
                        <ul style={{listStyleType: "none"}} className="data-list">
                            {this.state.ethAccountList.map(function (listValue) {
                                return (
                                    <li className="group2" key={listValue.account}>
                                        <EthAccount listValue={listValue}
                                                    setDefaultEthAccount={here.setDefaultEthAccount}
                                                    deleteEthAccount={here.deleteEthAccount}
                                                    onSubmit={here.onSubmit}
                                                    accBalance={here.state.accBalance}
                                                    defaultAccount={here.state.defaultAccount}
                                        />
                                    </li>
                                );
                            })}
                        </ul>
                    </div>
                </div>
            </div>
        )
    }
}
