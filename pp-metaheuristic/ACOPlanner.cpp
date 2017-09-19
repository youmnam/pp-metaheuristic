#include "ACO_Planner.h"
#include <iterator>
#include <stdlib.h>
#include <time.h>


void ACOPlanner:: findPath(unint32 start, unint32 end){


	 srand(time(NULL));

	 initalizeEdgesPheremones();
	 //int gridSize = graph.size()*graph[0].size();
	 int gridSize = graph.size();
	 for(int i =0; i<nIterations ; i++)
	 {
		 cout << "i=" << i <<endl;
		Ant ants[nAnts];
		for(int k = 0; k<nAnts; k++)
		 {
			 ants[k].position = start;
			 ants[k].npath.push_back(start);
			 //cout<< ants[k].position << endl;
			 int moveCount= 0;

			 while(ants[k].position != end && moveCount<gridSize )
			 {

				 //1. get adjacent nodes
				 vector<unint32> adjs = getAdjecentEdges(ants[k].position,ants[k].npath);

				 //2. select from  adjacent one node by computing a probability
				 int next =-1;
				 for(int l=0; l<adjs.size() ; l++)
				 {
					 //cout << "###: "<<adjs[l] << endl;
				 	if(adjs[l] == end)
				 	{
				 		next = adjs[l];
						ants[k].isFinished = true;
						//cout <<"EQ:" << adjs[l]<< endl;
						 break;
					}
				 }
				 if(next ==-1)
					 next = selectNextMove(k,ants[k].position,adjs);

				 if(next == end){
					 ants[k].isFinished = true;
				 }

				 if(next ==-1) //no adjacent -- blocked
					 break;

				 //cout<<"next:" << next << endl;


				//4. update ant path cost, path, and ant current position
				ants[k].path_cost += graph[ants[k].position][next];
				ants[k].npath.push_back(next);
				ants[k].epath.push_back(make_pair(ants[k].position,next));
				ants[k].position = next;

				moveCount++;
			 }
		// }

		//for(int k = 0; k<nAnts; k++)
			//	 {
			 //5.update pheremone : local update : depositing
			 // 5.1 ant is successfully reached the goal position
			 // 5.2 deposit Q/get total cost of path on all edges including in the path
			 if(ants[k].isFinished)
			 {
				 for(int n=0; n<ants[k].epath.size() ; n++)
				 {
					 ant_edge * curr_edge = getEdge(ants[k].epath[n].first,ants[k].epath[n].second);
					 curr_edge->pheremone = ((1-p)* curr_edge->pheremone) +  (Q/ants[k].path_cost);
					 //cout << ants[k].epath[n].first << " -->";
				 }
				 //cout << endl;
				// cout << ants[k].epath.size() << "cost:" << ants[k].path_cost <<endl;
			 }

			 //3. update pheremone : global update : evaporation
			// for(int n=0; n<edges.size() ; n++)
			// {
			//	 edges[n].pheremone = (1-p)* edges[n].pheremone;
			// }


			 //7.update best path so far
			 if(ants[k].path_cost < best_cost)
			 {
				 best_cost = ants[k].path_cost;
				 path = ants[k].npath;

			 }/* */
		}

	 }
	 cout << "Total cost" <<  best_cost <<endl;

}


void ACOPlanner::initalizeEdgesPheremones(){
	for(int i =0; i<edges.size() ; i++)
		edges[i].pheremone=1;
}

vector<unint32> ACOPlanner:: getAdjecentEdges(int nodei,vector<unint32> npath){
	vector<unint32> adjs;
	//cout << "width:" << edges.size() <<endl;
	for(int j=0; j<edges.size() ; j++){
		//cout <<  "edges ["<< j << "] "<< edges[j].i <<endl;
		if(edges[j].i == nodei /*!isNodeInPath(edges[j].j,npath)*/)
		{
			if(npath.size()>=2 && npath[npath.size()-2] == edges[j].j)
				continue;
			else
			adjs.push_back(edges[j].j);
		}
	}
	return adjs;
}

/*
 * Calculate the probability of all adjacent nodes
 * select next node based on roulette-wheel-style selection process
 */
int ACOPlanner::selectNextMove(int k,unint32 src,vector<unint32> adj){

	sfloat wheelPosition = 0;

	sfloat randn = 1.0/ ((rand() %10) +1);
	//cout << "random:" << randn <<endl;
	sfloat total=0;
	for(int i=0; i<adj.size() ; i++)
	{
		//cout << " *** " << src <<" " << adj[i] <<endl;
		sfloat phr = getPheremone(src, adj[i]);
		sfloat hur = 1/graph[src][adj[i]];
//		cout << "pher" << phr << "  hur" << hur <<endl;

		total += (pow(phr,a) *pow(hur,B));
	}
	//cout << "total" << total <<endl;
	for(int i=0; i<adj.size() ; i++)
	{
		sfloat phr = getPheremone(src, adj[i]);
		sfloat hur = 1/graph[src][adj[i]];
		if(total == 0 ) break;
		wheelPosition += ((pow(phr,a) *pow(hur,B))/total) ;

		//cout << "pher" << phr << "  hur" << hur <<endl;
		//cout << "prob:" << (pow(phr,a) *pow(hur,B))/total << endl;
		//cout << "wheelPosition:" << wheelPosition <<endl;

		if(wheelPosition >= randn){
		//	cout << "aktr mn rand" <<endl;
			 return adj[i];
		}

	}
	return  adj[0];

/*	int maxi=-1;
	sfloat max = -1;
	sfloat total=0;
	for(int i=0; i<adj.size() ; i++)
	{
		cout << " *** " << src <<" " << adj[i] <<endl;
		sfloat phr = getPheremone(src, adj[i]);
		sfloat hur = 1/graph[src][adj[i]];
		total += (phr*hur);
	}

	for(int i=0; i<adj.size() ; i++)
	{
		cout << " *** " << src <<" " << adj[i] <<endl;
		sfloat phr = getPheremone(src, adj[i]);
		sfloat hur = 1/graph[src][adj[i]];
		sfloat prob = pow(phr,a) *pow(hur,B)/total;
		cout << "pher" << phr << "  hur" << hur <<endl;
		cout << "prob:" << prob << endl;
		if(prob > max){
			max =prob;
			maxi=adj[i];
		}
	}
	return maxi;*/
}


sfloat  ACOPlanner:: getPheremone(int ii, int jj){
	for(int i =0; i<edges.size() ; i++)
		if(edges[i].i == ii &&  edges[i].j == jj )
		{
			//cout << "l2eet pheremone " << ii << "  " << jj << " :" << edges[i].pheremone << endl;
			return edges[i].pheremone;
		}
	return 100000;
}

ant_edge* ACOPlanner:: getEdge(int ii,int jj){

	for(int i =0; i<edges.size() ; i++){
	//cout << edges[i].i << " *** " << edges[i].j <<endl;
		if(edges[i].i == ii &&  edges[i].j == jj )
		{
			return &edges[i];
		}
	}
}

bool ACOPlanner:: isNodeInPath(unint32 ni, vector<unint32> list){
	for(int i =0; i<list.size() ; i++){
		if(list[i] == ni)
			return true;
	}
	return false;
}
