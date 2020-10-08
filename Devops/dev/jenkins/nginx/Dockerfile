FROM nginx:latest
RUN ls
RUN rm /etc/nginx/conf.d/default.conf
COPY nginx.conf /etc/nginx/conf.d/default.conf
COPY letsencrypt /etc/letsencrypt

RUN apt update 
RUN apt upgrade -y 
RUN apt install -y certbot python3-certbot-nginx

