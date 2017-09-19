/*
 * SAnnealingPlanner.h
 *
 *  Created on: Oct 28, 2016
 *      Author: youmna
 */

#ifndef SANNEALINGPLANNER_H_
#define SANNEALINGPLANNER_H_
#include "PathFinder.h"

class SAnnealingPlanner : public PathFinder{
public:
	SAnnealingPlanner(GraphBuilder g,double it,double ft, int n, int nt,double the);
	void findPath(unint32 start, unint32 end);
private:
	/*SA parameters*/
	double initT ;  // Initial Temperature IT
	double finalT ; //Final Temperature FT
	int nIteration; //Number of iterations N
	int nIteration_t; //Number of iterations per Temperature NT
	double theta; //Geometric Coefficient

	/*SA result*/
	vector<unint32> OP; //optimal_path
	double OL;          //optimal path length

	/*Helper Variables & functions*/
	vector<bool> vis;
	vector<unint32> getRandomPath(unint32 start, unint32 end );
	double calculatePathLength(vector<unint32> path);
	vector<unint32> getNeighboringPath(vector<unint32> path);
	vector<unint32> getAdjacentCells(unint32);

};

#endif /* SANNEALINGPLANNER_H_ */
