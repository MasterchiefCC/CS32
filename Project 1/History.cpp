#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
#include <cctype>
#include <cstring>
using namespace std;
#include "History.h"

History::History() {};

History::History(int nRows, int nCols) {
	m_Rows = nRows;
	m_Cols = nCols;
	for (int r = 0; r < nRows; r++)
		for (int c = 0; c < nCols; c++)
			count[r][c] = 0;
}


bool History::record(int r, int c) {
	if (r > m_Rows || c > m_Cols)return false;
	count[r - 1][c - 1] += 1;
	return true;

}
void History::display() const {
	clearScreen();
	int r, c;

	for (r = 0; r < m_Rows; r++) {
		for (c = 0; c < m_Cols; c++) {
			int numTimes = count[r][c];

			if (numTimes == 0)
				cout << '.';
			else if (numTimes > 26)
				cout << 'Z';
			else
				printf("%c", numTimes + 64);
		}
		cout << endl;
	}
	getchar();
	clearScreen();

}