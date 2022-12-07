/** MinimumSpanningTree.cpp (1) */

#include "MinimumSpanningTree.h"

void MinimumSpanningTree::fprintDistMtrx(ostream& fout)
{
	double** ppDistMtrx;
	Vertex* pVrtxArray;
	int num_nodes;
	double dist;
	int vID;
	string vName;
	pVrtxArray = graph.getpVrtxArray();
	num_nodes = getNumVertices();
	ppDistMtrx = getppDistMtrx();
	fout << "\nDistance Matrix of Graph (" << graph.getName() << ") :" << endl;
	fout << " |";
	for (int i = 0; i < num_nodes; i++) {
		vName = pVrtxArray[i].getName();
		fout << setw(5) << vName;
	}
	fout << endl;
	fout << "-------+";
	for (int i = 0; i < num_nodes; i++) {
		fout << "-----";
	}
	fout << endl;
	for (int i = 0; i < num_nodes; i++) {
		vName = pVrtxArray[i].getName();
		fout << setw(5) << vName << " |";
		for (int j = 0; j < num_nodes; j++) {
			dist = ppDistMtrx[i][j];
			if (dist == PLUS_INF)
				fout << " +oo";
			else
				fout << setw(5) << dist;
		}
		fout << endl;
	}
	fout << endl;
}


void MinimumSpanningTree::initDistMtrx()
{
	double** ppDistMtrx;
	int num_nodes;
	Vertex* pVrtxArray;
	EdgeList* pAdjLstArray;
	int curVrtx_ID, vrtxID;
	num_nodes = graph.getNumVertices();
	pVrtxArray = graph.getpVrtxArray();
	pAdjLstArray = graph.getpAdjLstArray();
	ppDistMtrx = getppDistMtrx();
	for (int i = 0; i < num_nodes; i++) {
		curVrtx_ID = pVrtxArray[i].getID();
		EdgeItor pe = pAdjLstArray[curVrtx_ID].begin();
		while (pe != pAdjLstArray[curVrtx_ID].end()) {
			vrtxID = (*(*pe).getpVrtx_2()).getID();
			ppDistMtrx[curVrtx_ID][vrtxID] = (*pe).getDistance();
			pe++;
		}
		ppDistMtrx[curVrtx_ID][curVrtx_ID] = 0.0;
	} // end for
}

enum VertexStatus{ NOT_SELECTED, SELECTED };
void MinimumSpanningTree::PrimJarnikMST(Vertex* pStart)
{
	int num_nodes, num_edges;
	Vertex vStart, * pVrtxArray;
	EdgeList* pAdjLstArray;
	int curVrtx_ID, vrtxID;
	double** ppDistMtrx;
	double* pDist;
	int start_vid, min_id, dist, min_dist, min_dist_org, min_dist_end, end_ID;
	VertexStatus* pVrtxStatus;
	Graph::Edge* pParentEdge, edge, min_edge; // edge that connects this node to the cloud
	std::list<Graph::Edge> selectedEdgeLst;
	std::list<Graph::Edge>::iterator edgeItor;
	num_nodes = graph.getNumVertices();
	pVrtxArray = graph.getpVrtxArray();
	pAdjLstArray = graph.getpAdjLstArray();
	initDistMtrx(); ppDistMtrx = getppDistMtrx();
	pDist = new double[num_nodes];
	pVrtxStatus = new VertexStatus[num_nodes];
	pParentEdge = new Graph::Edge[num_nodes];
	for (int i = 0; i < num_nodes; i++)
	{
		pDist[i] = PLUS_INF;
		pVrtxStatus[i] = NOT_SELECTED;
		pParentEdge[i] = Graph::Edge();
	}

	start_vid = pStart->getID();
	fout << "Start node : " << pStart->getName() << endl;
	pDist[start_vid] = 0;
	selectedEdgeLst.clear();
	for (int round = 0; round < num_nodes; round++)
	{
		min_dist = PLUS_INF;
		min_id = -1;
		for (int n = 0; n < num_nodes; n++)
		{
			if ((pVrtxStatus[n] == NOT_SELECTED) && (pDist[n] < min_dist)) {
				min_dist = pDist[n];
				min_id = n;
			}
		}
		if (min_id == -1)
		{
			fout << "Error in finding Prim-Jarnik's algorithm !!";
			break;
		}pVrtxStatus[min_id] = SELECTED;
		EdgeItor pe = pAdjLstArray[min_id].begin(); while (pe != pAdjLstArray[min_id].end())
		{
			end_ID = (*(*pe).getpVrtx_2()).getID();
			dist = (*pe).getDistance();
			if ((pVrtxStatus[end_ID] == NOT_SELECTED) && (dist <= pDist[end_ID])) {
				pDist[end_ID] = dist;
				pParentEdge[end_ID] = *pe;
			}
			pe++;
		}
		if (min_id != start_vid) {
			min_edge = pParentEdge[min_id];
			selectedEdgeLst.push_back(min_edge);
			fout << " => min_edge (" << pParentEdge[min_id] << ") is selected" << endl;
		}
		fout << "Dist after round [" << setw(2) << round << "] : ";
		for (int i = 0; i < num_nodes; i++) {
			if (pDist[i] == PLUS_INF)
				fout << " +oo ";
			else
				fout << setw(4) << pDist[i] << " ";
		}
		fout << endl;
	} // end for

	fout << "\nEnd of finding Minimum Spanning Tree by Prim-Jarnik's Algorithm;"; 
	fout << " selectedEdgeLst.size = " << selectedEdgeLst.size() << endl;
	fout << "Selected edges: " << endl;
	edgeItor = selectedEdgeLst.begin();
	int cnt = 0;
	while (edgeItor != selectedEdgeLst.end())
	{
		fout << *edgeItor << ", ";
		edgeItor++;
		if ((++cnt % 5) == 0)
			fout << endl;
	} // end while 
	fout << endl;
}
