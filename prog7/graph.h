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
#include <set>
#include <stdexcept>
#include <unordered_map>
#include <vector>

using namespace std;

template <typename VertexT, typename WeightT>
class graph {
 private:
  set<VertexT> vertices;
  unordered_map<VertexT, unordered_map<VertexT, WeightT> > adjList;

 public:
  graph() = default;

  graph& operator=(const graph& other) {
    if (this == &other) {
      return *this;
    }

    this->vertices = other.vertices;
    this->adjList = other.adjList;

    return *this;
  }

  //
  // NumVertices
  //
  // Returns the # of vertices currently in the graph.
  //
  int NumVertices() const { return static_cast<int>(this->vertices.size()); }

  //
  // NumEdges
  //
  // Returns the # of edges currently in the graph.
  //
  int NumEdges() const {
    int count = 0;

    for (const auto& source : vertices) {
      count += adjList.at(source).size();
    }

    return count;
  }

  //
  // addVertex
  //
  // Adds the vertex v to the graph if there's room, and if so
  // returns true.  If the graph is full, or the vertex already
  // exists in the graph, then false is returned.
  //
  bool addVertex(VertexT v) {
    if (vertices.count(v)) {  // already exists
      return false;
    }

    unordered_map<VertexT, WeightT> map;
    pair<VertexT, unordered_map<VertexT, WeightT> > p;
    p.first = v;
    p.second = map;

    this->vertices.insert(v);
    this->adjList.insert(p);

    return true;
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
    if (!vertices.count(from) || !vertices.count(to)) {
      // cout << "beep2" << endl;
      return false;
    }

    // edge already Exists
    adjList[from][to] = weight;

    /*
    if (adjList.at(from).count(to)) {

            cout << "exists" << endl;
            adjList.at(from).at(to) = weight;

    } else { // add new edge
            //cout << "add new stuff" << endl;

            pair<VertexT, WeightT> p;
            p.first = to;
            p.second = weight;

            adjList.at(from).insert(p);

    }*/

    return true;
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
    if (!vertices.count(from) || !vertices.count(to)) {
      return false;
    }

    if (adjList.at(from).count(to)) {
      weight = adjList.at(from).at(to);
      return true;
    } else {
      return false;
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

    if (vertices.count(v)) {
      for (auto item : adjList.at(v)) {
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
    // for (int i = 0; i < this->NumVertices(); ++i) {
    //	output << " " << i << ". " << this->Vertices[i] << endl;
    //}
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
        if (!this->adjList.at(from).count(to)) {
          output << "F ";
        } else {
          output << "(T," << this->adjList.at(from).at(to) << ") ";
        }
      }
      output << endl;
      ++row;
    }

    output << "**************************************************" << endl;
  }
};
