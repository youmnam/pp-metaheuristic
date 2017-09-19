#ifndef VORNOI_H
#define VORNOI_H
#include "GraphBuilder.h"

class VoronoiBuilder : public GraphBuilder{
public:
	vector<intPair> obstacles_list;
	vector<intPair> vertex_list;
	vector<edge> 	edges_list;
	vector<unint32> obstacles_types;
	sfloat ** vor_dist;
	bool ** vor_vis;
public:
	   VoronoiBuilder(string bma): GraphBuilder(bma) {}
	   void buildGraph();
	   bool isObstacleBoundary(int i, int j,int &type);
	   int getVertxIndex(int x,int y);
	   bool isEEdge(int s , int j);
	   void terminatingEdges();

};

#endif
