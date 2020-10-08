import React from 'react';
import {Link} from 'react-router-dom';
import {Badge, Button} from 'react-bootstrap';

class AdminInfo extends React.Component {

  render() {

      return(
          <div className="moduleArea">
              <div class="row">
                    <div class="col-sm-1 centerVertical">
                        {this.props.userlist._id ?
                            <Link
                                to={{
                                    pathname: "/admin/update",
                                    state:{
                                        _id: this.props.userlist._id,
                                    }
                                }}
                                className="iconButton hover"
                                variant="light"
                                >
                                <i className="far fa-edit hover paddedLeft"></i>
                            </Link>
                            : null
                        }
                    </div>
                    <div class="col-sm-2 centerVertical">
                        <span className="span1">{this.props.userlist.username}</span>
                    </div>

                    <div class="col-sm-3 centerVertical">
                        {this.props.userlist.emails !== undefined && this.props.userlist.emails.length > 0 ? this.props.userlist.emails[0].address : null}
                    </div>

                    <div class="col-sm-2 centerVertical">
                        {this.props.userlist.emails !== undefined && this.props.userlist.emails.length > 0 ? this.props.userlist.emails[0].verified ? 'Yes' : 'No' : null}
                    </div>

                    <div class="col-sm-2 centerVertical">
                        {this.props.userlist.roles !== undefined && this.props.userlist.roles.length > 0 ? this.props.userlist.roles[0] : null}
                    </div>

                    <div class="col-sm-2 centerVertical iconArea">
                        {this.props.userlist._id ?
                            <Button
                                className="iconButton"
                                variant="light"
                                onClick={() => this.props.deleteUser(this.props.userlist._id)}
                                >
                                <i className="far fa-trash-alt"></i>
                            </Button>
                            : null
                        }
                    </div>



                    <div class="col-sm-2 centerVertical">

                    </div>
              </div>
          </div>

      );
  }
};

export default AdminInfo;
