import React from 'react';
import {Redirect, Route, Switch} from "react-router-dom";
import SignIn from "../ui/pages/account/SignIn";
import {About} from "../ui/components/about";
import {MyWallet} from "../ui/pages/home/MyWallet";
import {ForgotPassword} from "../ui/pages/account/ForgotPassword";
import {EnrollAccount} from "../ui/pages/account/EnrollAccount";
import ListUsers from "../ui/pages/admindashboard/ListUsers";
import CreateAccount from "../ui/pages/account/CreateAccount";
import UpdateAccount from "../ui/pages/account/UpdateAccount";
import {Meteor} from "meteor/meteor";
import BrowseEnclave from "../ui/pages/model/BrowseEnclave";
import {TestEnclave} from "../ui/pages/test/testEnclave";
import {TaskManager} from "../ui/pages/taskmanager/taskManager";
import View from "../ui/pages/datasets/View";
import Manage from "../ui/pages/datasets/Manage";
import Contract from "../ui/pages/datasets/Contract";
import Analysis from "../ui/pages/analysis/Analysis";
import Enclaves from "../ui/pages/enclaves/Enclaves";
import RegisterEnclave from "../ui/pages/model/Enclaves";
import AccessData from "../ui/pages/access/AccessData";

export function HomeRouter(){
    return (
        <div>
            <Switch>
                <Route exact path={"/"} component={About} />
                <Route exact path={"/login"} component={SignIn} />
            </Switch>
        </div>
    )
}

export function AccountRouter(){
    return(
        <div>
            <Switch>
                <Route exact path="/wallet" component={MyWallet} />
                <Route exact path={"/forgotPassword"} component={ForgotPassword} />
                <Route exact path={"/enroll-account"} component={EnrollAccount} />
                <PrivateRoute exact path={"/admin"} component={ListUsers} />
                <PrivateRoute exact path={"/register"} component={CreateAccount} />
                <PrivateRoute exact path={"/update"} component={UpdateAccount} />
            </Switch>
        </div>
    )
}

export function EnclaveRouter(){
    return(
        <Switch>
            <Route exact path={"/browser"} component={BrowseEnclave} />
            <Route exact path={"/testEnclave"} component={TestEnclave} />
            <Route exact path={"/taskManager"} component={TaskManager} />
        </Switch>
    )
}

export function DatasetsRouter() {
    return (
        <div>
            <Switch>
                <Route exact path={"/view"} component={View} />
                <Route exact path={"/manage"} component={Manage} />
                <Route exact path={"/contract"} component={Contract} />
            </Switch>
        </div>

    )
}

export function AnalysisRouter() {
    return (
        <div>
            <Switch>
                <Route exact path={"/analysis"} component={Analysis} />
                <Route exact path={"/access"} component={AccessData}/>
            </Switch>
        </div>

    )
}


export function EnclavesRouter() {
    return (
        <div>
            <Switch>
                <Route exact path={"/enclaves"} component={Enclaves} />
                <Route exact path={"/enclaves/manage"} component={RegisterEnclave} />
            </Switch>
        </div>

    )
}


class PrivateRoute extends React.Component {
    state = {
        loading: true,
        isAuthenticated: false,
    }
    async componentDidMount() {

        Meteor.call('getRole', Meteor.userId(), 'admin', (err, res) => {
            if (err) reject('Something went wrong');
            else{
                this.setState({
                    loading: false,
                    isAuthenticated:res
                });
            }
        });
    }
/*
    async componentWillReceiveProps(nextProps, nextContext) {
        if (this.props !== nextProps){
            Meteor.call('getRole', Meteor.userId(), 'admin', (err, res) => {
                if (err) reject('Something went wrong');
                else{
                    this.setState({
                        loading: false,
                        isAuthenticated:res
                    });
                }
            });
        }
    }
*/
    render() {
        const { component: Component, ...rest } = this.props;
        if (this.state.loading) {
            return <div>Loading ....</div>;
        } else {
            return (
                <Route {...rest} render={props => (
                    <div>
                        {!this.state.isAuthenticated && <Redirect to={{ pathname: '/dummy', state: { from: this.props.location } }} />}
                        <Component {...this.props} />
                    </div>
                )}/>
            )
        }
    }
}

