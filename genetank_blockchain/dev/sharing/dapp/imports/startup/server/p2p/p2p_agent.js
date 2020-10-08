'use strict'
/* eslint-disable no-console */
import { Meteor } from 'meteor/meteor';
import { Messages } from '/imports/startup/both/collections.js'

const PeerId = require('peer-id')
const PeerInfo = require('peer-info')
const Node = require('./libp2p-bundle.js')
const pull = require('pull-stream')
const Pushable = require('pull-pushable')
const waterfall = require('async/waterfall')
var abortCb = require('pull-stream/util/abort-cb')
//const p = Pushable()

/*
p2p_status is array of 
{
node: the user's p2p node info
peers: array of connected peers
  send_msg: function for sending message
}
*/
let p2p_status = []

Meteor.publish('contact',function() {
  return Meteor.users.find({_id:this.userId},{fields:{contacts:1}})
})

// Find this list at: https://github.com/ipfs/js-ipfs/blob/master/src/core/runtime/config-nodejs.json
const bootstrapers = [
  '/ip4/104.131.131.82/tcp/4001/ipfs/QmaCpDMGvV2BGHeYERUEnRQAwe3N8SzbUtfsmvsqQLuvuJ',
  '/ip4/104.236.176.52/tcp/4001/ipfs/QmSoLnSGccFuZQJzRadHn95W2CrSFmZuTdDWP8HXaHca9z',
  '/ip4/104.236.179.241/tcp/4001/ipfs/QmSoLPppuBtQSGwKDZT2M73ULpjvfd3aZ6ha4oFGL1KrGM',
  '/ip4/162.243.248.213/tcp/4001/ipfs/QmSoLueR4xBeUbY9WZ9xGUUxunbKWcrNFTDAadQJmocnWm',
  '/ip4/128.199.219.111/tcp/4001/ipfs/QmSoLSafTMBsPKadTEgaXctDQVcqN88CNLHXMkTNwMKPnu',
  '/ip4/104.236.76.40/tcp/4001/ipfs/QmSoLV4Bbm51jM9C4gDYZQ9Cy3U6aXMJDAbzgu2fzaDs64',
  '/ip4/178.62.158.247/tcp/4001/ipfs/QmSoLer265NRgSp2LA3dPaeykiS1J6DifTC88f5uVQKNAd',
  '/ip4/178.62.61.185/tcp/4001/ipfs/QmSoLMeWqB7YGVLJN3pNLQpmmEk35v6wYtsMGLzSr5QBU3',
  '/ip4/104.236.151.122/tcp/4001/ipfs/QmSoLju6m7xTh3DuokvT3886QRYqxAzb1kShaanJgW36yx'
]

function dialAndSend(userId,peer_id,msg){
    const p = Pushable(Meteor.bindEnvironment((err) => {
      p2p_status[userId].peers[peer_id].send_msg=null
      console.log('stream closed!')
    }))

    const peerId = PeerId.createFromB58String(peer_id)
    var syncFindPeer = Meteor.wrapAsync(p2p_status[userId].node.peerRouting.findPeer)
    const peerInfo = syncFindPeer(peerId)
    //const peerInfo = p2p_status["9rJK9XZzJomXAK8Wm"].node.peerInfo

    //var syncDialProt = Meteor.wrapAsync(p2p_status[userId].node.dialProtocol)
    //const conn = syncDialProt(peerInfo, '/gtchat/1.0.0')
    p2p_status[userId].node.dialProtocol(peerInfo,
        '/gtchat/1.0.0', Meteor.bindEnvironment((err, conn) => {
      if (err) { throw err }
      pull(
        p,
        pull.map((data)=>{
          //console.log(data.toString())
          return data
        }),
        conn,
        pull.map(Meteor.bindEnvironment((data)=>{
          //console.log(data.toString())
          Messages.insert({user:userId,
            message:{peer:peer_id,
              received: true,
              text:data.toString()}})
          return data
        })),
        pull.drain((data) => {
          if (data)
            console.log('received echo:', data.toString())
          else
            console.log('received echo:', data)
        })
      )
      p2p_status[userId].peers[peer_id].send_msg=(msg)=>
        p.push(Buffer.from(msg))
      if (msg !== null)
        p.push(Buffer.from(msg))
    }))
}

