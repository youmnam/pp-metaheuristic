#include "VoronoiBuilder.h"
#define WINDOW 25


void VoronoiBuilder::buildGraph()
{

	vor_dist = new sfloat* [map_h];
	for(int m=0; m<map_h ; m++)
		vor_dist[m]=new sfloat[map_w];

	vor_vis = new bool* [map_h];
		for(int m=0; m<map_h ; m++)
			vor_vis[m]=new bool[map_w];


  for(unint32 i=0; i<map_h ; i++)
  {
	  for(unint32 j=0; j<map_w ; j++)
	   {
		  vor_dist[i][j] = 0;
		  vor_vis[i][j] = 0;
		  int type = _NAN;
		  if(i == 0 )		type = DS;
		  if(j == 0 ) 		type = RS;
		  if(i == map_h-1)  type = US;
		  if(j == map_w-1 )	type = LS;
		  if(map_matrix[i][j] == 1 ) isObstacleBoundary(i,j,type);
		  if (type != _NAN)
		  {
			  obstacles_list.push_back(make_pair(i,j));
			  obstacles_types.push_back(type);
		  }
	   }
   }

cout << obstacles_list.size() <<endl;

  for(int i=0; i< map_h ; i++)
  {
	  for(int j=0; j< map_w ; j++)
	   {
		  if(map_matrix[i][j] == 0 )
		  {
			  int min= 99999;
			  for(unint32 k=0; k<obstacles_list.size(); k++)
			  {
				  sfloat currdist = sqrt(pow(i-obstacles_list[k].first,2.0)+pow(j-obstacles_list[k].second,2.0));
				  // cout<<  i << "," << j << " "<< obstacles_list[k].first <<"," <<obstacles_list[k].second <<" "<<obstacles_types[k]<< endl;
				  if( currdist < min)
				  {
					  min = currdist;
				  }
			  }
			  vor_dist[i][j] = min;

		  }
	   }
   }


  for(unint32 k=0; k< obstacles_list.size(); k++)
  {
	  int i = obstacles_list[k].first;
	  int j = obstacles_list[k].second;
	  sfloat prev ;
	  if(obstacles_types[k] == US  || obstacles_types[k] == LU || obstacles_types[k] == RU)
	  {
		 do{
		  prev =  vor_dist[i][j];
		  i=i-1;
		  if(i < 0) break;
		  }
		while(prev<= vor_dist[i][j]);

		if (!vor_vis[i+1][j]) vertex_list.push_back(make_pair(i+1,j)); //i+1 because it exits the loop at cell that after see a change
		vor_vis[i+1][j] = 1;
	  }if(obstacles_types[k] == DS || obstacles_types[k] == LB || obstacles_types[k] == RB)
	  {
 	 	 do{
		  prev =  vor_dist[i][j];
		  i=i+1;
		  if(i >= map_h) break;

		}
		while(prev<= vor_dist[i][j]);

		if(!vor_vis[i-1][j])vertex_list.push_back(make_pair(i-1,j)); //i+1 because it exits the loop at cell that after see a change
		vor_vis[i-1][j] = 1;

	  }if(obstacles_types[k] == LS || obstacles_types[k] == LU || obstacles_types[k] == LB)
	  {
		  do{
			prev =  vor_dist[i][j];
			j=j-1;
			if( j <0) break;
			}
		  while(prev<= vor_dist[i][j]);

		  if (!vor_vis[i][j+1] ) vertex_list.push_back(make_pair(i,j+1)); //i+1 because it exits the loop at cell that after see a change
		  vor_vis[i][j+1] = 1;
	  }if(obstacles_types[k] == RS || obstacles_types[k] == RU || obstacles_types[k] == RB)
	  {
		  do{
			prev =  vor_dist[i][j];
			j=j+1;
			if( j>= map_w ) break;
			}
		  while(prev<= vor_dist[i][j]);

		  if (!vor_vis[i][j-1]) vertex_list.push_back(make_pair(i,j-1)); //i+1 because it exits the loop at cell that after see a change
		  vor_vis[i][j-1] = 1;
	  }
  }


  cout << "Dist:" <<endl;
    for(unint32 i=0; i<map_h ; i++)
    {
  	  for(unint32 j=0; j<map_w ; j++)
  	   {
  		  printf("%4.0f", vor_dist[i][j]);
  	   }
  	  cout << endl;
    }



  for(int i = 0 ; i<map_h ; i++)
	  delete [] vor_dist[i];

  delete [] vor_dist;

  cout <<"vertices:" <<endl;


bool flag1=0, flag2=0;
  for(unint32 k=0; k<vertex_list.size() ; k++){
 	 int i1=vertex_list[k].first ;
 	 int j1=vertex_list[k].second  ;

 	 //case 1 : window 1
 	 for(int starti = i1 - WINDOW ; starti <= i1 ; starti++ )
 	 {
 	 	 for(int startj = j1; startj <= j1+WINDOW ; startj++ )
 	 	 {
 	 		 if (starti < map_h && starti >=0 && startj < map_w && startj >=0 && vor_vis [starti][startj] ==1)
 	 	 		 {

				sfloat cost = sqrt(pow(i1- starti,2.0)+pow(j1- startj,2.0));
				int adj= getVertxIndex(starti,startj);
				if(adj != -1 && !isEEdge(k,adj))
				edges_list.push_back(edge(k,adj,cost));
				starti = i1+1;
				break;
 	 		 }
 	 	 }
 	 }

 	 //case 2 : window 2
 	 for(int starti = i1 - WINDOW ; starti <= i1 ; starti++ )
 	 {
 	 	 for(int startj = j1 -WINDOW ; startj <= j1 ; startj++ )
 	 	 {

 	 		 if (starti < map_h && starti >=0 && startj < map_w && startj >=0 && vor_vis [starti][startj] ==1 )
 	 	 		 {
 			sfloat cost = sqrt(pow(i1- starti,2.0)+pow(j1- startj,2.0));
				int adj= getVertxIndex(starti,startj);
				if(adj != -1 && !isEEdge(k,adj))
				edges_list.push_back(edge(k,adj,cost));
				starti = i1+1;
				break;
 	 		 }
 	 	 }
 	 }


 	 //case 3 : window 3
 	 for(int starti = i1  ; starti <= i1+WINDOW ; starti++ )
 	 {
 	 	 for(int startj = j1 -WINDOW ; startj <= j1 ; startj++ )
 	 	 {

 	 		 if (starti < map_h && starti >=0 && startj < map_w && startj >=0 && vor_vis [starti][startj] ==1)
 	 	 		 {

				sfloat cost = sqrt(pow(i1- starti,2.0)+pow(j1- startj,2.0));
				int adj= getVertxIndex(starti,startj);
				if(adj != -1 && !isEEdge(k,adj))
				edges_list.push_back(edge(k,adj,cost));
				starti = i1+WINDOW ;
				break;
 	 		 }

 	 	 }
 	 }


 	 //case 4 : window 4
 	 for(int starti = i1  ; starti <= i1+ WINDOW ; starti++ )
 	 {
 	 	 for(int startj = j1 ; startj <= j1 +WINDOW ; startj++ )
 	 	 {

 	 		 if (starti < map_h && starti >=0 && startj < map_w && startj >=0 && vor_vis [starti][startj] ==1 )
 	 		 {
				sfloat cost = sqrt(pow(i1- starti,2.0)+pow(j1- startj,2.0));
				int adj= getVertxIndex(starti,startj);
				if(adj != -1 && !isEEdge(k,adj))
				edges_list.push_back(edge(k,adj,cost));
				starti = i1+ WINDOW +1;
				break;
 	 		 }
 	 	 }
 	 }
  }



/*
 	for(unsigned int i =0 ; i<vertex_list.size() ; i++)
 		{
 			for(unsigned j =0 ; j< vertex_list.size() ; j++)
 			{
 				int count = 0;
 				if(i != j && !isEEdge(i,j))
 				{
 					cout << "i ="<< vertex_list[i].first <<"," <<vertex_list[i].second ;
 					cout << " j ="<< vertex_list[j].first <<"," <<vertex_list[j].second;
 				   //find x,y that lies on the line and is corner
 				    if( vertex_list[i].first  == vertex_list[j].first ) //Horizontal lines
 				    {
 						if(vertex_list[i].second < vertex_list[j].second )
 						{
 							for(int y=vertex_list[i].second ; y<vertex_list[j].second ; y++)
 							{
 								if(map_matrix[vertex_list[i].first][y] == 1)
 									count++;
 							}
 						}else{
 							for(int y=vertex_list[j].second ; y<vertex_list[i].second ; y++)
 							{
 								if(map_matrix[vertex_list[i].first][y] == 1)
 									count++;
 							}
 						}
 				    }
 				    else if ( vertex_list[i].second  == vertex_list[j].second  ){ //vertical line
 				    	if(vertex_list[i].second < vertex_list[j].second )
 						{
 							for(int x=vertex_list[i].first ; x<vertex_list[j].first ; x++)
 							{
 								if(map_matrix[x][vertex_list[i].second] == 1)
 									count++;
 							}
 						}else{
 							for(int x=vertex_list[j].first ; x<vertex_list[i].first ; x++)
 							{
 								if(map_matrix[x][vertex_list[i].second] == 1)
 									count++;
 							}
 						}

 				    }
 				    else{ //line with slop
 				    	sfloat m = (float)(vertex_list[i].second - vertex_list[j].second) / (vertex_list[i].first - vertex_list[j].first);
 				    	cout << "m:" <<m <<endl;
 				    	if(m > 0 )
 						{
 				    		if(vertex_list[i].first < vertex_list[j].first)
 				    		{
 								for(int x=vertex_list[i].first ; x<vertex_list[j].first ; x++)
 								{
 									int y = m*(x-vertex_list[i].first) + vertex_list[i].second;
 									//cout << x << " ++ "<<y << endl;
 									//cout <<map_matrix[x][y]<< endl;
 									if(map_matrix[x][y] == 1)
 										count++;
 								}
 				    		}else{
 				    			for(int x=vertex_list[i].first ; x>vertex_list[j].first ; x--)
 								{
 									int y = m*(x-vertex_list[i].first) + vertex_list[i].second;
 									cout << x << " -- "<<y << endl;
 									cout <<map_matrix[x][y]<< endl;
 									if(map_matrix[x][y] == 1)
 										count++;
 									}


 				    		}
 						}else {
 							if ( vertex_list[j].first >  vertex_list[i].first)
 							{
 								for(int x=vertex_list[i].first ; x<vertex_list[j].first ; x++)
 								{
 									int y = m*(x-vertex_list[i].first) + vertex_list[i].second;
 									//cout << x << " ++ "<<y << endl;
 									//cout <<map_matrix[x][y]<< endl;
 									if(map_matrix[x][y] == 1)
 										count++;
 								}
 							}
 							 if ( vertex_list[j].second >  vertex_list[i].second)
 								{
 									for(int x=vertex_list[i].first ; x>vertex_list[j].first ; x--)
 									{
 										int y = m*(x-vertex_list[i].first) + vertex_list[i].second;
 										//cout << x << " -- "<<y << endl;
 										//cout <<map_matrix[x][y]<< endl;
 										if(map_matrix[x][y] == 1)
 											count++;
 										}
 								}

 						}
 				    }
 					cout << "count=" << count <<endl;
 					if(count < 10){
 						int i1 = vertex_list[i].first;
 						int j1 = vertex_list[j].second;
 						sfloat cost = sqrt(pow(i1-(i1),2.0)+pow(j1-(j1),2.0));
 						edges_list.push_back(edge(i, j, cost));
 					}
 			 }
 		   }
 		}
*/

/*
  for(unint32 k=0; k<vertex_list.size() ; k++){
 	 int i1=vertex_list[k].first ;
 	 int j1=vertex_list[k].second  ;


for(int WINDOW =1; WINDOW<20 ; WINDOW++){
	 if(i1 == 127 && j1 == 100) cout <<"Iam here " << i1-WINDOW << " " << j1-WINDOW << " " << vor_vis[i1-WINDOW][j1-WINDOW] <<endl;
	if(i1-WINDOW >= 0 && vor_vis[i1-WINDOW][j1])
	 {  sfloat cost = sqrt(pow(i1-(i1-WINDOW),2.0)+pow(j1-j1,2.0));
	 	int adj= getVertxIndex(i1-WINDOW,j1);
	  	if(!isEEdge(k,adj))
	  	    edges_list.push_back(edge(k,adj,cost));
	 }
 	if(j1-WINDOW >= 0 && vor_vis[i1][j1-WINDOW])
	 {  sfloat cost = sqrt(pow(i1-(i1),2.0)+pow(j1-(j1-WINDOW),2.0));
		int adj= getVertxIndex(i1,j1-WINDOW);
		if(!isEEdge(k,adj))
			edges_list.push_back(edge(k,adj,cost));
	 }
 	if(i1-WINDOW >= 0 && j1-WINDOW >= 0 && vor_vis[i1-WINDOW][j1-WINDOW])
	 {  sfloat cost = sqrt(pow(i1-(i1-WINDOW),2.0)+pow(j1-(j1-WINDOW),2.0));

		int adj= getVertxIndex(i1-WINDOW,j1-WINDOW);
		if(!isEEdge(k,adj)){
			edges_list.push_back(edge(k,adj,cost));

		}
	 }
 	if(i1+WINDOW <  map_h && vor_vis[i1+WINDOW][j1])
 	 	 {  sfloat cost = sqrt(pow(i1-(i1+WINDOW),2.0)+pow(j1-j1,2.0));
 	 	 	int adj= getVertxIndex(i1+WINDOW,j1);
 	 	    if(!isEEdge(k,adj))
 	 	    	edges_list.push_back(edge(k,adj,cost));
 	 	 }
 	if(j1+WINDOW <  map_w && vor_vis[i1][j1+WINDOW])
 	 	 {  sfloat cost = sqrt(pow(i1-(i1),2.0)+pow(j1-(j1+WINDOW),2.0));
 	 	 	int adj= getVertxIndex(i1,j1+WINDOW);
 	 	    if(!isEEdge(k,adj))
 	 	    	edges_list.push_back(edge(k,adj,cost));
 	 	 }
 	if(i1+WINDOW < map_h && j1+WINDOW < map_w && vor_vis[i1+WINDOW][j1+WINDOW])
 		 {  sfloat cost = sqrt(pow(i1-(i1+WINDOW),2.0)+pow(j1-(j1+WINDOW),2.0));
 			int adj= getVertxIndex(i1+WINDOW,j1+WINDOW);
 			if(!isEEdge(k,adj))
 				edges_list.push_back(edge(k,adj,cost));
 		 }
 	if(i1+WINDOW < map_h && j1-WINDOW >=0 && vor_vis[i1+WINDOW][j1-WINDOW])
	 {  sfloat cost = sqrt(pow(i1-(i1+WINDOW),2.0)+pow(j1-(j1-WINDOW),2.0));
		int adj= getVertxIndex(i1+WINDOW,j1-WINDOW);
		if(!isEEdge(k,adj))
			edges_list.push_back(edge(k,adj,cost));
	 }
 	if(i1-WINDOW >=0 && j1+WINDOW < map_w && vor_vis[i1-WINDOW][j1+WINDOW])
	 {  sfloat cost = sqrt(pow(i1-(i1-WINDOW),2.0)+pow(j1-(j1+WINDOW),2.0));
		int adj= getVertxIndex(i1-WINDOW,j1+WINDOW);
		if(!isEEdge(k,adj))
			edges_list.push_back(edge(k,adj,cost));
	 }
	}

   }
*/


   for(int i = 0 ; i<map_h ; i++)
   	  delete [] vor_vis[i];

     delete [] vor_vis;


//     terminatingEdges();

  //write result on files//
  	fstream myfile;
  	graph_file = absfPath +"graph_file";
  	myfile.open(graph_file.c_str(),std::fstream::out);
  	if(myfile.is_open()){
  		cout << "Edges:" <<endl;
  		myfile << vertex_list.size()  << " " << edges_list.size() <<endl;
  		for(unsigned int i = 0; i<edges_list.size() ; i++)
  		{
  			myfile <<  edges_list[i].v1 << " " << edges_list[i].v2 << " " << edges_list[i].cost <<endl;;
  		}
  		cout <<"End of Edges" <<endl;
  		myfile.close();
  	}

  	coor_file = absfPath +"coor_file";
  	myfile.open(coor_file.c_str(),std::fstream::out);
  	if(myfile.is_open())
  	{  	myfile << vertex_list.size() <<endl;
  		for(unsigned int i = 0; i <vertex_list.size() ; i++)
  		{
  			myfile <<  i << " "<<vertex_list[i].first << " " << vertex_list[i].second  <<endl;;
  		}
  		cout <<"End of Edges" <<endl;
  		myfile.close();
  	}

}



