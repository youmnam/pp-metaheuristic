#include "PathExecuter.h"
#include <fstream>

void GraphicalPathExecuter :: drawLine( cv::Mat img, cv::Point start, cv::Point end, cv::Scalar sc )
	{
	  int thickness = 1;
	  int lineType = 8;
	  cv::line( img,
	    start,
	    end,
	    sc,
	    thickness,
	    lineType );
	}

	void GraphicalPathExecuter::drawFilledCircle( cv::Mat img, cv::Point center ,float rad, int r,int g, int b )
	{
	  int thickness = -1;
	  int lineType = 8;

	  cv::circle( img,
	      center,
	      rad,
	      cv::Scalar( b, g, r ),
	      thickness,
	      lineType );
	}

void GraphicalPathExecuter::executePath(vector<unint32> path,vector<intPair> coord){

	cv::Mat space_image = cv::Mat::zeros(w,h, CV_8UC3 );
	char space_window[] = "Generated Path";
	//Draw Map

	for (int i =0; i<bmap.size(); i++)
	{
		for (int j =0; j<bmap[0].size(); j++){
			if (bmap[i][j] == 1)
				//for(int k=0; k<10 ; k++)
				drawFilledCircle( space_image , cv::Point(j/**10*/,i/*10*/) , 0.05, 200, 0, 50);
		}
	}

	//Draw Path
	if (!path.empty()){
		int start = path[0];
		int end = path[path.size()-1];

		drawFilledCircle( space_image, cv::Point( coord[start].second /*10*/ , coord[start].first /*10*/ )  , 5, 0, 0, 255);
		drawFilledCircle( space_image, cv::Point( coord[end].second /*10*/ , coord[end].first /*10*/  ) , 5, 0, 0, 255);

		for (int i =0; i<path.size()-1; i++){
					cv::Point p1(coord[path[i]].second /*10*/,coord[path[i]].first /*10*/);
					cv::Point p2(coord[path[i+1]].second /*10*/,coord[path[i+1]].first /*10*/);
					drawLine( space_image, p1, p2 , cv::Scalar(0,255,0));
				}
	}


	 cv::imshow( space_window, space_image );
	 cv::waitKey( 0 );

	}


void GraphicalPathExecuter::drawVisibilityGraph(vector<edge> edges,vector<corner> coord){

	cv::Mat space_image = cv::Mat::zeros(w,h, CV_8UC3 );
	char space_window[] = "VisibilityGraph";

	for (int i =0; i<edges.size()-1; i++)
		{
			cv::Point p1(coord[edges[i].v1].y ,coord[edges[i].v1].x);
			cv::Point p2(coord[edges[i].v2].y,coord[edges[i].v2].x);
			drawLine( space_image, p1, p2 , cv::Scalar(0,250,130));
		}

	//Draw Map

	for (int i =0; i<bmap.size(); i++)
	{
		for (int j =0; j<bmap[0].size(); j++){
			if (bmap[i][j] == 1)
				drawFilledCircle( space_image , cv::Point(j,i) , 1, 200, 0, 50);
		}
	}


	//Draw Path
	for (int i =0; i<coord.size(); i++)
	{
		drawFilledCircle( space_image, cv::Point( coord[i].y, coord[i].x) , 8, 0, 0,255);
	}

	 cv::imshow( space_window, space_image );


}


void GraphicalPathExecuter::drawVoronoiGraph(vector<edge> path,vector<intPair> coord){

	cv::Mat space_image = cv::Mat::zeros(w,h, CV_8UC3 );
		char space_window[] = "Voronoi Graph";

		for (int i =0; i<path.size()-1; i++)
			{
				cv::Point p1(coord[path[i].v1].second ,coord[path[i].v1].first);
				cv::Point p2(coord[path[i].v2].second,coord[path[i].v2].first);
				drawLine( space_image, p1, p2 , cv::Scalar(0,250,130));
			}

		//Draw Map

		for (int i =0; i<bmap.size(); i++)
		{
			for (int j =0; j<bmap[0].size(); j++){
				if (bmap[i][j] == 1)
					drawFilledCircle( space_image , cv::Point(j,i) , 1, 200, 0, 50);
			}
		}


	/*	//Draw Path
		for (int i =0; i<coord.size(); i++)
		{
			drawFilledCircle( space_image, cv::Point( coord[i].second, coord[i].first) , .5, 0, 0,255);
		}
*/
		 cv::imshow( space_window, space_image );
	//	 cv::waitKey( 0 );
}


