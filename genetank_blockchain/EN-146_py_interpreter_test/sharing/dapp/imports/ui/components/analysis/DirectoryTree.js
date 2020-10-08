import React, {Component} from 'react';
import {ListGroup} from 'react-bootstrap'

export class ProjectTree extends Component {
    constructor(props){
        super(props);
        this.onToggle = this.onToggle.bind(this);
        this.createList = this.createList.bind(this);
        this.fHeader = this.fHeader.bind(this);
        this.handleClick = this.handleClick.bind(this);
    }

    componentDidMount(){
    }

    onToggle(node, toggled){
        const {cursor, projects} = this.props.projects;
        if (cursor) {
            this.setState(() => ({cursor, active: false}));
        }
        node.active = true;
        if (node.children) { 
            node.toggled = toggled; 
        }
        this.setState({cursor: node, projects: Object.assign({}, projects)}, ()=>{
          this.props.call(node);
        });
    }

    handleClick(node){
        this.props.call(node);
    }

    fHeader(node){
        const pyicon = <img src={'/icons/pyicon.png'} className="iconleft3"/>;
        const pyficon = <img src={'/icons/pyficon.png'} className="iconleft3"/>;
        const iconType = node.federated ? pyficon : pyicon;
        return (<div className = "iconhover">
                  <div>
                    {iconType}
                    {`${node.name}`}
                  </div>
                </div>);
    }

    createList= () =>(
        <ListGroup style={{display:'flex'}}>
            {this.props.projects.map((i)=>(
                <ListGroup.Item 
                  key={i.name} 
                  onClick={()=>{this.handleClick(i)}} 
                  style={{width:'150px', padding:'0.2rem 0.5rem', color:'white', backgroundColor:'black'}}
                >
                    {this.fHeader(i)}
                </ListGroup.Item>))}
        </ListGroup>
    )
    
    render(){
        return (
            <div>
              {this.createList()}
            </div>
        );
    }
  }