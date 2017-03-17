#include "provided.h"
#include "MyMap.h"
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <cctype>
using namespace std;

class NavigatorImpl
{
public:
	NavigatorImpl();
	~NavigatorImpl();
	bool loadMapData(string mapFile);
	NavResult navigate(string start, string end, vector<NavSegment>& directions) const;


	//////////////////////////////////////////////////////////////////////////////

	struct Node {//a Node to store points
		Node() { parent = nullptr; streetname = ""; }
		Node(Node*p, string name, GeoCoord c, double N, double G) :parent(p), streetname(name), coord(c), Ncost(N), Gcost(G) {
			Fcost = Ncost + Gcost;
		}
		string streetname;
		Node*parent;//pointer to the point who discover it
		GeoCoord coord;
		double Fcost;//N+G
		double Ncost;//the distance between destination and current point without any block
		double Gcost;//the distance between beginning and current use the path discoed
	};

	class HeapCompare_f {
	public://function for heap
		bool operator()(Node*x, Node*y) const {
			return x->Fcost > y->Fcost;
		}
	};//minHeap

	typedef MyMap<double, Node> Ymap;
	typedef MyMap<double, int> Xmap;

	class StoreCoord {//class for OpenTable and CloseTable
	
//////////////////////////////////////////////////////////////////////////////

	public:
		Node* getNode(const GeoCoord& gc) {
			const int*index = m_Xmap.find(gc.latitude);
			if (index == nullptr)return nullptr;
			Node*N = m_Ymap[*index]->find(gc.longitude);
			if (N == nullptr)return nullptr;

			return N;  // This compiles, but may not be correct
		}

		~StoreCoord();

		void connect(GeoCoord &Coord, Node Smep);//helper to connect a coordinate to data structure
	private:
		Xmap m_Xmap;//map contain X coordinate
		vector<Ymap*> m_Ymap;//contain each Y corresponding to each Y
		
	};

//////////////////////////////////////////////////////////////////////////////

	AttractionMapper m_AttractionMap;
	//data structure to store attraction map

	SegmentMapper m_SegmentMap;
	//data structure to store street segment map

	MyMap<string, StreetSegment> m_roadToPath;
	//data structure to store the street segment corresponding to a attraction

	void Streetinit(MapLoader& Map);
	//function to init the m_roadToPath

	//////////////////////////////////////////////////////////////////////////////

	bool pathFinding(GeoCoord start, GeoCoord end, vector<NavSegment> &directions,
		const StreetSegment*segmentStart, const StreetSegment*segmentEnd) const;
	// function to find path

	//////////////////////////////////////////////////////////////////////////////
	void generatePath(vector<NavSegment> &directions, Node *end) const;
	//if confirmed there is a path find the path

	bool confirmStartSame(const StreetSegment*segmentStart, const StreetSegment*segmetEnd) const;
	//helper function to confirm start on the same point

	void helper_toNaviSegment(NavSegment&target, const string&StreetName, GeoCoord&start, GeoCoord&end)const;
	//helper function to generate a NaviSegment

	bool confirmToEnd(const StreetSegment* end, GeoCoord&current)const;
	//helper function to confirm reach end point

	void exploreAround(Node*current, vector<Node*>&OPENTable,  StoreCoord& OPENTableFind, 
		StoreCoord &CLOSEDTable, GeoCoord&end, vector<Node*>&Trash)const;
	//find other point connected to current point

	void findConnectPoint(GeoCoord&start, StreetSegment&Street, GeoCoord&end)const;
	//helper function to find the point connected to current one
	
//////////////////////////////////////////////////////////////////////////////
	string direct(double angle) const;
	//helper function to find the angle of a segment
	string dirTurn(double angle)const;
	//helper function to find the turn
	void del(vector<Node *>&OPENTable, vector<Node*>&Trash, Node*start, Node*end)const;
};

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

NavigatorImpl::StoreCoord::~StoreCoord() {
	for (unsigned int a = 0; a < m_Ymap.size(); a++)
		delete m_Ymap[a];
}

