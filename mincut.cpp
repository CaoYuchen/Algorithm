#pragma once

#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<fstream>
#include<cmath>

#include <cstdlib>
#include <vector>
#include <deque>
#include <map>
#include <limits>
#include <time.h>

using namespace std;
int k;
int source, destination;
int nV;
int nE;
int delta;
int value;
FILE *fp = fopen("output.txt", "a");;



void initD();
void unarycosts();
void pairwise();
void stereo();
void init();
int maxFlow();
int findPath();
void update(int top);

const int SCALE = 450;
const int TOP = 3;
int matrix[TOP][SCALE] = { 0 };
int flow[TOP][SCALE] = { 0 };
int stk[SCALE] = { 0 };
int X[375][450], Y[375][450];
int dist[375][450][64];
int U[375][450][4];
int D[375][450];
int max = 64;
int K = 15;



class AdjacentNode {
public:
	int v, w;
	int edgeCapacity;
	int edgeFlow;
	friend class ResidualGraph;
	friend class MaxFlow;
public:
	int sourceVertex();
	int destinationVertex();
	int complementaryVertexOf(int vertex);
	int checkResidualCapacityOf(int vertex);
	void updateFlowOf(int vertex, int data);
	AdjacentNode(int v, int w, int capacity, int flow);
};

AdjacentNode::AdjacentNode(int v, int w, int capacity, int flow) {
	this->v = v;
	this->w = w;
	this->edgeCapacity = capacity;
	this->edgeFlow = flow;
}

int AdjacentNode::sourceVertex() {
	return v;
}

int AdjacentNode::destinationVertex() {
	return w;
}

int AdjacentNode::complementaryVertexOf(int vertex) {
	if (vertex == this->v) return w;
	else if (vertex == this->w) return v;
}

int AdjacentNode::checkResidualCapacityOf(int vertex) {
	// cout<<"Inside Residual capacity\n";
	if (vertex == this->v) {
		return this->edgeFlow;
	}
	else if (vertex == this->w) {
		//   cout<<capacity<<" "<<flow;
		int res = this->edgeCapacity - edgeFlow;
		// cout<<"Residual capacity:"<<res<<"\n";
		return res;
	}

}
// Module to update flow of either the reverse edge or forward edge of corresponding vertex

void AdjacentNode::updateFlowOf(int vertex, int data) {
	//cout<<"Inside Residual flow\n";
	if (vertex == this->v)
		edgeFlow -= data;
	else if (vertex == this->w)
		edgeFlow += data;
}
// Making data type of list of adjacent vertices and adjacency list as it is used often in program
typedef vector<AdjacentNode> AdjacentVertices;
//Map where key is the vertex and value are list of adjacent vertices
typedef map<int, AdjacentVertices> AdjacenyList;

// Gives residual graph in the form of adjacency list of map of vectors  

class ResidualGraph {
public:
	int V;
	AdjacenyList residualGraph;
	ResidualGraph(int V);
	AdjacentVertices adjacent(int v);
	void addEdge(AdjacentNode e);
};

// Class used to incorporate implementation of Ford Fulkerson algorithm

class MaxFlowMinCut {
public:
	bool *marked;
	AdjacentNode *path;
	int count;
	ResidualGraph *fg;
	vector<int> augmentPath;
	int min(int x, int y);
	int max(int x, int y);
	MaxFlowMinCut(ResidualGraph *rg);
	// Module to get the two cut-sets
	vector<int> getCutSet(ResidualGraph G, int source);
	//Module to compute min cut
	void getMinCut(ResidualGraph *rg, bool path);
	// Module to compute the Augmented path for Ford Fulkerson Algorithm.
	bool hasAugmentPath(ResidualGraph rg);
	// Module to check whether there is augmented path for Capacity Scaling
	bool hasAugmentPathForWidest(ResidualGraph rg);
	// Module to implement variant of Ford Fulkerson using Capacity Scaling Algorithm
	ResidualGraph getMaxFlowWidestPath();
	//Module to print output of the final residual graph
	void writeOutput(ResidualGraph *rg);
	//Module to construct Residual Graph after every iteration.
	ResidualGraph constructResidualGraph(ResidualGraph rg, int currentFlow);
	// Calculation of max flow using ford fulkerson Algorithm
	ResidualGraph getMaxFlow();
};

