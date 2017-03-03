#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Field.h"
#include <string>
#include<list>
#include<map>
class Actor;
enum itemType {
pebble,
food,
baby_grasshopper,
adult_grasshopper,
water,
poison,
anthill,
ant,
pheromone,
others};
using namespace std;
typedef pair<int, int> npair;
typedef list<Actor*> nlist;
typedef map<char, nlist> nmap;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir), m_time(2000) {
		for (int a = 0; a < 64; a++)for (int b = 0; b < 64; b++) {
			m_map[a][b] = 0;
			m_foodmap[a][b] = 0;
		}
		for (int a = 0; a < 4; a++)noAnt[a] = -100;
	}

	virtual int init();
	virtual int move();
	virtual void cleanUp();
	bool addActor(Actor*p,char type);
	nlist findObj(char type);
	bool DoDo(char type);
	char getmap(int x, int y);
	char getfmap(int x, int y);
	void setfmap(int x, int y);
	void setnoAnt(int index, int n) {
		noAnt[index] = n;
	}
	int getnoAnt(int index) {
		return noAnt[index];
	}
	void del();
private:
	nmap m_Actors;
	char m_map[64][64];
	char m_foodmap[64][64];//food
	int m_time;
	int noAnt[4];
	string nameOColony[4];
};

#endif // STUDENTWORLD_H_
