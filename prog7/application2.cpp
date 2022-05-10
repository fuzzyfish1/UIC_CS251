/* ---------------------------------------------
Program 7: Openstreet Maps

Course: CS 251, Spring 2022. Thurs 12pm lab
System: Wsl Ubuntu with CLion
Author: Zain Ali
---------------------------------------------
*/

#include <iostream>
#include <iomanip>  /*setprecision*/
#include <string>
#include <vector>
#include <map>
//#include <cstdlib>
//#include <cstring>
#include <cassert>
//#include <cmath>
#include <queue>
#include <cfloat>

#pragma once

#include "tinyxml2.h"
#include "dist.h"
#include "osm.h"
#include "graph.h"

using namespace std;
using namespace tinyxml2;

//
// Implement your creative component application here
// TO DO: add arguments
//
void creative() {

}

/** operator <<\n
 * @param os: usually cout
 * @param b: building to print info\n
 * << overload for BuildingInfo
 * */
void printBuilding(const BuildingInfo& b) {
	cout  << b.Fullname << "\n (" << b.Coords.Lat << ", " << b.Coords.Lon << ")\n";
}

/** operator <<\n
 * @param os: usually cout
 * @param c: building to print info\n
 * << overload for Coordinates
 * */
void printCoord(const Coordinates& c) {
	cout << c.ID << "\n (" << c.Lat << ", " << c.Lon << ")\n";
}

/** getBuilding\n
 * @param lookup: search query
 * @param Buildings: data set of buildings to search thru\n
 * @return nullptr if not found, Building* with matching query
 * */
BuildingInfo getBuilding(string lookup, vector <BuildingInfo>& Buildings) {

	for (auto& build: Buildings) {
		if (build.Abbrev == lookup || build.Fullname.find(lookup) != string::npos) {
			return build;
		}
	}

	return BuildingInfo();

}

/** DijkstraShortestPath\n
 * @param startV: ID of start node
 * @param G: Graph to search thru
 * @param predMap: fills with map of nodes to next node to get to start\n
 * */
void DijkstraShortestPath(long long startV, graph<long long, double>& G, map<long long, long long>& predMap) {

	predMap.clear();

	map<long long, double> distanceMap;

	auto comparison = [&] (long long& p1, long long& p2) -> bool const {
		return distanceMap.at(p1) > distanceMap.at(p2);
	};

	priority_queue<long long, vector<long long>, decltype(comparison)> unvisitedQueue(
			comparison);

	for (const auto& currentV: G.getVertices()) {
		distanceMap[currentV] = DBL_MAX;
		//distanceMap[currentV] = (currentV == startV) ? 0 : DBL_MAX;
		predMap[currentV] = 1;
		//predMap[currentV] = (currentV == startV) ? -2 : -1;
		unvisitedQueue.push(currentV);
	}

	predMap[startV] = -2;
	// startV has a distance of 0 from itself
	distanceMap[startV] = 0;

	while (!unvisitedQueue.empty()) {

		// Visit vertex with minimum distance from startV
		long long currentV = unvisitedQueue.top();

		unvisitedQueue.pop();

		auto neighbor = G.neighbors(currentV);

		double currDist = distanceMap.at(currentV);

		for (const auto& vertex: neighbor) {

			double edgeWeight;
			G.getWeight(vertex, currentV, edgeWeight);

			double alternativePathDistance = currDist + edgeWeight;

			// If shorter path from startV to adjV is found,
			// update adjV's distance and predecessor
			if (alternativePathDistance < distanceMap.at(vertex)) {

				distanceMap.at(vertex) = alternativePathDistance;
				unvisitedQueue.push(vertex);
				predMap.at(vertex) = currentV;
			}
		}
	}
}

/** buildingPrompt\n
 * @param p1b: building p1 at
 * @param p2b: building p2 at
 * @param Buildings: data set of valid buildings\n
 * @returns if user wants to quit or not
 * */
bool buildingPrompt(BuildingInfo& p1b,
					BuildingInfo& p2b, vector <BuildingInfo>& Buildings) {

	string person1Building, person2Building;
	// returns false to exit application
	// returns true to continue application

	do {
		cout << "\nEnter person 1's building (partial name or abbreviation), or #> ";
		getline(cin, person1Building);

		if (person1Building == "#") {
			return false;
		}

		cout << "Enter person 2's building (partial name or abbreviation)> \n";
		getline(cin, person2Building);

		p1b = getBuilding(person1Building, Buildings);
		p2b = getBuilding(person2Building, Buildings);

		if (p1b.Fullname.empty() || p2b.Fullname.empty()) {
			cout << " Person " << ((p1b.Fullname.empty()) ? 1: 2) << "'s building not found\n";
			continue;
		} else {
			return true;
		}

	} while (true);
}

