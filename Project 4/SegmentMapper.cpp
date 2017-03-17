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
	Xmap m_Xmap;//map contain X coordinate
	vector<Ymap*> m_Ymap;//contain each Y corresponding to each Y
	void connect(double X, double Y,StreetSegment Smep);//helper to connect a coordinate to data structure
};

SegmentMapperImpl::SegmentMapperImpl(){
}

SegmentMapperImpl::~SegmentMapperImpl(){
	for (int a = 0; a < m_Ymap.size(); a++)
		delete m_Ymap[a];
}

void SegmentMapperImpl::init(const MapLoader& ml){
	for (int a = 0; a < ml.getNumSegments(); a++) {//traverse all street segment
		StreetSegment Stemp;
		if (ml.getSegment(a, Stemp)) {//connect the starting point and ending point to data structure
			connect(Stemp.segment.start.latitude, Stemp.segment.start.longitude, Stemp);
			connect(Stemp.segment.end.latitude, Stemp.segment.end.longitude, Stemp);
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
	//the case when there is not a X value
	int*index=m_Xmap.find(X);
	if (index == nullptr) {
		Ymap* Ytemp=new Ymap;//creat Y map corresponding to a particle X
		vector<StreetSegment> Vtemp;
		Vtemp.push_back(Smep);
		Ytemp->associate(Y, Vtemp);//push a particle Y to the Y map
		m_Xmap.associate(X, m_Ymap.size());//connect X to the index of the vector which can find it's Y map
		m_Ymap.push_back(Ytemp);//push this Y map to vector
		return;
	}

	else {
		int i = *index;
		vector<StreetSegment>* Vtemp = m_Ymap[i]->find(Y);//if there is a X, but don't have the Y
		if (Vtemp == nullptr) {
			vector<StreetSegment> Vtemp2;
			Vtemp2.push_back(Smep);
			m_Ymap[i]->associate(Y, Vtemp2);//add a Y to Ymap
			return;
		}

		else if (Vtemp != nullptr) {
			Vtemp->push_back(Smep);//if two stree segment shared same coordinate
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
