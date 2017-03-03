#include "Actor.h"
#include "StudentWorld.h"
#include "GraphObject.h"
#include "Compiler.h"
#include <cmath>
/////////////////////////Auxiliary Function///////////////////////////////////
int ran(int a, int b) {
	return a + rand() % (b - a + 1);
}


void trapped(Actor*p, char type) {
	int x = p->getX(), y = p->getY();

	nlist temp = p->getworld()->findObj('A');
	nlist::iterator lit;
	for (lit = temp.begin(); lit != temp.end(); lit++)setTrapped(p, lit, type);

	temp = p->getworld()->findObj('B');
	for (lit = temp.begin(); lit != temp.end(); lit++) setTrapped(p, lit, type);

	temp = p->getworld()->findObj('D');
	for (lit = temp.begin(); lit != temp.end(); lit++) setTrapped(p, lit, type);

}

void setTrapped(Actor*p, nlist::iterator lit, char type) {
	if ((*lit)->gettrapped() != p && (*lit)->getX() == p->getX() && (*lit)->getY() == p->getY()) {
		switch (type) {
		default:break;
			break; case 'O':
		(*lit)->setHP((*lit)->getHP() - 150);
			break; case 'W':
		(*lit)->setSleep((*lit)->getSleep() + 2);
		(*lit)->setTrapped(p);
			break;
		}
	}
}
/////////////////////////Auxiliary Function/////////////////////////////
////////////////////////Actor  impentation/////////////////////////////
int Actor::setHP(int value) {
	m_HP = value;
	return m_HP;
}
StudentWorld* Actor::getworld() {
	return m_world;
}
int Actor::getHP() const { return m_HP; }
int Actor::getSleep() const { return m_sleep; }
int Actor::setSleep(int sleep) {
	m_sleep = sleep;
	return sleep;
}
Actor* Actor::gettrapped() {
	return m_trap;
}
Actor* Actor::setTrapped(Actor*temp) {
	m_trap = temp;
	return m_trap;
}

int Actor::ranDirection() {
	int dir = ran(1, 4);
	switch (dir) {
	default: setDirection(GraphObject::Direction::right);
		break; case 1: setDirection(GraphObject::Direction::up);
		break; case 2: setDirection(GraphObject::Direction::right);
		break; case 3: setDirection(GraphObject::Direction::left);
		break; case 4:setDirection(GraphObject::Direction::down);
	}
	return dir;
}

bool Actor::eat(int value) {
	if (this->getworld()->getfmap(getX(), getY()) == 'F') {
		nlist tfood = getworld()->findObj('F');
		for (nlist::iterator lit = tfood.begin(); lit != tfood.end(); lit++)
			if ((*lit)->getX() == this->getX() && (*lit)->getY() == this->getY()) {
				if ((*lit)->getHP() > value) {
					this->setHP(this->getHP() + value); (*lit)->setHP((*lit)->getHP() - value);
				}
				else {
					this->setHP(this->getHP() + (*lit)->getHP()); delete *lit; tfood.erase(lit); this->getworld()->setfmap(getX(), getY());
				}
				return 1;
			}
	}
	return 0;
}
////////////////////////Actor  impentation/////////////////////////////
////////////////////////Pebble  impentation/////////////////////////////
void Pebble::doSomething() { return; }
////////////////////////Pebble  impentation/////////////////////////////
////////////////////////Food  impentation/////////////////////////////
void Food::doSomething() { return; }
////////////////////////Food  impentation/////////////////////////////
////////////////////////Water  impentation/////////////////////////////
void Water::doSomething() { 
	trapped(this, 'W');
}
////////////////////////Water  impentation/////////////////////////////
////////////////////////Water  impentation/////////////////////////////
void Poison::doSomething() {
	trapped(this, 'O');
}
////////////////////////Water  impentation/////////////////////////////
////////////////////////Anthill  impentation/////////////////////////////
void Anthill::doSomething() {
	if (!setHP(getHP() - 1))return;

	if (eat(10000))return;

	while (getHP() >= 2000) {
		setHP(getHP() - 1500);
		switch (antID) {
		default:
			break; case 0:
				getworld()->addActor(new ants(IID_ANT_TYPE0, getX(), getY(), getworld(), m_compiler,antID), 'A');
				getworld()->setnoAnt(antID, getworld()->getnoAnt(antID)+1);
				break; case 1:
				getworld()->addActor(new ants(IID_ANT_TYPE1, getX(), getY(), getworld(), m_compiler, antID), 'A');
				getworld()->setnoAnt(antID, getworld()->getnoAnt(antID) + 1);
				break; case 2:
				getworld()->addActor(new ants(IID_ANT_TYPE2, getX(), getY(), getworld(), m_compiler, antID), 'A');
				getworld()->setnoAnt(antID, getworld()->getnoAnt(antID) + 1);
				break; case 3:
				getworld()->addActor(new ants(IID_ANT_TYPE3, getX(), getY(), getworld(), m_compiler,  antID), 'A');
				getworld()->setnoAnt(antID, getworld()->getnoAnt(antID) + 1);
				break;
		}
	}
}
Anthill::~Anthill() {
	delete m_compiler;
}
////////////////////////Anthill  impentation/////////////////////////////
////////////////////////Baby_Grasshopper  impentation/////////////////////////////
void Baby_Grasshopper::doSomething() {
	if (!setHP(getHP() - 1)) {this->getworld()->addActor(new Food(getX(), getY(), getworld(), 100), 'F'); return;}

	if (getSleep()) { setSleep(getSleep() - 1); return; }//check sleep
	if (getHP() >= 1600) {
		this->getworld()->addActor(new Adult_Grasshopper(getX(), getY(), getworld()), 'D');
		setHP(0);
		this->getworld()->addActor(new Food(getX(), getY(), getworld(), 100), 'F');
		return;
	}//to Adult_Grasshopper
	int c = rand() % 2;
	if (eat(200) && c) {
		setSleep(getSleep() + 2);
		return;
	}
	move();
}

