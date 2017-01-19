#include "ScoreList.h"
#include <iostream>
using namespace std;

ScoreList::ScoreList():m_score(){
}

bool ScoreList::add(unsigned long score) {
	if (score >= 0 && score <= 100) {
		m_score.insert(score);
		return 1;
	}
	return 0;
}

bool ScoreList::remove(unsigned long score) {
	int pos = m_score.find(score);
	if (!pos)return 0;
	m_score.erase(pos);
	return 1;
}

int ScoreList::size() const{
	return m_score.size();
}

unsigned long ScoreList::minimum() const {
	if (!m_score.size())return NO_SCORE;
	unsigned long minm;
	m_score.get(0, minm);
	for (int a = 0; a < m_score.size(); a++) {
		unsigned long temp;
		m_score.get(a, temp);
		if (temp < minm)minm = temp;
	}
	return minm;
}

unsigned long ScoreList::maximum()const {
	if (!m_score.size())return NO_SCORE;
	unsigned long maxm;
	m_score.get(0, maxm);
	for (int a = 0; a < m_score.size(); a++) {
		unsigned long temp;
		m_score.get(a, temp);
		if (temp > maxm)maxm = temp;
	}
	return maxm;
}