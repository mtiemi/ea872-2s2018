FLAGS=-lncurses -lpthread -lportaudio -std=c++11


client: client.cpp oo_model.cpp oo_model.hpp
	g++ -oclient client.cpp oo_model.cpp $(FLAGS)
	./client

server: server.cpp oo_model.cpp oo_model.hpp
	g++ -oserver server.cpp oo_model.cpp $(FLAGS)
	 ./server