/** closestSpot\n
 * @param Nodes: map of nodes to their coordinates
 * @param p1b: building to go to
 * @param Buildings: data set of valid buildings\n
 * @returns node closest to p1b on a footway
 * */
long long closestSpot(map<long long, Coordinates>& Nodes, BuildingInfo p1b, vector <FootwayInfo>& Footways, long long& thing) {

	double lat1, lon1, lat2, lon2;
	lat1 = p1b.Coords.Lat;
	lon1 = p1b.Coords.Lon;

	long long closestSpot;// = Footways.at(0).Nodes.at(0);
	double minDist = DBL_MAX;

	//for (const auto& way: Footways) {
	for (int i = 0; i < Footways.size(); i++) {

		//for (const auto& ID: Footways.at(i).Nodes) {
		for (int j = 0; j < Footways.at(i).Nodes.size(); j++) {
			Coordinates c2 = Nodes.at(Footways.at(i).Nodes.at(j));

			lat2 = c2.Lat;
			lon2 = c2.Lon;

			double dist = distBetween2Points(lat1, lon1, lat2, lon2);

			if (dist < minDist) {
				minDist = dist;
				closestSpot = Footways.at(i).Nodes.at(j);
			}
		}

	}

	thing = closestSpot;

	return closestSpot;
}

/** getClosestBuilding\n
 * @param buildings: dataset of valid buildings
 * @param mid: coordinate we trying to find building close to
 * @param dontVisit: fullnames of buildings we cant visit\n
 * @returns valid building in buildings closest to mid\n
 * this program was low key kinda mid
 * */
BuildingInfo getClosestBuilding(vector <BuildingInfo>& buildings, BuildingInfo& p1b, BuildingInfo& p2b, set<string>& dontVisit) {

	Coordinates mid = centerBetween2Points( p2b.Coords.Lat, p2b.Coords.Lon, p1b.Coords.Lat, p1b.Coords.Lon);

	BuildingInfo closestBuilding;
	Coordinates c2;

	double minDist = DBL_MAX;// = distBetween2Points( point.Lat, point.Lon, c2.Lat, c2.Lon);

	//for (auto& item: buildings) {
	for (int i = 0; i < buildings.size(); i++) {

		c2 = buildings.at(i).Coords;
		double dist = distBetween2Points(c2.Lat, c2.Lon, mid.Lat, mid.Lon);

		if (!dontVisit.count(buildings.at(i).Fullname) && dist < minDist) {
			minDist = dist;
			closestBuilding = buildings.at(i);
		}
	}

	if (p1b.Fullname == closestBuilding.Fullname) {
		closestBuilding = p2b;
	}

	return closestBuilding;
}
/*
void getStartEnd(long long& p1ClosestNod, long long& p2ClosestNod, long long& destClosestNod, BuildingInfo*& p1b,
				 BuildingInfo*& p2b, vector <BuildingInfo>& Buildings, vector <FootwayInfo>& Footways,
				 map<long long, Coordinates>& Nodes, set<long long>& dontVisit) {

	// destination stuff
	Coordinates mid = centerBetween2Points(p1b->Coords.Lat, p1b->Coords.Lon, p2b->Coords.Lat, p2b->Coords.Lon);
	BuildingInfo* midBuilding = getClosestBuilding(Buildings, mid, dontVisit);

	p1ClosestNod = closestSpot(Nodes, p1b, Footways);
	p2ClosestNod = closestSpot(Nodes, p2b, Footways);
	destClosestNod = closestSpot(Nodes, midBuilding, Footways);

	if (dontVisit.empty()) {
		cout << "Person 1's point:" << endl << " " << *p1b << "Person 2's point:" << endl << " " << *p2b;
		cout << "Destination Building:" << endl << " " << *midBuilding << endl;
		cout << "Nearest P1 node:" << endl << " " << Nodes.at(p1ClosestNod);
		cout << "Nearest P2 node:" << endl << " " << Nodes.at(p2ClosestNod);
	} else {
		cout << "New destination building:" << endl << " " << *midBuilding << endl;
	}
	cout << "Nearest destination node:" << endl << " " << Nodes.at(destClosestNod) << endl;

}
*/

