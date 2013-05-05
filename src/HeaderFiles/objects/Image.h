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

    /**
     * @param FLen
     * @param feas an array of floats over which the feature values are defined.
     */
	CImage(int FLen , float feas[] ); 

    /**
     * @param FLen
     * @param DLen
     * @param feas an array of floats over which the feature values are defined.
     * @param maxDist
     */
	CImage(int FLen , int DLen , float feas[] , double maxDist[]); 
	~CImage(void);

	static vector<CIndexObject*> loadData(string fileName , int cImage_Num, int  feas_Num);

    /**
     * @param index
     * @return
     */
	float getFeature(int index); 

	/**
	 * @Override
	 */
	int getSize(); 


	int compareTo(CIndexObject *oThat); 


	bool equals(CIndexObject *other);

	string toString(); 
	

private:

	/**
    * Floating numbers represent the features for an image.
    */
	float *m_Feas;
	int m_Feas_Len;
	double *max_Dist;
	int max_Dist_Len;
};

#endif;