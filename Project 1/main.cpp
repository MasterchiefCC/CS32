// rats.cpp
#include "Arena.h"
#include "globals.h"
#include "Rat.h"
#include "Player.h"
#include "Game.h"
#include "History.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <cctype>
#include <cassert>
#include <time.h>
using namespace std;
#include "History.h"


int main()
{
	// Create a game
	// Use this instead to create a mini-game:  
	Game g(3,5 ,2);
	//Game g(10, 12, 40);

	// Play the game
	g.play();
}
