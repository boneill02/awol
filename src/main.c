/*
    AWOL: A 2D top down survival sandbox game
    Copyright (C) 2019 Ben O'Neill <ben@oneill.sh>
*/

#include <stdlib.h>
#include <time.h>

#include "game.h"

int main(int argc, char *argv[])
{
	srand(time(NULL));
	start_game();
	return EXIT_SUCCESS;
}
