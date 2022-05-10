/* ---------------------------------------------
Program 7: Openstreet Maps

Course: CS 251, Spring 2022. Thurs 12pm lab
System: Wsl Ubuntu with CLion
Author: Zain Ali
---------------------------------------------
*/

#include <iomanip> /*setprecision*/
#include <iostream>
#include <map>
#include <string>
#include <vector>
//#include <cstdlib>
//#include <cstring>
#include <cassert>
//#include <cmath>
#include <cfloat>
#include <queue>

#pragma once

#include "dist.h"
#include "graph.h"
#include "osm.h"
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

//
// Implement your creative component application here
// TO DO: add arguments
//
void creative() {
	// I think im good on creative bruh
	// big thank to the TA grading this
}

/** operator <<\n
 * @param os: usually cout
 * @param b: building to print info\n
 * << overload for BuildingInfo
 * */
ostream& operator<<(ostream& os, const BuildingInfo& b) {
  os << b.Fullname << endl << " ";
  os << "(" << b.Coords.Lat << ", " << b.Coords.Lon << ")" << endl;
  return os;
}

/** operator <<\n
 * @param os: usually cout
 * @param c: building to print info\n
 * << overload for Coordinates
 * */
ostream& operator<<(ostream& os, const Coordinates& c) {
  os << c.ID << endl << " ";
  os << "(" << c.Lat << ", " << c.Lon << ")" << endl;
  return os;
}

/** getBuilding\n
 * @param lookup: search query
 * @param Buildings: data set of buildings to search thru\n
 * @return nullptr if not found, Building* with matching query
 * */
BuildingInfo* getBuilding(string lookup, vector<BuildingInfo>& Buildings) {
  for (auto& build : Buildings) {
    if (build.Abbrev == lookup) {
      return &build;
    }
  }

  for (auto& build : Buildings) {
    if (build.Fullname.find(lookup) != string::npos) {
      return &build;
    }
  }

  return nullptr;
}

/** DijkstraShortestPath\n
 * @param startV: ID of start node
 * @param G: Graph to search thru
 * @param predMap: fills with map of nodes to next node to get to start\n
 * */
