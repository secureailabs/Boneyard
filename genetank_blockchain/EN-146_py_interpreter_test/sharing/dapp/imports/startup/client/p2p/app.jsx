//file:app.jsx Authot:sz Version:0.1
import React from 'react'
import { Meteor } from 'meteor/meteor'
import { Mongo } from 'meteor/mongo'
import { Messages } from '/imports/startup/both/collections.js'

let myMsgHandle = Meteor.subscribe('messages');
let newMessage=null
let newContact=null
let myMessages

Tracker.autorun(() => {
  const isReady = myMsgHandle.ready();
  if (isReady){
    myMessages = Messages.find().fetch();
  }
  if (newMessage !== null)
    newMessage(myMessages)
  console.log(`Handle is ${isReady ? 'ready' : 'not ready'}`);
});

function set_ready_cb(nm,nc) {
  newMessage = nm
  newContact = nc
}

function clr_ready_cb() {
  newMessage = null
  newContact=null
}

let myCntHandle = Meteor.subscribe('contact');
Tracker.autorun(() => {
  const isReady = myCntHandle.ready();
  if (isReady){
    const user = Meteor.users.find().fetch()
    const myContact=user[0].contacts;
    if (newContact!==null)
      newContact(myContact)
  }
})

export class Chat extends React.Component {
  constructor() {
    super();
    this.state = {
      enter_msg:"",
      my_id:"",
      peer_name:"",
      peer_id:"",
      dial_status:"no connection",
      messages:"",
      serverMsg:"",
      reminder:"",
      showEdit:false,
      contacts:[]};

    this.handleSubmit = this.handleSubmit.bind(this);
    //this.handleGetId = this.handleGetId.bind(this);
    this.handleEnterName = this.handleEnterName.bind(this);
    this.handleEnterId = this.handleEnterId.bind(this);
    this.handleAddContact = this.handleAddContact.bind(this);
    this.handleDelContact = this.handleDelContact.bind(this);
    this.handleSelCnt = this.handleSelCnt.bind(this);
    this.handleEnterMsg = this.handleEnterMsg.bind(this);
    this.handleMessageChange = this.handleMessageChange.bind(this);
    this.handleDeleteMsg = this.handleDeleteMsg.bind(this);
    this.handleEditContact = this.handleEditContact.bind(this);
  }

  handleEditContact(e){
    const showEdit=this.state.showEdit
    this.setState({showEdit:!showEdit})
  }

  setMyId = (err, result) => {
      if (err)
        console.log("Error found:"+err)
      else {
        this.setState({my_id:result,
        });
      }
  }

  setContacts = (err, result) => {
      if (err)
        console.log("Error found:"+err)
      else {
        var peer_name=''
        var peer_id=''
        //console.log("Results:"+result)
        if (result === undefined)
          result = []
        if (result[0] !== undefined) {
          peer_name=result[0].name
          peer_id=result[0].id
        }
        result.map((cont)=>{
          if (cont.name === this.state.peer_name){
            peer_name=cont.name
            peer_id=cont.id
        }})
        const showMsg = this.getMessage(myMessages,peer_id,peer_name)
        this.setState({contacts:result, 
          messages:showMsg,
          peer_name:peer_name,
          peer_id:peer_id});
      }
  }

  updateMessage=(myMessages)=>{
    const showMsg=this.getMessage(myMessages,this.state.peer_id,
      this.state.peer_name)
    this.setState({messages:showMsg})
  }

  getMessage=(myMessages,peer_id,peer_name)=>{
    var showMsg=""
    myMessages.map((msg)=>{
      if (msg.message.peer !== peer_id)
        return
      var sender="Me"
      if (msg.message.received)
        sender=peer_name
      if (msg.message.text !== undefined)
        showMsg = showMsg+sender+": "+msg.message.text+'\n'
    })
    return showMsg
  }

  updateContact=(contacts)=> {
    if (contacts !== undefined)
      this.setState({contacts:contacts})
  }

