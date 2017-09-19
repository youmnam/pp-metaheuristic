#include "VisibilityBuilder.h"


void VisibilityBuilder::buildGraph()
{
	for(unsigned int k = 0 ; k < map_h ; k++){
		for(unsigned int n=0; n< map_w ; n++){
			if(isCorner(k,n))
			{
				addCorner(k,n);
			}
		 }
	   }
	cout << corners_list.size() <<endl;

	bool flag =0;

	for(unsigned int i =0 ; i<corners_list.size() ; i++)
	{
		for(unsigned j =0 ; j< corners_list.size() ; j++)
		{
			int count = 0;
			if(i != j && !isEEdge(i,j))
			{
				cout << "i ="<< corners_list[i].x <<"," <<corners_list[i].y ;
				cout << " j ="<< corners_list[j].x <<"," <<corners_list[j].y;
			   //find x,y that lies on the line and is corner
			    if( corners_list[i].x  == corners_list[j].x ) //Horizontal lines
			    {
					if(corners_list[i].y < corners_list[j].y )
					{
						for(int y=corners_list[i].y ; y<corners_list[j].y ; y++)
						{
							if(map_matrix[corners_list[i].x][y] == 1 && (map_matrix[corners_list[i].x-1][y] !=0 || map_matrix[corners_list[i].x+1][y] !=0 ))
								count++;
						}
					}else{
						for(int y=corners_list[j].y ; y<corners_list[i].y ; y++)
						{
							if(map_matrix[corners_list[i].x][y] == 1 && (map_matrix[corners_list[i].x-1][y] !=0 || map_matrix[corners_list[i].x+1][y] !=0 ))
								count++;
						}
					}
			    }
			    else if ( corners_list[i].y  == corners_list[j].y  ){ //vertical line
			    	if(corners_list[i].x < corners_list[j].x )
					{
						for(int x=corners_list[i].x ; x<corners_list[j].x ; x++)
						{
							if(map_matrix[x][corners_list[i].y] == 1 && (map_matrix[x][corners_list[i].y-1]!=0 || map_matrix[x][corners_list[i].y+1]!=0))
								count++;
						}
					}else{
						for(int x=corners_list[j].x ; x<corners_list[i].x ; x++)
						{
							if(map_matrix[x][corners_list[i].y] == 1 && (map_matrix[x][corners_list[i].y-1]!=0 || map_matrix[x][corners_list[i].y+1]!=0))
								count++;
						}
					}

			    }
			    else{ //line with slop
			    	sfloat m = (float)(corners_list[i].y - corners_list[j].y) / (corners_list[i].x - corners_list[j].x);
			    	cout << "m:" <<m <<endl;
			    	if(m > 0 )
					{
			    		if(corners_list[i].x < corners_list[j].x)
			    		{
							for(int x=corners_list[i].x ; x<corners_list[j].x ; x++)
							{
								int y = m*(x-corners_list[i].x) + corners_list[i].y;
								//cout << x << " ++ "<<y << endl;
								//cout <<map_matrix[x][y]<< endl;
								if(map_matrix[x][y] == 1){
									count++;
								//	flag =1;
								}//else
								//	if(flag == 1)
									//	count =0;
							}
			    		}else{
			    			for(int x=corners_list[i].x ; x>corners_list[j].x ; x--)
							{
								int y = m*(x-corners_list[i].x) + corners_list[i].y;
								cout << x << " -- "<<y << endl;
								cout <<map_matrix[x][y]<< endl;
								if(map_matrix[x][y] == 1)
									count++;
								}


			    		}
					}else {
						if ( corners_list[j].x >  corners_list[i].x)
						{
							for(int x=corners_list[i].x ; x<corners_list[j].x ; x++)
							{
								int y = m*(x-corners_list[i].x) + corners_list[i].y;
								//cout << x << " ++ "<<y << endl;
								//cout <<map_matrix[x][y]<< endl;
								if(map_matrix[x][y] == 1)
									count++;
							}
						}
						 if ( corners_list[j].y >  corners_list[i].y)
							{
								for(int x=corners_list[i].x ; x>corners_list[j].x ; x--)
								{
									int y = m*(x-corners_list[i].x) + corners_list[i].y;
									//cout << x << " -- "<<y << endl;
									//cout <<map_matrix[x][y]<< endl;
									if(map_matrix[x][y] == 1)
										count++;
									}
							}

					}
			    }
				cout << "count=" << count <<endl;
				if(count < 10)
					edges_list.push_back(edge(i, j, getCost(corners_list[i],corners_list[j])));

		 }
	   }
	}

	//write result on files//
	fstream myfile;
	graph_file = absfPath +"graph_file";
	myfile.open(graph_file.c_str(),std::fstream::out);
	if(myfile.is_open()){
		cout << "Edges:" <<endl;
		myfile << corners_list.size()  << " " << edges_list.size() <<endl;
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
	{  	myfile << corners_list.size() <<endl;
		for(unsigned int i = 0; i <corners_list.size() ; i++)
		{
			myfile <<  corners_list[i].i << " " << corners_list[i].x << " " << corners_list[i].y <<endl;;
		}
		cout <<"End of Edges" <<endl;
		myfile.close();
	}
}

bool VisibilityBuilder:: isEEdge(int s , int j){
	for(unsigned int i = 0; i<edges_list.size() ; i++)
		if(edges_list[i].v1 == s && edges_list[i].v2 == j
				||  edges_list[i].v1 == j && edges_list[i].v2 == s )
			return true;
return false;

}

void VisibilityBuilder::getVisibleCorners(corner src)
{
	int i = src.x;
	int j = src.y;

	if(src.type == "LU")
	{
		//move up
		for(int k = i-1 ; k >=0 ; k--) if(isCorner(k,j)) makeEdge(src,k,j);
		//move left
		for(int l = j-1 ; l >=0 ; l--) if(isCorner(i,l))  makeEdge(src,i,l);
		//move diagonal up
		for(int k = i-1,l= j-1 ; k >=0 && l>=0 ; k--,l--) if(isCorner(k,l)) makeEdge(src,k,l);

	}else if(src.type =="LB")
	{
		//move down
		for(int k = i+1 ; k < map_h ; k++) if(isCorner(k,j)) makeEdge(src,k,j);
		//move left
		for(int l = j-1 ; l >=0 ; l--) if(isCorner(i,l))  makeEdge(src,i,l);
		//move diagonal down
		for(int k = i+1,l= j-1 ; k < map_h && l>=0 ; k++,l--) if(isCorner(k,l)) makeEdge(src,k,l);

	}else if(src.type == "RU"){
		//move right
		for(int l = j+1 ; l <map_w ; l++) if(isCorner(i,l))  makeEdge(src,i,l);
		//move up
		for(int k = i-1 ; k >=0 ; k--) if(isCorner(k,j)) makeEdge(src,k,j);
		//move diagonal up
		for(int k = i-1,l= j+1 ; k >=0 && l< map_w ; k--,l++) if(isCorner(k,l)) makeEdge(src,k,l);

	}else if(src.type == "RB"){
		//move right
		for(int l = j+1 ; l <map_w ; l++) if(isCorner(i,l))  makeEdge(src,i,l);

		//move down
		for(int k = i+1 ; k < map_h ; k++) if(isCorner(k,j)) makeEdge(src,k,j);

		//move diagonal down
		for(int k = i+1,l= j+1 ; k < map_h && l < map_w ; k++,l++) if(isCorner(k,l)) makeEdge(src,k,l);

	}

}

void VisibilityBuilder::makeEdge(corner src, int i,int j){
	corner ecor = getCorner(i,j);
	if(ecor.i != -1){
		edges_list.push_back(edge(src.i, ecor.i, getCost(src,ecor)));
	}else{
		cout <<"I add from make edge "<< i << "," << j <<endl;
		corner ncor=addCorner(i,j);
		edges_list.push_back(edge(src.i, ncor.i, getCost(src,ncor)));
	}
}

sfloat VisibilityBuilder::getCost(corner s, corner d){
	return sqrt(pow(s.x-d.x,2.0)+pow(s.y-d.y,2.0));
}

corner VisibilityBuilder::getCorner(int i,int j){
	for(int k=0; k<corners_list.size(); i++){
		if(corners_list[k].x == i &&  corners_list[k].y == j)
			return corners_list[k];
	}
	return corner(-1,-1,-1,"NA");
}



corner VisibilityBuilder::addCorner(int i,int j){
	int newI=corners_list.size();
	if(isLeftUpper(i,j)){
		corners_list.push_back(corner(newI,i,j,"LU"));
	}else if(isLeftBottom(i,j)){
		corners_list.push_back(corner(newI,i,j,"LB"));
	}else if(isRightUpper(i,j)){
		corners_list.push_back(corner(newI,i,j,"RU"));
	}else if(isRightBottom(i,j)){
		corners_list.push_back(corner(newI,i,j,"RB"));
	}
//	cout <<  newI << " corner Added "
//		 <<  "("<< corners_list[newI].x <<"," << corners_list[newI].y <<")"
//		 <<  corners_list[newI].type <<endl;
	return corners_list[newI];
}


