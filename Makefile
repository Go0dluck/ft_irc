all: server client

server:
	clang++ server.cpp -o server -w
client:
	clang++ client.cpp -o client -w

clean:
	rm -f server
	rm -f client
re:	clean all