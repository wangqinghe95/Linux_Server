commonFile := InetAddress.cpp MyEpoll.cpp MySocket.cpp utils.cpp MyChannel.cpp EventLoop.cpp Server.cpp
logFile := Logger.cpp

all: server client

server:
	g++ main.cpp $(commonFile) $(logFile) -o server
client:
	g++ client.cpp $(logFile) -o client

.PHONY: clean

clean:
	rm server client