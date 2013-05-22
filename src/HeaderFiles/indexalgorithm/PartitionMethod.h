#ifndef PARTITIONMETHOD_H
#define PARTITIONMETHOD_H

#include "../metric/Metric.h"

#include "PartitionResults.h"
/** @file PartitionMethod.h
 * @classes about index algrithm used in the process of building a index structure
 * @author Fuli Lei,Yaoda Liu
 * @version 2012-12-09
*/

/**
* @class CPartitionMethod
* @abstract class about partition methods
* @author Fuli Lei,Yaoda Liu
* partiton the given data set into some small sections based on the distance from each of the objects in the data set to every given pivot
*/

class CPartitionMethod
{
public:
	/*
		@parameter metric: a function used to calculate the distance of two data objects
		@parameter pivots: a vector container consists of some vantage data points
		@parameter data: this is the whole data set, which include all the data objects in the index strcture
		@parameter first: the first object address of a fragment that will be partitioned into many small partitions. the fragment, which will be partitioned, is a small section of the whole data set
		@parameter size: the size of the fragment that will be partition into many small partitions
		@parameter numPartition: the maximal number of partitions into which the given data set will be partitioned
		@parameter maxLeafSize: the maximal number of objects included in the leaf node of mvp-tree

		@return : return a CPartitionResults object which pack the partition results together
	*/
	virtual CPartitionResults partition(CMetric *metric, const vector<CIndexObject*> &pivots,vector<CIndexObject*> &data,int first,int size,int numPartitions,int maxLeafSize)=0;

     virtual CPartitionResults partition(CMetric *metric, const vector<CIndexObject*> &pivots,vector<CIndexObject*> &data,int first,int size,double maxRadius,int numPartitions,int maxLeafSize,double middleProportion)=0;
	
	/**fucntion for serialization*/
	friend class boost::serialization::access;
	template<typename Archive>
	void serialize(Archive &ar,const unsigned int version)
	{
	}
};

/**@}*/
#endif