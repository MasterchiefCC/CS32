#include "newSequence.h"

Sequence::Sequence(int size) {
	if (size < 0)exit(1);
	m_size = 0;
	m_value = new ItemType[size];
	m_maxm = size;
}

bool Sequence::empty() const {
	return !m_size;
}

int Sequence::size() const {
	return m_size;
}

bool Sequence::insert(int pos, const ItemType& value) {
	if (pos > size() || pos < 0 || m_size >= m_maxm)return false;//the condition can't insert number into certain pos of sequence

	for (int a = size(); a > pos; a--)
		m_value[a] = m_value[a - 1];//loop to let position available

	m_value[pos] = value;//insert value to certain pos
	m_size++;//increase size
	return true;
}

int Sequence::insert(const ItemType& value) {
	if (m_size >= m_maxm)return -1;//condition that can't insert number into sequence

	int pos;//initiate a int to store the position should be insert
	for (pos = 0; pos < m_size; pos++)//loop to find that position
		if (m_value[pos] >= value)break;
	for (int a = m_size; a > pos; a--)//loop to let that position available
		m_value[a] = m_value[a - 1];

	m_value[pos] = value;//insert that value
	m_size++;//increase the size

	return pos;
}

bool Sequence::erase(int pos) {
	if (pos < 0 || pos >= size())return false;//condition that cant erase the value in certain position
	if (pos == size() - 1) { m_size--; return true; }
	for (int a = pos; a + 1 < size(); a++)m_value[a] = m_value[a + 1];//loop to full up certain position
	m_size--;//decrease the size
	return true;
}

int Sequence::remove(const ItemType& value) {
	int removed = 0;//initiate a value to store the number of  removed value
	for (int a = 0; a < size() && a - removed >= 0; a++) {
		if (m_value[a] == value) {
			removed++;//increase the number of removed value if such pos equal to value
			continue;//don't execute next step
		}
		m_value[a - removed] = m_value[a];//cover the pos if it is equal to value
	}
	m_size = m_size - removed;//adjust the value after remove certain value
	return removed;//return the value of removed
}

bool Sequence::get(int pos, ItemType& value) const {
	if (pos >= 0 && pos < size()) {
		value = m_value[pos];
		return 1;
	}
	return false;
}

bool Sequence::set(int pos, const ItemType& value) {
	if (pos >= 0 && pos < size()) {
		m_value[pos] = value;
		return true;
	}
	return false;
}

int Sequence::find(const ItemType& value) const {
	for (int a = 0; a < size(); a++) {
		if (value == m_value[a]) return a;
	}
	return -1;
}

void Sequence::swap(Sequence& other) {
	ItemType* temp;
	//exchange pointers
	temp = m_value;
	m_value = other.m_value;
	other.m_value = temp;
	//exchange maximum capacity
	this->m_maxm = this->m_maxm + other.m_maxm;
	other.m_maxm = this->m_maxm - other.m_maxm;
	this->m_maxm = this->m_maxm - other.m_maxm;
	//exchange size
	this->m_size = this->m_size + other.m_size;
	other.m_size = this->m_size - other.m_size;
	this->m_size = this->m_size - other.m_size;
}

Sequence::~Sequence() {
	delete[] m_value;
}

Sequence::Sequence(const Sequence&other) {//deep copy
	this->m_size = other.m_size;//copy size
	this->m_maxm = other.m_maxm;//copy maximum capacity
	this->m_value = new ItemType[this->m_maxm];//create value
	for (int a = 0; a < this->m_maxm; a++)
		this->m_value[a] = other.m_value[a];//deep copy values to address
}

Sequence& Sequence::operator=(const Sequence&other) {
	if (this == &other)return *this;//condition when two addresses are same
	this->m_size = other.m_size;//copy size
	this->m_maxm = other.m_maxm;//copy maximum capacity
	if(this->m_value!=nullptr)
		delete [] this->m_value;//delete original
	this->m_value = new ItemType[this->m_maxm];//new
	for (int a = 0; a < this->m_maxm; a++)//deep copy value to new address
		this->m_value[a] = other.m_value[a];
	return *this;
}