/** printPathString\n
 * @param destClosestNod: place we go
 * @param p1ClosestNod: place we start
 * @param Nodes: map of Nodes to coordinates
 * @param G: Graph we doing a walk on
 * @param predMap: map of node to next node to go to\n
 * prints paths of person skrrrt from p1closestNod to destClosestNod
 * */
void printPathString(long long& destClosestNod, long long& p1ClosestNod,
					 map<long long, long long>& predMap, map<long long, Coordinates>& Nodes,
					 graph<long long, double>& G) {
	string path = "";

	long long curr = destClosestNod;
	double dist = 0;

	/*
	 *
	 * if (path2.size() == 0) {
	 * 		cout << destination
	 * }
	 *
	 *
	 *
	 * */

	while (curr != p1ClosestNod) {
/*
		if (predMap.at(curr) == -1) {
			cout << "Sorry, destination unreachable." << endl;
			return;
		}
*/
		path.insert(0, "->" + to_string(curr));

		double weight = -1;
		long long next = predMap.at(curr);

		G.getWeight(curr, next, weight);
		dist += weight;

		curr = next;

	}

	cout << dist << " miles\n"
		 << "Path: " << Nodes.at(p1ClosestNod).ID << path << "\n"
		 << "\n";

}

/** printPathString\n
 * @param p1ClosestNod: place p1 start
 * @param p2ClosestNod: place p2 start
 * @param destClosestNod: place we go
 * @param Nodes: map nod ID to location, like google map but on crack
 * @param G: Graph we doing a walk on
 * @param predMap: Bingus\n
 * prints paths of person skrrrt from p1closestNod to destClosestNod
 * */
bool navigate(long long& p1ClosestNod, long long& p2ClosestNod, long long& destClosestNod,
			  map<long long, Coordinates>& Nodes,
			  vector <BuildingInfo>& Buildings, graph<long long, double>& G) {
	//queue<long long> unvisitedQueue;

	map<long long, long long> predMap;

	DijkstraShortestPath(p1ClosestNod, G, predMap);

	if (predMap.at(p2ClosestNod)) { // unreachable case
		cout << "Sorry, destination unreachable." << endl;
		return true;
	}

	map<long long, long long> predMap2;
	DijkstraShortestPath(p2ClosestNod, G, predMap2);

	if (predMap.at(destClosestNod) || predMap2.at(destClosestNod)) {
		cout << "At least one person was unable to reach the destination building. Finding next closest building..." << endl;
		return false;
	}

	cout << "Person 1's distance to dest: ";
	printPathString(destClosestNod, p1ClosestNod, predMap, Nodes, G);

	cout << "Person 2's distance to dest: ";
	printPathString(destClosestNod, p2ClosestNod, predMap2, Nodes, G);

/*
	string path2 = "";

	curr = destClosestNod;
	double dist2 = 0;

	while (curr != p2ClosestNod) {

		//cout << "beep" << endl;

		if (predMap.at(curr) == -1) {
			cout << "Sorry, destination unreachable." << endl;
			return true;
		}

		//path = to_string(curr) + path;
		path2.insert(0, "->" + to_string(curr));

		double weight = -1;
		G.getWeight(curr, predMap2.at(curr), weight);
		dist2 += weight;

		curr = predMap2.at(curr);
	}
*/
/*
	cout << "Person 1's distance to dest: " << dist1 << " miles\n"
		 << "Path: " << Nodes.at(p1ClosestNod).ID << path1 << "\n"
		 << "\n";

	cout << "Person 2's distance to dest: " << dist2 << " miles\n"
		 << "Path: " << Nodes.at(p2ClosestNod).ID << path2 << "\n"
		 << "\n";
*/
	return true;

}

/** printPathString\n
 * like the hole app thing is in this funcy
 * */
