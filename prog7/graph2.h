// graph.h <Starter Code>
// < Your name >
//
// Basic graph class using adjacency matrix representation.  Currently
// limited to a graph with at most 100 vertices.
//
// University of Illinois at Chicago
// CS 251: Fall 2021
// Project #7 - Openstreet Maps
//

#pragma once

#include <iostream>
#include <stdexcept>
#include <vector>
#include <set>
#include <unordered_map>

using namespace std;

template<typename VertexT, typename WeightT>
class graph {
	private:

		set<VertexT> vertices;
		unordered_map<VertexT, unordered_map<VertexT, WeightT> > adjList;

		int numEdge;

	public:

		graph(): numEdge(0) {};

		graph& operator=(const graph& other) {

			if (this == &other) {
				return *this;
			} else {

				this->numEdge = other.NumEdges();
				this->vertices = other.vertices;
				this->adjList = other.adjList;

				return *this;
			}
		}

		//
		// NumVertices
		//
		// Returns the # of vertices currently in the graph.
		//
		int NumVertices() const {
			return static_cast<int>(this->vertices.size());
		}

		//
		// NumEdges
		//
		// Returns the # of edges currently in the graph.
		//
		int NumEdges() const {

			return this->numEdge;
		}

		//
		// addVertex
		//
		// Adds the vertex v to the graph if there's room, and if so
		// returns true.  If the graph is full, or the vertex already
		// exists in the graph, then false is returned.
		//
		bool addVertex(VertexT v) {
			if (!vertices.count(v)) {  // already exists
				//unordered_map<VertexT, WeightT> map;
				pair<VertexT, unordered_map<VertexT, WeightT> > p;
				p.first = v;
				//p.second = map;

				this->vertices.insert(v);
				this->adjList.insert(p);

				return true;
			} else {
				return false;
			}


		}

		//
		// addEdge
		//
		// Adds the edge (from, to, weight) to the graph, and returns
		// true.  If the vertices do not exist or for some reason the
		// graph is full, false is returned.
		//
		// NOTE: if the edge already exists, the existing edge weight
		// is overwritten with the new edge weight.
		//
		bool addEdge(VertexT from, VertexT to, WeightT weight) {

			// vertex dont exist
			if (vertices.count(from) && vertices.count(to)) {
				return false;
			} else {
				adjList[from][to] = weight;
				return true;
			}

		}

		//
		// getWeight
		//
		// Returns the weight associated with a given edge.  If
		// the edge exists, the weight is returned via the reference
		// parameter and true is returned.  If the edge does not
		// exist, the weight parameter is unchanged and false is
		// returned.
		//
		bool getWeight(VertexT from, VertexT to, WeightT& weight) const {

			if ((!vertices.count(from) || !vertices.count(to)) || !adjList.at(from).count(to)) {
				return false;
			} else {
				weight = adjList.at(from).at(to);
				return true;
			}

		}

		//
		// neighbors
		//
		// Returns a set containing the neighbors of v, i.e. all
		// vertices that can be reached from v along one edge.
		// Since a set is returned, the neighbors are returned in
		// sorted order; use foreach to iterate through the set.
		//
		set<VertexT> neighbors(VertexT v) const {
			set<VertexT> S;

			if (vertices.count(v) > 0) {

				for (const auto& item : adjList.at(v)) {
					S.insert(item.first);
				}

			}

			return S;
		}

		//
		// getVertices
		//
		// Returns a vector containing all the vertices currently in
		// the graph.
		//
		vector<VertexT> getVertices() const {
			vector<VertexT> vVec;

			for (const auto& item : vertices) {
				vVec.push_back(item);
			}

			return vVec;  // returns a copy:
		}

		//
		// dump
		//
		// Dumps the internal state of the graph for debugging purposes.
		//
		// Example:
		//    graph<string,int>  G(26);
		//    ...
		//    G.dump(cout);  // dump to console
		//
		void dump(ostream& output) const {
			output << "***************************************************" << endl;
			output << "********************* GRAPH ***********************" << endl;

			output << "**Num vertices: " << this->NumVertices() << endl;
			output << "**Num edges: " << this->NumEdges() << endl;

			output << endl;
			output << "**Vertices:" << endl;

			int i = 0;
			for (const auto& vertex : vertices) {
				output << " " << i << ". " << vertex << endl;
				++i;
			}

			output << endl;
			output << "**Edges:" << endl;

			int row = 0;
			for (const auto& from : vertices) {
				output << " row " << row << ": ";

				for (const auto& to : vertices) {
					WeightT w;
					output << (this->getWeight(from,to, w)) ? "(T," << w << ") ": "F ";
				}
				output << endl;
				++row;

			}

			output << "**************************************************" << endl;
		}
};
