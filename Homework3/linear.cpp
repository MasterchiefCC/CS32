#include <iostream>
#include <cmath>
#include <streambuf>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <cassert>
using namespace std;



bool somePredicate(double x){
	return x>=0;  
}


// Return false if the somePredicate function returns false for at
// least one of the array elements; return true otherwise.
bool allTrue(const double a[], int n){
	if (n == 0)return true;
	if (n == 1)return somePredicate(a[0]);
	return (somePredicate(a[0]) && allTrue(a + 1, n - 1));
}

// Return the number of elements in the array for which the
// somePredicate function returns false.
int countFalse(const double a[], int n){
	if (n <= 0)return 0;
	int count = somePredicate(a[0]) ? 0 : 1;
	if (n <= 1)return count;
	return count+countFalse(a+1, n-1);
}

// Return the subscript of the first element in the array for which
// the somePredicate function returns false.  If there is no such
// element, return -1.
int firstFalse(const double a[], int n){
	if (n <= 0)return -1;
	if (somePredicate(a[0]) == false)return 0;
	int ok = firstFalse(a + 1, n - 1);
	if (ok!= -1) {
		return 1 + firstFalse(a + 1, n - 1);
	}
	return -1;

}

// Return the subscript of the smallest double in the array (i.e.,
// the one whose value is <= the value of all elements).  If more
// than one element has the same smallest value, return the smallest
// subscript of such an element.  If the array has no elements to
// examine, return -1.
int indexOfMin(const double a[], int n){
	if (n <= 0)return -1;
	if (n == 1)return 0;
	int j = 1 + indexOfMin(a + 1, n - 1);
	if (a[0] <= a[j])return 0;
	return j;
}

// If all n2 elements of a2 appear in the n1 element array a1, in
// the same order (though not necessarily consecutively), then
// return true; otherwise (i.e., if the array a1 does not include
// a2 as a not-necessarily-contiguous subsequence), return false.
// (Of course, if a2 is empty (i.e., n2 is 0), return true.)
// For example, if a1 is the 7 element array
//    10 50 40 20 50 40 30
// then the function should return true if a2 is
//    50 20 30
// or
//    50 40 40
// and it should return false if a2 is
//    50 30 20
// or
//    10 20 20
bool includes(const double a1[], int n1, const double a2[], int n2){
	if ( n2 == 0)return 1;
	if (n1 == 0 && n2 != 0)return 0;
	if (a1[0] == a2[0])return includes(a1 + 1, n1 - 1, a2 + 1, n2 - 1);
	else return includes(a1 + 1, n1 - 1, a2, n2);
}