void NavigatorImpl::StoreCoord::connect(GeoCoord &Coord, Node coord){
	double X = Coord.latitude;
	double Y = Coord.longitude;
	//the case when there is not a X value
	const int*index = m_Xmap.find(X);
	if (index == nullptr) {
		Ymap* Ytemp = new Ymap;//creat Y map corresponding to a particle X
		Ytemp->associate(Y, coord);//push a particle Y to the Y map
		m_Xmap.associate(X, m_Ymap.size());//connect X to the index of the vector which can find it's Y map
		m_Ymap.push_back(Ytemp);//push this Y map to vector
		return;
	}

	else {
		int i = *index;
		Node* Vtemp = m_Ymap[i]->find(Y);//if there is a X, but don't have the Y
		if (Vtemp == nullptr) {

			m_Ymap[i]->associate(Y, coord);//add a Y to Ymap
			return;
		}

		else if (Vtemp != nullptr) {
			*Vtemp=coord; //if two collision shared same coordinate
			return;
		}
	}
}


void NavigatorImpl::Streetinit(MapLoader& Map) {//to connect each attraction to a corresponding street segment
	for (unsigned int a = 0; a < Map.getNumSegments(); a++) {
		StreetSegment temp;
		Map.getSegment(a, temp);
		for (unsigned int b = 0; b < temp.attractions.size(); b++) {
			for (unsigned int c = 0; c < temp.attractions[b].name.size(); c++)
				temp.attractions[b].name[c] = tolower(temp.attractions[b].name[c]);
			m_roadToPath.associate(temp.attractions[b].name, temp);
		}
	}
}


bool NavigatorImpl::loadMapData(string mapFile) {//init all the data structure
	MapLoader tempMapLoader;
	if (!tempMapLoader.load(mapFile))return false;
	m_AttractionMap.init(tempMapLoader);
	m_SegmentMap.init(tempMapLoader);
	Streetinit(tempMapLoader);
	return true;
}

//////////////////////////////////////////////////////////////////////////////


NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const {//navigate function
	GeoCoord m_start, m_end;

	//change to lower case
	for (unsigned int a = 0; a < start.size(); a++)
		start[a] = tolower(start[a]);
	for (unsigned int a = 0; a < end.size(); a++)
		end[a] = tolower(end[a]);

	//find the coordinate for starting and ending point
	if (!m_AttractionMap.getGeoCoord(start, m_start))return NAV_BAD_SOURCE;
	if (!m_AttractionMap.getGeoCoord(end, m_end))return NAV_BAD_DESTINATION;

	//find the corresponding street segment
	const StreetSegment*begin = m_roadToPath.find(start);
	const StreetSegment*endSegemt = m_roadToPath.find(end);

	//if both start and end is on the same point then return a false type
	if (m_start.latitude == m_end.latitude&&m_start.longitude == m_end.longitude)return NAV_BAD_DESTINATION;

	//find path
	if (pathFinding(m_start, m_end, directions, begin, endSegemt))return NAV_SUCCESS;
	else return NAV_NO_ROUTE;
	return NAV_NO_ROUTE;
}


