#ifndef IMAGE_H
#define IMAGE_H

#include "IndexObject.h"


#include<fstream>
#include<string>
#include<vector>
#include<math.h>
using namespace std;

const int maxNum = 100;

class CImage :
	public CIndexObject
{
public: 

	CImage();
	CImage(int FLen , float feas[] ); 
	CImage(int FLen , int DLen , float feas[] , double maxDist[]); 
	~CImage(void);

	
	float getFeature(int index); 
	int getSize(); 
	int compareTo(CIndexObject *oThat); 
	string toString();
	int hashCode();
	static vector<CIndexObject*> loadData(string fileName , int cImage_Num, int  feas_Num);
	

private:

	/**
    * Floating numbers represent the features for an image.
    */
	float *m_Feas;
	int m_Feas_Len;
	double *max_Dist;
	int max_Dist_Len;
};

#endif