//////////////////move()

void Baby_Grasshopper::move() {
	setTrapped(nullptr);
	if (m_distance <= 0) {
		m_distance = ran(2, 8);
		ranDirection();
	}
	GraphObject::Direction dir = this->getDirection();
	while (m_distance--) {
		switch (dir) {
		default: {
			break; }case GraphObject::Direction::right: {//X+1
			if (getworld()->getmap(getX() + 1, getY()) == 'P') { setSleep(getSleep() + 2); m_distance = 0; return; }
			if (getworld()->getmap(getX() + 1, getY()) == 'O') {
				moveTo(getX() + 1, getY());
				if (setHP(getHP() - 100) <= 0)return;
			}
			if (getworld()->getmap(getX() + 1, getY()) == 'W') {
				moveTo(getX() + 1, getY()); return;
			}
			moveTo(getX() + 1, getY());
			break; } case GraphObject::Direction::left: {//X-1
				if (getworld()->getmap(getX() - 1, getY()) == 'P') { setSleep(getSleep() + 2); m_distance = 0; return; }
				if (getworld()->getmap(getX() - 1, getY()) == 'O') {
					moveTo(getX() - 1, getY());
					if (setHP(getHP() - 100) <= 0)return;
				}
				if (getworld()->getmap(getX() - 1, getY()) == 'W') {
					moveTo(getX() - 1, getY()); return;
				}
				moveTo(getX() - 1, getY()); 
			break; }case GraphObject::Direction::up: {
				if (getworld()->getmap(getX() , getY()-1) == 'P') { setSleep(getSleep() + 2); m_distance = 0; return; }
				if (getworld()->getmap(getX(), getY()-1) == 'O') {
					moveTo(getX(), getY()-1);
					if (setHP(getHP() - 100) <= 0)return;
				}
				if (getworld()->getmap(getX(), getY()-1) == 'W') {
					moveTo(getX(), getY()-1); return;
				}
				moveTo(getX(), getY() - 1);
			break;}case GraphObject::Direction::down:{//Y+1
				if (getworld()->getmap(getX(), getY() + 1)=='P'){ setSleep(getSleep() + 2); m_distance = 0; return; }
				if (getworld()->getmap(getX(), getY() + 1) == 'O') {
					moveTo(getX(), getY() + 1);
					if (setHP(getHP() + 100) <= 0)return;
				}
				if (getworld()->getmap(getX() , getY() + 1) == 'W') {
					moveTo(getX(), getY() + 1); return;
				}
				moveTo(getX(), getY() + 1);	
		break; }
		}
	}
}


////////////////////////Baby_Grasshopper  impentation/////////////////////////////
////////////////////////Adult_Grasshopper  impentation/////////////////////////////
void Adult_Grasshopper::doSomething() {
	if (!setHP(getHP() - 1)) { this->getworld()->addActor(new Food(getX(), getY(), getworld(), 100), 'F'); return; }

	if (getSleep()) { setSleep(getSleep() - 1); return; }//check sleep
	
	if (bite())return;

	if (jump())return;

	int c = rand() % 2;
	if (eat(200) && c) {
		setSleep(getSleep() + 2);
		return;
	}
	move();
}

