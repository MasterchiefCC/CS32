#include "Actor.h"
#include "StudentWorld.h"
#include "GraphObject.h"
#include "Compiler.h"
#include <cmath>
/////////////////////////Auxiliary Function///////////////////////////////////
int ran(int a, int b) {
	return a + rand() % (b - a + 1);
}//rand a number for a to b


void trapped(Actor*p, char type) {//trapped depended on type used by poison or water pool
	int x = p->getX(), y = p->getY();

	nlist temp = p->getworld()->findObj('A');//for ants
	nlist::iterator lit;
	for (lit = temp.begin(); lit != temp.end(); lit++)setTrapped(p, lit, type);

	temp = p->getworld()->findObj('B');//for baby grasshopper
	for (lit = temp.begin(); lit != temp.end(); lit++) setTrapped(p, lit, type);


}

void setTrapped(Actor*p, nlist::iterator lit, char type) {
	if ((*lit)->gettrapped() != true && (*lit)->getX() == p->getX() && (*lit)->getY() == p->getY()) {
		switch (type) {
		default:break;
			break; case 'O'://poison a insect
		if ((*lit)->setHP((*lit)->getHP() - 150) <= 0) { (*lit)->addFood(100); return; }
			break; case 'W'://trapped a insect
		(*lit)->setSleep((*lit)->getSleep() + 2);
		(*lit)->setTrapped(true);
			break;
		}
	}
}
/////////////////////////Auxiliary Function/////////////////////////////
////////////////////////Actor  impentation/////////////////////////////
int Actor::setHP(int value) {//set value for hit point
	m_HP = value;
	return m_HP;
}
StudentWorld* Actor::getworld() {//get student world
	return m_world;
}
int Actor::getHP() const { return m_HP; }//getHP
int Actor::getSleep() const { return m_sleep; }//get remaining ticks to sleep
int Actor::setSleep(int sleep) {
	m_sleep = sleep;
	return sleep;
}
bool Actor::gettrapped() {//return whether or not it got trapped
	return m_trapped;
}
void Actor::setTrapped(bool v) {//trapped by a water bool
	m_trapped = v;
}

