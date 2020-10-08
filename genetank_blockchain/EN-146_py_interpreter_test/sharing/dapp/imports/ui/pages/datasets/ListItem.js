import React, {Component} from 'react';
import {Link} from "react-router-dom";
import {Button} from "react-bootstrap";
/*
Display inputs:
props.listValue.id
props.listValue.name
props.listValue.size
props.listValue.tags
props.listValue.type
props.listValue.license
props.listValue.keywords
props.listValue.expirationDate
Callbacks:
props.deleteDataset()
  Inputs for the callback:
  props.pubKey
  props.ethAcc
props.setSelectedDatabaseInfo()
props.editData()
*/
export default class ListItem extends Component {

    expirationDate = () => {
      var expDate
      if (undefined === this.props.listValue.endDate) {
        expDate = new Date()
      } else { expDate = new Date(this.props.listValue.endDate); }
      const today = new Date()
      const dayCnt = (expDate.getTime() - today.getTime())/(1000*3600*24);
      if(dayCnt > 0) {
        return( expDate.toLocaleDateString()+
               " ("+dayCnt.toFixed(0)+" days left)")
      } else {
        return( expDate.toLocaleDateString())
      }
      //return("TBD")
    }

    render() {
      return(
        <div className="">
          <div className="buttoncopy8">
            <div className="rectangle1">
            </div>
          </div>
          <div className="ownerusernamecom">
            <span className="span1">File Size: </span>
            <span className="span2">{this.props.listValue.size} bytes</span>
          </div>
          {
            (Meteor.settings.public.mode === "sales" ?
              <div className="expiration083019">
                <span className="span1">Expiration: </span>
                <span className="span2">{this.expirationDate()}</span>
              </div>
            : null)
          }
          <div className="bitmap">
            <div className="mask"> </div>
            <div className="group31">
            <img src="img/share-data-group-3-7@2x.png" className="group32"/>
            </div>
          </div>
          { undefined !== this.props.deleteDataset &&
          <div className="colorblackcopy5">
            {this.props.listValue.id ?
              <i style={{position: "relative", bottom: "10px"}}
                className="far fa-trash-alt btn text-white" onClick={() =>
                  this.props.deleteDataset(this.props.pubKey,
                    this.props.ethAcc, this.props.listValue.id)}>
              </i> : null}
          </div>}
          <div className="datatitled123456" style={{overflow:"hidden", textOverflow:"ellipsis", width:"250px"}}>
            <span className="span1">{this.props.listValue.name}</span>
          </div>
          <div>
            <div className="colorblackcopy6">
              <div>
                {(undefined !== this.props.editData) &&
                <i onClick={() =>
                  this.props.editData(this.props.listValue.id)}>
                <img src="img/share-data-import-color-black-copy-6@2x.png" className="colorblackcopy61"/>
                </i>}
              </div>
            </div>
          </div>
          <div className="activecopy row">
            <div className="col-2" style={{color: "white"}}>Tags: </div>
            <div className="col-10">{this.props.listValue.tags}</div>
          </div>
        </div>
      );
    }
}
