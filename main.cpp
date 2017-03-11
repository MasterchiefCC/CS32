// The main.cpp you can use for testing will replace this file soon.

#include "provided.h"
#include <iostream>
using namespace std;

int main(){
	MapLoader temp;
	temp.load("C:/Users/hasee/Desktop/mapdata.txt");
	AttractionMapper tempA;
	tempA.init(temp);
	GeoCoord Gtemp;
	tempA.getGeoCoord("1067 Broxton Avenue", Gtemp);

	printf("%.7f, %.7f\n", Gtemp.latitude, Gtemp.longitude);

	SegmentMapper tempS;
	tempS.init(temp);
	string test1 = "34.0604893";
	string test2 = "-118.4460593";
	GeoCoord Gtemp2(test1, test2);
	vector<StreetSegment>testV=tempS.getSegments(Gtemp2);
	for (int a = 0; a < testV.size(); a++) {
		cout << testV[a].streetName << '\t';
		printf("%.7f, %.7f\t", testV[a].segment.start.latitude, testV[a].segment.start.longitude);
		printf("%.7f, %.7f\n", testV[a].segment.end.latitude, testV[a].segment.end.longitude);
	}


	return 0;
}
