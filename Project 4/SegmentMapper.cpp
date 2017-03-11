#include "provided.h"
#include "MyMap.h"
#include <vector>
using namespace std;

typedef MyMap<double, vector<StreetSegment>> Ymap;
typedef MyMap<double, int> Xmap;


class SegmentMapperImpl
{
public:
	SegmentMapperImpl();
	~SegmentMapperImpl();
	void init(const MapLoader& ml);
	vector<StreetSegment> getSegments(const GeoCoord& gc) const;
private:
	Xmap m_Xmap;
	vector<Ymap*> m_Ymap;
	void connect(double X, double Y,StreetSegment Smep);
};

SegmentMapperImpl::SegmentMapperImpl(){
}

SegmentMapperImpl::~SegmentMapperImpl(){
	for (int a = 0; a < m_Ymap.size(); a++)
		delete m_Ymap[a];
}

void SegmentMapperImpl::init(const MapLoader& ml){
	for (int a = 0; a < ml.getNumSegments(); a++) {
		StreetSegment Stemp;
		if (ml.getSegment(a, Stemp)) {
			connect(Stemp.segment.start.latitude, Stemp.segment.start.longitude, Stemp);
			connect(Stemp.segment.end.latitude, Stemp.segment.end.longitude, Stemp);
			for (int b = 0; b < Stemp.attractions.size(); b++) {
				StreetSegment Astemp;
				GeoCoord end(Stemp.segment.start.latitudeText, Stemp.segment.start.longitudeText);
				GeoSegment Gtemp(Stemp.attractions[b].geocoordinates, end);
				Astemp.segment = Gtemp;
				Astemp.streetName = Stemp.streetName;
				Astemp.attractions.push_back(Stemp.attractions[b]);
				connect(Astemp.segment.start.latitude, Astemp.segment.start.longitude, Astemp);
				connect(Astemp.segment.end.latitude, Astemp.segment.end.longitude, Astemp);
			}
		}
	}
}

vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
{
	vector<StreetSegment> segments;
	const int*index = m_Xmap.find(gc.latitude);
	if (index == nullptr)return segments;
	const vector<StreetSegment>* Vtemp=m_Ymap[*index]->find(gc.longitude);
	if (Vtemp == nullptr)return segments;
	 vector<StreetSegment>::const_iterator Sit = Vtemp->begin();
	while (Sit != Vtemp->end()) {
		segments.push_back((*Sit));
		Sit++;
	}
	return segments;  // This compiles, but may not be correct
}

void SegmentMapperImpl::connect(double X, double Y, StreetSegment Smep) {
	//找不到X，证明没有X和Y有联系，所以创造一个Y和一个vector，联系Y和vector，然后联系X和Y和vector
	int*index=m_Xmap.find(X);
	if (index == nullptr) {
		Ymap* Ytemp=new Ymap;
		vector<StreetSegment> Vtemp;
		Vtemp.push_back(Smep);
		Ytemp->associate(Y, Vtemp);
		m_Xmap.associate(X, m_Ymap.size());
		m_Ymap.push_back(Ytemp);
		return;
	}

	else {
		int i = *index;
		vector<StreetSegment>* Vtemp = m_Ymap[i]->find(Y);
		if (Vtemp == nullptr) {
			vector<StreetSegment> Vtemp2;
			Vtemp2.push_back(Smep);
			m_Ymap[i]->associate(Y, Vtemp2);
			return;
		}

		else if (Vtemp != nullptr) {
			Vtemp->push_back(Smep);
			return;
		}
	}
}

//******************** SegmentMapper functions ********************************

// These functions simply delegate to SegmentMapperImpl's functions.
// You probably don't want to change any of this code.

SegmentMapper::SegmentMapper()
{
	m_impl = new SegmentMapperImpl;
}

SegmentMapper::~SegmentMapper()
{
	delete m_impl;
}

void SegmentMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

vector<StreetSegment> SegmentMapper::getSegments(const GeoCoord& gc) const
{
	return m_impl->getSegments(gc);
}
