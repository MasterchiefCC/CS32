#ifndef HISTORY
#define  HISTORY
#include "globals.h"
class History {
public:
	History();
	History(int nRows, int nCols);
	bool record(int r, int c);
	void display() const;
private:
	int m_Rows;
	int m_Cols;
	int count[MAXROWS][MAXCOLS];
};



#endif