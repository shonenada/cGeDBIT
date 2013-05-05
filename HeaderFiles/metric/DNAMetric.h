#ifndef SQAI_METRIC_DNAMETRIC_H
#define SQAI_METRIC_DNAMETRIC_H

#include <iostream>
#include "Metric.h"
#include "../objects/DNA.h"

using namespace std;

/**
 *  @defgroup CDNAMetric
 *  @author   Lyd.
 *  @version  2013/3/18
 *  @date     2012/3/18
 *  @{
 * 
 * This class define a metric to compute distance of two DNA sequence.
*/
class CDNAMetric:
	public CMetric
{
public:

    /** A edit distance matrix of each two DNA symbols. */
	static double EditDistanceMatrix[DNASYMBOLNUMBER][DNASYMBOLNUMBER];

    /** A no parameter constructor, do nothing */
    CDNAMetric();

    /** A destructor, do nothing */
    ~CDNAMetric();

    /**
     * @override
     * This method return two IndexObjects' distance.
     */
	virtual double getDistance(CIndexObject* one, CIndexObject* two);

    /**
     * This method return two DNA' distance.
     *  - Sum up edit distance of two DNA.
     */
	double getDistance(CDNA* one, CDNA* two);

};

/**
 * @}   // CRNAMetric
*/

#endif  
// SQAI_METRIC_DNAMETRIC_H
