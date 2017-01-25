#include "Sequence.h"

Sequence::Sequence() {
	m_size = 0;//initiate with size 0
}

bool Sequence::empty() const {
	return !m_size;
}

int Sequence::size() const {
	return m_size;
}

bool Sequence::insert(int pos, const ItemType& value) {
	if (pos > size() || pos < 0 || m_size >= DEFAULT_MAX_ITEMS)return false;//the condition can't insert number into certain pos of sequence
	
	for (int a = size(); a > pos; a--)
		m_value[a] = m_value[a - 1];//loop to let position available

	m_value[pos] = value;//insert value to certain pos
	m_size++;//increase size
	return true;
}

int Sequence::insert(const ItemType& value) {
	if (m_size >= DEFAULT_MAX_ITEMS)return -1;//condition that can't insert number into sequence

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
	if (pos == size() - 1) {  m_size--; return true; }
	for (int a = pos; a+1 < size(); a++)m_value[a] = m_value[a + 1];//loop to full up certain position
	m_size--;//decrease the size
	return true;
}


int Sequence::remove(const ItemType& value) {
	int removed = 0;//initiate a value to store the number of  removed value
	for (int a = 0; a < size()&&a-removed>=0; a++) {
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
		return true;
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
	int maxm;
	if (this->m_size >= other.m_size)maxm = this->m_size;//find the long sequence
	else maxm = other.m_size;
	for (int a = 0; a < maxm; a++){//exchange value
		ItemType temp;
		temp = m_value[a];
		m_value[a] = other.m_value[a];
		other.m_value[a] = temp;
	}
	this->m_size = other.m_size + this->m_size;//exchange size
	other.m_size = this->m_size - other.m_size;
	this->m_size = this->m_size - other.m_size;
}


