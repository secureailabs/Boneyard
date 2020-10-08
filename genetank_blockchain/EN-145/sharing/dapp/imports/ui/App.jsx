import React from 'react';
import {BrowserRouter as Router, Redirect} from "react-router-dom";
import {Meteor} from 'meteor/meteor';
//import {NavBar} from '/imports/ui/components/NavBar';
//import {TopBar} from '/imports/ui/components/TopBar';
import {HomeRouter} from "../Routes/Routes";
import {AccountRouter} from "../Routes/Routes";
import {EnclaveRouter} from "../Routes/Routes";
import {DatasetsRouter} from "../Routes/Routes";
import {AnalysisRouter} from "../Routes/Routes";
import {EnclavesRouter} from "../Routes/Routes";
//import SignIn from "./pages/account/SignIn";


export class GtDapp extends React.Component {
    constructor(props) {
        super(props);
        this.state = {
            isLoggedIn: null
        }
    }

    componentDidMount() {
        this.setState({isLoggedIn: Meteor.userId()});
    }

    render() {
        return (
            <div>
                <Router>
                    <div>
                        <HomeRouter/>
                        <AccountRouter/>
                        <EnclaveRouter/>
                        <DatasetsRouter/>
                        <AnalysisRouter/>
                        <EnclavesRouter/>
                    </div>
                </Router>
            </div>
        )
    }
}