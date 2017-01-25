#include <iostream>
#include "ScoreList.h"
#include <cassert>

using namespace std;

int main()
{
	ScoreList scores;
	for (int a = 0; a < 200; a++)
		scores.add(2);
	assert(!scores.add(3));
	int test = scores.add(101);
	assert(!scores.remove(5));
	assert(scores.remove(2));
	
	
	
	return 0;
}
