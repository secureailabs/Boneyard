import React, {Component} from 'react';
import {withRouter} from 'react-router-dom';
import ListItem from "../datasets/ListItem";


class MyData extends Component {
	render() {
		return(
			<ul style={{listStyleType: "none"}} className="data-list">
				{console.log(this.props.data)}
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

export default withRouter(MyData);
