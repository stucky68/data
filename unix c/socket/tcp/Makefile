all:server client

server:server.c
	gcc $< -o $@
client:client.c
	gcc $< -o $@

.PHONT:clean
clean:
	rm -f server
	rm -f client
