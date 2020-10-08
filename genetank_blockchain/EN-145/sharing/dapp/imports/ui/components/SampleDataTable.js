import React, { Component } from 'react';

import ReactTable from 'react-table';
//import 'react-table/react-table.css';

export class SampleDataTable extends Component {
  constructor(props) {
    super(props);
    this.state = {
      sampleData : this.props.sampleData,
    }
  }

  getDataset() {
    return [{
      "sampleID": "CSM5FZ3N",
      "study|subset": "HMP2|Cedars-Sinai",
      "diagnosis": "CD",
      "participantID": "C3001",
      "visitNum": "1",
      "hbi|sccai": "4",
    }, {
      "sampleID": "CSM5MCWI",
      "study|subset": "HMP2|Cedars-Sinai",
      "diagnosis": "CD",
      "participantID": "C3001",
      "visitNum": "10",
      "hbi|sccai": "5",
    }, {
      "sampleID": "CSM5MCXD",
      "study|subset": "HMP2|Cedars-Sinai",
      "diagnosis": "CD",
      "participantID": "C3001",
      "visitNum": "11",
      "hbi|sccai": "5",
    }, {
      "sampleID": "CSM5MCYS",
      "study|subset": "HMP2|Cedars-Sinai",
      "diagnosis": "CD",
      "participantID": "C3002",
      "visitNum": "13",
      "hbi|sccai": "4",
    }, {
      "sampleID": "CSM79HM1",
      "study|subset": "HMP2|Cedars-Sinai",
      "diagnosis": "UC",
      "participantID": "C3011",
      "visitNum": "14",
      "hbi|sccai": "1",
    }, {
      "sampleID": "CSM79HM7",
      "study|subset": "HMP2|Cedars-Sinai",
      "diagnosis": "UC",
      "participantID": "C3011",
      "visitNum": "19",
      "hbi|sccai": "1",
    }, {
      "sampleID": "CSM67UH7",
      "study|subset": "HMP2|Cedars-Sinai",
      "diagnosis": "nonIBD",
      "participantID": "C3022",
      "visitNum": "1",
      "hbi|sccai": "5",
    }, {
      "sampleID": "CSM79HP2",
      "study|subset": "HMP2|Cedars-Sinai",
      "diagnosis": "nonIBD",
      "participantID": "C3022",
      "visitNum": "10",
      "hbi|sccai": "5",
    }];
  }

  render() {
    return (
        <div>
          {this.props.sampleData !== undefined
           ? <ReactTable
              data={this.props.sampleData.data}
              columns={this.props.sampleData.columns}
              defaultPageSize={5}
              className="-striped -highlight"
          />
              : <div className="row"><div className="col-md-2"><h3>Loading</h3></div><div className="col-md-10 mr-0 ml-0"> <i className="fas fa-spinner fa-spin"></i></div></div>}
        </div>
    )
  }
}