bool NavigatorImpl::pathFinding(GeoCoord start, GeoCoord end, vector<NavSegment> &directions,
	const StreetSegment*segmentStart, const StreetSegment*segmentEnd) const {

	if (confirmStartSame(segmentStart, segmentEnd)) {//if on the same segment then connect they
		NavSegment AttractOnSameSegment;
		helper_toNaviSegment(AttractOnSameSegment, segmentStart->streetName, start, end);
		directions.push_back(AttractOnSameSegment);
		return true;//return
	}
	vector<Node*> OPENTable;//open table
	vector<Node*>Trash;//Trash Node;
	StoreCoord OPENTableFind;//open table to Find Obj
	StoreCoord CLOSEDTable;//Closed table to Find Obj

	 Node* m_start=new Node(nullptr, "", start, distanceEarthKM(start, end), 0);//starting point
	Node* m_end=new Node(nullptr, "", end, 0, 0);//ending point

	Node*departureA=new Node(m_start, segmentStart->streetName, segmentStart->segment.start, distanceEarthKM(segmentStart->segment.start, end), distanceEarthKM(segmentStart->segment.start, start));
	//the point where departure
	Node*departureB=new Node(m_start, segmentStart->streetName, segmentStart->segment.end, distanceEarthKM(segmentStart->segment.end, end), distanceEarthKM(segmentStart->segment.end, start));
	//the another point where departure
	OPENTable.push_back(departureA);
	OPENTable.push_back(departureB);
	make_heap(OPENTable.begin(), OPENTable.end(), HeapCompare_f());
	OPENTableFind.connect(departureA->coord, *departureA);
	OPENTableFind.connect(departureB->coord, *departureB);

	for (int a = 0;; a++) {
		if (OPENTable.empty()) { //can't reach target
			del(OPENTable, Trash, m_start, m_end);
			return false;
		}

		Node*LowestGcostPoint;
		
		while (1) {
			LowestGcostPoint = OPENTable.front();//take the point with lowest Fcost
			pop_heap(OPENTable.begin(), OPENTable.end(), HeapCompare_f());
			OPENTable.pop_back();
			
			Node*temp = OPENTableFind.getNode((*LowestGcostPoint).coord);
			if ((*LowestGcostPoint).Gcost <= temp->Gcost)break;
		}


		if (confirmToEnd(segmentEnd, LowestGcostPoint->coord)) {//if it reach the ending point
			m_end->parent = LowestGcostPoint;
			m_end->streetname = LowestGcostPoint->streetname;

			generatePath(directions, m_end);//generate the path
			Trash.push_back(LowestGcostPoint);
			del(OPENTable, Trash, m_start, m_end);
			return true;
		}
		exploreAround(LowestGcostPoint, OPENTable, OPENTableFind, CLOSEDTable, end, Trash);//explore around point of current point
	}
	return false;
}


//////////////////////////////////////////////////////////////////////////////


bool NavigatorImpl::confirmStartSame(const StreetSegment*segmentStart, const StreetSegment*segmentEnd) const {
	if (segmentStart->segment.start.latitude == segmentEnd->segment.start.latitude&&
		segmentStart->segment.start.longitude == segmentEnd->segment.start.longitude&&
		segmentStart->segment.end.latitude == segmentEnd->segment.end.latitude&&
		segmentStart->segment.end.longitude == segmentEnd->segment.end.longitude)return true;
	return false;
}


void NavigatorImpl::helper_toNaviSegment(NavSegment&target, const string&StreetName, GeoCoord&start, GeoCoord&end) const {
	GeoSegment GS(start, end);
	target.m_direction = direct(angleOfLine(GS));
	target.m_distance = distanceEarthMiles(start, end);
	target.m_geoSegment = GS;
	target.m_streetName = StreetName;
	target.m_command = target.PROCEED;
}


bool NavigatorImpl::confirmToEnd(const StreetSegment* end, GeoCoord&current) const {
	if ((end->segment.start.latitude == current.latitude&&end->segment.start.longitude == current.longitude) ||
		(end->segment.end.latitude == current.latitude&&end->segment.end.longitude == current.longitude))
		return true;
	return false;
}


void  NavigatorImpl::exploreAround(Node*current, vector<Node*>&OPENTable, StoreCoord& OPENTableFind,
	StoreCoord &CLOSEDTable, GeoCoord&end, vector<Node*>&Trash)const {

	vector<StreetSegment>Streets = m_SegmentMap.getSegments(current->coord);

	for (unsigned int a = 0; a < Streets.size(); a++) {
		GeoCoord endPoint;

		findConnectPoint(current->coord, Streets[a], endPoint);

		double NewGV = distanceEarthKM(current->coord, endPoint) + current->Gcost;

		Node* CLOSEDTableResult = CLOSEDTable.getNode(endPoint);
		if (CLOSEDTableResult != nullptr&&CLOSEDTableResult->Gcost <= NewGV)continue;
		//if it appear and with smaller Gcost then continue

		Node *OPENTableResult = OPENTableFind.getNode(endPoint);
		if (OPENTableResult != nullptr&&OPENTableResult->Gcost <= NewGV)continue;
		//if it appear and with smaller Gcost then continue

		//it is the best node to destination
		Node*BestNode=new Node(current, Streets[a].streetName, endPoint, distanceEarthKM(endPoint, end), NewGV);
		
		//push to open table
		OPENTable.push_back(BestNode);

		//reheapification
		push_heap(OPENTable.begin(), OPENTable.end(), HeapCompare_f());

		OPENTableFind.connect(BestNode->coord, *BestNode);
	}

	CLOSEDTable.connect(current->coord, (*current));
	Trash.push_back(current);
}



