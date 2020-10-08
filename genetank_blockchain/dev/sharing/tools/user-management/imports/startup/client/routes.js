import React from 'react';
import { BrowserRouter as Router, Route, withRouter } from 'react-router-dom';

import Navigation from '../../ui/components/Navigation';

import Home from '../../ui/pages/Home';
import SignUp from '../../ui/pages/SignUp';
import SignIn from '../../ui/pages/SignIn';
import ForgottenPassword from '../../ui/pages/ForgottenPassword';
import ForgottenMnemonic from '../../ui/pages/ForgottenMnemonic';

export const renderRoutes = () =>
  <Router>
    <div>
      <Navigation history={history} />

      <Route
        exact path="/"
        component={() => <Home />}
      />
      <Route
        exact path="/register"
        component={() => <SignUp history={history} />}
      />
      <Route
        exact path="/signin"
        component={() => <SignIn history={history} />}
      />
      <Route
        exact path="/password"
        component={() => <ForgottenPassword history={history} />}
      />
       <Route
        exact path="/mnemonic"
        component={() => <ForgottenMnemonic history={history} />}
      />
    </div>
  </Router>
