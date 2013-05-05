#include "../../HeaderFiles/metric/ImageMetric.h"

/*
 * A constructor
 *
 * Details:
 * - The feaLength will be initialized to '3', '48', '15'.
 * - The minBool will be initialized to 'false'.
 * - The weights will be initialized to 3. 
 * - The maxDist will be initialized to '1.0', '60.0', '1.0'.
 */
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

/*
 * A destructor
 */
CImageMetric::~CImageMetric()
{
}

/*
 * @param one : the {@link Object} over which the keys are defined.
 * @param two : the other {@link Object} over which the keys are defined.
 * @return
 *
 * Details:
 * - The CIndexObject types of object will be changed into CImage types.
 * - Calls the funtion names "getDistance" to compute the distance, and then return the distance.
 */
double CImageMetric::getDistance(CIndexObject *one, CIndexObject *two) 
{
	CImage *Ione = dynamic_cast<CImage *> (one);
	CImage *Itwo = dynamic_cast<CImage *> (two);
	return getDistance(Ione, Itwo);
}

/*
 * @param one : the {@link Image} over which the keys are defined.
 * @param two : the other {@link Image} over which the keys are defined.
 * @return
 *
 * Details:
 * - The dist will be defined.
 * - Call the fution named "getDistance_Fea" to compute the distance. And then return the dist.
 */
double CImageMetric::getDistance(CImage *one, CImage *two) 
{
    double dist = 0.0;
    for (int i = 0; i < feaNum; i++)
		dist += (getDistance_Fea(one, two, i) / maxDist[i] * weights[i]);
	return dist;
}

/*
 * @param one : the {@link Image} over which the keys are defined.
 * @param two : the other {@link Image} over which the keys are defined.
 * @param FeaIndex : the feature on which distance is to be computed.
 * @return
 */
double CImageMetric::getDistance_Fea(CImage *one, CImage *two, int FeaIndex) 
{
	int StartIndex = 0, EndIndex = 0, cnt;
	double dist = 0.0, tempval = 0.0;

	for (int i = 0; i < FeaIndex; i++)
		StartIndex += feaLength[i];
	EndIndex = StartIndex + feaLength[FeaIndex] - 1;

    // The first method for computing image object distance.
	if (minBool[FeaIndex]) 
	{
		for (cnt = StartIndex; cnt <= EndIndex; cnt++) 
		{
			dist += (one->getFeature(cnt) >= two->getFeature(cnt) ? two->getFeature(cnt) : one->getFeature(cnt));
            tempval += one->getFeature(cnt);
            // to make it a symmetric Metric space, add the following line
            // tempval += two.m_Feas[cnt] ;
		}
		dist= abs(1.0 - (dist / tempval));
	}
		
	else // The second method for computing image object distance.
	{ 
		for (cnt = StartIndex; cnt <= EndIndex; cnt++) 
		{
			tempval = (one->getFeature(cnt) - two->getFeature(cnt));
			dist += (tempval * tempval);
		}
		dist= sqrt(dist);
	}
	cout<<"dist="<<dist<<endl;
	return dist;
}
