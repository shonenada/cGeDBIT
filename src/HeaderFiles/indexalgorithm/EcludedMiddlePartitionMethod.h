#pragma once
#include "PartitionMethod.h"
#include "..\type\DoubleIndexObjectPair.h"


#include <algorithm>

class CEcludedMiddlePartitionMethod :
    public CPartitionMethod
{
public:
    CEcludedMiddlePartitionMethod(void);
    ~CEcludedMiddlePartitionMethod(void);

    CPartitionResults partition(CMetric *metric, const vector<CIndexObject*> &pivots,vector<CIndexObject*> &data, int first, int size, int numPartitions, int maxLeafSize);

    CPartitionResults partition(CMetric *metric, const vector<CIndexObject*> &pivots,vector<CIndexObject*> &data,int first,int size,int maxRadius,int numPartitions,int maxLeafSize,double middleProportion);

    int sort(vector<CDoubleIndexObjectPair> &array, int fromIndex, int toIndex);

    int getMid(vector<double> &distance);
    int getMin(vector<double> &distance,int begin,int end);
    int getMax(vector<double> &distance,int begin,int end);
};

