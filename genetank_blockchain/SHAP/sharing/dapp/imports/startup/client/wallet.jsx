//file wallet.jsx
import React from 'react'
import { gtkt, gtkt_info } from './gtkt.js';
import { BigNumber } from 'bignumber.js';

class SendToken extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      transfering:false,
      number:0,
      account:"",
    }

    this.handleSubmit = this.handleSubmit.bind(this);
    this.showTxSubmit = this.showTxSubmit.bind(this);
    this.handleNumChange = this.handleNumChange.bind(this);
    this.handleAccChange = this.handleAccChange.bind(this);
  }

  showTxSubmit(event) {
    event.preventDefault();
    this.setState({transfering:true});
  }

  handleSubmit(event) {
    event.preventDefault();
    const getData = gtkt.transfer.getData(this.state.account,
      this.state.number*10**this.props.decimals);
    web3.eth.sendTransaction({from:web3.eth.defaultAccount,
      to:gtkt_info.address,data:getData,gas:300000},(err,txHash) =>{
      if (err)
        alert('Transfer token fail. Error:'+err);
      else {
        this.props.onTransfer(txHash);
        this.setState({transfering:false});
      }
    });
  }

  handleNumChange(event) {
    this.setState({number:event.target.value});  
  }

  handleAccChange(event) {
    this.setState({account:event.target.value});  
  }

  render() {
    if (this.state.transfering)
    return (
    <div>
        <form onSubmit={this.handleSubmit}>
          <ul><li>
          <label>Please enter the GTKTs to be transfered :</label>
            <input type="number" step="0.0001" id="GTKT_number"
              onChange={this.handleNumChange} />
          </li>
          <li>
          <label>Please enter the destination account :</label>
            <input type="text" id="GTKT_account"
              onChange={this.handleAccChange} />
          </li>
          <label>Click on this button to transfer GTKT:</label>
          <input type="submit" value="Transfer" />
          </ul>
        </form>
    </div>
    )
    else
      return (
        <div>
          <form onSubmit={this.showTxSubmit}>
            <input type="submit" value="Transfer GTKTs" />
          </form>
        </div>
      );
  }//render
}

export class Transactions extends React.Component {
  constructor(props) {
    super(props);
    const bl = new BigNumber('0');
    this.state = {
      balance: bl,
      decimals: 0,
    }
    this.handleTransfer = this.handleTransfer.bind(this);
  }

  setBalance = () => {
    gtkt.balanceOf(web3.eth.defaultAccount, (err, resp)=> {
      if (err)
        alert("Get GTKT balance fail. Error:"+err);
      else
        this.setState({balance:resp});
    });
  }

  componentDidMount() {
    this.setBalance();
    gtkt.decimals((err,resp)=>{
      if (err)
        alert("Get decimals of the contract fail. Error:"+err);
      else
        this.setState({decimals:resp});
    });
  }

  handleTransfer(txHash) {
    setTimeout(this.handleTimeout, 10*1000, txHash);
  }

  handleTimeout = (arg) => {
    web3.eth.getTransactionReceipt(arg,(err, resp)=> {
      if (err)
        alert("Get transaction receipt fail. Error:"+err);
      else
        if (resp === null)
          setTimeout(this.handleTimeout, 10*1000, arg);
        else
          this.setBalance();
    });
  }

