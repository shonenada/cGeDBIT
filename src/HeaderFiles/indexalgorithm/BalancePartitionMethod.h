#ifndef SQAI_INDEXALGORITHM_BALANCE_PARTITIONMETHOD_H
#define SQAI_INDEXALGORITHM_BALANCE_PARTITIONMETHOD_H

/**@file BalancePartitionMethod.h
 * @brief A partition method. This partition method will divide
 * data into some well-distributed partitions.
 * @author Yaoda Liu(2011150337@email.szu.edu.cn)
 * @date 2013/5/16
*/

#include "PartitionMethod.h"
#include "../type/DoubleIndexObjectPair.h"

#include <boost/serialization/base_object.hpp>

/**
 * @defgroup CBalancePartitionMethod
 * @author   Lyd.
 * @version  2013/5/16
 * @date     2013/5/16
 * @{
 * 
 * This class implements a kind of partition method,
 *  which partition the given data set into serveral samll partitions with
 *  the same cardinality.
 */

class CBalancePartitionMethod:
    public CPartitionMethod
{
public:

    /* Null parameter constructor */
    CBalancePartitionMethod();

    /* The destructor*/
    ~CBalancePartitionMethod();
    
    /*
     @parameter metric: a function used to calculate the distance of two data objects
     @parameter pivots: a vector container consists of some vantage data points
     @parameter data: this is the whole data set, which include all the data objects in the index strcture
     @parameter frist: the first object address of a fragment that will be partitioned into many small partitions. the fragment, which will be partitioned, is a small section of the whole data set
     @parameter size: the size of the fragment that will be partition into many small partitions
     @parameter numPartition: the maxmum number of partitions into which the given data set will be partitioned
     @parameter maxLeafSize: the maxmum number of objects included in the leaf node of mvp-tree
     @return : return a CPartitionResults object which pack the partition results together
    */
    CPartitionResults partition(CMetric *metric, const vector<CIndexObject*> &pivots,vector<CIndexObject*> &data, int first, int size, int numPartitions, int maxLeafSize);

	/*
	 * 
	 */
	virtual CPartitionResults partition(CMetric *metric, const vector<CIndexObject*> &pivots, vector<CIndexObject*> &data, int first, int size, double maxRadius, int numPartitions, int maxLeafSize, double middleProportion);
private:  

	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive & ar,const unsigned int version)
	{
		ar & boost::serialization::base_object<CPartitionMethod>(*this);
	}
};
/**
 * @}   //CBalancePartitionMethod
*/
#endif
// SQAI_INDEXALGORITHM_BALANCE_PARTITIONMETHOD_H
