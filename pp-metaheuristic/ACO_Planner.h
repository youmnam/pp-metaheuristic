#include <iostream>
#include "PathFinder.h"

using namespace std;

#define PI 3.14159265
/* each ant local information */
struct Ant{
		unint32 position;
		sfloat  path_cost;
		vector<intPair> epath;
		vector<unint32> npath;
		bool isFinished;
};
struct ant_edge{
	unint32 i,j;
	sfloat  pheremone;
	bool isVisited;
	ant_edge(int i,int j){
		this->i = i;
		this->j = j;
		pheremone = 1;
	}
};

class ACOPlanner : public PathFinder {
	private:
		/* Optimization results */
		sfloat best_cost = 99999999;
		//path attribute inherited from pathFinder

		/* Optimization results */
		int nEdges;
		std::vector<ant_edge> edges;

		/* ACO Parameters*/
		const  float B = 5;	    		//importance of heuristic in probability of selection
		const  float a = 5; 			//importance of pheromone in probability of selection
		const  float p =0.5;			//adjust the amount of pheromone evaporation
		const unsigned int   Q = 1;	    //adjust the amount of pheromone to deposit

		unsigned int nIterations = 600;
		const unsigned int nAnts = 60;

		void initalizeEdgesPheremones();
		vector<unint32> getAdjecentEdges(int nodei,vector<unint32> npath);
		int selectNextMove(int k,unint32 src,vector<unint32> adj);
		sfloat getPheremone(int i, int j);
		ant_edge* getEdge(int ii,int jj);
		bool isNodeInPath(unint32 ni, vector<unint32> list);

	public:
		ACOPlanner(GraphBuilder gb): PathFinder(gb) {
			for(int i =0; i<graph.size() ;i++)
			 for (int j=0; j<graph[0].size() ; j++)
				 if( i!=j && graph[i][j] != 0){
					 edges.push_back(ant_edge(i,j));
				 }
		}
		void findPath(unint32 start, unint32 end);

};


