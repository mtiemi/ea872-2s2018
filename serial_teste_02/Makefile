FLAGS=-lncurses -lpthread -lportaudio -std=c++11


serial: main_serialize.cpp serializable.cpp serializable.hpp
	g++ -oserial main_serialize.cpp serializable.cpp $(FLAGS)
	 ./serial
