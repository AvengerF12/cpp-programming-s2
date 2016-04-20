CXX=g++
CXXFLAGS=-Wall -std=c++11 -g 

all:
	$(CXX) $(CXXFLAGS) -o darts main.cpp player.cpp helpers.cpp
