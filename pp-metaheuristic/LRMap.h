/*
 * HRMap.h
 *
 *  Created on: Dec 17, 2016
 *      Author: youmna
 */

#ifndef LRMAP_H_
#define LRMAP_H_

#include <vector>
#include "datatypes.h"

using namespace std;

struct subMap{
	int istart;
	int jstart;
	int iend;
	int jend;
	subMap(int is,int js, int ie, int je){
		istart = is;
		jstart = js;
		iend = ie;
		jend = je;
	}
	bool operator==(const subMap& s){
		return istart==s.istart && jstart==s.jstart && iend == s.iend && jend == s.jend;
	}
};

class LR_Map {
public:
	LR_Map(string,int);
	void writeSubMaps(); //divide the big map into small chunks.
	string getSubMap(int i, int j); // get high resolution map info for a global i and j
	bmatrix getLRMap(); //return the large map (low resolution)
	bmatrix getOriginalMap();
private:
	vector<subMap> subMaps;
	bmatrix orignalMap;
	bmatrix LRMAP;
	string image_file;
	int width;
	int height;
	int factor;
	string absfPath = "/home/youmna/ARL_Lab_researchwork/PathPlanner_v4.1/mapdata/";

};

#endif /* LRMAP_H_ */
