#include <iostream>
#include "ScoreList.h"
#include <cassert>

using namespace std;

int main()
{
	ScoreList scores;
	assert(scores.size() == 0);
	for (int i = 0; i <= 2 * DEFAULT_MAX_ITEMS; i++)
	{
		scores.add(i / 2);
	}
	assert(!scores.add(110));
	assert(!scores.add(-10));
	ScoreList ScoreList2;
	ScoreList2 = scores;
	ScoreList ScoreList3(scores);
	scores.add(56);
	for (int k = 0; k < 30; k++)
	{
		scores.remove(k);
	}
	cout << "ScoreList size: " << scores.size() << endl;
	cout << "ScoreList max: " << scores.maximum() << endl;
	cout << "ScoreList min: " << scores.minimum() << endl;


}
