#include "StudentWorld.h"
#include "Actor.h"
#include "Compiler.h"
#include <sstream>
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}
int StudentWorld::init() {
	string fieldFileName;
	Field f;	string fieldFile = getFieldFilename();
	Field::LoadResult load = f.loadField(fieldFile);
	if (load != Field::load_success)
		return GWSTATUS_NO_WINNER;
	nlist temp;
	m_Actors['P'] = temp;//Pebble 
	m_Actors['F'] = temp;//Food

	m_Actors['B'] = temp;//Baby Grasshopper
	m_Actors['D'] = temp; //Adult Grasshopper

	m_Actors['W'] = temp;//Water
	m_Actors['O'] = temp;//Poison
	
	m_Actors['H'] = temp;//antHill
	m_Actors['A'] = temp; //Ants
	m_Actors['E'] = temp;//Pheromone

	for (int a = 0; a < 64; a++)
		for (int b = 0; b < 64; b++) {
			Field::FieldItem i = f.getContentsOf(a, b);
			switch (i) {
			default: continue;
			break; case Field::FieldItem::rock:
				addActor(new Pebble(a, b, this), 'P'); m_map[a][b] = 'P';//Pebble
			break; case Field::FieldItem::grasshopper:
				addActor(new Baby_Grasshopper(a, b, this), 'B');//Bady_Grasshopper
			break;case Field::FieldItem::food:
				addActor(new Food(a, b, this), 'F'); m_foodmap[a][b] = 'F';//Food
			break; case Field::FieldItem::water:
				addActor(new Water(a, b, this), 'W'); m_map[a][b] = 'W';//Water
			break; case Field::FieldItem::poison:
				addActor(new Poison(a, b, this), 'P'); m_map[a][b] = 'O';//Poison
			break; case Field::FieldItem::anthill0: {//anthill0
				Compiler*c=new Compiler;
				vector<string> FileName = getFilenamesOfAntPrograms();
				string error;
				if (c->compile(FileName[0], error))  addActor(new Anthill(a, b, this, c, 0), 'H'); 
				else addActor(new Anthill(a, b,nullptr, c, 0), 'H');
				nameOColony[0] = c->getColonyName(); noAnt[0] = 0; m_map[a][b] = '0';
			break; }case Field::FieldItem::anthill1: {//anthill1
				Compiler*c = new Compiler;
				vector<string> FileName = getFilenamesOfAntPrograms();
				string error;
				if (c->compile(FileName[1], error)) addActor(new Anthill(a, b, this, c, 1), 'H'); 
				else  addActor(new Anthill(a, b,nullptr, c, 1), 'H');
				noAnt[1] = 0; nameOColony[1] = c->getColonyName(); m_map[a][b] = '1'; 
			break; }case Field::FieldItem::anthill2: {//anthill2
				Compiler*c = new Compiler;
				vector<string> FileName = getFilenamesOfAntPrograms();
				string error;
				if (c->compile(FileName[2], error)) addActor(new Anthill(a, b, this, c, 2), 'H'); 
				else addActor(new Anthill(a, b, nullptr, c, 2), 'H');
				noAnt[2] = 0; nameOColony[2] = c->getColonyName(); m_map[a][b] = '2';
			break; }case Field::FieldItem::anthill3: {//anthill3
					Compiler*c = new Compiler;
					vector<string> FileName = getFilenamesOfAntPrograms();
					string error;
					if (c->compile(FileName[3], error))addActor(new Anthill(a, b, this, c, 3), 'H');
					else addActor(new Anthill(a, b, nullptr, c, 3), 'H');
					noAnt[3] = 0; nameOColony[3] = c->getColonyName(); m_map[a][b] = '3';
			break; }
			}
		}
	return GWSTATUS_CONTINUE_GAME;
}



int StudentWorld::move() {
	ostringstream oss;
	oss << "Ticks: " << m_time << " - ";
	for (int a = 0; noAnt[a] != -100 && a < 4; a++)
		oss << nameOColony[a] << ": " << noAnt[a] << " ants ";
	string s = oss.str();
	setGameStatText(s);
	
	if (!DoDo('H')) { printf("Error\n"); return GWSTATUS_NO_WINNER; }//antHill
	if (!DoDo('A')) { printf("Error\n"); return GWSTATUS_NO_WINNER; }//Ants
	if (!DoDo('D')) { printf("Error\n"); return GWSTATUS_NO_WINNER; }//Adult
	if (!DoDo('B')) { printf("Error\n"); return GWSTATUS_NO_WINNER; };//Baby
	
	
	if (!DoDo('O')) { printf("Error\n"); return GWSTATUS_NO_WINNER; };//Poison
	if (!DoDo('W')) { printf("Error\n"); return GWSTATUS_NO_WINNER; }//Water
	
	if(!DoDo('E')) { printf("Error\n"); return GWSTATUS_NO_WINNER; }//Pheromone
	del();
	m_time--;
	if(m_time>0)return GWSTATUS_CONTINUE_GAME;
	if (m_time == 0) {
		int win=0;
		for (int a = 0; a < 4; a++)
			if (noAnt[a] > win)win = a;
		setWinner(nameOColony[win]);
		return GWSTATUS_PLAYER_WON;
	}
	else {return GWSTATUS_NO_WINNER;}
}
void StudentWorld::cleanUp() {
	nmap::iterator mit = m_Actors.begin();
	while (!m_Actors.empty()) {
		while (!mit->second.empty()) {
			delete mit->second.back();
			mit->second.pop_back();
		}
		mit = m_Actors.erase(mit);
	}
}
bool StudentWorld::addActor(Actor*p, char type) {
	nmap::iterator it = m_Actors.find(type);
	if (it == m_Actors.end())return false;
	if (type == 'F') {
		nlist temp = it->second;
		for (nlist::iterator lit = temp.begin(); lit != temp.end(); lit++)
			if ((*lit)->getX() == p->getX() && (*lit)->getY() == p->getY()) {
				(*lit)->setHP((*lit)->getHP() + p->getHP());
				delete p;return true;
			}
	}
	it->second.push_back(p);
	if (type == 'F')m_foodmap[p->getX()][p->getY()] = 'F';
	return true;
}

nlist StudentWorld::findObj(char type) {
	nmap::iterator mit = m_Actors.find(type);
	return mit->second;
}

bool StudentWorld::DoDo(char type) {
	nlist temp; nlist::iterator lit; nmap::iterator mit;
	mit = m_Actors.find(type);
	if (mit != m_Actors.end()) {
		nlist temp = mit->second;
		for (lit = temp.begin(); lit != temp.end(); lit++)
			if(!(*lit)->isDead())(*lit)->doSomething();
		return true;
	}
	return false;
}

char StudentWorld::getmap( int x, int y){
	return m_map[x][y];
}

char StudentWorld::getfmap(int x, int y) {
	return m_foodmap[x][y];
}

void StudentWorld::setfmap(int x, int y) {
	if (x >= 64 || y >= 64 || x < 0 || y <= 0)return;
	m_foodmap[x][y] = 0;
}

void StudentWorld::del() {
	nmap::iterator mit;
	for (mit = m_Actors.begin(); mit != m_Actors.end(); mit++) {
		nlist temp = mit->second;
		for (nlist::iterator lit = temp.begin(); lit != temp.end();) {
			if ((*lit)->isDead()) {
				if (mit->first == 'A') {
					int c=(*lit)->getID();
					noAnt[c]--;
				}
				delete (*lit);
				lit=temp.erase(lit);
				continue;
			}
			else lit++;
		}
		mit->second = temp;
	}
}