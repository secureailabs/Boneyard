# ChainHub

# 1. Run a ChainHub server

## 1.1 Download the code of chainhub

## 1.2 Find out the MongoDB IP address and port number

You can run a Meteor App with a MongoDB. The port number for the MongoDB is the meteor port number plus 1. For example, if the meteor app port number is 3000, then the MongoDB port number is 3001.

### 1.2.1 Run a new instance of MongoDB

If you aren't already running a Meteor App with a MongoDB, you can run a standalone instance of MongoDB. Follow [these instructions](https://www.digitalocean.com/community/tutorials/how-to-install-mongodb-on-ubuntu-16-04) to install and run MongoDB. (Don't adjust the firewall options, we will only access the database from the local machine.)

Once installed, MongoDB will be running on port 27017.

## 1.3 Enter ChainHub Directory
```console
$ cd <chainhub directory>
```

## 1.4 Create configuration files

```console
$ cd config
```

### 1.4.1 Create a file `db.js` with below contents (suppose the MongoDB port number is 4201):
```
module.exports = {
  url: 'mongodb://127.0.0.1:4201/genetank'
}
```

### 1.4.2 Back to the ChainHub directory and continue the following steps

```console
$ cd ..
```

## 1.5 Install and configure Nginx

Set up Nginx to handle http traffic:

```console
$ sudo apt-get install nginx
$ sudo cp nginx/http-only /etc/nginx/sites-available/default
$ # Adjust /etc/nginx/sites-available/default file to match the expected port number and domain name.
$ sudo systemctl restart nginx
```

## 1.6 Install Node and NPM using NVM

Follow [these steps](https://github.com/creationix/nvm/blob/master/README.md#installation) to install nvm, which will allow us to install npm and node.

Then follow [these steps](https://github.com/creationix/nvm/blob/master/README.md#usage) to actually install Node (NPM comes for free).

## 1.7 Run 'npm install' if you haven't run it after downloading the codes

```console
$ npm install
```

## 1.8 Install the forever process manager, which will run the app

```console
$ npm install -g forever
```

## 1.9 Run the ChainHub server and listening on port number 4100

```console
$ PORT=4100 forever start ./bin/www
```
or
```console
$ PORT=4100 npm start
```

## 1.10 View running processes, and optionally stop the app

```console
$ forever list
$ forever stop <pid>
```

## 1.11 Set up SSL for https traffic

1. Install certbot following [these instructions](https://certbot.eff.org/lets-encrypt/ubuntuxenial-nginx). These instructions are for Ubuntu 16.04. If you are running a different version, select the relevant certbot installation instructions [here](https://certbot.eff.org/).

2. Generate certificates and have certbot edit the nginx configuration. Select the domain(s) you want to get certificates for when prompted. Allow nginx to redirect all requests to https. Once you're done, verify that going to your domain redirects you to https successfully.
```console
$ sudo certbot --nginx
```

3. Check that auto-renewal is set up properly. The `renew` command should run successfully, and a file `/etc/cron.d/certbot` should exist with a cron job specified in it to run every 12 hours. More details on what the cron job should look like [here](https://community.letsencrypt.org/t/cerbot-cron-job/23895/5).
```console
$ sudo certbot renew --dry-run
$ cat /etc/cron.d/certbot
```

### 1.11.1 Renewing certificates

Certificates should automatically be renewed. If you receive an email saying that your certificates are about to expire, something is likely wrong with the auto-renewal. Here are some troubleshooting steps you can take when auto-renewal isn't working.

- Check the certbot logs. These should be at `/var/log/letsencrypt/letsencrypt.log`. An example command printing out the last few lines in the logs:
```console
$ sudo tail -n 20 /var/log/letsencrypt/letsencrypt.log
```

- Check when the cron job last ran. You can view all cron jobs in the logfile by running:
```console
$ grep CRON /var/log/syslog
```

- Verify the cron job configuration. You can view the cron job's configuration in `/etc/cron.d/certbot`.

- Renew certificates manually.
```console
$ sudo certbot --nginx
```