ResidualGraph::ResidualGraph(int V) {
	this->V = V;
	//cout<<"Inside flow network\n";
}

AdjacentVertices ResidualGraph::adjacent(int v) {
	map<int, AdjacentVertices>::iterator it = residualGraph.find(v);
	if (it != residualGraph.end()) {
		return it->second;
	}

}

void ResidualGraph::addEdge(AdjacentNode e) {
	//e.flow = 0;
	int v = e.sourceVertex();
	//cout<<"Inside add edge\n";
	int w = e.destinationVertex();
	if (residualGraph.empty()) {
		AdjacentVertices adjacentVertexList;
		adjacentVertexList.push_back(e);
		residualGraph.insert(AdjacenyList::value_type(v, adjacentVertexList));
		residualGraph.insert(AdjacenyList::value_type(w, adjacentVertexList));
		// cout<<"Inside empty";
	}
	else {
		if (residualGraph.find(v) == residualGraph.end()) {
			AdjacentVertices adjacentVertexList;
			adjacentVertexList.push_back(e);
			residualGraph.insert(AdjacenyList::value_type(v, adjacentVertexList));
			//   cout<<"Inside v first time";

		}
		else {
			map<int, AdjacentVertices>::iterator it = residualGraph.find(v);
			if (it != residualGraph.end()) {
				//                    flows f = it->second;
				//                    f.push_back(e);
				//                mapGraph.insert(map_test::value_type(v, f));
				residualGraph[v].push_back(e);
			}
		}

		if (residualGraph.find(w) == residualGraph.end()) {
			AdjacentVertices adjacentVertexList;
			adjacentVertexList.push_back(e);
			residualGraph.insert(AdjacenyList::value_type(w, adjacentVertexList));
		}
		else {
			map<int, AdjacentVertices>::iterator it = residualGraph.find(w);
			if (it != residualGraph.end()) {
				//                    flows f = it->second;
				//                    f.push_back(e);
				//                mapGraph.insert(map_test::value_type(w, f));
				residualGraph[w].push_back(e);
			}
		}
	}

}



int MaxFlowMinCut::min(int x, int y) {
	return x < y ? x : y;
}

int MaxFlowMinCut::max(int x, int y) {
	return x > y ? x : y;
}

MaxFlowMinCut::MaxFlowMinCut(ResidualGraph *rg) {
	this->fg = rg;
}
// Module to get the two cut-sets

vector<int> MaxFlowMinCut::getCutSet(ResidualGraph G, int source) {
	marked = (bool *)malloc(sizeof (bool)*(nV + 1));
	for (int i = 1; i <= nV; i++) {
		marked[i] = false;
	}
	vector<int> cutSet;
	//FlowEdge tempEdge[100];
	path = (AdjacentNode *)malloc(sizeof (AdjacentNode)*(nV + 1));
	std::deque<int> bfsQueue;
	bfsQueue.push_front(source);
	marked[source] = true;
	while (!bfsQueue.empty()) {
		int v = bfsQueue.back();
		bfsQueue.pop_back();
		AdjacentVertices f = G.adjacent(v);
		for (std::vector<AdjacentNode>::iterator it = f.begin(); it != f.end(); ++it) {
			AdjacentNode e = *it;
			int w = e.complementaryVertexOf(v);
			if (e.checkResidualCapacityOf(w) > 0 && !marked[w]) {
				if (w == source || w == destination) {
					//do nothing
				}
				else {
					cutSet.push_back(w);
				}
				marked[w] = true;
				bfsQueue.push_front(w);
			}
		}
	}
	return cutSet;
}