  render() {
    return (
      <div>
        {web3.eth.defaultAccount && 
        <div className="wallet-container">
          <table className="wallet-balance">
            <tbody>
              <tr>
                <td>
                  <img className="wallet-image" src="/gtt_green.png" />
                </td>
                <td>
                  <span className="your-balance">Your balance :</span>
                  <br />
                  <span className="balance-value">{this.state.balance.dividedBy(10**this.state.decimals).toNumber()} GTKT</span>
                </td>
              </tr>
            </tbody>
          </table>
          <SendToken onTransfer={this.handleTransfer}
             decimals = {this.state.decimals} />
          <div className="row txns">
            <div className="txns-header">Your Transactions</div>
            <ul className="list-group">
              <a className="list-group-item">
                <div className="txn-date pull-left">
                  <div className="txn-date txn-date-month text-center">JAN</div>
                  <div className="txn-date txn-date-day text-center">12</div>
                </div>
                <div className="txn-icon pull-left">
                  <svg xmlns="http://www.w3.org/2000/svg" width="30" height="30" viewBox="0 0 30 30" className="TransactionIcon-hhVIAM jGCZMs" currency="BTC"><g fill="none" fillRule="evenodd" transform="translate(1 1)"><path d="M14 5.9v11.78M18.42 13.26L14 17.68l-4.42-4.42M20.27 20.63H7.73"></path><circle cx="14" cy="14" r="14"></circle></g></svg>
                </div>
                <div className="txn-amt pull-right">
                  <div className="txn-amt-gtt text-right">+5.4 GTKT</div>
                  <div className="txn-amt-usd text-right">+$540.00</div>
                </div>
                <div className="txn-header list-group-item-heading">Shared Model (Diabetes_v2_2)</div>
                <div className="list-group-item-text">With Pfizer</div>
              </a>
              <a className="list-group-item">
                <div className="txn-date pull-left">
                  <div className="txn-date txn-date-month text-center">JAN</div>
                  <div className="txn-date txn-date-day text-center">10</div>
                </div>
                <div className="txn-icon pull-left">
                  <svg xmlns="http://www.w3.org/2000/svg" width="32" height="32" viewBox="0 0 32 32" className="TransactionIcon-kQSElj exnUIm" currency="BTC"><g fill="none" fillRule="evenodd" strokeLinecap="round" strokeLinejoin="round" transform="translate(1 1)"><circle cx="15" cy="15" r="15"></circle><path d="M23.68 17.37H7.9l5.53 5.52M6.32 12.63H22.1L16.57 7.1"></path></g></svg>
                </div>
                <div className="txn-amt pull-right">
                  <div className="txn-amt-gtt text-right">-3 GTKT</div>
                  <div className="txn-amt-usd text-right">-$300.00</div>
                </div>
                <div className="txn-header list-group-item-heading">Sold GTKT</div>
                <div className="list-group-item-text">Using Bank of America - ********1234</div>
              </a>
              <a className="list-group-item">
                <div className="txn-date pull-left">
                  <div className="txn-date txn-date-month text-center">JAN</div>
                  <div className="txn-date txn-date-day text-center">07</div>
                </div>
                <div className="txn-icon pull-left">
                  <svg xmlns="http://www.w3.org/2000/svg" width="30" height="30" viewBox="0 0 30 30" className="TransactionIcon-hhVIAM jGCZMs" currency="BTC"><g fill="none" fillRule="evenodd" transform="translate(1 1)"><path d="M14 5.9v11.78M18.42 13.26L14 17.68l-4.42-4.42M20.27 20.63H7.73"></path><circle cx="14" cy="14" r="14"></circle></g></svg>
                </div>
                <div className="txn-amt pull-right">
                  <div className="txn-amt-gtt text-right">+14.5 GTKT</div>
                  <div className="txn-amt-usd text-right">+$1,500.00</div>
                </div>
                <div className="txn-header list-group-item-heading">Shared Model (Cystic_Fibrosis_v3_7)</div>
                <div className="list-group-item-text">With Johns Hopkins Cystic Fibrosis Center</div>
              </a>
              <a className="list-group-item">
                <div className="txn-date pull-left">
                  <div className="txn-date txn-date-month text-center">JAN</div>
                  <div className="txn-date txn-date-day text-center">07</div>
                </div>
                <div className="txn-icon pull-left">
                  <svg xmlns="http://www.w3.org/2000/svg" width="30" height="30" viewBox="0 0 30 30" className="TransactionIcon-hhVIAM jGCZMs" currency="BTC"><g fill="none" fillRule="evenodd" transform="translate(1 1)"><path d="M14 5.9v11.78M18.42 13.26L14 17.68l-4.42-4.42M20.27 20.63H7.73"></path><circle cx="14" cy="14" r="14"></circle></g></svg>
                </div>
                <div className="txn-amt pull-right">
                  <div className="txn-amt-gtt text-right">+15 GTKT</div>
                  <div className="txn-amt-usd text-right">+$1,500.00</div>
                </div>
                <div className="txn-header list-group-item-heading">Shared Model (Alzheimer's_v13_1)</div>
                <div className="list-group-item-text">With GSK</div>
              </a>
              <a className="list-group-item">
                <div className="txn-date pull-left">
                  <div className="txn-date txn-date-month text-center">JAN</div>
                  <div className="txn-date txn-date-day text-center">07</div>
                </div>
                <div className="txn-icon pull-left">
                  <svg xmlns="http://www.w3.org/2000/svg" width="30" height="30" viewBox="0 0 30 30" className="TransactionIcon-ctIIfP eyBbVy" currency="BTC"><g fill="none" fillRule="evenodd" transform="translate(1 1)"><path d="M14 17.68V5.9M18.42 10.32L14 5.89l-4.42 4.43M20.27 20.63H7.73"></path><circle cx="14" cy="14" r="14"></circle></g></svg>
                </div>
                <div className="txn-amt pull-right">
                  <div className="txn-amt-gtt text-right">-10 GTKT</div>
                  <div className="txn-amt-usd text-right">-$1,000.00</div>
                </div>
                <div className="txn-header list-group-item-heading">Sent GTKT</div>
                <div className="list-group-item-text">To Anne</div>
              </a>
              <a className="list-group-item">
                <div className="txn-date pull-left">
                  <div className="txn-date txn-date-month text-center">JAN</div>
                  <div className="txn-date txn-date-day text-center">01</div>
                </div>
                <div className="txn-icon pull-left">
                  <svg xmlns="http://www.w3.org/2000/svg" width="32" height="32" viewBox="0 0 32 32" className="TransactionIcon-kQSElj exnUIm" currency="BTC"><g fill="none" fillRule="evenodd" strokeLinecap="round" strokeLinejoin="round" transform="translate(1 1)"><circle cx="15" cy="15" r="15"></circle><path d="M23.68 17.37H7.9l5.53 5.52M6.32 12.63H22.1L16.57 7.1"></path></g></svg>
                </div>
                <div className="txn-amt pull-right">
                  <div className="txn-amt-gtt text-right">+12.5 GTKT</div>
                  <div className="txn-amt-usd text-right">+$1,250.00</div>
                </div>
                <div className="txn-header list-group-item-heading">Bought GTKT</div>
                <div className="list-group-item-text">Using Bank of America - ********1234</div>
              </a>
            </ul>
          </div>
        </div>
        }
      </div>
    );
  }
}
