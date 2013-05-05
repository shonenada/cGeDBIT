#ifndef PIVOTSELECTIONMETHOD_H
#define PIVOTSELECTIONMETHOD_H

#include "..\metric\Metric.h"
#include <vector>

#include <boost/serialization/vector.hpp>

/**@defgroup PIVOTSELECTIONMETHOD SQAI:PIVOTSELECTIONMETHOD
 * @author Fuli lei, Yuxuan Qiu
 * @version 2013-4-18
 * @{
 */
/*
	select some data objects from the given data set as the pivots, which will be used in the process of search to exclude some part of the data objects so that we can
	what we want with fewer distance calculations
*/

class CPivotSelectionMethod
{
public:
	/**none parameter constructor*/
	CPivotSelectionMethod(){}
	virtual ~CPivotSelectionMethod(){}
	/*
		@para metric: this is a function used to calculate the distance of two data objects
		@para data: this is a data set, from which vantage points are selected
		@para first: the first object address of a small object section, from which this function will find all the vantage points address
		@para dataSize: the size of a small section, from which this function will find all the vantage points address
		@para numPivots: this is the maxmum number of pivots selected from the data set

		@return : return a container consists of the indexes of the selected pivots in the given data set 
	*/
	virtual vector<int> selectPivots(CMetric *metric,vector<CIndexObject*> &data,int first,int size,int numPivots)=0;
	
	/*
		@parameter metric: this is a function used to calculate the distance of two data objects
		@parameter data: this is a data set, from which vantage points are selected
		@parameter numPivots: this is the maxmum number of pivots selected from the data set

		@return : return a container consists of the indexes of the selected pivots in the given data set  
	*/
	virtual vector<int> selectPivots(CMetric *metric, vector<CIndexObject*> &data,int numPivots) = 0;
private:
	/**function for serialization*/
	friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar,const unsigned int version)
    {
	}
};
/**@}*/
#endif