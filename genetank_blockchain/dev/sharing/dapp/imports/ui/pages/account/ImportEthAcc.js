import React, {Component} from 'react';
import ReactTable from 'react-table';
//import 'react-table/react-table.css';
import {Container, Col, Form, Button} from 'react-bootstrap';


class ImportEthAcc extends Component {
    constructor(props) {
        super(props);
        this.state = {
            importEthAccName: '',
            importEthPrivateKey: '',
            importEthPassword: '',
            importEthReEnterPassword: '',
            error: null,
        }
    }

    componentDidMount() {
    }

    importAccount(importEthAccName, importEthPrivateKey, importEthPassword, importEthReEnterPassword){
        if(importEthPassword === importEthReEnterPassword) {
            this.props.importEthAccByKey(importEthAccName, importEthPrivateKey, importEthPassword)
            this.setState({
                importEthAccName: '',
                importEthPrivateKey: '',
                importEthPassword: '',
                importEthReEnterPassword: '',
            })
        }
        else
            alert("Passwords don't match")
    }

    render() {
        const {
            importEthAccName,
            importEthPrivateKey,
            importEthPassword,
            importEthReEnterPassword,
            error,
        } = this.state;

        return (
            <div>
                <Form>
                    <legend className="border border-top-0 border-left-0 border-right-0">Import Eth Account
                    </legend>
                    <Form.Group as={Col} controlId="importEthAccName">
                        <Form.Label>Account Name</Form.Label>
                        <Form.Control
                            value={importEthAccName}
                            type="text"
                            onChange={event =>
                                this.setState({'importEthAccName': event.target.value})}
                            placeholder="Please Enter a Name For This Account"
                        />
                    </Form.Group>
                    <Form.Group as={Col} controlId="importEthPrivateKey">
                        <Form.Label>Private Key (without 0x)</Form.Label>
                        <Form.Control
                            value={importEthPrivateKey}
                            type="text"
                            onChange={event =>
                                this.setState({'importEthPrivateKey': event.target.value})}
                            placeholder="Please a private key to import"
                        />
                    </Form.Group>
                    <Form.Group as={Col} controlId="ethEnterPassword">
                        <Form.Label>Passphrase</Form.Label>
                        <Form.Control
                            value={importEthPassword}
                            type="password"
                            onChange={event =>
                                this.setState({'importEthPassword': event.target.value})}
                            placeholder="Please Enter a Passphrase"
                        />
                    </Form.Group>
                    <Form.Group as={Col} controlId="importEthReEnterPassword">
                        <Form.Label>Re-enter Passphrase</Form.Label>
                        <Form.Control
                            value={importEthReEnterPassword}
                            type="password"
                            onChange={event =>
                                this.setState({'importEthReEnterPassword': event.target.value})}
                            placeholder="Re-enter Passphrase To Confirm"
                        />
                    </Form.Group>
                    <Button variant="success" as={Col} block
                            onClick={()=>this.importAccount(importEthAccName, importEthPrivateKey, importEthPassword, importEthReEnterPassword)}>
                        Submit</Button>
                    <br/>
                    {error && <p>{error.reason}</p>}
                    <br/>
                </Form>
            </div>
        );
    }
}

export default ImportEthAcc;