//////////////////////bite

bool Adult_Grasshopper::bite() {
	vector<Actor*> biteA;
	nlist insect = getworld()->findObj('A');
	for (nlist::iterator lit = insect.begin(); lit != insect.end(); lit++)
		if (!(*lit)->isDead() && (*lit) != this && (*lit)->getX() == this->getX() && (*lit)->getY() == this->getY())
			biteA.push_back(*lit);

	int antI = biteA.size();
	insect = getworld()->findObj('B');
	for (nlist::iterator lit = insect.begin(); lit != insect.end(); lit++)
		if (!(*lit)->isDead() && (*lit) != this && (*lit)->getX() == this->getX() && (*lit)->getY() == this->getY())
			biteA.push_back(*lit);

	int ab = biteA.size();
	insect = getworld()->findObj('D');
	for (nlist::iterator lit = insect.begin(); lit != insect.end(); lit++)
		if (!(*lit)->isDead() && (*lit) != this && (*lit)->getX() == this->getX() && (*lit)->getY() == this->getY())
			biteA.push_back(*lit);
	int chance = rand() % 3;
	if (biteA.size() > 0 && !chance) {
		int index = rand() % biteA.size();
		biteA[index]->setHP(biteA[index]->getHP() - 50);
		if (index >= ab) {
			int tc = rand() % 2;
			if (tc)setHP(getHP() - 50);
		}
		if (index < antI)biteA[index]->setbite();
		setSleep(2);
		return true;
	}
	return false;
}
//////////////////////jump
bool Adult_Grasshopper::jump() {
	setTrapped(nullptr);
	int chance = rand() % 10;
	if (!chance) {
		int dis, deg, moveX, moveY;
		while (1) {
			dis = ran(1, 10);
			deg = ran(0, 360);
			moveY = (int)(dis*sin(deg));
			moveX = (int)(dis*cos(deg));
			if (moveY + getY() > 0 && moveY + getY() < 63 && moveX + getX() > 0 && moveX + getX() < 63
				&& getworld()->getmap(getX() + moveX, getY() + moveY) != 'P')break;
		}

		moveTo(moveX + getX(), moveY + getY());
		setSleep(2);
		return 1;
	}
	return 0;
}
////////////////////////Adult_Grasshopper  impentation/////////////////////////////


////////////////////////Pheromone/////////////////////////////
void Pheromone::doSomething() {
	setHP(getHP() - 1);
}
////////////////////////Pheromone/////////////////////////////


////////////////////////Ants/////////////////////////////

void ants::doSomething() {
	if (!setHP(getHP() - 1)) { this->getworld()->addActor(new Food(getX(), getY(), getworld(), 100), 'F'); return; }
	if (getSleep()) { setSleep(getSleep() - 1); return; }//check sleep
	Compiler::Command cmd;
	int ic = 0;
	if (m_coloney==nullptr) { setHP(0); return; }
	while (1) {
		if (!m_coloney->getCommand(ic, cmd))return;
		switch (cmd.opcode) {
		default:
			break; case Compiler::moveForward:
				move();
				ic++;
			break; case Compiler::eatFood:
				Eeat();
				ic++; 
			break; case Compiler::dropFood: 
				getworld()->addActor(new Food(getX(), getY(), getworld(), m_hfood), 'F'); 
				ic++;
			break; case Compiler::bite: 
				bite();
				ic++;
			break; case Compiler::pickupFood:
				eat(min(400, 1800 - m_hfood));
				ic++;
			break; case Compiler::emitPheromone: 
				emit();
				ic++;
			break; case Compiler::generateRandomNumber: 
				ic++;
				m_randnum = rand()%(StringtoInt(cmd.operand1)+1);
			break; case Compiler::faceRandomDirection:
				ranDirection();
				ic++;
			break; case Compiler::goto_command: 
					ic = StringtoInt(cmd.operand1);
			break; case Compiler::if_command:
						if (readIF(cmd)) ic = StringtoInt(cmd.operand2);
						else ic++;
			break;		
		}
		if (ic == 0)break;
	}
}

