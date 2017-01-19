#include "Sequence.h"
#include <iostream>
#include <cassert>
#include <string>
using namespace std;

int main()
{
	Sequence s;
	s.insert(0, "dosa");
	s.insert(1, "pita");
	s.insert(2, "");
	s.insert(3, "matzo");
	assert(s.find("") == 2);
	s.remove("dosa");
	assert(s.size() == 3 && s.find("pita") == 0 && s.find("") == 1 &&
		s.find("matzo") == 2);
	cerr << "pass\n";
}