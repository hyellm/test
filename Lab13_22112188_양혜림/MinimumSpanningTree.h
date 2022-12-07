/** MinimumSpanningTree.h (1) */

#ifndef MINIMUMSPANNINGTREE_H
#define MINIMUMSPANNINGTREE_H

#include <iostream>
#include "Graph.h"
typedef Graph::Vertex Vertex;
typedef Graph::Edge Edge;
typedef std::list<Graph::Vertex> VrtxList;
typedef std::list<Graph::Edge> EdgeList;
typedef std::list<Graph::Vertex>::iterator VrtxItor;
typedef std::list<Graph::Edge>::iterator EdgeItor;
using namespace std;

class MinimumSpanningTree
{
private:
	Graph& graph;
	ostream& fout;
	double** ppDistMtrx;
protected:
	bool isValidvID(int vid) { return graph.isValidVrtxID(vid); }
	int getNumVertices() { return graph.getNumVertices(); }
public:
	MinimumSpanningTree(Graph& g, ostream& ofStr)
		:graph(g), fout(ofStr) // constructor
	{
		int num_nodes;
		num_nodes = g.getNumVertices();
		// initialize DistMtrx
		for (int i = 0; i < num_nodes; i++)
			ppDistMtrx = new double* [num_nodes];
		for (int i = 0; i < num_nodes; i++)
			ppDistMtrx[i] = new double[num_nodes];
		for (int i = 0; i < num_nodes; i++) {
			for (int j = 0; j < num_nodes; j++)
			{
				ppDistMtrx[i][j] = PLUS_INF;
			}
		}
		initDistMtrx();
		fout << "Distance Matrix after initialization :" << endl;
		fprintDistMtrx(fout);
	} // end of constructor
	void initialize();
	void initDistMtrx();
	void fprintDistMtrx(ostream& fout);
	void KruskalMST(); // implementation of Kruskal MST alrogithm
	void PrimJarnikMST(Vertex* pStart); // implementation of PrimJarnikMST algorithm
	Graph& getGraph() { return graph; }
	double** getppDistMtrx() { return ppDistMtrx; }
};

#endif