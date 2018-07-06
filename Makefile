all: compile
compile: 
	gcc server.c -o server
	gcc client.c -o client
sc:
	./client 127.0.0.1 22222
ss:
	./server 22222
clean:
	rm server
	rm client
