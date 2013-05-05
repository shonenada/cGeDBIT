#ifndef PARTITIONRESULTS_H
#define PARTITIONRESULTS_H

#include "../objects/IndexObject.h"
#include "../index/IndexNode.h"
#include <boost/serialization/vector.hpp>

/**@defgroup PARTITIONRESULTS SQAI:PARTITIONRESULTS
 * @author Fuli Lei
 * @version 2013-4-18
 * @{
 */
/*this class is used to package the partition results of the function partition in class CPartitionMethod so that we can use all the results packaged to create an internal node*/
class CPartitionResults
{
public:
	/*
		null parameter constructor function
	*/
	CPartitionResults();
	/*
		this is a constructor, used to create an instance of class CPartitionResults

		@param offsets: this is an address container,each element of this vector represents the first object address of a small partition
		@param lowerBounds  In this two dimension vector contains the minimum distance value of data objects in each sub-partition to every given pivots. The total  number of rows of this vector is equal to the number of all pivots in this node while the number of column is equal to the number of child nodes in this node
		@param upperBounds:In this two dimension vector contains the minimum distance value of data objects in each sub-partition to every given pivots. The total  number of rows of this vector is equal to the number of all pivots in this node while the number of column is equal to the number of child nodes in this node
	*/
	CPartitionResults(vector<int> &offsets,vector<vector<double> > &lower,vector<vector<double> > &upper);

	/*
		the destructor
	*/
	~CPartitionResults();

	/*
		@return: return the size of partitions
	*/
	int partitionSize();

	/*
		@return: return the start address of an partition base on the given partition index 
	*/
	int getPartition(int partitionIndex);

	/*
		@return: return a distance value container
	*/
	vector<vector<double> >& getLowerBounds();

	/*
		@return: return a distance value container
	*/
	vector<vector<double> >& getUpperBounds();

private:

	/**each element of this vector represents the first object address of a small partition*/
	vector<int> offsets;

	/**In this two dimension vector contains the minimum distance value of data objects in each sub-partition to every given pivots. The total  number of rows of this vector is equal to the number of all pivots in this node while the number of column is equal to the number of child nodes in this node*/
	vector<vector<double> > lowerBounds;
	/**In this two dimension vector contains the maximum distance value of data objects in each sub-partition to every given pivots. The total  number of rows of this vector is equal to the number of all pivots in this node while the number of column is equal to the number of child nodes in this node*/
	vector<vector<double> > upperBounds;

};
/**@}*/
#endif
