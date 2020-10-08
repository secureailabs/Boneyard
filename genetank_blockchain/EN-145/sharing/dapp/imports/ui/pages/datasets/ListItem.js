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
        <div className="moduleArea">
          <div class="row">
            <div class="col-md-2 centerVertical iconArea">
              {
                undefined !== this.props.deleteDataset &&
                <div>
                  {(Meteor.settings.public.mode === "sales" && undefined !== this.props.setSelectedDatabaseInfo) &&
                    <Link to={{ pathname: "/analysis", }}>
                      <Button
                        className="iconButton"
                        variant="light"
                        onClick={() => this.props.setSelectedDatabaseInfo(
                        this.props.pubKey, this.props.ethAcc, this.props.listValue.id)}>
                        <i className="fas fa-play"></i>
                      </Button>
                    </Link>
                  }
                  {(undefined !== this.props.editData) &&
                    <Button
                      className="iconButton"
                      variant="light"
                      onClick={() =>
                        this.props.editData(this.props.listValue.id)}>
                          <i className="far fa-edit"></i>
                    </Button>
                  }
                  {this.props.listValue.id ?
                    <Button
                      className="iconButton"
                      variant="light"
                      onClick={() =>
                        this.props.deleteDataset(this.props.pubKey,
                        this.props.ethAcc, this.props.listValue.id)}>
                          <i className="far fa-trash-alt"></i>
                        </Button>
                    : null
                  }
                </div>
              }
            </div>
            <div class="col-md-2 centerVertical">
              {this.props.listValue.name}
            </div>
            <div class="col-md-2 centerVertical">
              {this.props.listValue.size} bytes
            </div>
            <div class="col-md-3 centerVertical">
              {this.props.listValue.tags}
            </div>
            {
              (Meteor.settings.public.mode === "sales" ?
                <div class="col-md-3 centerVertical">
                  {this.expirationDate()}
                </div>
              : null)
            }
          </div>
        </div>
      );
    }
}
