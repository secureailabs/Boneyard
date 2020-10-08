import {Meteor} from "meteor/meteor";
import React, {Component} from 'react';
import {withRouter} from 'react-router-dom';
import {Redirect} from 'react-router'
import NavBar from '/imports/ui/components/NavBar';
import ListItem from "../datasets/ListItem";


class PublicData extends Component {
	render() {
	  return(
	    <ul style={{listStyleType: "none"}} className="data-list">
		{this.props.data.map(function (listValue) {
		  return (
		    <li className="group2" key={listValue.id}>
			<ListItem listValue={listValue}/>
		    </li>
		  );
		})}
	    </ul>
	  )
	}
}

export default withRouter(PublicData);
