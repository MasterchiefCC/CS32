#include "provided.h"
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

class MapLoaderImpl
{
public:
	MapLoaderImpl();
	~MapLoaderImpl();
	bool load(string mapFile);
	size_t getNumSegments() const;
	bool getSegment(size_t segNum, StreetSegment& seg) const;
private:
	vector<StreetSegment> m_StreetSegment;
};

MapLoaderImpl::MapLoaderImpl()
{
}

MapLoaderImpl::~MapLoaderImpl()
{
}

bool MapLoaderImpl::load(string mapFile){
	ifstream fin(mapFile.c_str());
	if (!fin)return false;
	string s;//based on the format to form street segment
	while (getline(fin, s)) {
		StreetSegment temp;
		//street name
		for (unsigned int a = 0; a < s.size(); a++)temp.streetName += s[a];
		//find the coordinate
		getline(fin, s);
		string SX, SY, EX, EY;
		unsigned int a = 0;
		while (s[a] != ','&&a < s.size()) { SX += s[a]; a++; }
		a+=2;
		while (s[a] != ' '&&a < s.size()) { SY += s[a]; a++; }
		a++;
		while (s[a] != ','&&a < s.size()) { EX += s[a]; a++; }
		a++;
		while (a < s.size()) { EY += s[a]; a++; }
		GeoCoord start(SX, SY);
		GeoCoord end(EX, EY);
		GeoSegment temp_stree(start, end);
		temp.segment = temp_stree;
		getline(fin, s);//count the number of attractions around a street segment
		int counter = stod(s);
		while (counter--) {
			getline(fin, s);
			Attraction temp_Attraction;
			a = 0;//name of the attractions
			while (a < s.size() && s[a] != '|') { temp_Attraction.name += s[a]; a++; }
			a++;
			string ASX, ASY;
			while (s[a] != ','&&a < s.size()) { ASX += s[a]; a++; }
			a++;
			while (a < s.size()) { ASY += s[a]; a++; }
			GeoCoord temp_coordA(ASX, ASY);//coordinate for the attractions
			temp_Attraction.geocoordinates = temp_coordA;
			temp.attractions.push_back(temp_Attraction);
		}
		m_StreetSegment.push_back(temp);
	}
	return true;
}

size_t MapLoaderImpl::getNumSegments() const{
	return m_StreetSegment.size();
}

bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const {
	if (segNum < 0 || segNum >= getNumSegments())return false;
	else {
		seg = m_StreetSegment[segNum];//use the index to find the segment
		return true;
	}
}

//******************** MapLoader functions ************************************

// These functions simply delegate to MapLoaderImpl's functions.
// You probably don't want to change any of this code.

MapLoader::MapLoader()
{
	m_impl = new MapLoaderImpl;
}

MapLoader::~MapLoader()
{
	delete m_impl;
}

bool MapLoader::load(string mapFile)
{
	return m_impl->load(mapFile);
}

size_t MapLoader::getNumSegments() const
{
	return m_impl->getNumSegments();
}

bool MapLoader::getSegment(size_t segNum, StreetSegment& seg) const
{
   return m_impl->getSegment(segNum, seg);
}
