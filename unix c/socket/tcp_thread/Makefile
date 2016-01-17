all:server client

server:wrap.c server.c
	gcc $^ -o $@ -lpthread
client:client.c wrap.c
	gcc $^ -o $@ -lpthread

.PHONT:clean
clean:
	rm -f server
	rm -f client