void testone(int n)
{

	double a[10] = { 3, 6, -5, 5, -2, 1, -7, -4, 8, 2 };
	double b[4] = { 6, -5, -2, -4 };
	double c[8] = { 10, 50, 40, 20, 50, 40, 30 };
	double d[12] = { 10, 20, 40, 10, 40, 30, 20, 10, 40, 50, 40, 30 };
	double e[12] = { 10, 10, 10, 20, 20, 30, 30, 40, 40, 40, 40, 50 };
	double f[16] = { 40, 70, 10, 120, 30, 40, 50, 110,
        90, 20, 10, 60, 100, 110, 40, 80 };
	double g[16] = { 120, 110, 110, 100, 90, 80, 70, 60,
        50, 40, 40, 40, 30, 20, 10, 10 };
    
	switch (n)
	{
        default: {
            assert(true);
        } break; case  1: {
            assert(!allTrue(a, 10));
        } break; case  2: {
            assert(!allTrue(a, 8));
        } break; case  3: {
            assert(!allTrue(a+2, 8));
        } break; case  4: {
            assert(allTrue(a+8, 2));
        } break; case  5: {
            assert(!allTrue(a+6, 2));
        } break; case  6: {
            assert(allTrue(a, 1));
        } break; case  7: {
            assert(!allTrue(a+2, 1));
        } break; case  8: {
            assert(allTrue(a, 0)  &&  allTrue(a+2, 0));
        } break; case  9: {
            assert(countFalse(a, 10) == 4);
        } break; case 10: {
            assert(countFalse(a, 8) == 4);
        } break; case 11: {
            assert(countFalse(a+2, 8) == 4);
        } break; case 12: {
            assert(countFalse(a+8, 2) == 0);
        } break; case 13: {
            assert(countFalse(a+6, 2) == 2);
        } break; case 14: {
            assert(countFalse(a, 1) == 0);
        } break; case 15: {
            assert(countFalse(a+2, 1) == 1);
        } break; case 16: {
            assert(countFalse(a, 0) == 0  &&  countFalse(a+2, 0) == 0);
        } break; case 17: {
            assert(firstFalse(a, 10) == 2);
        } break; case 18: {
            assert(firstFalse(a, 8) == 2);
        } break; case 19: {
            assert(firstFalse(a+2, 8) == 0);
        } break; case 20: {
            assert(firstFalse(a+8, 2) == -1);
        } break; case 21: {
            assert(firstFalse(a+6, 2) == 0);
        } break; case 22: {
            assert(firstFalse(a, 1) == -1);
        } break; case 23: {
            assert(firstFalse(a+2, 1) == 0);
        } break; case 24: {
            assert(firstFalse(a, 0) == -1  &&  firstFalse(a+2, 0) == -1);
        } break; case 25: {
            assert(indexOfMin(a, 10) == 6);
        } break; case 26: {
            assert(indexOfMin(a, 7) == 6);
        } break; case 27: {
            assert(indexOfMin(a+6, 4) == 0);
        } break; case 28: {
            assert(indexOfMin(a+8, 2) == 1);
        } break; case 29: {
            assert(indexOfMin(a+7, 2) == 0);
        } break; case 30: {
            assert(indexOfMin(a, 1) == 0);
        } break; case 31: {
            assert(indexOfMin(a+2, 1) == 0);
        } break; case 32: {
            double x[8] = { 3, 6, -5, 2, -5, -5, -5, 2 };
            assert(indexOfMin(x, 8) == 2);
        } break; case 33: {
            assert(indexOfMin(a, 0) == -1  &&  indexOfMin(a+2, 0) == -1);
        } break; case 34: {
			bool test = includes(a, 10, b, 4);
            assert(includes(a, 10, b, 4));
        } break; case 35: {
            assert(includes(a+1, 9, b, 4));
        } break; case 36: {
            assert(!includes(a+2, 8, b, 4));
        } break; case 37: {
            assert(!includes(a+1, 6, b, 4));
        } break; case 38: {
            assert(includes(a+1, 7, b, 4));
        } break; case 39: {
            assert(includes(a, 10, b+2, 1));
        } break; case 40: {
            assert(includes(a+1, 9, b, 1));
        } break; case 41: {
            assert(!includes(a+2, 8, b, 1));
        } break; case 42: {
            assert(includes(a, 10, b, 0));
        } break; case 43: {
            assert(includes(a, 0, b, 0));
        } break; case 44: {
            assert(includes(a+1, 2, b, 2));
        } break; case 45: {
            assert(!includes(a+2, 2, b+1, 2));
        } break; case 46: {
            assert(!includes(a+2, 2, b+1, 3));
        } break; case 47: {
            assert(!includes(a+1, 1, b, 2));
        } break; case 48: {
            assert(!includes(a+1, 0, b, 1));
        } break; case 49: {
            assert(includes(c, 7, d+3, 3));
        } break; case 50: {
            assert(!includes(c, 7, d+6, 3));
        } break; case 51: {
            assert(includes(c, 7, d+9, 3));
        } break; 
        }
	}
	


int main(){
	for (int a = 0; a < 67; a++) 
		testone(a);
	cerr << "pass all test\n" << endl;
	double test[10];
	for (int a = 0; a < 10; a++)test[a] = 1+a;
	assert(allTrue(test, 10));
	for (int a = 5; a < 10; a++)test[a] = -a;
	assert(!allTrue(test, 10));
	assert(countFalse(test, 10) == 5);
	int z = firstFalse(test, 10);
	assert(firstFalse(test, 10) == 5);
	for (int a = 0; a < 10; a++)test[a] = 1 + a;
	z = countFalse(test, 10);
	assert(countFalse(test, 10) == 0);
	assert(firstFalse(test, 10) == -1);
	cerr << "pass all test\n" << endl;
	
}