# genetank-server
GeneTank server-side code.

## Setup
1. Install git, node.js, and npm.

2. Clone this repository:
```
git clone https://github.com/lahuang4/genetank-server
```

3. Enter the directory and install node packages.
```
cd genetank-server
npm install
```

4. Obtain the `config/` folder from someone (Lauren or Zhizhuo). It contains secret stuff that should not be committed into the GitHub repository. The `config/` folder should go under the `src/` folder.

5. Install MySQL. Ensure that MySQL is running.

6. Create the database tables. You can do this with the script in `setup/dbsetup.js`. Just run:
```
node setup/dbsetup.js
```

7. Add the diseases to the diseases database table. You can do this by obtaining a .csv of diseases and a script to add them (add_diseases.js) from someone (Lauren). Once you have both, run the script with `node`.

8. Install Docker. Docker is used to run RabbitMQ, the messaging service, which is used to run the analysis tasks. You may need to restart your computer.

9. Obtain the Docker image for RabbitMQ (if you are on Windows, do not use sudo):
```
docker pull rabbitmq
```

10. Start up the RabbitMQ Docker container. If you already have the container running (or aren't sure if you have the container running), you can list Docker containers with `sudo docker ps -a` and then use `sudo docker start <containerid>` to start up an exited container (if you are on Windows, do not use sudo).
```
sudo docker run -d --name rabbit-0 -p 5672:5672 -p 15672:15672 rabbitmq:latest
```

11. Build the project, which compiles the TypeScript into JavaScript. The compiled files will be in a folder called `dist/`. Each time you make changes and want to re-run the app, you'll need to re-build the app.
```
npm run build
```

12. The app comes in two parts: the API (ApiApp) and the analysis worker (AnalysisApp). You can run the API with:
```
npm start
```
or alternatively
```
npm run api
```
and you can run the analysis app with
```
npm run analysis
```

13. Make sure you have R installed -- in particular, ensure that the `rscript` command works. It's used to run some of the analysis scripts.

14. If you're running the GeneTank server locally for development, you may need to change the URL of the API in the Angular project, which is in `src/app/app.config.ts`. For local development, change the IP address in `BASEURL` to `localhost`.

15. Point your domain name to your server IP address. If you are not using the domain name api.genetank.xyz, make sure to change the `BASEURL` in `src/app/app.config.ts`.

16. Acquire a certificate for your domain name from Let's Encrypt (instructions [here](https://certbot.eff.org/#ubuntuxenial-other)). You can run the Let's Encrypt certbot installer with the `certonly --standalone` option, which will create a fullchain.pem file and a privkey.pem file (certbot will print out the location of the .pem file, so be sure to take a look at the output). Copy these files into your `config/` folder (these examples use my .pem file locations, which may be different than yours, please adjust accordingly):
```
sudo cp /etc/letsencrypt/live/api.genetank.xyz/fullchain.pem ~/genetank-server/src/config/cert.pem
sudo cp /etc/letsencrypt/live/api.genetank.xyz/privkey.pem ~/genetank-server/src/config/key.pem
```

17. Set up your certificates to auto-renew. First, verify that `sudo certbot renew --dry-run` seems to work fine. Then we'll add a scheduled cron job to try renewing the certificate once a week. Open your cron jobs with:
```
crontab -e
```
It may ask you which editor you want to use. I recommend nano, unless you are familiar with the other editors. At the bottom of the file, add this line:
```
  0 9  *   *   1     ~/genetank-server/setup/renew_certs >> ~/log_renew_certs
```
Then save the file by pressing Ctrl-X, then press Enter to save the file in the same location as before. The `renew_certs` script will run certbot renew and copy the generated files over to your `config/` folder every Sunday at 9 AM UTC (5 AM EST).

18. Before the job can run properly, we need to make sure the renew_certs script is able to run properly. Make sure the ubuntu user can run the script by changing its permissions:
```
sudo chmod u+x ~/genetank-server/setup/renew_certs
```

19. After you're done with setting up certificates, restart the Node.js app. We'll eventually put the app into a Docker container, and will update these instructions with that setup once it's finalized.

20. You should be all set! Let Lauren know if you have any trouble.