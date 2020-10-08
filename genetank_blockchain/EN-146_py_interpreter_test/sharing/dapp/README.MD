# install the codes into a new directory

install all npm modules

$ meteor npm install

remove autopublish for safer messaging

$ meteor remove autopublish


# Getting your First User(Admin) and Configuring your platform mode

To get the default Admin User created, Please create a settings.json file at the root of dapp. A sample file looks like

{
  "defaultAccount": [
      {
        "username": "admin",
        "email": "admin@admin.com",
        "password": "admin",
        "role": "admin"
        }
  ],
  "public": {
    "mode": "production",
    "logo": "kpmg_logo.png"
  }
}

Then run the meteor app with a flag "meteor --settings setting.json". This will create the default Admin User for you, with the credentials as mentioned in the file.
Now you can login as this user to access the admin page through which you can make more users and create them as Admin.

# Sending Email invites to new users
Via the admin page you can send email invites for new user to sign up which allows them to create their own password and provides more security as without that feature admin will be creating passwords for those users.
To actually send invitation emails, an environment variable named "MAIL_URL" needs to be set that refers to an SMTP service (More-info https://docs.meteor.com/api/email.html), You can use https://sendgrid.com/ for a free service. (Check your spam folder as well for the emails)

If you don't set the MAIL_URL variable the, invitation emails and password reset links get printed on the console itself and you can access them from the console then.

# Blockchain Connection

You will need to set the URL in server/main.js file to connect to a blockchain network.
You can use Ganache CLI https://github.com/trufflesuite/ganache-cli/releases to emulate blockchain on localhost for dev purpose.
You also need to make sure all the smart contracts that are required are deployed on that blockchain. You can use https://remix.ethereum.org/ for that,
also you need to make sure that the runner trainer and data donor accounts have some ethers in them to execute the transactions, you can use Metamask to transfer ethers between accounts.
Check Confluence and Jira for more info.

# Chainhub Connection

The smart contracts push data to chainhub when a transaction happens,
so run chainhub server as well and specify the URL for that server in server/main.js

# Setup First Admin User
As public User Sign Up should not be allowed , thus to make new user accounts
an Admin account is required. To get your first admin user when the app
starts, add a "settings.json" file in the root directory of your application
 with the credentials you want for your first user. A sample file is as shown below
 
 {
   "defaultAccount": [
     {
       "username": "admin",
       "email": "admin@admin.com",
       "password": "admin",
       "role": "admin"
     }
   ]
 }
 
 Now for meteor to actually read this file , start meteor with the 
 following command.
 
 $ meteor --settings settings.json
 
 This ensures that you are never locked out from 
 accessing the register new user page


# remote debugging on the Meteor server

Start meteor with the command:

$ meteor --inspect=0.0.0.0:9229

Install NIM (Node Inspector Manager) plug-in on local browser (e.g. Chrome). Configure the IP address of the Meteor server and port number (9229) in NIM.

# Run meteor server for mobile app

# Meteor unit test

## Testing file naming rules for testing

*.test[s].*, or *.spec[s].*

## Install Mocha testing package
```
$ meteor add meteortesting:mocha
````

## Writing test files with chai assertion package
Example:
```
import { Factory } from 'meteor/dburles:factory';
import React from 'react';
import { shallow } from 'enzyme';
import chai from 'chai';
import TodoItem from './TodoItem.jsx';

describe('TodoItem', () => {
  it('should render', () => {
    const todo = Factory.build('todo', { text: 'testing', checked: false });
    const item = shallow(<TodoItem todo={todo} />);
    chai.assert(item.hasClass('list-item'));
    chai.assert(!item.hasClass('checked'));
    chai.assert.equal(item.find('.editing').length, 0);
    chai.assert.equal(item.find('input[type="text"]').prop('defaultValue'), 'testing');
  });
});
```

## Running unit test

```
$ TEST_WATCH=1 meteor test --driver-package meteortesting:mocha
```

Access this webpage for more information: https://guide.meteor.com/testing.html
