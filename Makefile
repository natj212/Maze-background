all: mazepng.o link

mazepng.o: mazepng.cpp
	g++ --std=c++11 -c mazepng.cpp

link: mazepng.o
	g++ --std=c++11 mazepng.o -lpng -o mazepng

clean: 
	rm mazepng.o mazepng
