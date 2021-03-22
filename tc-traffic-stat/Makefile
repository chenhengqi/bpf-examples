init:
	# 512MB = 4KB * 128K
	dd if=/dev/zero of=/tmp/testfile bs=4096 count=131072

run:
	sudo docker run -d --rm \
		-p 10086:80 \
		-v /tmp/testfile:/home/data/testfile \
		-v $(PWD)/default.conf:/etc/nginx/conf.d/default.conf \
		--name my-nginx \
		--network my-tc-net \
		nginx:alpine

kill:
	sudo docker kill my-nginx

exec:
	sudo docker exec -it my-nginx /bin/sh

download:
	curl http://localhost:10086/downloads/testfile --output testfile

clean:
	rm -f testfile
