commonFile := InetAddress.cpp MyEpoll.cpp MySocket.cpp utils.cpp MyChannel.cpp EventLoop.cpp Server.cpp Acceptor.cpp Connection.cpp Buffer.cpp ThreadPool.cpp
logFile := Logger.cpp
Lib := -lpthread

all: server client

server:
	g++ main.cpp $(commonFile) $(logFile) $(Lib) -o server
client:
	g++ client.cpp $(logFile) -o client

.PHONY: clean cleanall

clean:
	rm server

cleanall:
	rm client server