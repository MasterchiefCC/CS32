#include "provided.h"
#include "MyMap.h"
#include <string>
#include <cctype>
using namespace std;
typedef MyMap<string, GeoCoord> mmap;
class AttractionMapperImpl
{
public:
	AttractionMapperImpl();
	~AttractionMapperImpl();
	void init(const MapLoader& ml);
	bool getGeoCoord(string attraction, GeoCoord& gc) const;
private:
	mmap m_AttractionMap;
};

AttractionMapperImpl::AttractionMapperImpl()
{
}

AttractionMapperImpl::~AttractionMapperImpl()
{
}

void AttractionMapperImpl::init(const MapLoader& ml){
	for (int a = 0; a < ml.getNumSegments(); a++) {
		StreetSegment temp;//get each segments
		if (ml.getSegment(a, temp)) {
			int len = temp.attractions.size();
			for (int b = 0; b < len; b++) {//traverse every attraction on that street segment
				for (int c = 0; c < temp.attractions[b].name.size(); c++)
					temp.attractions[b].name[c] = tolower(temp.attractions[b].name[c]);//change to lower case
				m_AttractionMap.associate(temp.attractions[b].name, temp.attractions[b].geocoordinates);//connect key value and strored value
			}
		}
	}
}

bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{
	for (int a = 0; a < attraction.size(); a++)
		attraction[a] = tolower(attraction[a]);//change to lower case
	const GeoCoord*temp = m_AttractionMap.find(attraction);//find my data structure
	if (temp == nullptr)return false;//can't find it
	gc = *temp;
	return true;
}

//******************** AttractionMapper functions *****************************

// These functions simply delegate to AttractionMapperImpl's functions.
// You probably don't want to change any of this code.

AttractionMapper::AttractionMapper()
{
	m_impl = new AttractionMapperImpl;
}

AttractionMapper::~AttractionMapper()
{
	delete m_impl;
}

void AttractionMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

bool AttractionMapper::getGeoCoord(string attraction, GeoCoord& gc) const
{
	return m_impl->getGeoCoord(attraction, gc);
}
