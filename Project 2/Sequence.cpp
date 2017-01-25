#include "Sequence.h"

Sequence::Sequence() {
	head = new Node;
	head->m_next = head;
	head->m_previous = head;
}

Sequence::~Sequence() {
	Node*pre = head;
	Node*p = head->m_next;
	while (1) {
		delete pre;
		if (p == head)break;
		pre = p;
		p = p->m_next;
	}
}

bool Sequence::empty()const {
	return !m_size;
}

int Sequence::size() const {
	return m_size;
}

bool Sequence::insert(int pos, const ItemType&value) {
	if (pos<0 || pos>size())return false;
	Node*p = head;
	for (int a = 0; a <= pos; a++)
		p = p->m_next;
	Node*insert = new Node;
	insert->m_next = p;
	insert->m_previous = p->m_previous;
	p->m_previous->m_next = insert;
	p->m_previous = insert;
	insert->m_value = value;
	m_size++;
	return true;
}

bool Sequence::insert(const ItemType& value) {
	Node*p = head;
	while (1) {
		if (p->m_next->m_value == value)break;
		if (p->m_next == head)break;
		p = p->m_next;
	}
	Node*insert = new Node;
	insert->m_previous = p;
	insert->m_next = p->m_next;
	insert->m_value = value;
	p->m_next->m_previous = insert;
	p->m_next = insert;
	m_size++;
	return true;
}

bool Sequence::erase(int pos) {
	if (pos < 0 || pos >= size())return false;
	Node*p = head;
	for (int a = 0; a <= pos; a++)
		p = p->m_next;
	p->m_next->m_previous = p->m_previous;
	p->m_previous->m_next = p->m_next;
	m_size--;
	delete p;
	return false;
}

int Sequence::remove(const ItemType& value) {
	Node*p = head->m_next;
	int removed = 0;
	while (p!=head) {
		if (value == p->m_value) {
			p->m_previous->m_next = p->m_next;
			p->m_next->m_previous = p->m_previous;
			Node*temp = p;
			p = p->m_next;
			removed++;
			m_size--;
			delete temp;
			continue;
		}
		p = p->m_next;
	}
	return removed;
}

bool Sequence::get(int pos, ItemType& value) const {
	if (pos < 0 || pos >= size())return false;
	Node*p = head;
	for (int a = 0; a <= pos; a++)
		p=p->m_next;
	value = p->m_value;
	return true;
}

bool Sequence::set(int pos, const ItemType& value) {
	if (pos < 0 || pos >= size())return false;
	Node*p = head;
	for (int a = 0; a <= pos; a++)
		p = p->m_next;
	p->m_value = value;
	return true;
}

int Sequence::find(const ItemType& value) const {
	Node*p = head->m_next;
	for (int a = 0; p != head; a++) {
		if (p->m_value == value)return a;
		p = p->m_next;
	}
	return -1;
}

void Sequence::swap(Sequence& other) {
	Node*temp = this->head;
	this->head = other.head;
	other.head = temp;

	this->m_size = this->m_size + other.m_size;
	other.m_size = this->m_size - other.m_size;
	this->m_size = this->m_size - other.m_size;
}

Sequence::Sequence(const Sequence& other) {
	this->m_size = other.size();
	this->head = new Node;
	Node*ThisP = head;
	Node*OtherP = other.head->m_next;
	while (OtherP != other.head) {
		ThisP->m_next = new Node;
		ThisP->m_next->m_previous = ThisP;
		ThisP->m_next->m_value = OtherP->m_value;
		OtherP = OtherP->m_next;
		ThisP = ThisP->m_next;
	}
	ThisP->m_next = head;
}

Sequence& Sequence::operator=(const Sequence& other) {
	if (this != &other){
		Sequence temp(other);
		this->swap(temp);
	}
	return *this;
}

int subsequence(const Sequence& seq1, const Sequence& seq2) {
	if (seq2.empty() || seq2.size() > seq1.size())return -1;
	char ok = 0;
	int pos = -1;
	ItemType Seq1V, Seq2V;
	for (int a = 0; a < seq1.size(); a++) {
		seq1.get(a, Seq1V);
		seq2.get(0, Seq2V);
		if (Seq2V == Seq1V) {
			ok = 1;
			pos = a;
			for (int b = 1; b < seq2.size(); b++) {
				if (!seq1.get(a + b, Seq1V)) { ok = 0; break; }
				if (!seq2.get(b, Seq2V)) { ok = 0; break; }
				if (Seq1V != Seq2V) {
					ok = 0;
					break;
				}
			}
			if (ok)return pos;
		}
	}
	return -1;
}

void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result) {
	while (!result.empty())result.erase(0);
	if (seq1.empty()) {result = seq2; return;}
	if (seq2.empty()) {result = seq1; return;}
	ItemType Seq1V, Seq2V;
	int Seq1P = 0, Seq2P = 0, ResultP = 0;
	while (Seq1P < seq1.size() || Seq2P < seq2.size()) {
		if (seq1.get(Seq1P, Seq1V)) {
			result.insert(ResultP, Seq1V);
			Seq1P++; ResultP++;
		}
		if (seq2.get(Seq2P, Seq2V)) {
			result.insert(ResultP, Seq2V);
			Seq2P++; ResultP++;
		}
	}
}