void DijkstraShortestPath(long long startV, graph<long long, double>& G,
                          map<long long, long long>& predMap) {
  predMap.clear();

  map<long long, double> distanceMap;

  auto comparison = [&](long long& p1, long long& p2) -> bool const {
    return distanceMap.at(p1) > distanceMap.at(p2);
  };

  priority_queue<long long, vector<long long>, decltype(comparison)>
      unvisitedQueue(comparison);

  for (const auto& currentV : G.getVertices()) {
    distanceMap[currentV] = DBL_MAX;
    // distanceMap[currentV] = (currentV == startV) ? 0 : DBL_MAX;
    predMap[currentV] = -1;
    // predMap[currentV] = (currentV == startV) ? -2 : -1;
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

    for (const auto& vertex : neighbor) {
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
bool buildingPrompt(BuildingInfo*& p1b, BuildingInfo*& p2b,
                    vector<BuildingInfo>& Buildings) {
  string person1Building, person2Building;
  // returns false to exit application
  // returns true to continue application

  do {
    cout << endl
         << "Enter person 1's building (partial name or abbreviation), or #> ";
    getline(cin, person1Building);

    if (person1Building == "#") {
      return false;
    }

    cout << "Enter person 2's building (partial name or abbreviation)> "
         << endl;
    getline(cin, person2Building);

    p1b = getBuilding(person1Building, Buildings);
    p2b = getBuilding(person2Building, Buildings);

    if (!p1b) {
      cout << " Person 1's building not found" << endl;
      continue;
    }

    if (!p2b) {
      cout << " Person 2's building not found" << endl;
      continue;
    }
  } while (!p1b || !p2b);

  return true;
}

/** closestSpot\n
 * @param Nodes: map of nodes to their coordinates
 * @param p1b: building to go to
 * @param Buildings: data set of valid buildings\n
 * @returns node closest to p1b on a footway
 * */
long long closestSpot(map<long long, Coordinates>& Nodes, BuildingInfo* p1b,
                      vector<FootwayInfo>& Footways) {
  long long closestSpot = Footways.at(0).Nodes.at(0);
  double minDist =
      distBetween2Points(p1b->Coords.Lat, p1b->Coords.Lon,
                         Nodes.at(closestSpot).Lat, Nodes.at(closestSpot).Lon);

  for (const auto& way : Footways) {
    for (const auto& ID : way.Nodes) {
      Coordinates c2 = Nodes.at(ID);
      double dist =
          distBetween2Points(p1b->Coords.Lat, p1b->Coords.Lon, c2.Lat, c2.Lon);

      if (dist < minDist) {
        minDist = dist;
        closestSpot = ID;
      }
    }
  }

  return closestSpot;
}

/** getClosestBuilding\n
 * @param buildings: dataset of valid buildings
 * @param mid: coordinate we trying to find building close to
 * @param dontVisit: fullnames of buildings we cant visit\n
 * @returns valid building in buildings closest to mid\n
 * this program was low key kinda mid
 * */
BuildingInfo* getClosestBuilding(vector<BuildingInfo>& buildings,
                                 const Coordinates& mid,
                                 set<string>& dontVisit) {
  BuildingInfo* closestBuilding = nullptr;
  Coordinates c2;

  double minDist =
      DBL_MAX;  // = distBetween2Points( point.Lat, point.Lon, c2.Lat, c2.Lon);

  for (auto& item : buildings) {
    if (dontVisit.count(item.Fullname)) {
      continue;
    }

    c2 = item.Coords;
    double dist = distBetween2Points(c2.Lat, c2.Lon, mid.Lat, mid.Lon);

    if (dist < minDist) {
      minDist = dist;
      closestBuilding = &item;
    }
  }

  return closestBuilding;
}

/** printPathString\n
 * @param destClosestNod: place we go
 * @param p1ClosestNod: place we start
 * @param Nodes: map of Nodes to coordinates
 * @param G: Graph we doing a walk on
 * @param predMap: map of node to next node to go to\n
 * prints paths of person skrrrt from p1closestNod to destClosestNod
 * */
void printPathString(long long& destClosestNod, long long& p1ClosestNod,
                     map<long long, long long>& predMap,
                     map<long long, Coordinates>& Nodes,
                     graph<long long, double>& G) {
  string path = "";

  long long curr = destClosestNod;
  double dist = 0;

  while (curr != p1ClosestNod) {

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
bool navigate(long long& p1ClosestNod, long long& p2ClosestNod,
              long long& destClosestNod, map<long long, Coordinates>& Nodes,
              vector<BuildingInfo>& Buildings, graph<long long, double>& G) {
  // queue<long long> unvisitedQueue;

  map<long long, long long> predMap;

  DijkstraShortestPath(p1ClosestNod, G, predMap);

  if (predMap.at(p2ClosestNod) == -1) {  // unreachable case
    cout << "Sorry, destination unreachable." << endl;
    return true;
  }

  map<long long, long long> predMap2;
  DijkstraShortestPath(p2ClosestNod, G, predMap2);

  if (predMap.at(destClosestNod) == -1 || predMap2.at(destClosestNod) == -1) {
    cout << "At least one person was unable to reach the destination building. "
            "Finding next closest building..."
         << endl;
    return false;
  }

  cout << "Person 1's distance to dest: ";
  printPathString(destClosestNod, p1ClosestNod, predMap, Nodes, G);

  cout << "Person 2's distance to dest: ";
  printPathString(destClosestNod, p2ClosestNod, predMap2, Nodes, G);

  return true;
}

/** printPathString\n
 * like the hole app thing is in this funcy
 * */
void application(map<long long, Coordinates>& Nodes,
                 vector<FootwayInfo>& Footways, vector<BuildingInfo>& Buildings,
                 graph<long long, double>& G) {
  BuildingInfo* p1b = nullptr;
  BuildingInfo* p2b = nullptr;  // = getBuilding(person1Building, Buildings);

  set<string> dontVisit;

  while (true) {
    if (dontVisit.empty() && !buildingPrompt(p1b, p2b, Buildings)) {
      break;
    }

    long long p1ClosestNod, p2ClosestNod, destClosestNod;

    Coordinates mid = centerBetween2Points(p2b->Coords.Lat, p2b->Coords.Lon,
                                           p1b->Coords.Lat, p1b->Coords.Lon);
    BuildingInfo* midBuilding = getClosestBuilding(Buildings, mid, dontVisit);

    if (p1b->Fullname == midBuilding->Fullname) {
      midBuilding = p2b;
    }

    p1ClosestNod = closestSpot(Nodes, p1b, Footways);
    p2ClosestNod = closestSpot(Nodes, p2b, Footways);
    destClosestNod = closestSpot(Nodes, midBuilding, Footways);

    if (dontVisit.empty()) {
      cout << "Person 1's point:" << endl
           << " " << *p1b << "Person 2's point:" << endl
           << " " << *p2b;
      cout << "Destination Building:" << endl << " " << *midBuilding << endl;
      cout << "Nearest P1 node:" << endl << " " << Nodes.at(p1ClosestNod);
      cout << "Nearest P2 node:" << endl << " " << Nodes.at(p2ClosestNod);
    } else {
      cout << "New destination building:" << endl
           << " " << *midBuilding << endl;
    }

    cout << "Nearest destination node:" << endl
         << " " << Nodes.at(destClosestNod) << endl;

    if (!navigate(p1ClosestNod, p2ClosestNod, destClosestNod, Nodes, Buildings,
                  G)) {
      dontVisit.insert(midBuilding->Fullname);
      continue;
    } else {
      dontVisit.clear();
    }
  }
}

int main() {
  // maps a Node ID to it's coordinates (lat, lon)
  map<long long, Coordinates> Nodes;
  // info about each footway, in no particular order
  vector<FootwayInfo> Footways;
  // info about each building, in no particular order
  vector<BuildingInfo> Buildings;
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
  assert(nodeCount == (int)Nodes.size());
  assert(footwayCount == (int)Footways.size());
  assert(buildingCount == (int)Buildings.size());

  cout << endl;
  cout << "# of nodes: " << Nodes.size() << endl;
  cout << "# of footways: " << Footways.size() << endl;
  cout << "# of buildings: " << Buildings.size() << endl;

  //
  // TO DO: build the graph, output stats:
  //

  graph<long long, double> G;

  for (const auto& nod : Nodes) {
    G.addVertex(nod.first);
  }

  for (const auto& path : Footways) {
    for (int i = 0; i < path.Nodes.size() - 1; i++) {
      Coordinates c1 = Nodes.at(path.Nodes.at(i));
      Coordinates c2 = Nodes.at(path.Nodes.at(i + 1));

      G.addEdge(path.Nodes.at(i), path.Nodes.at(i + 1),
                distBetween2Points(c1.Lat, c1.Lon, c2.Lat, c2.Lon));
      G.addEdge(path.Nodes.at(i + 1), path.Nodes.at(i),
                distBetween2Points(c1.Lat, c1.Lon, c2.Lat, c2.Lon));
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
    application(Nodes, Footways, Buildings, G);
  } else if (userInput == "c") {
    creative();
  }
  //
  // done:
  //
  cout << "** Done **" << endl;
  return 0;
}
