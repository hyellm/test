/* -- DepthFirstSearch.h -- */

#ifndef GRAPH__H
#define GRAPH__H

#include <iostream>
#include "Graph.h"
using namespace std;

typedef Graph::Vertex Vertex;
typedef Graph::Edge Edge;
typedef std::list<Graph::Vertex> VrtxList;
typedef std::list<Graph::Vertex>::iterator VertexItor;
typedef std::list<Graph::Edge> EdgeList;
typedef std::list<Graph::Edge>::iterator EdgeItor;

class DepthFirstSearch
{
protected:
	Graph& graph;
	Vertex start;
	bool done; // flag of search done
protected:
	void initialize();
	void dfsTraversal(Vertex& v, Vertex& target, VrtxList& path);
	virtual void traverseDiscovery(const Edge& e, const Vertex& from) { }
	virtual void traverseBack(const Edge& e, const Vertex& from) { }
	virtual void finishVisit(const Vertex& v) {}
	virtual bool isDone() const { return done; }
	// marking utilities
	void visit(Vertex& v);
	void visit(Edge& e);
	void unvisit(Vertex& v);
	void unvisit(Edge& e);
	bool isVisited(Vertex& v);
	bool isVisited(Edge& e);
	void setEdgeStatus(Edge& e, EdgeStatus es);
	EdgeStatus getEdgeStatus(Edge& e);
public:
	DepthFirstSearch(Graph& g);
	void findPath(Vertex& s, Vertex& t, VrtxList& path);
	Graph& getGraph() { return graph; }
	void showConnectivity(ostream& fout);
private:
	VrtxStatus* pVrtxStatus;
	EdgeStatus** ppEdgeStatus;
	double** ppDistMtrx; // two dimensional array; table of distance[v1][v2]
}; // end of class DepthFirstSearch

#endif