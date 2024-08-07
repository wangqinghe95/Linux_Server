commonFile := InetAddress.cpp MyEpoll.cpp MySocket.cpp utils.cpp

all: server client

server:
	g++ server.cpp $(commonFile) -o server
client: client.cpp
	g++ client.cpp -o client

.PHONY: clean

clean:
	rm server client