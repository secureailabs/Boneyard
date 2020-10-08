import {Dropdown} from 'react-bootstrap';
import DropdownButton from 'react-bootstrap/DropdownButton'
import React, {Component} from 'react';

export class FormDropDown extends Component {
  render(){
      return(
          <div>
              {this.props.Title}
              <DropdownButton
                size="sm"
                variant="secondary"
                title={this.props.Default}
              >
                {
                    this.props.menu.map((item)=>{
                        return (<Dropdown.Item>{item}</Dropdown.Item>);
                    })
                }
              </DropdownButton>    
          </div>
      );
  }
}