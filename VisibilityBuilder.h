/*
 * VisibilityGraphBuilder.h
 *
 *  Created on: Jul 27, 2016
 *      Author: youmna
 */
#ifndef VISIBILITYBUILDER_H_
#define VISIBILITYBUILDER_H_

#include "GraphBuilder.h"

class VisibilityBuilder : public GraphBuilder{
	public:
		vector<corner> corners_list;
		vector<edge> edges_list;
	public:
		VisibilityBuilder(string mapf) : GraphBuilder(mapf) {};
		void buildGraph();
		void getVisibleCorners(corner src);
		void makeEdge(corner src, int i, int j);
        corner addCorner(int i,int j);
        corner getCorner(int i,int j);
		sfloat getCost(corner s, corner d);
		bool isEEdge(int s , int j);

};





#endif
