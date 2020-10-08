import React from "react";
import { render } from "react-dom";
// Import React Table
import ReactTable from "react-table";
import "react-table/react-table.css";
//import {getData} from "./Utils.js";

export class DisplayTable extends React.Component {
  constructor() {
    super();
    this.state = {
      request: [],
      response: []
    }
  }

  componentDidMount() {
    Meteor.call("getData", (error, result) => {
      if (!error) {
        var req = [], resp = []
        result.map((element) => {
          if (element.type === "request")
            req.push(element)
          else
            resp.push(element)
        })
        this.setState({ request: req, response: resp })
      }
    })
  }

  render() {
    const { request, response } = this.state;
    // console.log(this.state)
    return (
      <div>
        <ReactTable
          data={request}
          columns={[
            {
              Header: "Attestation Request",
              columns: [
                {
                  Header: "TimeStamp",
                  accessor: "timestamp",
                  width: 120,
                },
                {
                  Header: "Size",
                  accessor: "messageBody.msgSize",
                  width: 60,
                },
                {
                  Header: "Quote",
                  accessor: "messageBody.quote",
                }
              ]
            },
          ]}
          defaultPageSize={5}
          className="-striped -highlight"
          SubComponent={row => {
            // console.log(row)

            // const { Buffer } = require('buffer');
            // let quote = row.original.messageBody.quote;
            // const buf1 = new Buffer(quote, 'base64');
            return (
              <div style={{ padding: "20px", wordBreak: "break-all" }}>
                <strong>TimeStamp:</strong><br /><br />
                {row.original.timestamp}<br /><br />
                <strong>Size:</strong><br /><br />
                {row.original.messageBody.msgSize}<br /><br />
                <strong>Quote:</strong><br /><br />
                {row.original.messageBody.quote}<br /><br />

              </div>
            );
          }}
        />
        <br />

        <br />
        <ReactTable
          data={response}
          columns={[
            {
              Header: "Attestation Response",
              columns: [
                {
                  Header: "TimeStamp",
                  accessor: "timestamp",
                  width: 120,
                },
                {
                  Header: "Id",
                  accessor: "messageBody.attestation_report.id",
                },
                {
                  Header: "Time",
                  accessor: "messageBody.attestation_report.timestamp",
                },
                {
                  Header: "EpidPseudonym",
                  accessor: "messageBody.attestation_report.epidPseudonym",
                },

                {
                  Header: "IsvEnclaveQuoteStatus",
                  accessor: "messageBody.attestation_report.isvEnclaveQuoteStatus",
                },
                {
                  Header: "PlatformInfoBlob",
                  accessor: "messageBody.attestation_report.platformInfoBlob",
                },
                {
                  Header: "IsvEnclaveQuoteBody",
                  accessor: "messageBody.attestation_report.isvEnclaveQuoteBody",
                }
              ]
            },
          ]}
          defaultPageSize={5}
          className="-striped -highlight"
          SubComponent={row => {
            // console.log(row)
            return (
              <div style={{ padding: "20px", wordBreak: "break-all" }}>
                <strong>TimeStamp:</strong><br /><br />
                {row.original.timestamp}<br /><br />
                <strong>Id:</strong><br /><br />
                {row.original.messageBody.attestation_report.id}<br /><br />
                <strong>Time:</strong><br /><br />
                {row.original.messageBody.attestation_report.timestamp}<br /><br />
                <strong>EpidPseudonym:</strong><br /><br />
                {row.original.messageBody.attestation_report.epidPseudonym}<br /><br />
                <strong>IsvEnclaveQuoteStatus:</strong><br /><br />
                {row.original.messageBody.attestation_report.isvEnclaveQuoteStatus}<br /><br />
                <strong>PlatformInfoBlob:</strong><br /><br />
                {row.original.messageBody.attestation_report.platformInfoBlob}<br /><br />
                <strong>IsvEnclaveQuoteBody:</strong><br /><br />
                {row.original.messageBody.attestation_report.isvEnclaveQuoteBody}<br /><br />
                <strong>Signature:</strong><br /><br />
                {row.original.messageBody["X-IASReport-Signature"]}<br /><br />
                <strong>Certificate:</strong><br /><br />
                {row.original.messageBody["X-IASReport-Signing-Certificate"]}<br /><br />
              </div>
            );
          }}
        />
      </div>

    );
  }
}

