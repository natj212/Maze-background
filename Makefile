all: mazepng.o link

mazepng.o: mazepng.cpp
	g++ --std=c++11 -c mazepng.cpp -I/usr/X11R6/include

link: mazepng.o
	g++ --std=c++11 mazepng.o -L/usr/X11R6/lib -lpng -o mazepng

clean: 
	rm mazepng.o mazepng
