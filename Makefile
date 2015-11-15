all: maze.o link

maze.o: maze.cpp
	g++ --std=c++11 -c maze.cpp

link: maze.o
	g++ --std=c++11 maze.o -lpng -o mazepng

clean: 
	rm maze.o mazepng