void application(
		map<long long, Coordinates>& Nodes, vector <FootwayInfo>& Footways,
		vector <BuildingInfo>& Buildings, graph<long long, double>& G) {

	BuildingInfo p1b;// = nullptr;
	BuildingInfo p2b;// = nullptr;// = getBuilding(person1Building, Buildings);

	set<string> dontVisit;

	while (!dontVisit.empty() || buildingPrompt(p1b, p2b, Buildings)) {

		long long p1ClosestNod, p2ClosestNod, destClosestNod;

		BuildingInfo midBuilding = getClosestBuilding(Buildings, p1b, p2b, dontVisit);

		if (dontVisit.empty()) {
			cout << "Person 1's point:\n ";
			printBuilding(p1b);

			cout << "Person 2's point:\n ";
			printBuilding(p2b);

			cout << "Destination Building:\n ";
			printBuilding(midBuilding);

			cout << "\nNearest P1 node:\n ";
			printCoord(Nodes.at(closestSpot(Nodes, p1b, Footways, p1ClosestNod)));

			cout << "Nearest P2 node:\n ";
			printCoord(Nodes.at(closestSpot(Nodes, p2b, Footways, p2ClosestNod)));

		} else {
			cout << "New destination building:\n ";
			printBuilding(midBuilding);
			cout << "\n";
		}

		cout << "Nearest destination node:\n ";
		printCoord(Nodes.at(closestSpot(Nodes, midBuilding, Footways, destClosestNod)));
		cout << "\n";

		if (!navigate(p1ClosestNod, p2ClosestNod, destClosestNod, Nodes, Buildings, G)) {
			dontVisit.insert(midBuilding.Fullname);
			continue;
		}

		dontVisit.clear();

	}
}

int main() {
	// maps a Node ID to it's coordinates (lat, lon)
	map<long long, Coordinates> Nodes;
	// info about each footway, in no particular order
	vector <FootwayInfo> Footways;
	// info about each building, in no particular order
	vector <BuildingInfo> Buildings;
	XMLDocument xmldoc;

	cout << "** Navigating UIC open street map **" << endl;
	cout << endl;
	cout << std::setprecision(8);

	string def_filename = "map.osm";
	string filename = "";

	cout << "Enter map filename> ";
	getline(cin, filename);

	if (filename == "") {
		filename = def_filename;
	}

	//
	// Load XML-based map file
	//
	if (!LoadOpenStreetMap(filename, xmldoc)) {
		cout << "**Error: unable to load open street map." << endl;
		cout << endl;
		return 0;
	}

	//
	// Read the nodes, which are the various known positions on the map:
	//
	int nodeCount = ReadMapNodes(xmldoc, Nodes);

	//
	// Read the footways, which are the walking paths:
	//
	int footwayCount = ReadFootways(xmldoc, Footways);

	//
	// Read the university buildings:
	//
	int buildingCount = ReadUniversityBuildings(xmldoc, Nodes, Buildings);

	//
	// Stats
	//
	assert(nodeCount == (int) Nodes.size());
	assert(footwayCount == (int) Footways.size());
	assert(buildingCount == (int) Buildings.size());

	cout << endl;
	cout << "# of nodes: " << Nodes.size() << endl;
	cout << "# of footways: " << Footways.size() << endl;
	cout << "# of buildings: " << Buildings.size() << endl;


	//
	// TO DO: build the graph, output stats:
	//

	graph<long long, double> G;

	for (const auto& nod: Nodes) {
		if (!G.addVertex(nod.first)) {

			cout << "ERROR" << endl;
		}

	}

	for (const auto& path: Footways) {
		for (int i = 0; i < path.Nodes.size() - 1; i++) {

			Coordinates c1 = Nodes.at(path.Nodes.at(i));
			Coordinates c2 = Nodes.at(path.Nodes.at(i + 1));

			if (!G.addEdge(path.Nodes.at(i), path.Nodes.at(i + 1), distBetween2Points(c1.Lat, c1.Lon, c2.Lat, c2.Lon)) ||
					!G.addEdge(path.Nodes.at(i + 1), path.Nodes.at(i), distBetween2Points(c1.Lat, c1.Lon, c2.Lat, c2.Lon))) {
				cout << "ERROR" << endl;
			}
		}
	}

	cout << "# of vertices: " << G.NumVertices() << endl;
	cout << "# of edges: " << G.NumEdges() << endl;
	cout << endl;

	//
	// Menu
	//
	string userInput;
	cout << "Enter \"a\" for the standard application or "
		 << "\"c\" for the creative component application> ";
	getline(cin, userInput);
	if (userInput == "a") {
		// TO DO: add argument for the graph you make.
		application(Nodes, Footways, Buildings, G);
	} else if (userInput == "c") {
		// TO DO: add arguments
		creative();
	}
	//
	// done:
	//
	cout << "** Done **" << endl;
	return 0;
}