bool ants::readIF(Compiler::Command cmd) {
	Compiler::Condition cond = covertoCondition(cmd.operand1);
	if (cond == Compiler::Condition::invalid_if)return false;
	switch (cond) {
	default:
		return false;
	break; case Compiler::Condition::last_random_number_was_zero:
		if (m_randnum == 0)return true;
		else return false;
	break; case Compiler::Condition::i_am_carrying_food:
		if (m_hfood>0)return true;
		else return false;
	break; case Compiler::Condition::i_am_standing_with_an_enemy: {
		vector<Actor*> biteA;
		nlist insect = getworld()->findObj('A');
		for (nlist::iterator lit = insect.begin(); lit != insect.end(); lit++)
			if (!(*lit)->isDead() &&(*lit)->getID()!=m_ID&& (*lit) != this && (*lit)->getX() == this->getX() && (*lit)->getY() == this->getY())
				biteA.push_back(*lit);

		insect = getworld()->findObj('B');
		for (nlist::iterator lit = insect.begin(); lit != insect.end(); lit++)
			if (!(*lit)->isDead() && (*lit) != this && (*lit)->getX() == this->getX() && (*lit)->getY() == this->getY())
				biteA.push_back(*lit);

		int ab = biteA.size();
		insect = getworld()->findObj('D');
		for (nlist::iterator lit = insect.begin(); lit != insect.end(); lit++)
			if (!(*lit)->isDead() && (*lit) != this && (*lit)->getX() == this->getX() && (*lit)->getY() == this->getY())
				biteA.push_back(*lit);
		if (biteA.size() > 0)return true;
		else return false;
	}break; case Compiler::Condition::i_am_standing_on_food:
		if (getworld()->getfmap(getX(), getY()) == 'F') { return true; }
		else return false;
	break; case Compiler::Condition::i_am_standing_on_my_anthill:
		if (getworld()->getmap(getX(), getY()) - '0' == m_ID) {
			cout << "1";
			return true; 
		}
		else return false;
	break; case Compiler::Condition::i_smell_pheromone_in_front_of_me: {
		Direction dir = getDirection();
		nlist ee = getworld()->findObj('E');
		switch (dir) {
			default:return false;
			case GraphObject::Direction::left:
				for (nlist::iterator lit = ee.begin(); lit != ee.end(); lit++)
					{if ((*lit)->getID() == m_ID&&getX() - 1 == (*lit)->getX() && getY() == (*lit)->getY())return true;}
				return false;
			break; case GraphObject::Direction::right:
				for (nlist::iterator lit = ee.begin(); lit != ee.end(); lit++)
					{if ((*lit)->getID() == m_ID&&getX() + 1 == (*lit)->getX() && getY() == (*lit)->getY())return true;}
				return false;
			break; case GraphObject::Direction::up:
				for (nlist::iterator lit = ee.begin(); lit != ee.end(); lit++)
					{if ((*lit)->getID() == m_ID&&getX()  == (*lit)->getX() && getY()-1 == (*lit)->getY())return true;}
				return false;
			break; case GraphObject::Direction::down:
				for (nlist::iterator lit = ee.begin(); lit != ee.end(); lit++)
					{if ((*lit)->getID() == m_ID&&getX() == (*lit)->getX() && getY() + 1 == (*lit)->getY())return true;}
				return false;
	}
		return false;
	}break; case Compiler::Condition::i_was_bit:
			return m_bite;
	break; case Compiler::Condition::i_was_blocked_from_moving: {
			Direction dir = getDirection();
								switch (dir) {
								case GraphObject::Direction::left:
									if (getworld()->getmap(getX() - 1, getY()) == 'P')return true;
								return false;
									break; case GraphObject::Direction::right:
									if (getworld()->getmap(getX() + 1, getY()) == 'P')return true;
								return false;
										break; case GraphObject::Direction::up:
									if (getworld()->getmap(getX(), getY()-1) == 'P')return true;
								return false;
										break; case GraphObject::Direction::down:
									if (getworld()->getmap(getX(), getY() + 1) == 'P')return true;
								return false;
								}
							}break;			
	}
	return false;
}
Compiler::Condition ants::covertoCondition(string ope) {

	if(ope=="0")return Compiler::Condition::i_smell_danger_in_front_of_me;
	if (ope == "1")return Compiler::Condition::i_smell_pheromone_in_front_of_me;
	if (ope == "2")return Compiler::Condition::i_was_bit;
	if (ope == "3")return Compiler::Condition::i_am_carrying_food;
	if (ope == "4")return Compiler::Condition::i_am_hungry;
	if (ope == "5")return Compiler::Condition::i_am_standing_on_my_anthill;
	if (ope == "6")return Compiler::Condition::i_am_standing_on_food;
	if (ope == "7")return Compiler::Condition::i_am_standing_with_an_enemy;
	if (ope == "8")return Compiler::Condition::i_was_blocked_from_moving;
	if (ope == "9")return Compiler::Condition::last_random_number_was_zero;
	return Compiler::Condition::invalid_if;


}

