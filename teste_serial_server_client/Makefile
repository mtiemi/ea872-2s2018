FLAGS=-lncurses -lpthread -lportaudio -std=c++11

client: client.cpp serializable.cpp serializable.hpp
	g++ -oclient client.cpp serializable.cpp $(FLAGS)
	./client

server: server.cpp serializable.cpp serializable.hpp
	g++ -oserver server.cpp serializable.cpp $(FLAGS)
	 ./server

serial: main_serialize.cpp serializable.cpp serializable.hpp
	g++ -oserial main_serialize.cpp serializable.cpp $(FLAGS)
	 ./serial
