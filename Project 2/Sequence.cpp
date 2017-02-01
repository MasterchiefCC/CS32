#include "Sequence.h"

Sequence::Sequence() {// Create an empty sequence (i.e., one whose size() is 0).
	head = new Node;//Create a node store the head 
	head->m_next = head;//point the head pointer's next and previous to head
	head->m_previous = head;
	m_size = 0;//set the size to 0
}

Sequence::~Sequence() {
	Node*pre = head;//pre pointer point to head
	Node*p = head->m_next;//pos pointer point to the first obj
	while (1) {
		delete pre;//delete the obj in pre pointer
		if (p == head)break;//condition when loop stop
		pre = p;//move to pre to pre->next to delete next obj
		p = p->m_next;//move p to p->next to give direction to next obj
	}
}
/*Sequence::~Sequence() {
	Node*p = head->m_next;
	while (p != head) {
		Node*n = p->m_next;
		delete p;
		p = n;
	}
	delete head;
}*/

bool Sequence::empty()const {
	return !m_size;//return the reverse value of size
}

int Sequence::size() const {
	return m_size;//return the size
}

bool Sequence::insert(int pos, const ItemType&value) {
	if (pos<0 || pos>m_size)return false;//the condition when can't insert a value
	Node*p = head;//create pointer p point head
	for (int a = 0; a <= pos; a++)
		p = p->m_next;//loop to move to certain position
	Node*insert = new Node;//create a new Node
	insert->m_next = p;//link new node's next to original pos
	insert->m_previous = p->m_previous;//link new node's pre to p's pre
	p->m_previous->m_next = insert;//link p's pre 's next' s pointer to new node
	p->m_previous = insert;//link p's pre pointer to new node
	insert->m_value = value;//assign the value to new node
	m_size++;//increase the size
	return true;//successfully insert
}

int Sequence::insert(const ItemType& value) {
	Node*p = head;//initiate a pointer p point head
	int pos = 0;//counter for pointer
	while (1) {//loop to find previous position which should insert the value
		if (value<=p->m_next->m_value)break;
		if (p->m_next == head)break;
		p = p->m_next;
		pos++;
	}
	Node*insert = new Node;//create a new node
	insert->m_previous = p;//link the new node 's pre to p pointer
	insert->m_next = p->m_next;//link the new node 's next to p's next node
	insert->m_value = value;//assign the value
	p->m_next->m_previous = insert;//point the pre node of the next node of p to the new node
	p->m_next = insert;//point the p's next to the new node
	m_size++;//increase the size
	return pos;//return the position where insert
}

bool Sequence::erase(int pos) {
	if (pos < 0 || pos >= size())return false;//the condition when can't erase pos
	Node*p = head;//initiate a pointer point to head
	for (int a = 0; a <= pos; a++)//loop to find that position
		p = p->m_next;
	p->m_next->m_previous = p->m_previous;//link the pre node of the next node of position to the previous node of P
	p->m_previous->m_next = p->m_next;//link the next node of the previous node of position to the next node of P
	m_size--;//decrease the size
	delete p;//delete p
	return true;//successfully deleted
}

int Sequence::remove(const ItemType& value) {
	Node*p = head->m_next;//initiate a pointer to the first obj
	int removed = 0;//counter for removed obj
	while (p!=head) {//loop to find the value
		if (value == p->m_value) {
			p->m_previous->m_next = p->m_next;//link the pre node to the next node
			p->m_next->m_previous = p->m_previous;
			Node*temp = p;//initiate a temp pointer point to p
			p = p->m_next;//move to next obj
			removed++;//increase the size
			m_size--;//decrease the size
			delete temp;//delete the node has same value
			continue;//move to next step
		}
		p = p->m_next;//if the value is not same move to next obj
	}
	return removed;//return the number of removed obj
}

bool Sequence::get(int pos, ItemType& value) const {
	if (pos < 0 || pos >= size())return false;//the condition false
	Node*p = head;//initiate a pointer points to head
	for (int a = 0; a <= pos; a++)//loop to the pos
		p=p->m_next;
	value = p->m_value;//assign the value to the value on position
	return true;//successfully return the value
}

