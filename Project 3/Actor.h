#ifndef ACTOR_H_
#define ACTOR_H_
#include "Field.h"
#include "GraphObject.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include "Compiler.h"
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;
class Compiler;
int ran(int a, int b);

void trapped(Actor*p, char type);
void setTrapped(Actor*p, nlist::iterator lit, char type);


class Actor :public GraphObject {
public:
	Actor(int imageID, int startX, int startY, StudentWorld*s, int HP,Direction dir = none, int depth = 0, double size = 0.25)
		:GraphObject(imageID, startX, startY, dir, depth, size), m_HP(HP), m_sleep(0) {
		m_trapped = 0;
		m_world = s;
	}
	virtual void doSomething() = 0;//every actor to do something, because actor don't have to do something, it is pure virtual function
	virtual ~Actor() {};//because lots of class is based on Actor, so destructor has to be virtual
	int setHP(int value);//set Hit point
	StudentWorld* getworld();//get the pointer to student world
	int getHP() const;//get the value of Hit Point
	int setSleep(int sleep);//set Actor to sleep
	int getSleep() const;//get the value that how many ticks should be sleep
	int ranDirection();//set a random direction
	bool gettrapped();//determine which water pool trap actor
	void setTrapped(bool v);//set which water pool trap actor
	bool isDead() {//determine whether or not the actor dead
		if (m_HP > 0)return false;
		else return true;
	}
	void addFood(int value);
	virtual void setbite() {};//set bite for ant class
	virtual int getID() { return 0; }//get colony number for ant
	bool eat(int value);
private:
	StudentWorld*m_world;
	int m_HP;
	int m_sleep;
	bool m_trapped;
};



class Pheromone : public Actor {
public:
	Pheromone(int imageID, int startX, int startY, StudentWorld*stud, int ID, int HP=256)
		: Actor(imageID, startX, startY, stud, 256, right, 2, 0.25), m_ID(ID) {};
	virtual void doSomething();
	virtual  int getID() {//get colony number
		return m_ID;
	}
private:
	int m_ID;
};



class ants :public Actor {
public:
	ants(int imageID, int startX, int startY, StudentWorld* stud, Compiler *compilerForEntrant, int ID)
		: Actor(imageID, startX, startY, stud, 1500, none, 1, 0.25), m_coloney(compilerForEntrant), m_ID(ID), m_randnum(0), m_hfood(0), m_Smove(0), m_bite(0){
		ranDirection();
		ic = 0;
	};
	virtual void doSomething();//do something
	void move();//ant's move
	int StringtoInt(string temp);//convert a string to Int
	void eat();// ant's eat
	void setbite() {
		m_bite = true;
	}//ant being bite by other insect
	void bite();//ant 's bite
	Compiler::Condition covertoCondition(string ope);//covert a string to enum condition
	bool readIF(Compiler::Command cmd);//read if statement
	void emit();//emit Pheromone
	virtual int getID() {//get colony number
		return m_ID;
	}
	void pickup();
private:
	Compiler *m_coloney;
	int m_ID;
	int m_distance;
	int m_randnum;
	int m_hfood;
	bool m_Smove;
	bool m_bite;
	int ic;
	int cou;
};

class Pebble : public Actor {
public:
	Pebble( int startX, int startY, StudentWorld*stud, int HP = 9999)
		: Actor(IID_ROCK, startX, startY, stud, HP, right, 1, 0.25) {};
	virtual void doSomething();
};

class Food : public Actor {
public:
	Food( int startX, int startY, StudentWorld*stud, int HP = 6000)
		: Actor(IID_FOOD, startX, startY, stud, HP, right, 2, 0.25) {}
	virtual void doSomething();
	};

class Water : public Actor {
public:
	Water(int startX, int startY, StudentWorld*stud, int HP = 9999)
		: Actor(IID_WATER_POOL, startX, startY, stud, 9999, right, 2, 0.25) {};
	virtual void doSomething();
};

class Anthill : public Actor {
public:
	Anthill(int startX, int startY, StudentWorld*stud, Compiler *compilerForEntrant,int antD)
		: Actor(IID_ANT_HILL, startX, startY, stud, 8999, right, 2, 0.25), m_compiler(compilerForEntrant), antID(antD) {
	}
	virtual void doSomething();
	virtual int getID() {
		return antID;
	}
private:
	Compiler* m_compiler;
	int antID;
};



class Poison : public Actor {
public:
	Poison( int startX, int startY, StudentWorld*stud)
		: Actor(IID_POISON, startX, startY, stud, 9999, right, 2, 0.25) {};
	virtual void doSomething();
};

class Baby_Grasshopper : public Actor {
public:
	Baby_Grasshopper( int startX, int startY, StudentWorld*stud, int imageID=IID_BABY_GRASSHOPPER, int HP = 500, Direction direct = none, int depth = 1, double size = 0.25)
		: Actor(IID_BABY_GRASSHOPPER, startX, startY, stud, HP, direct, depth, size), m_distance(0){
		int dir=ranDirection();
		m_distance = ran(2, 10);
	};
	void move();
	virtual void doSomething();
private:
	int m_distance;
};

class Adult_Grasshopper : public Baby_Grasshopper {
public:
	Adult_Grasshopper(int startX, int startY, StudentWorld*stud, int imageID=IID_ADULT_GRASSHOPPER, int HP = 1600, Direction direct = none, int depth = 1, double size = 0.25)
		: Baby_Grasshopper( startX, startY, stud, imageID, HP, direct, 1, 0.25), m_distance(0) {
		ranDirection();
		m_distance = ran(2, 10);
	};
	virtual void doSomething();
	bool bite();
	bool jump();

private:
	int m_distance;

};



#endif // ACTOR_H_