ResidualGraph MaxFlowMinCut::getMaxFlowWidestPath() {
	delta = 1;
	count = 0;
	ResidualGraph rg = *fg;
	int maxCapacity = 0, cap = 0;
	AdjacentVertices f = rg.adjacent(source);
	for (int i = 0; i < f.size(); i++)
	{
		AdjacentNode fl = f[i];
		int v = fl.complementaryVertexOf(source);
		int w = fl.edgeCapacity;
		//cout << w << " " << " max" << maxCapacity << " " << v << endl;
		cap = max(cap, w);
		maxCapacity = maxCapacity + w;
	}

	// cout << "max capacity is " << maxCapacity << endl;
	// cout << "C is " << cap << endl;
	if (cap > 1) {
		while (delta < cap) {
			delta <<= 1;
		}
		if (delta > cap) {
			delta >>= 1;
		}
		// cout << "delta:" << delta << endl;
	}
	else if (cap == 1) {
		delta = 1;
	}
	else {
		exit(1);
	}
	while (delta >= 1) {

		while (hasAugmentPathForWidest(rg)) {
			//    cout << "Max Flow Inside while\n";
			int flowCurrent = std::numeric_limits<int>::max();
			for (int v = destination; v != source; v = path[v].complementaryVertexOf(v)) {
				flowCurrent = min(flowCurrent, path[v].checkResidualCapacityOf(v));
				//      cout << "vertex:" << v << " bottleneck:" << flowCurrent << endl;
			}
			rg = constructResidualGraph(rg, flowCurrent);
			value += flowCurrent;
			count++;
		}
		delta = delta / 2;
	}
	cout << "Total Number of Augmented Paths Needed:" << count << endl;
	fprintf(fp, "Total Number of Augmented Paths Needed:\n");
	fprintf(fp, "%d\n", count);

	cout << "Flow value:" << value << endl;
	fprintf(fp, "Flow Value:\n");
	fprintf(fp, "%d\n", value);
	//  time = clock() - time;
	return rg;
}
//Module to compute min cut

void MaxFlowMinCut::getMinCut(ResidualGraph *rg, bool path) {
	ResidualGraph G = *rg;
	cout << "The min cut capacity is:" << value << endl;
	fprintf(fp, "Min-Cut capacity:\n");
	fprintf(fp, "%d\n", value);
	vector<int> S, T;
	bool flag = false;
	if (path == false) {
		S = getCutSet(G, source);
		S.push_back(source);
		for (int i = 0; i < S.size(); i++)
		{
			cout << S[i] << "\t";
			fprintf(fp, "%d\t", S[i]);
		}
		cout << "\n";
		fprintf(fp, "\n");

		T = getCutSet(G, destination);
		T.push_back(destination);
		for (int i = 0; i < T.size(); i++) {
			flag = false;
			for (int j = 0; j < S.size(); j++) {
				if (T[i] == S[j]) {
					flag = true;
				}
			}
			if (flag == false) {
				cout << T[i] << "\t";
				fprintf(fp, "%d\t", T[i]);
			}

		}
		fprintf(fp, "\n");

		cout << endl << endl;
	}

}
// Module to compute the Augmented path for Ford Fulkerson Algorithm.

bool MaxFlowMinCut::hasAugmentPath(ResidualGraph rg) {
	//cout<<"Inside Augment path\n";
	marked = (bool *)malloc(sizeof (bool)*(nV + 1));
	for (int i = 1; i <= nV; i++) {
		marked[i] = false;
	}

	//FlowEdge tempEdge[100];
	path = (AdjacentNode *)malloc(sizeof (AdjacentNode)*(nV + 1));
	std::deque<int> adjVertices;
	adjVertices.push_front(source);
	marked[source] = true;
	while (!adjVertices.empty()) {
		int v = adjVertices.back();
		// cout<<"From vertex:"<<v<<"\n";
		adjVertices.pop_back();
		AdjacentVertices f = rg.adjacent(v);
		//cout<<"Size:"<<f.size()<<"\n";
		for (std::vector<AdjacentNode>::iterator edge = f.begin(); edge != f.end(); ++edge) {
			AdjacentNode e = *edge;
			int w = e.complementaryVertexOf(v);
			// cout<<"To vertex delta value:"<<w<<"\n";
			if (e.checkResidualCapacityOf(w) > 0 && !marked[w]) {
				path[w] = e;
				marked[w] = true;
				adjVertices.push_front(w);
			}
		}
	}
	if (count + 1 == k) {
		int i = 0;

		cout << "The path at iteration " << k << " is\n";
		fprintf(fp, "The path at iteration %d is\n", k);

		for (int v = destination; v != source; v = path[v].complementaryVertexOf(v)) {
			augmentPath.push_back(v);
			i++;
			//       cout<<v<<"\t";
		}
		augmentPath.push_back(source);
		for (int j = augmentPath.size() - 1; j >= 0; j--) {
			cout << augmentPath[j] << "\t";
			fprintf(fp, "%d\t", augmentPath[j]);
		}


		fprintf(fp, "\n");
		cout << "\n";
	}
	return marked[destination];
}