bool Sequence::set(int pos, const ItemType& value) {
	if (pos < 0 || pos >= size())return false;//condition of false
	Node*p = head;//initiate a pointer points to head
	for (int a = 0; a <= pos; a++)//loop to the pos
		p = p->m_next;
	p->m_value = value;//reassign the value
	return true;//successfully set the new value
}

int Sequence::find(const ItemType& value) const {
	Node*p = head->m_next;//point the first obj
	for (int a = 0; p != head; a++) {//loop to find obj 's value equals to value
		if (p->m_value == value)return a;//if find such value return position
		p = p->m_next;//if not move to next obj
	}
	return -1;//if no such value return -1
}

void Sequence::swap(Sequence& other) {
	Node*temp = this->head;//initiate a temp pointer points to itself 's head
	this->head = other.head;//change the head of obj itself to other
	other.head = temp;//change the head of other obj to this 's head
	//exchange the size
	this->m_size = this->m_size + other.m_size;
	other.m_size = this->m_size - other.m_size;
	this->m_size = this->m_size - other.m_size;
}

Sequence::Sequence(const Sequence& other) {
	this->m_size = other.size();//assign this obj size
	this->head = new Node;//create a node for this
	Node*ThisP = head;//initiate a pointer point to this 's head
	Node*OtherP = other.head->m_next;//initiate a pointer point to other's head
	while (OtherP != other.head) {
		ThisP->m_next = new Node;//create a new node
		ThisP->m_next->m_previous = ThisP;//point next node ' s previous pointer to the previous one
		ThisP->m_next->m_value = OtherP->m_value;//assign value
		OtherP = OtherP->m_next;//move to next obj
		ThisP = ThisP->m_next;
	}
	ThisP->m_next = head;//link the last obj to head
	head->m_previous = ThisP;//link the head to last obj
}

Sequence& Sequence::operator=(const Sequence& other) {
	if (this != &other){//when addresses are not the same
		Sequence temp(other);//create a temp sequence which copy the other
		this->swap(temp);//this swap with temp
	}
	return *this;
}

int subsequence(const Sequence& seq1, const Sequence& seq2) {
	if (seq2.empty() || seq2.size() > seq1.size())return -1;//condition of false
	char ok = 0;//switch
	int pos = -1;//position
	ItemType Seq1V, Seq2V;//two variables store value
	for (int a = 0; a < seq1.size(); a++) {//loop to check
		seq1.get(a, Seq1V);//get value
		seq2.get(0, Seq2V);
		if (Seq2V == Seq1V) {//if at the value of sequence 1 at pos a is equal to the value of sequence 2 at pos 0
			ok = 1;//switch on
			pos = a;//pos equal to a
			for (int b = 1; b < seq2.size(); b++) {//loop to check is the value a+b in sequence 1 is equal to value a in sequence 2
				if (!seq1.get(a + b, Seq1V)) { ok = 0; break; }//if false to get value switch off and break the loop
				if (!seq2.get(b, Seq2V)) { ok = 0; break; }
				if (Seq1V != Seq2V) {//if not equal, switch off and break the loop
					ok = 0;
					break;
				}
			}
			if (ok)return pos;//if successfully pass the loop above, return pos
		}
	}
	return -1;//if no such pos return -1
}

void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result) {
	while (!result.empty())result.erase(0);//clear result
	if (seq1.empty()) {result = seq2; return;}//if seq1 is empty, assign seq2 to result
	if (seq2.empty()) {result = seq1; return;}//if seq2 is empty assign seq1 to result
	ItemType Seq1V, Seq2V;//create two value to store value
	int Seq1P = 0, Seq2P = 0, ResultP = 0;//position counter
	while (Seq1P < seq1.size() || Seq2P < seq2.size()) {//loop to insert value
		if (seq1.get(Seq1P, Seq1V)) {//get value from seq1
			result.insert(ResultP, Seq1V);//insert value to sequence result
			Seq1P++; ResultP++;//increase the position counter
		}
		if (seq2.get(Seq2P, Seq2V)) {//get value for seq2
			result.insert(ResultP, Seq2V);//insert value to sequence result
			Seq2P++; ResultP++;//increase the position counter
		}
	}
}