void GraphicalPathExecuter::executePath(vector<RRTNode> path)
{

	cv::Mat space_image = cv::Mat::zeros(w,h, CV_8UC3 );
	char space_window[] = "RRT";


	for (int i =0; i<path.size()-1; i++)
	{
		cv::Point p1(path[i].p.y ,path[i].p.x);
		cv::Point p2(path[i+1].p.y,path[i+1].p.x);
		drawLine( space_image, p1, p2 , cv::Scalar(0,250,130));

	}

	cv::Point start(path[0].p.y ,path[0].p.x);
	cv::Point end (path[path.size()-1].p.y,path[path.size()-1].p.x);
	drawFilledCircle( space_image , start, 4, 0, 0, 255);
	drawFilledCircle( space_image , end, 4, 0, 0, 255);


	//Draw Map
	for (int i =0; i<bmap.size(); i++)
	{
		for (int j =0; j<bmap[0].size(); j++){
			if (bmap[i][j] == 1)
				drawFilledCircle( space_image , cv::Point(j,i) , 1, 200, 0, 50);
		}
	}


		 cv::imshow( space_window, space_image );
		 cv::waitKey( 0 );


}


void VrepExecuter:: executePath(vector<unint32> path,vector<intPair> coord){

	int count=1, count1=1;
	float x = -4.5;
	float y =  4.5;
	vector<pair<float,float>  > vrep_map;
	vector<pair<float,float>  > vrep_map_obs;
	vrep_map.resize(h*w);
	vrep_map_obs.resize(h*w);
	for(int i=0; i<h ; i++)
	{
		for(int j=0; j<w; j++)
		{
			if(bmap[i][j] !=1)
			{
				vrep_map[count] = make_pair(x,y);
				count++;
			}else{
				vrep_map_obs[count1] = make_pair(x,y);
				count1++;
			}
			x+=0.3;
		}
		x= -4;
		y-=0.3;
		//cout << endl;

	}

	//write result on file
	string vrepPath = "/home/youmna/ARL_Lab_researchwork/vrep_map.txt";
 	fstream myfile;
   		myfile.open(vrepPath.c_str(),std::fstream::out);
   		if (!myfile.is_open()) cout<< "Can't open vrep map file!" <<endl;

   	     if (myfile.is_open())
   	     {
   	   	  myfile << count << endl;
   	   	  for(int i = 0 ; i<count; i++)
   	   	  {
   	   		  float x = vrep_map[i].first;
   	   		  float y = vrep_map[i].second;
   	   		  myfile << i << " " << x << " " << y <<endl;
   	   	  }
   	   myfile.close();

   	 }

   		vrepPath = "/home/youmna/ARL_Lab_researchwork/vrep_map_obs.txt";

   		myfile.open(vrepPath.c_str(),std::fstream::out);
   	   		if (!myfile.is_open()) cout<< "Can't open vrep map file!" <<endl;

   	   	     if (myfile.is_open())
   	   	     {
   	   	   	  myfile << count1 << endl;
   	   	   	  for(int i = 0 ; i<count1 ; i++)
   	   	   	  {
   	   	   		  float x = vrep_map_obs[i].first;
   	   	   		  float y = vrep_map_obs[i].second;
   	   	   		  myfile << i << " " << x << " " << y <<endl;
   	   	   	  }
   	   	   myfile.close();

   	   	 }

   	//write result on file
	vrepPath = "/home/youmna/ARL_Lab_researchwork/vrep_path.txt";

	myfile.open(vrepPath.c_str(),std::fstream::out);
   		if (!myfile.is_open()) cout<< "Can't open vrep map file!" <<endl;

   	     if (myfile.is_open())
   	     {
   	   	  myfile << path.size() << endl;
   	   	  for(int i = 0 ; i<path.size() ; i++)
   	   	  {
   	   		  //int v= vrep_map[i].first;
   	   		  //float x = vrep_map[i].second.first;
   	   		  //float y = vrep_map[i].second.second;
   	   		  myfile << vrep_map[path[i]].first << " " << vrep_map[path[i]].second <<endl;
   	   	  }
   	   myfile.close();

   	 }

   	  vrepPath = "/home/youmna/ARL_Lab_researchwork/path.txt";
   	  myfile.open(vrepPath.c_str(),std::fstream::out);

   	  if (!myfile.is_open()) cout<< "Can't open vrep map file!" <<endl;

   	  if (myfile.is_open())
   	  {
   	 	 myfile << path.size() <<endl;
   	   	 for(int i = 0 ; i<path.size() ; i++)
   	   	 {
   	   	  myfile <<path[i]<< endl;
   	   	 }
   	   	myfile.close();
   	   }


}
