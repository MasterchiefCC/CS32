#include "provided.h"
#include "MyMap.h"
#include <string>
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
		StreetSegment temp;
		if (ml.getSegment(a, temp)) {
			int len = temp.attractions.size();
			for (int b = 0; b < len; b++)
				m_AttractionMap.associate(temp.attractions[b].name, temp.attractions[b].geocoordinates);
		}
	}
}

bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{
	const GeoCoord*temp = m_AttractionMap.find(attraction);
	if (temp == nullptr)return false;
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