  componentDidMount() {
    Meteor.call("getP2pId", this.setMyId);
    Meteor.call("getContacts", this.setContacts); 
    set_ready_cb(this.updateMessage,this.updateContact)
  }

  componentWillUnmount(){
    clr_ready_cb()
  }

  handleSelCnt(event) {
    const peer_name = event.target.value;
    var peer_id=""
    this.state.contacts.map((contact)=>{
      if (contact.name === peer_name)
        peer_id=contact.id
    })
    const showMsg=this.getMessage(myMessages,peer_id,peer_name)
    this.setState({peer_name:peer_name,peer_id:peer_id,
      messages:showMsg});
  }

  handleEnterId(event) {
    const peer_id = event.target.value;
    this.setState({peer_id:peer_id});
  }
  handleEnterName(event) {
    const peer_name = event.target.value;
    this.setState({peer_name:peer_name});
  }

  handleAddContact(event) {
    const contact = {name:this.state.peer_name,id:this.state.peer_id}
    Meteor.call("addContacts", contact)//, this.setContacts); 
  }

  handleDelContact(event) {
    var answer = confirm("Delete contact " + this.state.peer_name +"?")
    if (!answer)
      return;
    const contact = {name:this.state.peer_name,id:this.state.peer_id}
    Meteor.call("delContacts", contact)//, this.setContacts); 
  }

  handleDeleteMsg(event) {
    Meteor.call("deleteMessage") 
  }

  handleMessageChange(event) {
    const msg = this.state.messages;
    this.setState({messages:msg});
  }

  handleEnterMsg(event) {
    const msg = event.target.value;
    this.setState({enter_msg:msg});
  }

  handleSubmit(event) {
    event.preventDefault();
    if (this.state.enter_msg === "") {
      this.setState({reminder:"You enter nothing for sending"})
      return
    }
    Meteor.call("SendMessage",this.state.enter_msg, this.state.peer_id, (err,result) => {
      if (err){
        const err_msg = "Error found:"+err
        console.log(err_msg)
      }
    });
    this.setState({
      enter_msg:"",
      reminder:""});
  }

/*
    this.handleDial = this.handleDial.bind(this);
        <button onClick={this.handleDial}> Dial </button>
        <label>{this.state.dial_status}</label>
  handleDial(event) {
    Meteor.call("DialPeer", this.state.peer_id,(err,result) => {
      if (err){
        const err_msg = "Error found:"+err
        this.setState({dial_status:err_msg})
        //console.log(err_msg)
      }
      else {
        this.setState({dial_status:"Dial results: "+result})
      }
    });
  }

*/
  render() {
    const listContacts = this.state.contacts.map((contact)=>{
      return(<option value={contact.name} key={contact.name}>
                {contact.name}</option>)
    })
    return (
      <div>
        <label>My ID:{" "+this.state.my_id}</label>
        <br />
        <label>Contact:</label>
        <select value={this.state.peer_name}
           onChange={this.handleSelCnt}>
           {listContacts}
        </select>
        <label>{this.state.peer_id}</label>
        <button onClick={this.handleEditContact}> 
          {!this.state.showEdit && "Edit" }
          {this.state.showEdit && "Hide Edit"}
        </button>
        <br />
        {this.state.showEdit && (<div>
          <label>Name:</label>
          <input type="text" size={8} value={this.state.peer_name}
           onChange={this.handleEnterName} />
          <label>ID:</label>
          <input type="text" size={40} value={this.state.peer_id}
           onChange={this.handleEnterId} />
          <button onClick={this.handleAddContact}> Add </button>
          <button onClick={this.handleDelContact}> Delete </button>
        </div>)}
        <form onSubmit={this.handleSubmit}>
          <textarea rows="25" cols="60" value={this.state.messages} 
             onChange={this.handleMessageChange} />
          <br />
          <input type="text" value={this.state.enter_msg}
             onChange={this.handleEnterMsg} />
          <input type="submit" value="send" />
          <label>{this.state.reminder}</label>
        </form>
        <button onClick={this.handleDeleteMsg}> Delete All Messages </button>
      </div>
    )
  }
}
