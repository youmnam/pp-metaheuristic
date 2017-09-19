/*
 * PSOPlanner.h
 *
 *  Created on: Nov 16, 2016
 *      Author: youmna
 */

#ifndef PSOPLANNER_H_
#define PSOPLANNER_H_


#include "PathFinder.h"
#include "GraphBuilder.h"

struct Particle{
	pair<unint32,unint32> pos;
	unint32 node_no;
	sfloat fitness;
	sfloat velocity;
	Particle(){}
	Particle(int i, int j, int n){
		pos = make_pair(i,j);
		node_no = n;
		velocity =1;
	}
};

class PSO_Planner : public PathFinder{
private:
	unint32 nIteration;
	unint32 stepSize;
	vector<Particle> particles;
	double w,c1,c2,omega;


public:
	void findPath(unint32 start, unint32 end);
	PSO_Planner(GraphBuilder g, unint32 n, unint32 s): PathFinder(g), nIteration(n), stepSize(s){
		w = 0.4;
		c1 = 4;
		c2 = 4;
		omega = 4;

	};
	void generateInitialPop(unint32 start);
	sfloat evaluateFitness(unint32 goal);
	vector<unint32> getAdjacentCells(unint32 c);
	sfloat getDistance(pair<unint32,unint32> s, pair<unint32,unint32> d);
};

#endif /* PSOPLANNER_H_ */
