set -xe

if [ ! -f "letsencrypt.tar.gz" ]; then
    aws s3 cp s3://sail-jenkins-backups/letsencrypt.tar.gz ./
    mkdir letsencrypt
    tar -C letsencrypt -xvf letsencrypt.tar.gz 
fi

docker build . -t sail_nginx:latest

docker run \
	--name sail_nginx \
	--rm \
	--detach \
	--network jenkins \
	--network-alias nginx \
	--publish 80:80 \
	--publish 443:443 \
	sail_nginx:latest

rm -rf letsencrypt.tar.gz letsencrypt
