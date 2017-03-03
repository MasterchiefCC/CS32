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
	//put a linked list to every first value
	m_Actors['P'] = temp;//Pebble 
	m_Actors['F'] = temp;//Food

	m_Actors['B'] = temp;//Baby Grasshopper
	m_Actors['D'] = temp; //Adult Grasshopper

	m_Actors['W'] = temp;//Water
	m_Actors['O'] = temp;//Poison
	
	m_Actors['H'] = temp;//antHill
	m_Actors['A'] = temp; //Ants
	m_Actors['E'] = temp;//Pheromone
	//add actor into file
	for (int a = 0; a < 64; a++)
		for (int b = 0; b < 64; b++) {
			Field::FieldItem i = f.getContentsOf(a, b);
			switch (i) {
			default: continue;
			break; case Field::FieldItem::rock://add rock
				addActor(new Pebble(a, b, this), 'P'); m_map[a][b] = 'P';//Pebble
			break; case Field::FieldItem::grasshopper:
				addActor(new Baby_Grasshopper(a, b, this), 'B');//Bady_Grasshopper
			break;case Field::FieldItem::food://add food
				addActor(new Food(a, b, this), 'F'); m_foodmap[a][b] = 'F';//Food
			break; case Field::FieldItem::water:
				addActor(new Water(a, b, this), 'W'); m_map[a][b] = 'W';//Water
			break; case Field::FieldItem::poison:
				addActor(new Poison(a, b, this), 'O'); m_map[a][b] = 'O';//Poison
			break; case Field::FieldItem::anthill0: {//anthill0
				Compiler*c=new Compiler;
				vector<string> FileName = getFilenamesOfAntPrograms();
				string error;
				if (c->compile(FileName[0], error)) {
					addActor(new Anthill(a, b, this, c, 0), 'H'); m_C[0] = c;
				}
				else { 
					addActor(new Anthill(a, b, this, nullptr, 0), 'H'); m_C[0] = nullptr;
				}
				
				nameOColony[0] = c->getColonyName(); noAnt[0] = 0; m_map[a][b] = 0;
			break; }case Field::FieldItem::anthill1: {//anthill1
				Compiler*c = new Compiler;
				vector<string> FileName = getFilenamesOfAntPrograms();
				string error;
				if (c->compile(FileName[1], error)) {
					addActor(new Anthill(a, b, this, c, 1), 'H'); m_C[1] = c;
				}
				else {
					addActor(new Anthill(a, b, this, nullptr, 1), 'H'); m_C[1] = nullptr;
				}
				noAnt[1] = 0; nameOColony[1] = c->getColonyName(); m_map[a][b] = 1; 
			break; }case Field::FieldItem::anthill2: {//anthill2
				Compiler*c = new Compiler;
				vector<string> FileName = getFilenamesOfAntPrograms();
				string error;
				if (c->compile(FileName[2], error)) {
					addActor(new Anthill(a, b, this, c, 2), 'H'); m_C[2] = c;
				}
				else {
					addActor(new Anthill(a, b, this, nullptr, 2), 'H'); m_C[2] = nullptr;
				}
				noAnt[2] = 0; nameOColony[2] = c->getColonyName(); m_map[a][b] = 2;
				break; }case Field::FieldItem::anthill3: {//anthill3
				Compiler*c = new Compiler;
				vector<string> FileName = getFilenamesOfAntPrograms();
				string error;
				if (c->compile(FileName[3], error)) {
					addActor(new Anthill(a, b, this, c, 3), 'H'); m_C[3] = c;
				}
				else {
					addActor(new Anthill(a, b, this, nullptr, 3), 'H'); m_C[3] = nullptr;
			}
					noAnt[3] = 0; nameOColony[3] = c->getColonyName(); m_map[a][b] = 3;
			break; }
			}
		}
	return GWSTATUS_CONTINUE_GAME;
}