void ants::move(){
	Direction dir = getDirection();
	switch (dir) {
	default:
		break; case GraphObject::down:
			if (getworld()->getmap(getX(), getY() + 1) == 'P') { m_Smove = false; return; }
			moveTo(getX(), getY() + 1); m_Smove = true; m_bite = false;
		break; case GraphObject::left:
			if (getworld()->getmap(getX() - 1, getY()) == 'P') { m_Smove =  false; return; }
			moveTo(getX() - 1, getY()); m_Smove = true; m_bite = false;
		break; case GraphObject::right:
			if (getworld()->getmap(getX() + 1, getY()) == 'P') { m_Smove = m_bite = false; return; }
			moveTo(getX() + 1, getY()); m_Smove = true; m_bite = false;
		break; case GraphObject::up:
			if (getworld()->getmap(getX(), getY() - 1) == 'P') { m_Smove = m_bite = false; return; }
			moveTo(getX(), getY() - 1); m_Smove = true; m_bite = false;
		break; 
	}
}

/////////////////////////eat

void ants::Eeat() {
	if (m_hfood > 100) { setHP(getHP() + 100); m_hfood -= 100; }
	else { setHP(getHP() + m_hfood); m_hfood = 0; }
}

///////////////////////bite

void ants::bite() {
	vector<Actor*> bv;

	vector<Actor*> biteA;
	nlist insect = getworld()->findObj('A');//push ants
	for (nlist::iterator lit = insect.begin(); lit != insect.end(); lit++)
		if (!(*lit)->isDead() && (*lit) != this && (*lit)->getX() == this->getX() && (*lit)->getY() == this->getY())
			biteA.push_back(*lit);

	insect = getworld()->findObj('B');//push baby
	for (nlist::iterator lit = insect.begin(); lit != insect.end(); lit++)
		if (!(*lit)->isDead() && (*lit) != this && (*lit)->getX() == this->getX() && (*lit)->getY() == this->getY())
			biteA.push_back(*lit);

	int ab = biteA.size();
	insect = getworld()->findObj('D');//push adult
	for (nlist::iterator lit = insect.begin(); lit != insect.end(); lit++)
		if (!(*lit)->isDead() && (*lit) != this && (*lit)->getX() == this->getX() && (*lit)->getY() == this->getY())
			biteA.push_back(*lit);
	int i = rand() % biteA.size();
	biteA[i]->setHP(biteA[i]->getHP() - 15);
	if (i >= ab) {
		int tc = rand() % 2;
		if (tc)setHP(getHP() - 50);
	}
}

////////////////////////emit
void ants::emit() {
	nlist tp = getworld()->findObj('E');

	for (nlist::iterator lit; lit != tp.end(); lit++) 
		if ((*lit)->getX() == getX() && (*lit)->getY() == getY() && (*lit)->getID() == m_ID && (*lit)->getHP() < 700) {
			int e = min(256, 768 - (*lit)->getHP());
			(*lit)->setHP((*lit)->getHP() + e); 
			return;
		}
	
	switch (m_ID) {
		default:
	break; case 0:
		getworld()->addActor(new Pheromone(IID_PHEROMONE_TYPE0, getX(), getY(), getworld(), m_ID), 'E');
	break; case 1:
		getworld()->addActor(new Pheromone(IID_PHEROMONE_TYPE1, getX(), getY(), getworld(), m_ID), 'E');
	break; case 2:
		getworld()->addActor(new Pheromone(IID_PHEROMONE_TYPE2, getX(), getY(), getworld(), m_ID), 'E');
	break; case 3:
		getworld()->addActor(new Pheromone(IID_PHEROMONE_TYPE3, getX(), getY(), getworld(), m_ID), 'E');
	break;
	}
}

int ants::StringtoInt(string temp) {
		int v = 0;
		for (unsigned int a = 0; a < temp.size(); a++)
			v = v * 10 + temp[a] - '0';
		return v;
}

////////////////////////Ants/////////////////////////////