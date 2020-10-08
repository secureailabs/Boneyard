import React, { Component } from "react";
import { Meteor } from "meteor/meteor";
import { Redirect } from "react-router-dom";

var savedState = { value: 1, state: "abc" };

export class About extends Component {
  constructor(props) {
    super(props);
    this.state = {
      value: savedState.value,
      state: savedState.state,
      gotoView:false,
    };
  }

  updateState(obj) {
    for (var i in obj) savedState[i] = obj[i];
    this.setState(obj);
  }

  handleChange(event) {
    this.updateState({ value: event.target.value });
  }

  handleStChange(event) {
    this.updateState({ state: event.target.value });
  }

  render() {
    const usrId = Meteor.userId();
    if (undefined === usrId || usrId === null) {
      return <Redirect push to="/login" />;
    } else {
      if (this.state.gotoView)
        return <Redirect push to='/view'/>
      else
        return (
        <div
          style={{margin:0, background: "rgba(255, 255, 255, 1.0)" }}
        >
          <input id="anPageName" name="page" type="hidden" value="home" />
          <div className="home">
            <div style={{width: "1440px", height: "100%", position:"relative",margin:"auto"}}>
              <div className="browsercopy">
                <div className="browser">
                  <div className="topbar">
                    <img
                      src="./img/home-bg.png"
                      className="bg"
                    />
                    <div className="search"></div>
                    <img
                      src="./img/home-circle@2x.png"
                      className="circle"
                    />
                    <img
                      src="./img/home-circle-1@2x.png"
                      className="circle1"
                    />
                    <img
                      src="./img/home-circle-2@2x.png"
                      className="circle2"
                    />
                  </div>
                </div>
              </div>
              <img onClick={()=>{
                this.setState({gotoView:true})
              }}
                src="./img/home-bitmap.png"
                className="bitmap"
              />
            </div>
          </div>
        </div>
      );
    }
  }
}