// Module to check whether there is augmented path for Capacity Scaling

bool MaxFlowMinCut::hasAugmentPathForWidest(ResidualGraph rg) {
	//cout<<"Inside Augment path\n";
	marked = (bool *)malloc(sizeof (bool)*(nV + 1));
	for (int i = 1; i <= nV; i++) {
		marked[i] = false;
	}

	//FlowEdge tempEdge[100];
	path = (AdjacentNode *)malloc(sizeof (AdjacentNode)*(nV + 1));
	std::deque<int> adjVertices;
	adjVertices.push_front(source);
	marked[source] = true;
	while (!adjVertices.empty()) {
		int v = adjVertices.back();
		// cout<<"From vertex:"<<v<<"\n";
		adjVertices.pop_back();
		AdjacentVertices f = rg.adjacent(v);
		//cout<<"Size:"<<f.size()<<"\n";
		for (std::vector<AdjacentNode>::iterator it = f.begin(); it != f.end(); ++it) {
			AdjacentNode e = *it;
			int w = e.complementaryVertexOf(v);
			// cout<<"To vertex delta value:"<<w<<"\n";
			if (e.checkResidualCapacityOf(w) >= delta && !marked[w]) {
				path[w] = e;
				marked[w] = true;
				adjVertices.push_front(w);
			}
		}
	}
	return marked[destination];
}

//Module to print output of the final residual graph

void MaxFlowMinCut::writeOutput(ResidualGraph *rg) {
	ResidualGraph G = *rg;
	cout << "The max-flow is:" << endl;
	for (int v = 0; v <= nV; v++) {
		map<int, AdjacentVertices>::iterator iter = G.residualGraph.find(v);
		if (iter != G.residualGraph.end()) {
			AdjacentVertices f = G.residualGraph[v];
			for (std::vector<AdjacentNode>::iterator it = f.begin(); it != f.end(); ++it) {
				if ((*it).v == v) {
					cout << (*it).v << "\t" << (*it).w << "\t" << (*it).edgeFlow << endl;
					fprintf(fp, "%d\t%d\t%d\n", (*it).v, (*it).w, (*it).edgeFlow);
				}
			}
		}
	}
	cout << endl;
}

//Module to construct Residual Graph after every iteration.

ResidualGraph MaxFlowMinCut::constructResidualGraph(ResidualGraph rg, int currentFlow) {
	for (int v = destination; v != source; v = path[v].complementaryVertexOf(v)) {
		int w = path[v].complementaryVertexOf(v);
		map<int, AdjacentVertices>::iterator it = (rg).residualGraph.find(v);
		if (it != (rg).residualGraph.end()) {
			AdjacentVertices f = (rg).residualGraph[v];
			for (std::vector<AdjacentNode>::iterator it = f.begin(); it != f.end(); ++it) {
				if ((*it).v == w) {
					(*it).updateFlowOf(v, currentFlow);
				}
			}

			rg.residualGraph[v] = f;
		}
		it = rg.residualGraph.find(w);
		if (it != rg.residualGraph.end()) {
			AdjacentVertices f = rg.residualGraph[w];
			for (std::vector<AdjacentNode>::iterator it = f.begin(); it != f.end(); ++it) {
				if ((*it).w == v) {
					(*it).updateFlowOf(v, currentFlow);
				}
			}
			rg.residualGraph[w] = f;
		}
	}
	return rg;
}

// Module to read Input from the file

