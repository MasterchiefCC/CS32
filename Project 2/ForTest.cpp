#include <iostream>
#include <cassert>
#include "Sequence.h"
using namespace std;
void testSubSeq() {
	Sequence e;
	e.insert(0, 32);
	e.insert(1, 21);
	e.insert(2, 63);
	e.insert(3, 42);
	e.insert(4, 17);
	e.insert(5, 63);
	e.insert(6, 17);
	e.insert(7, 29);
	e.insert(8, 8);
	e.insert(9, 32);
	Sequence a;
	a.insert(0, 63);
	a.insert(1, 17);
	a.insert(2, 29);
	assert(subsequence(e, a)==5);
	Sequence b;
	b.insert(0, 17);
	b.insert(1, 63);
	b.insert(2, 29);
	assert(subsequence(e, b) == -1);
	Sequence c;
	c.insert(0, 8);
	c.insert(1, 32);
	assert(subsequence(e, c) == 8);
	c.insert(2, 100);
	assert(subsequence(e, c) == -1);
	interleave(e, c, a);

}
int main() {
	Sequence s;
	assert(s.insert(0, 1));
	assert(s.insert(1, 2));
	assert(s.size() == 2);
	
	for (int a = 1; a < 5; a++)
		s.insert(a);
	s.erase(4);
	ItemType a;
	s.get(1, a);
	assert(s.get(1, a) == 1 && a == 1);
	assert(s.remove(1)==2);
	s.set(1, 100);
	assert(s.find(100) == 1);

	Sequence b;
	for (int y = 0; y < 100; y++)
		b.insert(y);
	b.swap(s);
	Sequence c(b);
	Sequence d;
	d = c;
	testSubSeq();
	Sequence t;
	for (int a = 0; a < 3; a++)
		t.insert(a);
	Sequence u;
	for (int y = 0; y < 7; y++)
		u.insert(y);
	interleave(u, t, d);
	return 0;
}