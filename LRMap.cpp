/*
 * HRMap.cpp
 *
 *  Created on: Dec 17, 2016
 *      Author: youmna
 */

#include "LRMap.h"
#include "StateSpace.h"
#include <algorithm>

LR_Map::LR_Map(string _img_file,int _factor) {
	image_file =  absfPath+ _img_file;
	factor = _factor;
	LRMAP.resize(factor);
	for(int i =0; i<factor; i++)
	   LRMAP[i].resize(factor);
}


void LR_Map:: writeSubMaps() //divide the big map into small chunks.
{
	StateSpace s_space;
	s_space.readMap(image_file);
	height = s_space.getWidth();
	width = s_space.getHieght();
	orignalMap = s_space.getMapMatrix();

	cout << width << "x" << height <<endl;
	cout << "large map has been loaded!" <<endl;

	cout << "start generating sub maps..." <<endl;
	int szi = (width/factor) + (width%factor) ;
	int szj = (height/factor) + (height%factor) ;
	//cout << szi << "  " << (width/factor)  << "," << (width%factor)  <<endl;
	//cout << szj << "  " << (height/factor)  << "," << (height%factor)  <<endl;
//int h;
	bool fg2=false;
	for(int i =0; i< width; i+=szi)
	{
		if(fg2){ szi -= (width%factor); fg2=false;} //executed after first row computed
		bool fg=false;
		for(int j=0; j<height; j+=szj)
		{
			//cout << i << " "<< j << "  " << i+szi-1 << "  " << j+szj-1 <<endl;
			subMaps.push_back(subMap(i,j,i+szi-1,j+szj-1));
			if(fg) {szj -= (height%factor); fg =false;} //executed after first column computed
			if(j == 0 ) fg =true;

		}
		szj += (height%factor); //undo the - so first column next iteration become h+factor
		if(i == 0) fg2 =true;
		//cin>> h;
	}

	//cin>> h;
	/*writing sub images on the disk*/
	 vector<int> compression_params;
	 compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	 compression_params.push_back(95);
	 cout << "number of sub images:" << subMaps.size() <<endl;
	 int k =0;
	 for(int n = 0; n<factor; n++)
	{
		for(int m = 0; m<factor; m++)
			{
		int sh = subMaps[k].jend - subMaps[k].jstart;
		int sw = subMaps[k].iend - subMaps[k].istart;
		string submap_fn = absfPath + "/submaps/submap_";
		stringstream str;
		str << submap_fn  << k << ".jpg" ;
		str >> submap_fn;


		cv::Mat submap_imag= cv::Mat::eye(sw, sh, CV_8UC1);
		int ii=0;
		int count_black = 0;
		for(int i = subMaps[k].istart ; i< subMaps[k].iend ; i++)
		{
			int jj=0;
			for(int j = subMaps[k].jstart ; j< subMaps[k].jend ; j++)
			{
			//	cout << " i and j "<<i << " "<< j <<endl;

				if (orignalMap[j][i] == 0)
					submap_imag.at<uchar>(ii,jj++) = 255; //white
				else{
					submap_imag.at<uchar>(ii,jj++) = 0; //black
					count_black++;
				}
			}
			ii++;
		}

		if(count_black != sh*sw){

			LRMAP[n][m] = 0;
			try {
				cv::imwrite(submap_fn, submap_imag, compression_params);
			}catch (runtime_error& ex) {
					fprintf(stderr, "Exception converting image to JPGE format: %s\n", ex.what());
					return;
			 }
		}else{
			LRMAP[n][m] = 1;
			subMaps.erase(std::find(subMaps.begin(), subMaps.end(), subMaps[k]));
		}
		k++;
		//cout << "image:"<<submap_imag <<endl;
		//cin>> h;
	 }
	}
	cout << "Sub images has been generated!" <<endl;
}


string LR_Map:: getSubMap(int i, int j) // get high resolution map info for a global i and j
{

	for(int k=0; k<subMaps.size() ; k++)
	{
		int sh = subMaps[k].jend - subMaps[k].jstart;
		int sw = subMaps[k].iend - subMaps[k].istart;

		if( i >=subMaps[k].istart && i <=subMaps[k].iend &&
			j >=subMaps[k].jstart && j <=subMaps[k].jend )
		{
			string submap_fn = "submaps/submap_";
			stringstream str;
			str << submap_fn  << k << ".jpg" ;
			str >> submap_fn;
			return submap_fn;
		}
	}
	return "null";
}

bmatrix LR_Map:: getLRMap(){
	return LRMAP;
}

bmatrix LR_Map:: getOriginalMap(){
	return orignalMap;
}