ResidualGraph readInput() {
	FILE* input = fopen("out.txt", "r");
	int v, w, capacity;
	if (input == NULL) {
		cout << "File not present";
	}
	else {
		fscanf(input, "%d %d", &nV, &nE);
		//cout << "The number of vertices:" << nV << "The number of edges" << nE << "\n";
		fscanf(input, "%d %d %d", &source, &destination, &k);
		ResidualGraph flow(nV);
		while (!feof(input)) {
			fscanf(input, "%d %d %d", &v, &w, &capacity);
			AdjacentNode e(v, w, capacity, 0);
			flow.addEdge(e);
		}
		return flow;
	}
}


// Calculation of max flow using ford fulkerson Algorithm

ResidualGraph MaxFlowMinCut::getMaxFlow() {
	//cout<<"Max Flow Inside\n";

	ResidualGraph rg = *fg;
	value = 0;
	count = 0;
	delta = 0;
	bool isPath;
	while ((isPath = hasAugmentPath(rg)) != false) {
		// cout<<"Max Flow Inside while\n";

		int flowCurrent = std::numeric_limits<int>::max();
		for (int v = destination; v != source; v = path[v].complementaryVertexOf(v)) {
			flowCurrent = min(flowCurrent, path[v].checkResidualCapacityOf(v));
			//     cout<<"vertex:"<<v<<" bottleneck:"<<bottle;
		}
		rg = constructResidualGraph(rg, flowCurrent);
		value += flowCurrent;
		count++;
	}

	cout << "Total Number of Augmented Paths Needed:" << count << endl;
	fprintf(fp, "Total Number of Augmented Paths Needed:\n");
	fprintf(fp, "%d\n", count);

	fprintf(fp, "Flow Value:\n");
	fprintf(fp, "%d\n", value);
	cout << "Flow value:" << value << "\n";
	//  time = clock() - time;
	return rg;
}


int main(int argc, char** argv)
{
	

	int choice;
	ResidualGraph f = readInput();
	MaxFlowMinCut max(&f);
	f = max.getMaxFlow();
	max.writeOutput(&f);
	max.getMinCut(&f, false);
	ResidualGraph flowNetwork = readInput();
	MaxFlowMinCut max1(&flowNetwork);
	value = 0;
	fprintf(fp, "The variant of max flow ford fulkerson\n");
	flowNetwork = max1.getMaxFlowWidestPath();
	max1.writeOutput(&flowNetwork);
	max1.getMinCut(&flowNetwork, false);
	fprintf(fp, "Thank you..The end\n");
	cout << "Thank you..The end" << endl;

	


	ifstream in_L("L.txt", ios::binary);
	ifstream in_R("R.txt", ios::binary);
	ofstream out("out.txt");

	
	for (int i = 0; i < 375; i++)
	{
		for (int j = 0; j < 450; j++)
		{
			in_L >> X[i][j];
			in_R >> Y[i][j];
			
		}
	}
	//LCS(X, Y, 450, 450);
	
	unarycosts();
	pairwise();
	initD();

	for (int i = 0; i < 375; i++)
	{
		for (int z = 0; z < 450; z++)
		{
			out << (double) D[i][z]/16000*255 << ' ';
			//out << (double) dist[i][k][1] << ' ';
			if (z == 449)
				out << endl;
		}
	}

	//	LCS(xx, yy, 10, 10);
	//	cout << endl;
	return 0;
}



void unarycosts()
{
	for (int i = 0; i < 375; i++)
	{
		for (int j = 0; j < 450; j++)
		{
			for (int z = 0; z < max; z++)
			{
				if (j + z < 450)
				{
					dist[i][j][z] = (X[i][j] - Y[i][j + z])*(X[i][j] - Y[i][j + z]);
				}
				else
					dist[i][j][z] = 4*K*K; // a random high number
			}
		}
	}
	
}