int Actor::ranDirection() {//rand a direction
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

bool Actor::eat(int value) {//eat for all insect and anthill
	if (this->getworld()->getfmap(getX(), getY()) == 'F') {
		nlist tfood = getworld()->findObj('F');
		for (nlist::iterator lit = tfood.begin(); lit != tfood.end(); lit++)
			if ((*lit)->getX() == this->getX() && (*lit)->getY() == this->getY()) {
				if ((*lit)->getHP() > value) {
					this->setHP(this->getHP() + value); (*lit)->setHP((*lit)->getHP() - value);
				}
				else {
					this->setHP(this->getHP() + (*lit)->getHP());
					delete (*lit);
					lit = tfood.erase(lit);
					this->getworld()->setfmap(getX(), getY());
				}
				getworld()->setFood(tfood);
				return 1;
			}
	}
	return 0;
}

void Actor::addFood(int value){//add food to map depended on value
		this->getworld()->addActor(new Food(getX(), getY(), getworld(), value), 'F');
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
	if (setHP(getHP() - 1)<=0)//decreases its (queen¡¯s) hit points by 1 unit.
		return;//reach 0 The anthill must immediately return

	if (eat(10000))return;//eat 100000 if eat return

	while (getHP() >= 2000) {//produce a new ant deepens on colony
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

////////////////////////Anthill  impentation/////////////////////////////
////////////////////////Baby_Grasshopper  impentation/////////////////////////////
void Baby_Grasshopper::doSomething() {
	if (setHP(getHP() - 1)<=0) {//lost hit point
		addFood(100); //if dead to food
		return;}//return

	if (getSleep()) { setSleep(getSleep() - 1); return; }//check sleep
	if (getHP() >= 1600) {
		this->getworld()->addActor(new Adult_Grasshopper(getX(), getY(), getworld()), 'D');
		setHP(0);
		this->getworld()->addActor(new Food(getX(), getY(), getworld(), 100), 'F');
		return;
	}//to Adult_Grasshopper
	int c = rand() % 2;
	if (eat(200) && c) {//if C is 1 and successfully eat 200 unit
		setSleep(getSleep() + 2);//set it to sleep 2 
		return;
	}
	move();//if none of those happen move
}

//////////////////move()

void Baby_Grasshopper::move() {
	setTrapped(false);
	setTrapped(false);//set Trapped status to none
	if (m_distance <= 0) {//if don't have desire distance
		m_distance = ran(2, 10);//rand a distance
		ranDirection();//random a direction
	}
	GraphObject::Direction dir = this->getDirection();//get direction
	
		switch (dir) {
		default: {
			return; }case GraphObject::Direction::right: {//X+1
			if (getworld()->getmap(getX() + 1, getY()) == 'P') { setSleep(getSleep() + 2); m_distance = 0; return; }//check rock
			moveTo(getX() + 1, getY());
			break; } case GraphObject::Direction::left: {//X-1
				if (getworld()->getmap(getX() - 1, getY()) == 'P') { setSleep(getSleep() + 2); m_distance = 0; return; }//check rock
				moveTo(getX() - 1, getY()); 
			break; }case GraphObject::Direction::up: {//Y+1
				if (getworld()->getmap(getX() , getY()+1) == 'P') { setSleep(getSleep() + 2); m_distance = 0; return; }//check rock
				moveTo(getX(), getY() + 1);
			break;}case GraphObject::Direction::down:{//Y-1
				if (getworld()->getmap(getX(), getY() - 1)=='P'){ setSleep(getSleep() + 2); m_distance = 0; return; }//check rock
				moveTo(getX(), getY() - 1);	
		break; }
		}
		m_distance--;//decrease distance
	}



////////////////////////Baby_Grasshopper  impentation/////////////////////////////
////////////////////////Adult_Grasshopper  impentation/////////////////////////////
void Adult_Grasshopper::doSomething() {
	if (setHP(getHP() - 1)<=0) { //lost 1 hit point
		addFood(100); 
		return; 
	}

	if (getSleep()) { setSleep(getSleep() - 1); return; }//check sleep

	if (bite())return;//bite

	if (jump())return;//jump

	int c = rand() % 2;
	if (eat(200) && c) {//chance to eat
		setSleep(getSleep() + 2);
		return;
	}
	move();//none of those happened sleep
}


//////////////////////bite

bool Adult_Grasshopper::bite() {
	vector<Actor*> biteA;//declare a vector to store insect
	nlist insect = getworld()->findObj('A');//push back ant
	for (nlist::iterator lit = insect.begin(); lit != insect.end(); lit++)
		if (!(*lit)->isDead() && (*lit) != this && (*lit)->getX() == this->getX() && (*lit)->getY() == this->getY())
			biteA.push_back(*lit);

	
	int antI = biteA.size();//store size of ant
	insect = getworld()->findObj('B');//push back baby
	for (nlist::iterator lit = insect.begin(); lit != insect.end(); lit++)
		if (!(*lit)->isDead() && (*lit) != this && (*lit)->getX() == this->getX() && (*lit)->getY() == this->getY())
			biteA.push_back(*lit);

	int ab = biteA.size();//store the poison of adult
	insect = getworld()->findObj('D');//push back adult
	for (nlist::iterator lit = insect.begin(); lit != insect.end(); lit++)
		if (!(*lit)->isDead() && (*lit) != this && (*lit)->getX() == this->getX() && (*lit)->getY() == this->getY())
			biteA.push_back(*lit);
	int chance = rand() % 3;//rand a chance to bite
	if (biteA.size() > 0 && !chance) {//1/3 and there is object on same target
		int index = rand() % biteA.size();//random a target to bite
		biteA[index]->setHP(biteA[index]->getHP() - 50);//set the target to be bite
		if (index >= ab) {
			int tc = rand() % 2;//if it is a adult there is 50% chance be bite back
			if (tc)setHP(getHP() - 50);
		}
		if (index < antI)biteA[index]->setbite();//if it is a ant, then set the ant been bite
		setSleep(2);//if it bite then rest
		return true;
	}
	return false;
}
//////////////////////jump
bool Adult_Grasshopper::jump() {
	setTrapped(false);
	int chance = rand() % 10;//chance to jump is very small
	if (!chance) {
		int dis, deg, moveX, moveY;
		while (1) {//rand a reasonable to jump
			dis = ran(1, 10);
			deg = ran(0, 360);
			moveY = (int)(dis*sin(deg));
			moveX = (int)(dis*cos(deg));
			if (moveY + getY() > 0 && moveY + getY() < 63 && moveX + getX() > 0 && moveX + getX() < 63
				&& getworld()->getmap(getX() + moveX, getY() + moveY) != 'P')break;
		}

		moveTo(moveX + getX(), moveY + getY());//move to that position
		setSleep(2);//set to sleep
		return 1;
	}
	return 0;
}
////////////////////////Adult_Grasshopper  impentation/////////////////////////////


////////////////////////Pheromone/////////////////////////////
void Pheromone::doSomething() {
	setHP(getHP() - 1);//get hit point lost
}
////////////////////////Pheromone/////////////////////////////


////////////////////////Ants/////////////////////////////

void ants::doSomething() {
	if (setHP(getHP() - 1)<=0) { //lost 1 hit point
		addFood(100); 
		return; 
	}
	if (getSleep()) { setSleep(getSleep() - 1); return; }//check sleep

	Compiler::Command cmd;
	if (m_coloney==nullptr) { setHP(0); return; }
	cou = 0;
	while (1) {
		if (!m_coloney->getCommand(ic, cmd))return;
		if (cou == 10)return;
		switch (cmd.opcode) {
		default:
			break; case Compiler::moveForward:
				move();
				ic++;
				cou++;
				return;
			break; case Compiler::eatFood:
				eat();
				ic++; 
				cou++;
				return;
			break; case Compiler::dropFood: 
				getworld()->addActor(new Food(getX(), getY(), getworld(), m_hfood), 'F'); 
				m_hfood = 0;
				ic++;
				cou++;
				return;
			break; case Compiler::bite: 
				bite();
				ic++;
				cou++;
				return;
			break; case Compiler::pickupFood:
				pickup();
				ic++;
				cou++;
				return;
			break; case Compiler::emitPheromone: 
				emit();
				ic++;
				cou++;
				return;
			break; case Compiler::generateRandomNumber: 
				ic++;
				m_randnum = rand()%(StringtoInt(cmd.operand1)+1);
				cou++;
			break; case Compiler::faceRandomDirection:
				ranDirection();
				ic++;
				cou++;
				return;
			break; case Compiler::goto_command: 
					ic = StringtoInt(cmd.operand1);
					cou++;
			break; case Compiler::if_command:
				cou++;
				if (readIF(cmd)) ic = StringtoInt(cmd.operand2);
				else ic++;
			break;	case Compiler::rotateClockwise:
				if (getDirection() == GraphObject::up)setDirection(GraphObject::right);
				if (getDirection() == GraphObject::right)setDirection(GraphObject::down);
				if (getDirection() == GraphObject::down)setDirection(GraphObject::left);
				if (getDirection() == GraphObject::left)setDirection(GraphObject::up);
				ic++;
				cou++;
				return;
				break; case Compiler::rotateCounterClockwise:
					if (getDirection() == GraphObject::up)setDirection(GraphObject::left);
					if (getDirection() == GraphObject::right)setDirection(GraphObject::up);
					if (getDirection() == GraphObject::down)setDirection(GraphObject::right);
					if (getDirection() == GraphObject::left)setDirection(GraphObject::down);
					ic++;
					cou++;
					return;
				break;
		}
	}
}

void ants::pickup() {
	int value = min(400, 1800 - m_hfood);
	if (this->getworld()->getfmap(getX(), getY()) == 'F') {
		nlist tfood = getworld()->findObj('F');
		for (nlist::iterator lit = tfood.begin(); lit != tfood.end(); lit++)
			if ((*lit)->getX() == this->getX() && (*lit)->getY() == this->getY()) {
				if ((*lit)->getHP() > value) {
					m_hfood+= value; (*lit)->setHP((*lit)->getHP() - value);
				}
				else {
					m_hfood += (*lit)->getHP();//add to m_hfood
					delete(*lit);
					tfood.erase(lit);
					this->getworld()->setFood(tfood);
					this->getworld()->setfmap(getX(), getY());
				}
				return;
			}
	}
}

bool ants::readIF(Compiler::Command cmd) {
	Compiler::Condition cond = covertoCondition(cmd.operand1);//convert it to condition
	if (cond == Compiler::Condition::invalid_if)return false;
	switch (cond) {
	default:
		return false;
	break; case Compiler::Condition::last_random_number_was_zero:
		cou++;
		if (m_randnum == 0)return true;
		else return false;
	break; case Compiler::Condition::i_am_carrying_food:
		cou++;
		if (m_hfood>0)return true;
		else return false;
	break; case Compiler::Condition::i_am_standing_with_an_enemy: {
		cou++;
		vector<Actor*> biteA;
		nlist insect = getworld()->findObj('A');//push back ant
		for (nlist::iterator lit = insect.begin(); lit != insect.end(); lit++)
			if (!(*lit)->isDead() &&(*lit)->getID()!=m_ID&& (*lit) != this && (*lit)->getX() == this->getX() && (*lit)->getY() == this->getY())
				biteA.push_back(*lit);

		insect = getworld()->findObj('B');//push back baby
		for (nlist::iterator lit = insect.begin(); lit != insect.end(); lit++)
			if (!(*lit)->isDead() && (*lit) != this && (*lit)->getX() == this->getX() && (*lit)->getY() == this->getY())
				biteA.push_back(*lit);

		insect = getworld()->findObj('D');//push back adult
		for (nlist::iterator lit = insect.begin(); lit != insect.end(); lit++)
			if (!(*lit)->isDead() && (*lit) != this && (*lit)->getX() == this->getX() && (*lit)->getY() == this->getY())
				biteA.push_back(*lit);
		if (biteA.size() > 0)return true;
		else return false;
	}break; case Compiler::Condition::i_am_standing_on_food:
		cou++;
		if (getworld()->getfmap(getX(), getY()) == 'F') { return true; }
		else return false;
		break; case Compiler::Condition::i_am_standing_on_my_anthill: {
			cou++;
			nlist temp = getworld()->findObj('H');
			for (nlist::iterator lit = temp.begin(); lit != temp.end(); lit++) {
				if (m_ID == (*lit)->getID() && getX() == (*lit)->getX() && getY() == (*lit)->getY())
					return true;
			}
	 return false;
	break; }case Compiler::Condition::i_smell_pheromone_in_front_of_me: {
		cou++;
		Direction dir = getDirection();
		nlist ee = getworld()->findObj('E');
		switch (dir) {
			default:return false;
			case GraphObject::Direction::left:
				for (nlist::iterator lit = ee.begin(); lit != ee.end(); lit++)
					{if (!(*lit)->isDead()&&(*lit)->getID() == m_ID&&getX() - 1 == (*lit)->getX() && getY() == (*lit)->getY())return true;}
				return false;
			break; case GraphObject::Direction::right:
				for (nlist::iterator lit = ee.begin(); lit != ee.end(); lit++)
					{if (!(*lit)->isDead() && (*lit)->getID() == m_ID&&getX() + 1 == (*lit)->getX() && getY() == (*lit)->getY())return true;}
				return false;
			break; case GraphObject::Direction::up:
				for (nlist::iterator lit = ee.begin(); lit != ee.end(); lit++)
					{if (!(*lit)->isDead() && (*lit)->getID() == m_ID&&getX()  == (*lit)->getX() && getY()+1 == (*lit)->getY())return true;}
				return false;
			break; case GraphObject::Direction::down:
				for (nlist::iterator lit = ee.begin(); lit != ee.end(); lit++)
					{if (!(*lit)->isDead() && (*lit)->getID() == m_ID&&getX() == (*lit)->getX() && getY() - 1 == (*lit)->getY())return true;}
				return false;
	}
		return false;
	}break; case Compiler::Condition::i_was_bit:
		cou++;
		return m_bite;
	break; case Compiler::Condition::i_was_blocked_from_moving: {
		cou++;
		Direction dir = getDirection();
		switch (dir) {
			case GraphObject::Direction::left:
		if (getworld()->getmap(getX() - 1, getY()) == 'P')return true;
		return false;
			break; case GraphObject::Direction::right:
		if (getworld()->getmap(getX() + 1, getY()) == 'P')return true;
		return false;
			break; case GraphObject::Direction::up:
		if (getworld()->getmap(getX(), getY()+1) == 'P')return true;
		return false;
			break; case GraphObject::Direction::down:
		if (getworld()->getmap(getX(), getY() - 1) == 'P')return true;
			return false;
		}
	}break;	case Compiler::Condition::i_am_hungry:
		cou++;
		if (getHP() <= 25)return true;
		else return false;
	}
	return false;
}


Compiler::Condition ants::covertoCondition(string ope) {//convert operator string

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

	//i_smell_danger_in_front_of_me,		// 0
	//i_smell_pheromone_in_front_of_me,	// 1
	//i_was_bit,							// 2
	//i_am_carrying_food,					// 3
		//i_am_hungry,						// 4
		//i_am_standing_on_my_anthill,		// 5
		//i_am_standing_on_food,				// 6
		//i_am_standing_with_an_enemy,		// 7
		//i_was_blocked_from_moving,			// 8
	//	last_random_number_was_zero			// 9
}

void ants::move(){
	Direction dir = getDirection();
	switch (dir) {
	default:
		break; case GraphObject::down://Y-1
			if (getworld()->getmap(getX(), getY() - 1) == 'P') { m_Smove = false; return; }//check rock if there is a rock set it to the successfully move to false
			moveTo(getX(), getY() - 1); m_Smove = true; m_bite = false;
		break; case GraphObject::left://X-1
			if (getworld()->getmap(getX() - 1, getY()) == 'P') { m_Smove =  false; return; }//check rock if there is a rock set it to the successfully move to false
			moveTo(getX() - 1, getY()); m_Smove = true; m_bite = false;
		break; case GraphObject::right://X+1
			if (getworld()->getmap(getX() + 1, getY()) == 'P') { m_Smove = false; return; }//check rock if there is a rock set it to the successfully move to false
			moveTo(getX() + 1, getY()); m_Smove = true; m_bite = false;
		break; case GraphObject::up://Y+1
			if (getworld()->getmap(getX(), getY() + 1) == 'P') { m_Smove  = false; return; }//check rock if there is a rock set it to the successfully move to false
			moveTo(getX(), getY() + 1); m_Smove = true; m_bite = false;
		break; 
	}
	setTrapped(false);//successfully move set it to untrapped 
}

/////////////////////////eat

void ants::eat() {
	if (m_hfood > 100) { setHP(getHP() + 100); m_hfood -= 100; }
	else { setHP(getHP() + m_hfood); m_hfood = 0; }
}

///////////////////////bite

void ants::bite() {
	vector<Actor*> bv;

	vector<Actor*> biteA;//push ant
	nlist insect = getworld()->findObj('A');//push ants
	for (nlist::iterator lit = insect.begin(); lit != insect.end(); lit++)
		if (!(*lit)->isDead() && (*lit) != this && (*lit)->getX() == this->getX() && (*lit)->getY() == this->getY())
			biteA.push_back(*lit);

	int antI = biteA.size();//store ant' s poison
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
		int tc = rand() % 2;//bite back by grasshopper
		if (tc)setHP(getHP() - 50);
	}
	if (i < antI)biteA[i]->setbite();
}

////////////////////////emit
void ants::emit() {
	nlist tp = getworld()->findObj('E');

	for (nlist::iterator lit; lit != tp.end(); lit++) 
		if ((*lit)->getX() == getX() && (*lit)->getY() == getY() && (*lit)->getID() == m_ID && (*lit)->getHP() < 700) {//if there is a pheromone and the the length of pheromone less than 700
			int e = min(256, 768 - (*lit)->getHP());
			(*lit)->setHP((*lit)->getHP() + e); 
			return;
		}
	
	switch (m_ID) {//if there don't have pheromone on this crood or is bigger than 700
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

int ants::StringtoInt(string temp) {//transfer a string value to int
		int v = 0;
		for (unsigned int a = 0; a < temp.size(); a++)
			v = v * 10 + temp[a] - '0';
		return v;
}

////////////////////////Ants/////////////////////////////