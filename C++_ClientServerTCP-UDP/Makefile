PORT = 8082
IP_SERVER = 127.0.0.1

all: server subscriber

server: server.cpp
	g++ -Wall server.cpp -o server

subscriber: subscriber.cpp
	g++ -Wall subscriber.cpp -o subscriber

run_server:
	./server ${PORT}

clean:
	rm subscriber server
