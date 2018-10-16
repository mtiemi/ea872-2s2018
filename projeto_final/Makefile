FLAGS=-lncurses -lpthread -lportaudio -std=c++11

all: snake

snake: model_mainloop.cpp oo_model.cpp oo_model.hpp
	g++ -osnake model_mainloop.cpp oo_model.cpp $(FLAGS)
	./snake
