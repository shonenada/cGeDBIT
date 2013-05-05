#include "../../HeaderFiles/metric/ImageMetric.h"

CImageMetric::CImageMetric()
{
	int i;

	feaLength[0] = 3;
	feaLength[1] = 48;
	feaLength[2] = 15;

	for(i = 0; i < 3; i++)
		minBool[i] = false;
	for(i = 0; i < 3; i++)
		weights[i] = 0.333333;

	maxDist[0] = 1.0;
	maxDist[1] = 60.0;
	maxDist[2] = 1.0;
}

CImageMetric::~CImageMetric()
{
}

double CImageMetric::getDistance(CIndexObject *one, CIndexObject *two) 
{
	CImage *Ione = dynamic_cast<CImage *> (one);
	CImage *Itwo = dynamic_cast<CImage *> (two);
	return getDistance(Ione, Itwo);
}

double CImageMetric::getDistance(CImage *one, CImage *two) 
{
    double dist = 0.0;
    for (int i = 0; i < feaNum; i++)
		dist += (getDistance_Fea(one, two, i) / maxDist[i] * weights[i]);
	return dist;
}

double CImageMetric::getDistance_Fea(CImage *one, CImage *two, int FeaIndex) 
{
	int StartIndex = 0, EndIndex = 0, cnt;
	double dist = 0.0, tempval = 0.0;

	for (int i = 0; i < FeaIndex; i++)
		StartIndex += feaLength[i];
	EndIndex = StartIndex + feaLength[FeaIndex] - 1;

        // The first method for computing image object distance.
	if (minBool[FeaIndex]) {
		for (cnt = StartIndex; cnt <= EndIndex; cnt++) {
			dist += (one->getFeature(cnt) >= two->getFeature(cnt) ? two->getFeature(cnt) : one->getFeature(cnt));
            tempval += one->getFeature(cnt);
                // to make it a symmetric Metric space, add the following line
                // tempval += two.m_Feas[cnt] ;
		}
		dist= abs(1.0 - (dist / tempval));
	}
		
	else { // The second method for computing image object distance.

	for (cnt = StartIndex; cnt <= EndIndex; cnt++) {
		tempval = (one->getFeature(cnt) - two->getFeature(cnt));
		dist += (tempval * tempval);
	}
	dist= sqrt(dist);
	}
	cout<<"dist="<<dist<<endl;
	return dist;
}
