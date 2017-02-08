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
	queue<Coord> coordStack;
	char **mazeC = (char**)malloc(sizeof(char*)*nRows);
	for (int a = 0; a < nRows; a++)
		mazeC[a] = (char*)malloc(sizeof(char)*nCols);
	for (int a = 0; a < nRows; a++)
		for (int b = 0; b < nCols; b++)
			mazeC[a][b] = maze[a][b];

	Coord init(sr, sc);
	coordStack.push(init);
	while (!coordStack.empty()) {
		Coord temp = coordStack.front();
		coordStack.pop();
		int tr = temp.r();
		int tc = temp.c();
		if (tr == er&&tc == ec) { for (int a = 0; a < nRows; a++){free(mazeC[a]);} free(mazeC); return true; }
		mazeC[tr][tc] = 'X';
		for (int a = -1; a <= 1; a++)
			for (int b = -1; b <= 1; b++) {
				if (a!=b&&a+b!=0&&tr + a >= 0 && tc + b >= 0&& tr+a<nRows && tc+b<nCols && mazeC[tr + a][tc + b] == '.') {
					Coord pushin(tr + a, tc + b);
					coordStack.push(pushin);
				}
			}
	}
	for (int a = 0; a < nRows; a++){ free(mazeC[a]);} free(mazeC);
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

	if (pathExists(maze, 10, 10, 6, 4, 6, 1))
		cout << "Solvable!" << endl;
	else
		cout << "Out of luck!" << endl;
	if (pathExists(maze, 10, 10, 6, 4, 1, 1))
		cout << "Solvable!" << endl;
	else
		cout << "Out of luck!" << endl;
}
