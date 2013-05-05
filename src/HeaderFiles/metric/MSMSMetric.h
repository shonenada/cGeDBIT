#ifndef MSMSMETRIC_H
#define MSMSMETRIC_H

#include "..\..\HeaderFiles\metric\Metric.h"
#include "..\..\HeaderFiles\objects\IndexObject.h"
#include "..\..\HeaderFiles\objects\TandemSpectra.h"

/**
* MSMSMetric is an implementation of a fuzzy cosine distance metric for comparing tandem spectra
* signatures. Elements of the vectors are equal, within a given tolerance.
* 
* @author Smriti Ramakrishnan, Willard
* @version 2004.11.29
*/
class CMSMSMetric :
	public CMetric
{
public:

	/**
	* default constructor provides default values for min, max, step, tolerance
	* min = 0; 
	* max = 0;
	* step = 0; 
	* tol = MSMSConstants.MS_TOLERANCE
	*/
	CMSMSMetric(void);


	CMSMSMetric(int ,int , double , double );


	/**
	* @param v1
	*        the {@link IndexObject} over which the keys are defined.
	* @param v2
	*        the other {@link IndexObject} over which the keys are defined.
	*/
	double getDistance(CIndexObject*,CIndexObject*);


	double	getAbsPrecursorMassDiff(CTandemSpectra* ,CTandemSpectra* );

	/**
	* @param v1
	*        the {@link SpectraWithPrecursorMass} over which the keys are defined.
	* @param v2
	*        the other {@link SpectraWithPrecursorMass} over which the keys are defined.
	*/
	double getCosine(CTandemSpectra* , CTandemSpectra*);

	/**
	* Computes inner product within a certain tolerance-- "fuzzy" inner product.
	*/
	int getInnerProduct(double * ,int ,double * , int   );

	double getMagnitude(int );


	static  const double MS_PRECURSOR_TOLERANCE;
	static	const double MS_TOLERANCE;



	~CMSMSMetric(void);

private:

	int min ;
	int max ;
	double step ;
	double tol ;

	double mscosdist;

	double absMassDiff;

	double massDiffTerm;

	const double  COS_THRESHOLD ;
};
#endif