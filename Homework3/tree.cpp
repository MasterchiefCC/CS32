#include <iostream>
#include <cassert>
using namespace std;

// Return the number of ways that all n2 elements of a2 appear
// in the n1 element array a1 in the same order (though not
// necessarily consecutively).  The empty sequence appears in a
// sequence of length n1 in 1 way, even if n1 is 0.
// For example, if a1 is the 7 element array
//	10 50 40 20 50 40 30
// then for this value of a2     the function must return
//	10 20 40			1
//	10 40 30			2
//	20 10 40			0
//	50 40 30			3
int countIncludes(const double a1[], int n1, const double a2[], int n2){
	if (n2 <= 0)return 1;
	if (n1 <= 0)return 0;
	int counter=0;
	if (a1[0] == a2[0])counter+=countIncludes(a1 + 1, n1 - 1, a2 + 1, n2 - 1);
	else countIncludes(a1 + 1, n1 - 1, a2, n2);
	return counter + countIncludes(a1 + 1, n1 - 1, a2, n2);
}

// Exchange two doubles
void exchange(double& x, double& y)
{
	double t = x;
	x = y;
	y = t;
}

// Rearrange the elements of the array so that all the elements
// whose value is > separator come before all the other elements,
// and all the elements whose value is < separator come after all
// the other elements.  Upon return, firstNotGreater is set to the
// index of the first element in the rearranged array that is
// <= separator, or n if there is no such element, and firstLess is
// set to the index of the first element that is < separator, or n
// if there is no such element.
// In other words, upon return from the function, the array is a
// permutation of its original value such that
//   * for 0 <= i < firstNotGreater, a[i] > separator
//   * for firstNotGreater <= i < firstLess, a[i] == separator
//   * for firstLess <= i < n, a[i] < separator
// All the elements > separator end up in no particular order.
// All the elements < separator end up in no particular order.
void separate(double a[], int n, double separator,int& firstNotGreater, int& firstLess){
	if (n < 0)
		n = 0;

	// It will always be the case that just before evaluating the loop
	// condition:
	//  firstNotGreater <= firstUnknown and firstUnknown <= firstLess
	//  Every element earlier than position firstNotGreater is > separator
	//  Every element from position firstNotGreater to firstUnknown-1 is
	//    == separator
	//  Every element from firstUnknown to firstLess-1 is not known yet
	//  Every element at position firstLess or later is < separator

	firstNotGreater = 0;
	firstLess = n;
	int firstUnknown = 0;
	while (firstUnknown < firstLess)
	{
		if (a[firstUnknown] < separator)
		{
			firstLess--;
			exchange(a[firstUnknown], a[firstLess]);
		}
		else
		{
			if (a[firstUnknown] > separator)
			{
				exchange(a[firstNotGreater], a[firstUnknown]);
				firstNotGreater++;
			}
			firstUnknown++;
		}
	}
}

// Rearrange the elements of the array so that
// a[0] >= a[1] >= a[2] >= ... >= a[n-2] >= a[n-1]
// If n <= 1, do nothing.
void order(double a[], int n){
	if (n == 0)return;
	if (a[0] <= a[n - 1])exchange(a[0], a[n - 1]);
	order(a, n - 1);
	order(a + 1, n - 1);
}
void testone(int n) {
	double a[10] = { 3, 6, -5, 5, -2, 1, -7, -4, 8, 2 };
	double b[4] = { 6, -5, -2, -4 };
	double c[8] = { 10, 50, 40, 20, 50, 40, 30 };
	double d[12] = { 10, 20, 40, 10, 40, 30, 20, 10, 40, 50, 40, 30 };
	double e[12] = { 10, 10, 10, 20, 20, 30, 30, 40, 40, 40, 40, 50 };
	double f[16] = { 40, 70, 10, 120, 30, 40, 50, 110,
		90, 20, 10, 60, 100, 110, 40, 80 };
	double g[16] = { 120, 110, 110, 100, 90, 80, 70, 60,
		50, 40, 40, 40, 30, 20, 10, 10 };
	switch (n) {
	case 1: {
		assert(countIncludes(c, 7, d, 3) == 1);
	} break; case 2: {
		assert(countIncludes(c, 7, d + 3, 3) == 2);
	} break; case 3: {
		assert(countIncludes(c, 7, d + 6, 3) == 0);
	} break; case 4: {
		assert(countIncludes(c, 7, d + 9, 3) == 3);
	} break; case 5: {
		assert(countIncludes(c, 7, d + 2, 1) == 2);
	} break; case 6: {
		assert(countIncludes(c, 2, d + 2, 1) == 0);
	} break; case 7: {
		assert(countIncludes(c, 7, d, 0) == 1);
	} break; case 8: {
		assert(countIncludes(c, 0, d, 0) == 1);
	} break; case 9: {
		order(f, 16);
		assert(equal(f, f + 16, g));
	} break; case 10: {
		order(f, 1);
		assert(f[0] == 40);
	} break; case 11: {
		order(f, 0);
		assert(f[0] == 40);
	} break; default:break;
	}
}
int main() {
	for (int a = 1; a <= 11; a++)testone(a);
	double none[1] = { 0 };

	double a1[7] = { 10, 50, 40, 20, 50, 40, 30 };
	double a21[3] = { 10, 20, 40 };
	double a22[3] = { 10, 40, 30 };
	double a23[3] = { 20, 10, 40 };
	double a24[3] = { 50, 40, 30 };
	assert(countIncludes(none, 0, none, 5) == 0);
	assert(countIncludes(none, 0, none, 0) == 1);
	cerr << countIncludes(a1, 7, a21, 3) << endl; //1
	cerr << countIncludes(a1, 7, a22, 3) << endl; //2
	cerr << countIncludes(a1, 7, a23, 3) << endl; //0
	cerr << countIncludes(a1, 7, a24, 3) << endl; //3
	assert(countIncludes(a1, 7, a21, 3) == 1);
	assert(countIncludes(a1, 7, a22, 3) == 2);
	assert(countIncludes(a1, 7, a23, 3) == 0);
	assert(countIncludes(a1, 7, a24, 3) == 3);
	cerr << "Passed basic countIncludes tests." << endl;
	double sortMe1[7] = { 10, 50, 40, 20, 50, 40, 30 };
	order(sortMe1, 7);
	cerr << "SortMe1-----" << endl;
	for (int i = 0; i < 7; i++)
		cerr << sortMe1[i] << endl;
	cerr << "SortMe1-----" << endl;
	double sortMe2[10] = { 5, 3, 8, 9, 1, 7, 2, 4, 6, 0 };
	order(sortMe2, 10);
	cerr << "SortMe2-----" << endl;
	for (int i = 0; i < 10; i++)
		cerr << sortMe2[i] << endl;
	cerr << "SortMe2-----" << endl;
	double sortMe3[5] = { 3, 3, 3, 3, 3 };
	order(sortMe3, 5);
	cerr << "SortMe3-----" << endl;
	for (int i = 0; i < 5; i++)
		cerr << sortMe3[i] << endl;
	cerr << "SortMe3-----" << endl;
	order(none, 0);
	cerr << none[0] << endl;
	order(none, 1);
	cerr << none[0] << endl;
	cerr << "Passed basic order tests." << endl;




	return 0;
}