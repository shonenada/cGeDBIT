#include "..\..\HeaderFiles\indexalgorithm\FFTPivotSelectionMethod.h"

#include <cfloat>

/*
#define POSITIVE_INFINITY 1000000
	//!!!!NOTE:POSITIVE_INFINITY !!!!
	*/

/**
* check the array of pivots, remove the duplicate.
* @param metric
* @param data
* @param pivots
* @return
*/
vector<int> removeDuplicate(CMetric *metric,vector<CIndexObject*> data,vector<int> &pivots,int first,int dataSize);

CFFTPivotSelectionMethod::CFFTPivotSelectionMethod()
{

}

CFFTPivotSelectionMethod::~CFFTPivotSelectionMethod()
{

}

vector<int> CFFTPivotSelectionMethod::selectPivots(CMetric *metric, vector<CIndexObject*> &data,int numPivots)
{
	 return selectPivots(metric, data,0,data.size(),numPivots);
}

vector<int> CFFTPivotSelectionMethod::selectPivots(CMetric *metric, vector<CIndexObject*>& data, int first, int dataSize, int numPivots)
{
	        int firstPivot = first; // Math.floor(first + Math.random() *
                                    // dataSize);
            return selectPivots(metric,data,first, dataSize, numPivots,firstPivot);
}

vector<int> CFFTPivotSelectionMethod::selectPivots(CMetric *metric, vector<CIndexObject*> &data,int first, int dataSize, int numPivots, int firstPivot)
{
	if (numPivots >= dataSize)
	{
		vector<int>* pivots = new vector<int>;
		for (int i = first; i < first+dataSize + 0; i++)
			pivots->push_back(i);

		return removeDuplicate(metric, data, *pivots,first,dataSize);
	}

	bool* isCenter = new bool[dataSize];
	double* minDist = new double[dataSize];
	for (int i = 0; i < dataSize; i++)
	{
		isCenter[i] = false;
		minDist[i] = DBL_MAX;
	}

	 isCenter[firstPivot-first] = true;

	 int* indices = new int[numPivots]; // indices is used to record the offsets
                                       // of the pivots in the original data list
	 indices[0] = firstPivot;

	 for (int i = 1; i < numPivots; i++)
                indices[i] = -1;
	 // transparently firstPivot is found already 


	 for (int centerSize = 1; centerSize < numPivots; centerSize++)
	 {
		        double currMaxDist = -1;
                CIndexObject* const lastCenter = data[indices[centerSize - 1]];
				
                for (int i = 0; i < dataSize; i++)
                {
                    if (isCenter[i] == false) // if the point is not a center, we should calculate the distance
											// between this point and the set of Centers, for each centerSize we
											// grasp one Center form the set of Centers.
                    {
                        double tempDist = metric->getDistance(data[i + first], lastCenter);

                        minDist[i] = (tempDist < minDist[i]) ? tempDist : minDist[i];

                        // TODO
                        if (minDist[i] > currMaxDist)
                        {
                            indices[centerSize] = i+first; // save the index the
                                                     // current farthest
                                                     // point
                            currMaxDist = minDist[i];
                        }

                    }
                }

                if (indices[centerSize] == -1)
                    break;
                else
                    isCenter[indices[centerSize]-first] = true;
	 }

	int returnSize = 0;
	while ((returnSize < numPivots) && (indices[returnSize] >= 0))
		returnSize++;

	if (returnSize > numPivots)
		returnSize = numPivots;
	vector<int> result;
		for(int i=0; i<returnSize; i++)
			result.push_back(indices[i]);
	return result;


}
vector<int> removeDuplicate(CMetric *metric,vector<CIndexObject*> data,vector<int> &pivots,int first,int dataSize)
{
	const int size = dataSize;
    bool* isDuplicate = new bool[size];
    for (int i=0; i<size; i++)
        isDuplicate[i] = false;
    for (int i=0; i<size-1; i++)
    {
        if (isDuplicate[i])
            continue;
        for (int j=i+1; j<size; j++)
        {
            if (isDuplicate[j])
                continue;
            if (metric->getDistance(data[i+first], data[j+first]) == 0)
                isDuplicate[j] = true;
        }
    }
        
    int counter = 0;
    for (int i=0; i< size; i++)
	{
        if (!isDuplicate[i])
            counter ++;
	}
        
    if (counter == size)
        return pivots;
    else
    {
        vector<int> temp;
        counter = 0;
        for (int i=0; i<size; i++)
            if (!isDuplicate[i])
				temp.push_back(pivots[i]);
        return temp;
    }
}


//vector<int> CFFTPivotSelectionMethod::selectPivots(CMetric *metric,vector<CIndexObject*> data,int numPivots)
//{
//	vector<int> ve;
//	for(int i=0;i<numPivots;i++)
//		ve.push_back(i);
//
//	return ve;
//}