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

10. Start up the RabbitMQ Docker container. If you already have the container running (or aren't sure if you have the container running), you can list Docker containers with `sudo docker ps -a` and then use `sudo docker run <containerid>` to start up an exited container (if you are on Windows, do not use sudo).
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

15. You should be all set! Let Lauren know if you have any trouble.