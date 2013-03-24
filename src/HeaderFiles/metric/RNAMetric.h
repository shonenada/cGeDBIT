#ifndef SQAI_METRIC_RNAMETRIC_H
#define SQAI_METRIC_RNAMETRIC_H

#include <iostream>
#include "Metric.h"
#include "../objects/RNA.h"

using namespace std;

/**
 *  @defgroup CRNAMetric
 *  @author   Lyd., Wuxing Feng
 *  @version  2013/3/18
 *  @date     2012/3/18
 *  @{
 * 
 * This class define a metric to compute distance of two RNA sequence.
*/
class CRNAMetric:
	public CMetric
{
public:
    /** A edit distance matrix of each two rna symbols. */
	static double EditDistanceMatrix[RNASYMBOLNUMBER][RNASYMBOLNUMBER];

    /** A no parameter constructor, do nothing */
	CRNAMetric();

    /** A destructor, do nothing */
    ~CRNAMetric();

    /**
     * @override
     * This method return two IndexObjects' distance.
     */
	virtual double getDistance(CIndexObject* one, CIndexObject* two);

    /**
     * This method return two RNA' distance.
     *  - Sum up edit distance of two RNA.
     */
	double getDistance(CRNA* one, CRNA* two);

};

/**
 * @}   // CRNAMetric
*/


#endif
// SQAI_METRIC_RNAMETRIC_H
