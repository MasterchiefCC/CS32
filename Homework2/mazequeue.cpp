#include <iostream>
#include <queue>
#include <string.h>
using namespace std;
class Coord {
public:
	Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
	int r() const { return m_r; }
	int c() const { return m_c; }
private:
	int m_r;
	int m_c;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec) {
	int counter = 0;
	queue<Coord> coordStack;
	Coord init(sr, sc);
	coordStack.push(init);
	while (!coordStack.empty()) {
		Coord temp = coordStack.front();
		coordStack.pop();
		int tr = temp.r();
		int tc = temp.c();
		if (tr == er&&tc == ec) return true;
		maze[tr][tc] = 'X';
		for (int a = -1; a <= 1; a++)
			for (int b = -1; b <= 1; b++) {
				if (a == b)continue;
				if (tr + a >= 0 && tr + a < nRows&&tc + b < nCols&& tc + b >= 0 && maze[tr + a][tc + b] == '.') {
					Coord pushin(tr + a, tc + b);
					coordStack.push(pushin);
				}
			}
	}
	return false;
}


int main()
{
	string maze[10] = {
		"XXXXXXXXXX",
		"X........X",
		"XX.X.XXXXX",
		"X..X.X...X",
		"X..X...X.X",
		"XXXX.XXX.X",
		"X.X....XXX",
		"X..XX.XX.X",
		"X...X....X",
		"XXXXXXXXXX"
	};

	if (pathExists(maze, 10, 10, 6, 4, 1, 1))
		cout << "Solvable!" << endl;
	else
		cout << "Out of luck!" << endl;
}