bool VoronoiBuilder::isObstacleBoundary(int i, int j,int &type)
{
	if(isCorner(i,j,type))
	{
		return true;
	}
	if( i-1 >=0  && map_matrix[i-1][j] ==0 )
	{
		type =  US;
		return true;
	}
	if( j-1 >=0  && map_matrix[i][j-1] == 0 )
	{
		type = LS;
		return true;
	}
	if( i+1 <map_h  && map_matrix[i+1][j] == 0 )
	{
		type = DS;
		return true;
	}
	if( j+1 <map_w  && map_matrix[i][j+1] == 0 )
	{
		type = RS;
		return true;
	}

	return false;
}

int VoronoiBuilder::getVertxIndex(int x,int y)
{

for(int i = 0; i<vertex_list.size() ;i++)
	if(vertex_list[i].first == x && vertex_list[i].second == y )
		return i;
return 0;
}

bool VoronoiBuilder:: isEEdge(int s , int j){
	for(unsigned int i = 0; i<edges_list.size() ; i++)
		if(edges_list[i].v1 == s && edges_list[i].v2 == j
				||  edges_list[i].v1 == j && edges_list[i].v2 == s )
			return true;
return false;

}

void VoronoiBuilder::terminatingEdges()
{
	vector<edge> newEdges;
	int src =edges_list[0].v1;
	int prev =edges_list[0].v1;
	sfloat acost=0;
	int count = 0;
	for(int j = 0; j< edges_list.size() ; j++)
	{
		for(int i = j ; i<edges_list.size(); i++){
			if(edges_list[i].v1 == prev)
			{
				prev   = edges_list[i].v2;
				acost += edges_list[i].cost;
				count++;
			}
			if(count  == 5)
			{
				newEdges.push_back(edge(src,prev,acost));
				count =0;
				acost=0;
				src =  prev;
			}
		}
	}
	edges_list.clear();
	for(int i=0; i<newEdges.size(); i++)
		edges_list.push_back(newEdges[i]);

}