void pairwise()
{
	
	for (int i = 0; i < 375; i++)
	{
		for (int j = 0; j < 450; j++)
		{
				//up
				if (i == 0)
				{
					U[i][j][0] = 2 * K;
				}
				else if (X[i][j] - X[i - 1][j]<5 && X[i][j] - X[i - 1][j]>-5)
				{
					U[i][j][0] = 2 * K;
				}
				else
				{
					U[i][j][0] = K;
				}
				//down
				if (i == 374)
				{
					U[i][j][1] = 2 * K;
				}
				else if (X[i][j] - X[i + 1][j]<5 && X[i][j] - X[i + 1][j]>-5)
				{
					U[i][j][1] = 2 * K;
				}
				else
				{
					U[i][j][1] = K;
				}
				//left
				if (j == 0)
				{
					U[i][j][2] = 2 * K;
				}
				else if (X[i][j] - X[i][j - 1]<5 && X[i][j] - X[i][j - 1]>-5)
				{
					U[i][j][2] = 2 * K;
				}
				else
				{
					U[i][j][2] = K;
				}
				//right
				if (j == 449)
				{
					U[i][j][3] = 2 * K;
				}
				else if (X[i][j] - X[i][j + 1]<5 && X[i][j] - X[i][j + 1]>-5)
				{
					U[i][j][3] = 2 * K;
				}
				else
				{
					U[i][j][3] = K;
				}
			
		}
	}
}

void initD()
{
	for (int i = 0; i < 375; i++)
	{
		for (int j = 0; j < 450; j++)
		{
			D[i][j] = 4*K*K;
			for (int z = 0; z < max; z++)
			{
				if (D[i][j] < dist[i][j][z])
				{
					D[i][j] = dist[i][j][z];
				}
			}
		}
	}
}

void stereo()
{
	for (int z = 0; z < max; z++)
	{
		for (int i = 0; i < 375; i++)
		{
			for (int j = 0; j < 450; j++)
			{
				if (D[i][j] < dist[i][j][z])
				{
					D[i][j] = dist[i][j][z];
				}
			}
		}
	}
}



void init()
{
	matrix[0][1] = 4;
	matrix[1][2] = 4;
	matrix[2][3] = 2;
	matrix[4][3] = 6;
	matrix[4][5] = 9;
	matrix[0][2] = 8;
	matrix[2][4] = 2;
	matrix[3][5] = 7;
	matrix[1][3] = 4;
	matrix[1][4] = 1;
}

int findPath()
{
	int mark[SCALE];
	for (int i = 0; i<SCALE; i++)mark[i] = 0;

	int top = 0;
	stk[top] = 0;
	mark[top] = 1;

	while (top >= 0)
	{
		int current = abs(stk[top]);
		int i = 0;
		for (i = 0; i<SCALE; i++)
		{
			if (mark[i] == 0)
			{
				if (matrix[current][i]>flow[current][i])
				{
					top += 1;
					stk[top] = i;
					mark[i] = 1;
					//if (top == (TOP - 1))
					if (i == (SCALE - 1))
					{
						return top;
					}
					break;
				}
				else if (flow[i][current]>0)
				{
					top += 1;
					stk[top] = (-i);
					mark[i] = 1;
					break;
				}
			}
		}
		//if (top == (TOP))
		if (i == SCALE)
		{
			top = top - 1;
		}
	}
	return 0;
}

void update(int top)
{
	int addValue = 65535;
	for (int i = 1; i <= top; i++)
	{
		int temp = 0;
		int first = abs(stk[i - 1]);
		int second = abs(stk[i]);

		if (stk[i]>0)
		{
			temp = matrix[first][second] - flow[first][second];
		}
		else
		{
			temp = flow[second][first];
		}

		if (addValue>temp)
		{
			addValue = temp;
		}
	}


	for (int i = 1; i <= top; i++)
	{
		int first = abs(stk[i - 1]);
		int second = abs(stk[i]);

		if (stk[i]>0)
		{
			flow[first][second] = flow[first][second] + addValue;
		}
		else
		{
			flow[second][first] = flow[second][first] - addValue;
		}
	}

}

int maxFlow()
{
	int val = 0;
	while ((val = findPath())>0)update(val);

	int maxVolumn = 0;
	for (int i = 1; i<SCALE; i++)
	{
		maxVolumn += flow[0][i];
	}

	return maxVolumn;
}