int StudentWorld::move() {
	ostringstream oss;
	oss << "Ticks: " << m_time << " - ";
	for (int a = 0; noAnt[a] != -100 && a < 4; a++)
		oss << nameOColony[a] << ": " << noAnt[a] << " ants ";//create a string to set ticks
	string s = oss.str();
	setGameStatText(s);
	
	if (!DoDo('H')) { printf("Error\n"); return GWSTATUS_NO_WINNER; }//antHill
	if (!DoDo('A')) { printf("Error\n"); return GWSTATUS_NO_WINNER; }//Ants
	if (!DoDo('D')) { printf("Error\n"); return GWSTATUS_NO_WINNER; }//Adult
	if (!DoDo('B')) { printf("Error\n"); return GWSTATUS_NO_WINNER; };//Baby
	
	
	if (!DoDo('O')) { printf("Error\n"); return GWSTATUS_NO_WINNER; };//Poison
	if (!DoDo('W')) { printf("Error\n"); return GWSTATUS_NO_WINNER; }//Water
	
	if(!DoDo('E')) { printf("Error\n"); return GWSTATUS_NO_WINNER; }//Pheromone
	del();//delete all dead value
	m_time--;

	int count = 0;//when it need to stop
	for (int a = 0; a < 4; a++) 
		if (noAnt[a] == 0 && (!hill2000(a))) {
			dead[a] = true; count++;
		}

	if (count == 3) {
		for(int a=0; a<4; a++)
			if (dead[a] == false) {
				setWinner(nameOColony[a]);
				return GWSTATUS_PLAYER_WON;
			}
	}

	if(m_time>0)return GWSTATUS_CONTINUE_GAME;//continue game
	
	if (m_time == 0) {//time is up
		int win=0;
		for (int a = 0; a < 4; a++)
			if (noAnt[a] > win)win = a;
		setWinner(nameOColony[win]);
		return GWSTATUS_PLAYER_WON;
	}
	else {return GWSTATUS_NO_WINNER;}
}
void StudentWorld::cleanUp() {//Traversal the data struct to delete obj
	nmap::iterator mit = m_Actors.begin();
	while (!m_Actors.empty()) {
		while (!mit->second.empty()) {
			delete mit->second.back();
			mit->second.pop_back();
		}
		mit = m_Actors.erase(mit);
	}
	for (int a = 0; a < 4; a++)
		if (m_C[a] != nullptr)delete m_C[a];
}
bool StudentWorld::addActor(Actor*p, char type) {//add actor
	nmap::iterator it = m_Actors.find(type);//find type
	if (it == m_Actors.end()) {
		return false; }
	if (type == 'F') {//if it is a food check whether the crood contain a Food class
		nlist temp = it->second;
		for (nlist::iterator lit = temp.begin(); lit != temp.end(); lit++)
			if ((*lit)->getX() == p->getX() && (*lit)->getY() == p->getY()) {
				(*lit)->setHP((*lit)->getHP() + p->getHP());
				delete p;
				return 1;
			}
	}
	it->second.push_back(p);//push back obj
	if (type == 'F')m_foodmap[p->getX()][p->getY()] = 'F';//if new a food in new obj then set the map to 'F'
	return true;
}
bool StudentWorld::hill2000(int index) {//to check whether or not the hill has 2000 hit points
	nlist temp = findObj('H');
	for (nlist::iterator lit = temp.begin(); lit != temp.end(); lit++)
		if ((*lit)->getID() == index && (*lit)->getHP() >=2000)return true;
	return false;
}

nlist StudentWorld::findObj(char type) {//find a obj's linked list
	nmap::iterator mit = m_Actors.find(type);
	
	return mit->second;
}

bool StudentWorld::DoDo(char type) {//Traversal to do something for a obj
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

char StudentWorld::getmap( int x, int y){// get map for pebble and water pool and poison
	return m_map[x][y];
}

char StudentWorld::getfmap(int x, int y) {//get a food map
	return m_foodmap[x][y];
}

void StudentWorld::setfmap(int x, int y) {//set food map
	
	m_foodmap[x][y] = -1;
}

void StudentWorld::del() {//Traversal to delete dead obj
	nmap::iterator mit;
	for (mit = m_Actors.begin(); mit != m_Actors.end(); mit++) {
		for (nlist::iterator lit = mit->second.begin(); lit != mit->second.end();) {
			if ((*lit)->isDead()) {
				if (mit->first == 'A') {
					int c=(*lit)->getID();
					noAnt[c]--;
				}
				if (mit->first == 'H')H_dead[(*lit)->getID()] = true;
				delete (*lit);
				mit->second.erase(lit);
				lit = mit->second.begin();
				continue;
			}
			else lit++;
		}
	}
}

void StudentWorld::setFood(nlist temp) {//reset food 's linked list
	m_Actors['F'] = temp;
}