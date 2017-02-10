#include <iostream>
#include <queue>
#include <string.h>
#include<cassert>
using namespace std;

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec) {
	//a != b&&a + b != 0 && tr + a >= 0 && tc + b >= 0 && tr + a < nRows && tc + b < nCols && mazeC[tr + a][tc + b] == '.'
	if (sr == er&&sc == ec)return true;
	maze[sr][sc] = 'X';
		for (int a = -1; a <= 1; a++)
			for (int b = -1; b <= 1; b++)
				if (a != b&&a + b != 0 && sr + a >= 0 && sc + b >= 0 && sr + a < nRows && sc + b < nCols && maze[sr + a][sc + b] == '.')
					if(pathExists(maze, nRows, nCols, sr + a, sc + b, er, ec))return true;
	return false;
}
void testone(int n) {
	static string maze1[10] = {
		"XXXXXXXXXX",
		"X.X..X...X",
		"X.XX.X.XXX",
		"X....X.X.X",
		"XX.X.X...X",
		"XXX..X.X.X",
		"X...X...XX",
		"X.XX..X.XX",
		"X....X...X",
		"XXXXXXXXXX",
	};

	static string maze2[10] = {
		"XXXXXXXXXX",
		"X.X..X...X",
		"X.XX.X.XXX",
		"X....X.X.X",
		"XX.X.X...X",
		"XXX.XX.X.X",
		"X...X...XX",
		"X.XX..X.XX",
		"X....X...X",
		"XXXXXXXXXX",
	};

	static string maze3[10] = {
		"XXXXXXXXXX",
		"XX.....XXX",
		"X..XX....X",
		"X...X...XX",
		"X.X.XXX..X",
		"XXXX..X..X",
		"XX....X..X",
		"X.......XX",
		"X..XXXXXXX",
		"XXXXXXXXXX",
	};

	static string maze4[10] = {
		"XXXXXXXXXX",
		"XX.....XXX",
		"X..XX....X",
		"X...X...XX",
		"X.X.XXX..X",
		"XXXX..X..X",
		"XX....X..X",
		"X.....X.XX",
		"X..XXXXXXX",
		"XXXXXXXXXX",
	};
	switch (n) {
	default: {break; }
	case 1: {
		assert(pathExists(maze1, 10, 10, 8, 6, 1, 1));
		break;
	} break; case 2: {
		assert(!pathExists(maze2, 10, 10, 8, 6, 1, 1));
		break;
	} break; case 3: {
		assert(pathExists(maze3, 10, 10, 4, 3, 7, 1));
		break;
	} break; case 4: {
		assert(!pathExists(maze4, 10, 10, 4, 3, 7, 1));
		break;
	}
	}
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
		for (int a = 1; a <= 4; a++)testone(a);
		cerr << "pass all test\n";
}