#include "newSequence.h"
#include <iostream>
#include <cassert>
using namespace std;

void test()
{
	Sequence s;
	assert(s.insert(0, 10));
	assert(s.insert(0, 20));
	assert(s.size() == 2);
	ItemType x = 999;
	assert(s.get(0, x) && x == 20);
	assert(s.get(1, x) && x == 10);
}

int main()
{
	test();
	Sequence a(5);
	a.insert(1);
	a.insert(3);
	Sequence b(10);
	Sequence c(a);
	b = a;
	b.insert(2);
	cout << "Passed all tests" << endl;
}