import {Meteor} from "meteor/meteor";
import React, {Component} from 'react';
import {withRouter} from 'react-router-dom';
import {Redirect} from 'react-router'
import NavBar from '/imports/ui/components/NavBar';
import {IDE} from '/imports/ui/components/analysis/IDE.js'

class Analysis extends Component {
    constructor(props) {
        super(props);
        this.state = {
            loading: true,
            data: [],
            ide: [],
            notebook: [],
            ethereumAddress: '',
            enclaveInfo: [],
            trainerEthAdd: ''
        };
        this.loadData = this.loadData.bind(this);
        this.getSampleData = this.getSampleData.bind(this);
        this.formatBytes = this.formatBytes.bind(this);
        this.mountInit = this.mountInit.bind(this);
    }

    getFormattedDataset(json) {
        if (json === "") {
            return json
        }

        var tempData = []
        var headers = json[1].split(",");
        for (var key in json) {
            var temp = {}
            if (json.hasOwnProperty(key) && key !== "1") {
                var i;
                var values = json[key].split(",");
                for (i = 0; i < headers.length; i++) {
                    temp[headers[i]] = values[i];
                }
            }
            tempData.push(temp)
        }
        var columns = []
        headers.forEach((head) => {
            var col = {
                Header: head,
                accessor: head,
            }
            columns.push(col)
        })
        const sampleData = {
            columns: columns,
            data: tempData
        }
        return sampleData
    }

    getSampleData = (result) => {
        Meteor.call("userLoadSingleDataView", result.selectedDatabaseInfo.name,
            result.selectedDatabaseInfo.pubKey, result.selectedDatabaseInfo.ethAcc,
            (error, result) => {
                if (error) {
                    if ('object' === typeof (error))
                        error = JSON.stringify(error)
                    alert("Error:" + error)
                } else if (result.error) {
                    console.log("Error:" + result.error)
                    alert("Please select a dataset and try again!")
                } else {
                    this.loadData(this.getFormattedDataset(result.result));
                }
            });
    }

    componentDidMount() {
        Meteor.call("userGetCfg", (err, result) => {
            const user = Meteor.user();
            if (!err) {
                if (result !== null) {
                    this.setState(()=>{
                        let tmpEnclaveInfo = [];
                        result.enclaves.forEach((item)=>{
                            let json = JSON.parse(item.infoJson);
                            let host = null
                            if(item.hosts) {
                              host = item.hosts.find(host => host.host === user.username);
                            }
                            tmpEnclaveInfo.push({name:json["name"], pubKey:item.pubKey, hosts:host});

                        });
                        let role = undefined;
                        if(result.enclaves.length > 0 && result.enclaves[0].roleInfos.length > 1) {
                          role = result.enclaves[0].roleInfos.find(
                                   r => r.role === 1 &&
                                   result.blockchainAccounts.find(acc => acc.account === r.ethAddr)
                            );
                        }
                        return {
                            ethereumAddress: result.defaultEthAccount,
                            enclaveInfo: tmpEnclaveInfo,
                            trainerEthAdd: role !== undefined ? role.ethAddr : undefined,
                            loading: false
                        };
                    });
                }
            }
        });
    }

    mountInit(){

    }

    formatBytes(bytes, decimals = 2) {
        if (bytes === 0) return '0 Bytes';

        const k = 1024;
        const dm = decimals < 0 ? 0 : decimals;
        const sizes = ['Bytes', 'KB', 'MB', 'GB', 'TB', 'PB', 'EB', 'ZB', 'YB'];

        const i = Math.floor(Math.log(bytes) / Math.log(k));

        return parseFloat((bytes / Math.pow(k, i)).toFixed(dm)) + ' ' + sizes[i];
    }

    getDataset(sampleData) {
        return {
            "name": this.state.selectedDatabaseInfo.name,
            "description": this.state.dsInfo === undefined || this.state.dsInfo.description === '' ? "Description not available" : this.state.dsInfo.description,
            "license": "Open source",
            "tags": this.state.dsInfo === undefined || this.state.dsInfo.tags === '' ? "Tags not available" : this.state.dsInfo.tags,
            "size": this.state.dsInfo === undefined || this.state.dsInfo.size === '' ? "Size not available" : this.formatBytes(this.state.dsInfo.size),
            "usage": {
                "analysis": {
                    "linked": 1,
                    "sasOnly": 1,
                    "basicMl": 0,
                    "deepMl": 0
                },
                "privacy": {
                    "type": "k-Anonymity",
                    "k": "50"
                },
                "expiration": "2019-04-22",
                "location": ""
            },
            "sampleData": sampleData
        };
    }

