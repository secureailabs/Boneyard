# genetank-node

## Setup

### Setting up http traffic

1. Download code from repository and install dependencies.
```console
$ git clone https://github.com/lahuang4/genetank-node
$ cd ~/genetank-node
$ npm install
$ npm install -g forever
```

2. Install and configure nginx for http traffic. You may need to change the domain name in `/etc/nginx/sites-available/default` after copying the file, if you have a different domain name.
```console
$ sudo apt-get install nginx
$ sudo cp nginx/http-only /etc/nginx/sites-available/default
$ sudo systemctl restart nginx
```

3. Start running the app.
```console
$ forever start ./bin/www
```

4. You should be able to view the website if you navigate to your machine's IP address. If you are having trouble accessing the website, check your inbound rules on the instance that is running the website (make sure inbound ports 80 and 443 are open).

5. Change the DNS record for your domains to point at your machine's IP address. For example, this is what a GoDaddy DNS record would look like:

| Type | Name | Value               | TTL    |
|------|------|---------------------|--------|
| A    | @    | `<your IP address>` | 1 Hour |

### Setting up SSL (for https traffic)

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

### Create config files

1. Create a folder called `config/` in the base of the repository. This folder does not exist by default because it (and its contents) are excluded by this repository's `.gitignore`.

2. Get the following files from the current website administrator and place them in the new `config/` folder:

    - `database.js`
    - `email.js`

### Setting up database

1. The website uses a MySQL database. Either set up a managed MySQL instance with your server provider or [set up MySQL on your local machine](https://dev.mysql.com/downloads/mysql/).

2. Edit the `config/database.js` file you created in the previous step to match the connection host and login for your MySQL setup.

3. Run this script to set up your database:
```console
$ node scripts/dbsetup.js
```

## Renewing certificates

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