//helper function to the connected point on a street segment
void NavigatorImpl::findConnectPoint(GeoCoord&start, StreetSegment &Street, GeoCoord&end) const {
	if (start.latitude == Street.segment.start.latitude&&start.longitude == Street.segment.start.longitude) {
		end.latitude = Street.segment.end.latitude;
		end.latitudeText = Street.segment.end.latitudeText;
		end.longitude = Street.segment.end.longitude;
		end.longitudeText = Street.segment.end.longitudeText;
	}

	else if (start.latitude == Street.segment.end.latitude&&start.longitude == Street.segment.end.longitude) {
		end.latitude = Street.segment.start.latitude;
		end.latitudeText = Street.segment.start.latitudeText;
		end.longitude = Street.segment.start.longitude;
		end.longitudeText = Street.segment.start.longitudeText;
	}
}


void NavigatorImpl::generatePath(vector<NavSegment> &directions, Node *end) const {
	stack<Node*> path;
	Node*p = end;
	while (1) {//based the end to find the path
		if (p->parent == nullptr) {
			path.push(p);
			break;
		}
		path.push(p);
		p = p->parent;
	}

	//based on the coordinate to form a path
	while (!path.empty()) {
		Node*current = path.top();//the starting point
		path.pop();
		if (path.empty())break;
		Node*connect = path.top();//the ending point

		GeoSegment GeoS(current->coord, connect->coord);//connect to a segment

		if (current->streetname != connect->streetname) {
			int len = directions.size();
			if (len > 0) {
				//turn type
				double angle = angleBetween2Lines(directions[len - 1].m_geoSegment, GeoS);
				string dir = dirTurn(angle);
				NavSegment TurnType(dir, connect->streetname);
				directions.push_back(TurnType);
			}
		}
		//normal process type
		NavSegment ProccessType;
		helper_toNaviSegment(ProccessType, connect->streetname, GeoS.start, GeoS.end);
		directions.push_back(ProccessType);
	}
}

string NavigatorImpl::direct(double angle) const {
	if (angle >= 0 && angle <= 22.5)return "east";
	if (angle > 22.5&&angle <= 67.5)return "northeast";
	if (angle > 67.5&&angle <= 112.5)return "north";
	if (angle > 112.5&&angle <= 157.5)return "northwest";
	if (angle > 157.5&&angle <= 202.5)return "west";
	if (angle > 202.5&&angle <= 247.5)return "southwest";
	if (angle > 247.5&&angle <= 292.5)return "south";
	if (angle > 292.5&&angle <= 337.5)return"southeast";
	if (angle > 337.5&&angle < 360)return "east";
	return "Error";
}



string NavigatorImpl::dirTurn(double angle)const {
	if (angle < 180)return "left";
	else return "right";
}

void NavigatorImpl::del(vector<Node *>&OPENTable, vector<Node *>&Trash, Node*start, Node*end)const {
	for (unsigned int a = 0; a < OPENTable.size(); a++)
		Trash.push_back(OPENTable[a]);
	for (unsigned int a = 0; a < Trash.size(); a++)
		delete Trash[a];
	delete start;
	delete end;
}

//////////////////////////////////////////////////////////////////////////////

NavigatorImpl::NavigatorImpl()
{
}

NavigatorImpl::~NavigatorImpl()
{
}



//******************** Navigator functions ************************************

// These functions simply delegate to NavigatorImpl's functions.
// You probably don't want to change any of this code.

Navigator::Navigator()
{
	m_impl = new NavigatorImpl;
}

Navigator::~Navigator()
{
	delete m_impl;
}

bool Navigator::loadMapData(string mapFile)
{
	return m_impl->loadMapData(mapFile);
}

NavResult Navigator::navigate(string start, string end, vector<NavSegment>& directions) const
{
	return m_impl->navigate(start, end, directions);
}
