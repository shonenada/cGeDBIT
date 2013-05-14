#ifndef PeptideMETRIC_H
#define PeptideMETRIC_H

#include <iostream>
#include "../../HeaderFiles/metric/Metric.h"
#include "../../HeaderFiles/objects/Peptide.h"

using namespace std;

//#define PeptideSYMBOLNUMBER 21 //OR 24 numbers

class CPeptideMetric:
	public CMetric
{
public:

	/** A edit distance matrix of each two Peptide symbols. */
	static double EditDistanceMatrix[PeptideSYMBOLNUMBER-3][PeptideSYMBOLNUMBER-3];
   
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
     * This method return two peptides' distance.
     *  - Sum up edit distance of two peptide.
     */
	double getDistance(CPeptide* one, CPeptide* two);
};

/**
 * @}   // CPeptideMetric
*/

#endif
// SQAI_METRIC_PeptideMETRIC_H