
#include "../include/Board.hpp"

int main(int argc, char const *argv[])
{
	Board *B = new Board();
	B->Move();

	return 0;
}



//g++ -c Board.cpp main.cpp  `sdl2-config --cflags --libs`
//g++ Board.o main.o -o exe  `sdl2-config --cflags --libs` -lSDL2_image 