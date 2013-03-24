#ifndef SQAI_METRIC_PEPTIDEMETRIC_H
#define SQAI_METRIC_PEPTIDEMETRIC_H

#include <iostream>
#include "Metric.h"
#include "../objects/Peptide.h"

using namespace std;

/**
 *  @defgroup CRNAMetric
 *  @author   Lyd.
 *  @version  2013/3/18
 *  @date     2012/3/18
 *  @{
 * 
 * This class define a metric to compute distance of two peptide.
*/
class CPeptideMetric:
	public CMetric
{
public:

    /** A edit distance matrix of each two amino acid. */
	static double mPAM250aExtendedWeightMatrix[AMINOACIDNUMBER-2][AMINOACIDNUMBER-2];

    /** A no parameter constructor, do nothing */
    CPeptideMetric();

    /** A destructor, do nothing */
    ~CPeptideMetric();

    /**
     * @override
     * This method return two IndexObjects' distance.
     */
	virtual double getDistance(CIndexObject* one, CIndexObject* two);

    /**
     * This method return two peptide' distance.
     *  - Sum up edit distance of two peptide.
     */
	double getDistance(CPeptide* one, CPeptide* two);

};

#endif
// SQAI_METRIC_PEPTIDEMETRIC_H