Meteor.methods({
  SendMessage(msg,peer_id){
    let userId = this.userId;
    const user = Meteor.user()
    if (user === null)
      throw new Meteor.Error("logged-out", "The user must be logged in to initialize the p2p messaging."); 
    if (p2p_status[userId] === undefined)
      throw new Meteor.Error("need-dial-peer", "The p2p hasn't dialed th        e peer.");
      //p2p_status[userId]={}
    if (p2p_status[userId].node === undefined)
     throw new Meteor.Error("need-init-node", "The p2p node hasn't been inited.");
    if (p2p_status[userId].peers === undefined)
     //throw new Meteor.Error("need-dial-peer", "The p2p hasn't dialed the peer.");
      p2p_status[userId].peers=[]
    if (p2p_status[userId].peers[peer_id] === undefined)
      //throw new Meteor.Error("need-dial-peer", "The p2p hasn't dialed the peer.");
      p2p_status[userId].peers[peer_id]={}
    if (p2p_status[userId].peers[peer_id].send_msg === undefined) {
      //throw new Meteor.Error("need-dial-peer", "The p2p hasn't dialed the peer.");
      dialAndSend(userId, peer_id, msg)
    } else
      p2p_status[userId].peers[peer_id].send_msg(msg)
    Messages.insert({user:userId,
            message:{peer:peer_id,
              received: false,
              text:msg}})
  },

  deleteMessage() {
    let userId = this.userId;
    if (userId === null)
      return
    Messages.remove({user:userId})
  },

  getP2pId(){
    let userId = this.userId;
    const user = Meteor.user()
    if (user === null)
      return "Please login first.";
      //throw new Meteor.Error("logged-out", "The user must be logged in to initialize the p2p messaging."); 
    if (user.peerIdJson === undefined) {
      var syncCreatePeerId = Meteor.wrapAsync(PeerId.create);
      const id = syncCreatePeerId();
      const idJson = id.toJSON()
      Meteor.users.update(userId, {
        $set: {
          peerIdJson: idJson
        }
      });
    }
    if (p2p_status[userId] === undefined)
      p2p_status[userId]={}
    if (p2p_status[userId].node !== undefined)
      return p2p_status[userId].node.peerInfo.id.toB58String()

    var syncCreateFromJson = Meteor.wrapAsync(PeerId.createFromJSON)
    const peerId = syncCreateFromJson(user.peerIdJson)
    const syncCreateNode = Meteor.wrapAsync(createNode)
    const node = syncCreateNode(userId, peerId)
    p2p_status[userId].node = node;
    return p2p_status[userId].node.peerInfo.id.toB58String()
  }, //getP2pId()
 
  getContacts() {
    const userId = this.userId;
    if (userId === undefined)
      return "Please login first.";
    const user = Meteor.user()
    return user.contacts;
  },

  addContacts(contact) {
    const userId = this.userId;
    if (userId === undefined)
      return "Please login first.";
    var exist = false
    const user = Meteor.user()
    if (user.contacts == undefined)
      user.contacts = []
    user.contacts.map((cont) => {
      if (cont.name === contact.name)
        exist = true
    })
    if (exist)
      return user.contacts
    //var syncUpdate=Meteor.wrapAsync(Meteor.users.update)
    Meteor.users.update(userId, {
    //syncUpdate(userId, {
        $push: {
          contacts: contact
        }
    });
    return user.contacts
  },

  delContacts(contact) {
    const userId = this.userId;
    if (userId === undefined)
      return "Please login first.";
    Meteor.users.update(userId, {
        $pull: {
          contacts: contact
        }
    });
    const user = Meteor.user()
    return user.contacts;
  }

})

function createNode(userId, peerId, callback){
  let node
  const p = Pushable(function (err) {
    console.log('stream closed!')
  })

 waterfall([
  (cb) => PeerInfo.create(peerId,cb),
  (peerInfo, cb) => {
    var mybootstrapers = []
    for (var k in p2p_status) {
      const nd = p2p_status[k].node
      if (nd !== undefined)
        if (nd.peerInfo !== undefined) {
          const ma = nd.peerInfo.multiaddrs
          ma.toArray().forEach((a)=>
            mybootstrapers.push(a.toString())
          )
        }
    }
    peerInfo.multiaddrs.add('/ip4/0.0.0.0/tcp/0')
    node = new Node(peerInfo, null, //peerBook
      //{dht:true,bootstrap:bootstrapers}
      {dht:true,bootstrap:mybootstrapers}
    )
    node.switch.on('peer-mux-established', (peerInfo) => {
      console.log('received dial to me from:', peerInfo.id.toB58String())
    })

    node.handle('/gtchat/1.0.0', Meteor.bindEnvironment((protocol, conn) => {
      let sndPeerId
      conn.getPeerInfo((err, peerInfo)=>
        sndPeerId=peerInfo.id.toB58String())
      pull(
        p,
        pull.map((data)=>{
          //console.log(data.toString())
          return data
        }),
        conn,
        pull.map(Meteor.bindEnvironment((data)=>{
          Messages.insert({user:userId,
            message:{peer:sndPeerId,
              received: true,
              text:data.toString()}})
          //p.push(data) //echo
          return data
        })),
        pull.drain((data) => {
          if (data)
            console.log('Listener received:', data.toString())
          else
            console.log('Listener received:',data)
        })
      )
      if (p2p_status[userId].peers == undefined)
        p2p_status[userId].peers = []
      if (p2p_status[userId].peers[sndPeerId] == undefined)
        p2p_status[userId].peers[sndPeerId] = {}
      p2p_status[userId].peers[sndPeerId].send_msg=(msg)=>
        p.push(Buffer.from(msg))
    }))

    node.start(cb)
  }
], (err) => {
  if (err) { throw err }

  node.on('peer:discovery', (peer) => {
    //console.log('Discovered:', peer.id.toB58String())
    node.dial(peer, () => {})
  })

  node.on('peer:connect', (peer) => {
    console.log('Connection established to:',
      node.peerInfo.id.toB58String(), '->', peer.id.toB58String())
  })

  node.on('peer:disconnect', (peer) => {
    console.log('Connection disconnected:',
      node.peerInfo.id.toB58String(), '->', peer.id.toB58String())
  })

  callback(null,node)
})
}
