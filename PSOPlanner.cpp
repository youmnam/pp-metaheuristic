/*
 * PSOPlanner.cpp
 *
 *  Created on: Nov 16, 2016
 *      Author: youmna
 */

#include "PSOPlanner.h"
#define MAX_NUM 99999999

void PSO_Planner :: findPath(unint32 start, unint32 end){
	srand(time(NULL));
	w = 0.3;
			c1 = 2;
			c2 = 4;
			omega = 4;


	for(int i=0 ; i<1/*nIteration */; i++)
	{
		generateInitialPop(start);
		path.clear();
		path.push_back(start);
		double  gfbest = MAX_NUM;
		unint32 gpbest = start;
		int count =0;

		while(getDistance(all_pos[gpbest], all_pos[end])> 2 && count <1000)
		{

			double lfbest= MAX_NUM;
			unint32 lpbest = -1;

			evaluateFitness(end);

			for(int j=0; j <particles.size() ;j++)
			{
				if(particles[j].fitness < lfbest)
				{
					lfbest = particles[j].fitness;
					lpbest = particles[j].node_no;
				}
			}
			cout << "best particle:" << lfbest << endl;
			if(lfbest < gfbest)
			{

				gfbest = lfbest;
				gpbest = lpbest;
				cout <<"best particle in swarm "<< gpbest <<endl;
				path.push_back(gpbest);
			}
			//update particles' position and velocity
			double x= 2 / abs((2- omega - sqrt((omega*omega)-4*omega) ));
			for(int j=0; j <particles.size() ;j++)
			{
				//update velocity
				float r1 = 1.0/ (rand()%10 +1);
				float r2 = 1.0/ (rand()%10 +1);
				//cout << " rr" << r1 << "  " << r2 <<endl;

				particles[j].velocity = x* ( (w* particles[j].velocity )
						                + (c1*r1*getDistance(all_pos[lpbest],particles[j].pos))
										+ (c2*r2*getDistance(all_pos[gpbest],particles[j].pos)) );

				if(particles[j].node_no + particles[j].velocity < graph.size()* graph[0].size())
				particles[j].node_no = particles[j].node_no + particles[j].velocity;

				particles[j].pos.first = all_pos[particles[j].node_no].first ;
			    particles[j].pos.second = all_pos[particles[j].node_no].second ;
				cout <<" next pos of particle " << j << " =" << particles[j].node_no << " at"<< particles[j].pos.first << "," << particles[j].pos.second
						 << "  velociy =" << particles[j].velocity<<endl;
			}
			count++;
			cout << "with gpbest goal: "<<getDistance(all_pos[gpbest], all_pos[end]) <<endl;
		}
		if( getDistance(all_pos[gpbest], all_pos[end]) !=0)
			//draw path between gpbest and end
			path.push_back(end);
		cout << " =" << count <<endl;

	}

}


void PSO_Planner ::  generateInitialPop(unint32 start){

	vector<unint32> adjCells = getAdjacentCells(start);
	particles.clear();
	for(int i =0; i<adjCells.size() ; i++)
	{
		cout << "pushing" << adjCells[i]<<endl;
		particles.push_back(Particle(all_pos[adjCells[i]].first,all_pos[adjCells[i]].second,adjCells[i]));

	}
}


sfloat PSO_Planner ::  evaluateFitness(unint32 goal){
	for(int i =0; i<particles.size() ; i++)
		{
		 particles[i].fitness = getDistance(particles[i].pos, all_pos[goal]);
		//particles[i].fitness = graph[particles[i].node_no][goal];
		cout << "fitt "<<particles[i].fitness <<endl;
		}
}

vector<unint32> PSO_Planner:: getAdjacentCells(unint32 c){
	vector<unint32> push;
	for(int i=0 ; i <graph[c].size() ;i++ ){
		if(graph[c][i] != 0  /*&& !vis[i]*/)
			push.push_back(i);
	}
	return push;
}

sfloat PSO_Planner ::getDistance(pair<unint32,unint32> s, pair<unint32,unint32> d)
{
		return sqrt(pow((int)d.first-(int)s.first,2.0)+pow((int)d.second-(int)s.second,2.0));
}
