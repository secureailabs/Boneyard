import React from 'react'
import { FlowRouter } from 'meteor/kadira:flow-router';
import {mount} from 'react-mounter';

import { Home } from './home.jsx'
//import { Blockchain } from './blockchain.jsx'
import { Trainer } from './trainer.jsx'
import { Runner } from './runner.jsx'
import { Query } from './query.jsx'
import { Chat } from './p2p/app.jsx'

export default class MyLayout extends React.Component {
  render() {
    var category = FlowRouter.getQueryParam();
    //console.log(category);
    return (
      <div className='react-container'>
        <div className='nav'>
          {this.props.nav}
        </div>
        <div className='nav-spacer'></div>
        <div className='content'>
          {this.props.content}
        </div>
      </div>
    )
  }
}

function Layout(){
  return (
    <div>
      <div className='nav'>
        {this.props.nav}
      </div>
      <div className='content'>
        {this.props.content}
      </div>
    </div>
  );
}

//<a href="/home"><li><img className="nav-icon" src="/icons/gtt.png" />My GTT Wallet</li></a>
/*
          <div class="dropdown">
            <button class="dropbtn"> 
              <li><img className="nav-icon" src="/icons/gtt.png" />My GTT Wallet</li>
            </button>
            <div class="dropdown-content">
              <a href="#">Link 1</a>
              <a href="#">Link 2</a>
              <a href="#">Link 3</a>
            </div>
          </div> 
*/

class HomeMenu extends React.Component {
  render() {
    return (
      <ul className="nav__submenu">
        <li className="nav__submenu-item ">
          <a href="/chat">Chat room</a>
        </li>
        <li className="nav__submenu-item ">
          <a>Wallet</a>
        </li>
        <li className="nav__submenu-item ">
          <a>Blockchain browser</a>
        </li>
      </ul>
    )
  }
}

            //<a href="/trainer">
            //<a href="/runner">
            //<a href="/query">
        //<ul className='nav-list {props.listType}'>
function  MainMenu(props) {
    return (
        <ul className={'nav-list '+props.listType}>
          <li className="nav-list-item" onMouseLeave={props.handleLeave} 
              onMouseEnter={props.handleHover} 
          >
          <a className="nav-list-item-a" href="/app/home?" 
          > 
              <img className="nav-icon" src="/icons/gtt.png" />
              { (props.listType == 'nav-list-lg') && 'My GTT Wallet' ||
                (props.listType == 'nav-list-md') && 'Wallet' ||
                (props.listType == 'nav-list-sm') && '' }
          </a>
            { props.showHomeMenu && <HomeMenu /> }
          </li>
          <a className="nav-list-item-a" href="/app/trainer?">
          <li className="nav-list-item">
              <img className="nav-icon" src="/icons/brain.png" />
              { (props.listType == 'nav-list-lg') && 'Trainer Operations' ||
                (props.listType == 'nav-list-md') && 'Trainer' ||
                (props.listType == 'nav-list-sm') && '' }
          </li>
          </a>
          <a className="nav-list-item-a" href="/app/runner?">
          <li className="nav-list-item">
              <img className="nav-icon" src="/icons/runner.png" />
              { (props.listType == 'nav-list-lg') && 'Runner Operations' ||
                (props.listType == 'nav-list-md') && 'Runner' ||
                (props.listType == 'nav-list-sm') && '' }
          </li>
          </a>
          <a className="nav-list-item-a" href="/app/query?">
          <li className="nav-list-item">
              <img className="nav-icon" src="/icons/query.png" />
              { (props.listType == 'nav-list-lg') && 'Query Operations' ||
                (props.listType == 'nav-list-md') && 'Query' ||
                (props.listType == 'nav-list-sm') && '' }
          </li>
          </a>
        </ul>
    )
  }

class MyRoot extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      showHomeMenu: false
    };
  }

  handleHover = () => {
    this.setState({ showHomeMenu: true });
  };

  handleLeave = () => {
    this.setState({ showHomeMenu: false });
  };

/*
*/
  render() {
    return (
      <div className='nav-container'>
        <div className='login-spacer'></div>
        <MainMenu listType={'nav-list-lg'} 
           handleLeave = {this.handleLeave}
           handleHover = {this.handleHover}
           showHomeMenu = {this.state.showHomeMenu}
        />
        <MainMenu listType={'nav-list-md'} 
           handleLeave = {this.handleLeave}
           handleHover = {this.handleHover}
           showHomeMenu = {this.state.showHomeMenu}
        />
        <MainMenu listType={'nav-list-sm'} 
           handleLeave = {this.handleLeave}
           handleHover = {this.handleHover}
           showHomeMenu = {this.state.showHomeMenu}
        />
      </div>
    );
  }
}

FlowRouter.route("/", {
  triggersEnter: [function(context, redirect)
    { if (Meteor.user()===null) redirect('/login'); else redirect('/app/home?'); }],
  name: 'root',
  action(params, queryParams) {
    mount(MyLayout, {
      nav: (<MyRoot  _id={params._id} category={queryParams.category}/>),
      content: (<PleaseSignin  _id={params._id} category={queryParams.category}/>)
    })
  }
});

FlowRouter.route("/chat", {
    triggersEnter: [function(context, redirect)
      { if (Meteor.user()===null) redirect('/login'); }],
    name: 'chat',
    action(params, queryParams) {
      mount(MyLayout, {
        nav: (<MyRoot  _id={params._id} category={queryParams.category}/>),
        content: (<Chat  _id={params._id} category={queryParams.category}/>)
      })
    }
});

function App(props){
  if(props._id == "home")
    return(<Home />)
  else if (props._id == "trainer")
    return (<Trainer />)
  else if (props._id == "runner")
    return (<Runner />)
  else if (props._id == "query")
    return (<Query />)
  return (<div> <p>Wrong ID {_id}</p></div>)
}

FlowRouter.route("/app/:_id", {
    triggersEnter: [function(context, redirect)
      { if (Meteor.user()===null) redirect('/login'); }],
    name: 'app',
    action(params, queryParams) {
      mount(MyLayout, {
        nav: (<MyRoot  _id={params._id} category={queryParams.category}/>),
        content: (<App  _id={params._id} category={queryParams.category}/>)
      })
    }
});

FlowRouter.route("/login", {
    name: 'login',
    action(params, queryParams) {
      mount(MyLayout, {
        nav: (<MyRoot  _id={params._id} category={queryParams.category}/>),
        content: (<PleaseSignin  _id={params._id} category={queryParams.category}/>)
      })
    }
});


const PleaseSignin = () => (
  <div>
    <div className='welcome-message'>
      <img className='welcome-image' src='/genetank.png' />
      <div>Welcome to</div>
      <div className='welcome-title'>GeneTank</div>
    </div>
    <div className='login-message'>
      To continue, please sign in.
    </div>
  </div>
)