    getIdeData() {
        return {
            "input1":
                "import numpy\n" +
                "import scipy\n" +
                "from scipy import stats\n" +
                "\n" +
                "# Supporting functions\n" +
                "def dfrow_testranksums(s, index_subset1, index_subset2):\n" +
                "  x1 = s[index_subset1]\n" +
                "  x2 = s[index_subset2]\n" +
                "  Nnotnull1 = sum(x1 != 0)\n" +
                "  Nnotnull2 = sum(x2 != 0)\n" +
                "  t = stats.ranksums(x1, x2)\n" +
                "  return pandas.Series([Nnotnull1, Nnotnull2, t.statistic, t.pvalue])\n" +
                "\n" +
                "print(PanGenome_CDxnonIBD_xf)",


            "output1":
                "msp_name\tmodule_name\tNnotnull1\tNnotnull2\tstatistic\t\tpvalue\n" +
                "msg_0001\tcore\t\t\t8.0\t\t\t14.0\t\t\t0.093503\t\t0.925504\n" +
                "msg_0002\tcore\t\t\t2.0\t\t\t8.0\t\t\t-0.476865\t0.633459\n" +
                "msg_0003\tcore\t\t\t28.0\t\t\t56.0\t\t\t0.794774\t\t0.426745\n" +
                "msg_0004\tcore\t\t\t3.0\t\t\t7.0\t\t\t-0.130904\t0.895851\n" +
                "msg_0005\tcore\t\t\t28.0\t\t\t57.0\t\t\t-1.028532\t0.303700\n" +
                "msg_0006\tcore\t\t\t1.0\t\t\t1.0\t\t\t0.140254\t\t0.888459\n" +
                "msg_0007\tcore\t\t\t28.0\t\t\t54.0\t\t\t0.000000\t\t1.000000\n" +
                "msg_0008\tcore\t\t\t28.0\t\t\t57.0\t\t\t-1.898108\t0.0057682\n" +
                "msg_0009\tcore\t\t\t28.0\t\t\t57.0\t\t\t-0.972430\t0.330837\n",

            "input2":
                "import numpy\n" +
                "import scipy\n" +
                "from scipy import stats\n" +
                "\n" +
                "all_the_data = index_subset1+index_subset2\n" +
                "print(PanGenome)",

            "output2":
                "Traceback (most recent call last):\n" +
                "  File <stdin>, line 1, in <module>\n" +
                "PrivacyError: user cannot print the whole dataset\n",

            "input3": "",
            "output3": "",
        };
    }

    getJupyterData() {
        return [{
            "type": "code",
            "value":
                "import numpy\n" +
                "import scipy\n" +
                "from scipy import stats\n" +
                "import pandas"
        }, {
            "type": "heading",
            "value": "Support Functions"
        }, {
            "type": "code",
            "value":
                "def dfrow_testranksums(s, index_subset1, index_subset2):\n" +
                "  x1 = s[index_subset1]\n" +
                "  x2 = s[index_subset2]\n" +
                "  Nnotnull1 = sum(x1 != 0)\n" +
                "  Nnotnull2 = sum(x2 != 0)\n" +
                "  t = stats.ranksums(x1, x2)\n" +
                "  return pandas.Series([Nnotnull1, Nnotnull2, t.statistic, t.pvalue])"
        }, {
            "type": "heading",
            "value": "Test CD x nonIBD"
        }, {
            "type": "code",
            "value": "print(PanGenome_CDxnonIBD_df)"
        }, {
            "type": "output",
            "value":
                "msp_name\tmodule_name\tNnotnull1\tNnotnull2\tstatistic\tpvalue\n" +
                "msg_0001\tcore\t\t8.0\t\t14.0\t\t0.093503\t0.925504\n" +
                "msg_0002\tcore\t\t2.0\t\t8.0\t\t-0.476865\t0.633459\n" +
                "msg_0003\tcore\t\t28.0\t\t56.0\t\t0.794774\t0.426745\n" +
                "msg_0004\tcore\t\t3.0\t\t7.0\t\t-0.130904\t0.895851\n" +
                "msg_0005\tcore\t\t28.0\t\t57.0\t\t-1.028532\t0.303700\n" +
                "msg_0006\tcore\t\t1.0\t\t1.0\t\t0.140254\t0.888459\n" +
                "msg_0007\tcore\t\t28.0\t\t54.0\t\t0.000000\t1.000000\n" +
                "msg_0008\tcore\t\t28.0\t\t57.0\t\t-1.898108\t0.0057682\n" +
                "msg_0009\tcore\t\t28.0\t\t57.0\t\t-0.972430\t0.330837\n"
        }];

    }

    loadData(sampleData) {
        // TODO: Load real datasets by calling the backend
        this.setState({data: this.getDataset(sampleData), ide: this.getIdeData(), notebook: this.getJupyterData()},
            () => console.log(this.state.data));
    }

    render() {
        const data = this.state.data;
        const ide = this.state.ide;
        const usrId = Meteor.userId();
        if (undefined === usrId || null === usrId) {
            return <Redirect push to='/login'/>
        }
        const user = Meteor.user()
        var username=""
        if (user) { username = user.username }

        // unique enclaves
        var userEnclaves = this.state.enclaveInfo.filter((enclave1, index, self) =>
          index === self.findIndex((enclave2) => (enclave2.pubKey === enclave1.pubKey))
        );

        const enclave_list  = userEnclaves.filter(e => {
          if (e.hosts) {
            return e.hosts.host === username
          }
        })

        return (
            <div className="container-fluid">
                <div className="row">
                    <div className="navbar-base">
                        <NavBar/>
                    </div>
                    <div className="main-content col-10">
                    <IDE ide={ide}
                         trainerEthAdd={this.state.trainerEthAdd}
                         enclaveInfo={enclave_list}
                         />
                    </div>
                </div>
            </div>
        )

    };
}

export default withRouter(Analysis);
