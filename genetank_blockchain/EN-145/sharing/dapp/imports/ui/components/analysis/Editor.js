import React, { Component } from 'react';
import AceEditor, {split as SplitEditor} from 'react-ace';
import 'brace/mode/python';
import 'brace/theme/terminal';

export class Editor extends Component {
    constructor(props){
        super(props);
        this.state={
            lang:"python",
            code: this.props.code
        }

        this.onChange = this.onChange.bind(this);
    }

    componentWillReceiveProps(nextProps) {
      this.state.code = nextProps.code
    }

    onChange(newVal){
        this.setState({code: newVal});
        this.props.call(this.state.code, this.props.name);
    }

    render(){
        return(
          <AceEditor
            style={{width:'100%'}}
            placeholder="#Please input your code here"
            mode={this.state.lang}
            theme="ambiance"
            name={this.props.name}
            fontSize={20}
            showPrintMargin={false}
            showGutter={true}
            highlightActiveLine={true}
            value={this.state.code}
            onChange={this.onChange}
            setOptions={{
            showLineNumbers: true,
            tabSize: 2}}
          />
        );
    }

}
