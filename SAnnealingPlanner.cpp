/*
 * SAnnealingPlanner.cpp
 *
 *  Created on: Oct 28, 2016
 *      Author: youmna
 */

#include "SAnnealingPlanner.h"
#include "GridGraphBuilder.h"
#include <stack>
#include <cmath>

SAnnealingPlanner::SAnnealingPlanner(GraphBuilder g,double it,double ft, int n, int nt,double the) : PathFinder(g){
	initT 		 = it;
	finalT 		 = ft;
	nIteration 	 = n;
	nIteration_t = nt;
	theta 		 = the;
}

void printPath1(vector<unint32> path){
	cout << "Path: " ;
	for(int i=0; i<path.size(); i++){
		cout<< path[i] << "-->";
	}
cout << endl;
}

void SAnnealingPlanner:: findPath(unint32 start, unint32 end){

	srand(time(NULL));
	int currT = initT;
    vector<unint32> cp = getRandomPath(start,end);
	double cl = calculatePathLength(cp);
	cout << "cost:"<<cl << endl;
	cout << "number of nodes:" << cp.size() <<endl;
	//path = cp;

	OP = cp;
	OL = cl;


	for(int i=0; i<nIteration ; i++)
	{
		cout << "i=" << i <<  " cl:"<< cl<<  endl;

		for(int j=0; j<nIteration_t ; j++)
		{

			vector<unint32> rp = getNeighboringPath(cp);
		    double rl = calculatePathLength(rp);

			//cout << "CP:" <<endl;
			//printPath1(cp);
			//cout << "RP:" <<endl;
			//printPath1(rp);
		//}}

			if(rl < cl)
			{
				cp = rp;
				cl = rl;
			}else{

				double p = exp(-1*((rl-cl)/currT));

				double r = 1.0/((rand()%10) +1);
				if(r<p)
				{
					cp=rp;
					cl=rl;
				}
			}
			if(cl<OL)
			{
				OP = cp;
				OL = cl;
			}
		}
		currT = currT * theta ;
		if(currT <= finalT)
			break;
	}
	path = OP;
	cout << "optimal cost" << OL << endl;
	cout << "number of nodes:" << OP.size() << " -" << calculatePathLength(OP) <<endl;
	printPath1(OP);
	/**/
}



vector<unint32> SAnnealingPlanner:: getRandomPath(unint32 start, unint32 end ){

	int V= graph[0].size();
	vis.resize(V);
    for(int i = 0; i < V; i++){
	    vis[i] = false;
    }

	vector<unint32> lpath;
	int curr = start;
	lpath.push_back(curr);
	vis[curr] = true;
	bool flag = false;
	for(int i=0; i<V*10; i++)
	{
		vector<unint32> negL = getAdjacentCells(curr);
		int ng = rand() % negL.size();
		//int ng=0;
		for(int n=0; n<negL.size() ; n++)
		{
			if(negL[n] == end){
				ng = n;
				break;
			}
			if(!getAdjacentCells(negL[ng]).empty()){
				//ng=n;
			}
		}

		if(!flag){
			curr = negL[ng];
			if(!vis[curr]) lpath.push_back(curr);
			vis[curr] = true;
		}
		//cout << i <<" "<< ng << "  goto" << negL[ng] <<endl;

		if(curr == end)
		{
			//lpath.push_back(end);
			return lpath;
		}
    }


	vector<unint32> lpath1;


	vector<bool> visited;
	vector<int> parent;
	visited.resize(V);
	parent.resize(V);
	    for(int i = 0; i < V; i++){
	        visited[i] = false;
	        parent[i] = -1;
	    }


	    stack<unint32> queue;
	    visited[curr] = true;
	    queue.push(curr);
	    while(!queue.empty())
	    {
	    	unint32 s = queue.top();
	        //cout << s << " ";
	        if ( s == end )
	        	   break;
	        queue.pop();

	        for(int i =0; i<V ; i++)
	        {
	        	unint32 adj = graph[s][i];
	            if((graph[s][i] !=0 || graph[i][s]) && !visited[i])
	            {
	                visited[i] = true;
	                //lpath.push_back(i);
	            	parent[i] = s;
	                queue.push(i);
	                if(i == end) break;
	            }
	        }
	    }

	    int k= parent[end];
	    int SZ=0;
	    while(k != -1){
	    	k = parent[k];
	    	SZ++;
	    }

	    lpath1.resize(SZ+1);
	    int c=SZ;
	    k= parent[end];
	    lpath1[c--] = end;
	    while(k != -1){
	    	lpath1[c--] = k;
	    	 k = parent[k];
	    }

	    for(int i=0; i<lpath1.size() ; i++)
	    {
	    	lpath.push_back(lpath1[i]);
	    }
	    return lpath;

}


vector<unint32> SAnnealingPlanner:: getAdjacentCells(unint32 c){
	vector<unint32> push;
	for(int i=0 ; i <graph[c].size() ;i++ ){
		if(graph[c][i] != 0  /*&& !vis[i]*/)
			push.push_back(i);
	}
	return push;
}

double SAnnealingPlanner:: calculatePathLength(vector<unint32> path){

	double cost=0;
	for(int i=0; i<path.size()-1 ; i++)
		cost += graph[path[i]][path[i+1]];
	return cost;
}

vector<unint32> SAnnealingPlanner::getNeighboringPath(vector<unint32> lpath){

	int v1=rand()%lpath.size();
	int v2=rand()%lpath.size();
	//cout << v1 << " --- " << v2 <<endl;
	int i = 0;
	int j = 0;
	vector<unint32> nep;
	if(v1>v2){
	   nep =  getRandomPath(lpath[v2],lpath[v1]);
	    i = v2;
	    j = v1;
	}else{
	   nep = getRandomPath(lpath[v1],lpath[v2]);
	   i = v1;
	   j = v2;
	}
	vector<unint32> npath;
	//npath.resize(lpath.size() - (j-i+1) + nep.size());
	//cout << "random size inside func"<<npath.size() <<endl;
	for( int k=0; k<i ; k++){
		npath.push_back(lpath[k]);
	}

	for( int w=0; w<nep.size() ; w++){
		 npath.push_back(nep[w++]);
	 }

	for( int k=j; k<lpath.size() ; k++){
			npath.push_back(lpath[k]);
	}

	return npath;
}
