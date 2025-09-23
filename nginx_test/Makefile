all: up

up:
	docker run -d --name nginx \
	--rm \
	-p 8080:80 \
	-v "./http.conf":/etc/nginx/conf.d/http.conf:ro \
	nginx:1.28.0

exec:
	docker exec -it nginx /bin/bash
