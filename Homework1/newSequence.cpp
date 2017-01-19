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
	if (pos > size() || pos < 0 || m_size >= m_maxm)return false;
	
	for (int a = size(); a > pos; a--)
		m_value[a] = m_value[a - 1];

	m_value[pos] = value;
	m_size++;
	return 1;
}

int Sequence::insert(const ItemType& value) {
	if (m_size >= m_maxm)return -1;

	int pos=m_size;
	ItemType minMax=m_value[0];
	for (int a = 0; a < m_size; a++)
		if (m_value[a] > minMax)minMax = m_value[a];
	for (int a = m_size-1; a >= 0; a--) {
		if (m_value[a] <= minMax&&m_value[a] >= value) {
			for (int b = pos; b > a; b--)
				m_value[b] = m_value[b - 1];
			pos = a;
			minMax = m_value[a];
		}
	}
	
	m_value[pos] = value;
	m_size++;
	
	return pos;
}

bool Sequence::erase(int pos) {
	if (pos < 0 || pos >= size())return 0;
	if (pos == size() - 1) {  m_size--; return 1; }
	for (int a = pos; a+1 < size(); a++)m_value[a] = m_value[a + 1];
	m_size--;
	return 1;
}

int Sequence::remove(const ItemType& value) {
	int removed = 0;
	for (int a = 0; a < size()&&a-removed>=0; a++) {
		m_value[a - removed] = m_value[a];
		if (m_value[a] == value)
			removed++;
	}
	m_size = m_size - removed;
	return removed;
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

	temp = m_value;
	m_value = other.m_value;
	other.m_value = temp;

	this->m_maxm = this->m_maxm + other.m_maxm;
	other.m_maxm = this->m_maxm - other.m_maxm;
	this->m_maxm = this->m_maxm - other.m_maxm;

	this->m_size = this->m_size + other.m_size;
	other.m_size = this->m_size - other.m_size;
	this->m_size = this->m_size - other.m_size;

	
}




