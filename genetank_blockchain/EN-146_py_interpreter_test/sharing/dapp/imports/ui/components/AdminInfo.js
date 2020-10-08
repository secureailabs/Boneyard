import React from 'react';
import {Link} from 'react-router-dom';
import {Badge, Button} from 'react-bootstrap';

class AdminInfo extends React.Component {
 
  render() {

      return(
          <div className="">
              <div>
                  <div className="">
                      <div className="buttoncopy8">
                          <div className="rectangle1">
                          </div>
                      </div>
                      <div className="ownerusernamecom">
                          <span className="span1">Email: </span>
                          <span className="span2">
                              {this.props.userlist.emails !== undefined && this.props.userlist.emails.length > 0 ?
                                  this.props.userlist.emails[0].address : null}

                          </span>
                      </div>
                      <div className="expiration083019">
                          <span className="span1">Verified: </span>
                          <span className="span2">
                          {this.props.userlist.emails !== undefined && this.props.userlist.emails.length > 0 ?
                          this.props.userlist.emails[0].verified ? 'Yes' : 'No' : null}
                          </span>
                      </div>
                      <div className="bitmap">
                          <div className="mask">
                          </div>
                          <div className="group31">
                              <img src="img/share-data-group-3-7@2x.png" className="group32"/>
                          </div>
                      </div>
                      <div className="colorblackcopy5">
                          {this.props.userlist._id ?
                              <i style={{position: "relative", bottom: "10px"}}
                                 className="far fa-trash-alt btn text-white" 
                                 onClick={() => this.props.deleteUser(this.props.userlist._id)}>
                              </i> : null}
                      </div>
                      <div className="datatitled123456">
                          <span className="span1">{this.props.userlist.username}</span>
                      </div>
                      <div>
                          <div className="colorblackcopy6">
                              <div>
                                  {this.props.userlist._id ?
                                      <Link to={{
                                          pathname: "/update",
                                          state:{
                                              _id: this.props.userlist._id,
                                          }
                                      }} >
                                          <i>
                                              <img src="img/share-data-import-color-black-copy-6@2x.png"
                                                   className="colorblackcopy61"/>
                                          </i>
                                      </Link> : null}
                              </div>
                          </div>
                      </div>
                      <div className="activecopy row">
                          <div className="col-2" style={{color: "white"}}>Role: </div>
                          <div className="col-10">
                              {this.props.userlist.roles !== undefined && this.props.userlist.roles.length > 0 ?
                                  this.props.userlist.roles[0] : null}

                          </div>
                      </div>
                  </div>
              </div>
          </div>

      );
  }
};

export default AdminInfo;
