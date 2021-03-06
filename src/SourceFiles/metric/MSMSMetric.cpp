#include "../../HeaderFiles/metric/Metric.h"
#include "../../HeaderFiles/metric/MSMSMetric.h"
#include "../../HeaderFiles/objects/TandemSpectra.h"

#include<iostream>
#include<cmath>
#include <string>

using namespace std;

/**
* MSMSMetric is an implementation of a fuzzy cosine distance metric for comparing tandem spectra
* signatures. Elements of the vectors are equal, within a given tolerance.
* 
* @author Smriti Ramakrishnan, Willard
* @version 2004.11.29
*/

/**
* default constructor provides default values for min, max, step, tolerance
* min = 0; 
* max = 0;
* step = 0; 
* tol = MSMSConstants.MS_TOLERANCE
*/
CMSMSMetric::CMSMSMetric(void):COS_THRESHOLD(0.2)
{

	min = 0;
	max = 0;
	step = 0;
	tol = MS_TOLERANCE;

	mscosdist = 0.0;
	absMassDiff = 0.0;
	massDiffTerm = 0.0;




}

CMSMSMetric::CMSMSMetric(int _min, int _max, double _step, double _tol):COS_THRESHOLD(0.2) {
	// super(min, max, step);
	min = _min;
	max = _max;
	step = _step;
	tol = _tol;

	mscosdist = 0.0;
	absMassDiff = 0.0;
	massDiffTerm = 0.0;
}

/**
* @param v1
*        the {@link IndexObject} over which the keys are defined.
* @param v2
*        the other {@link IndexObject} over which the keys are defined.
*/
double CMSMSMetric::getDistance(CIndexObject* one,CIndexObject* two)
{
	CTandemSpectra* v1=(CTandemSpectra *)one;
	CTandemSpectra* v2=(CTandemSpectra *)two;

	double cos=getCosine(v1,v2);

	if (fabs(fabs(cos) - 1) < COS_THRESHOLD) { // precision
		if (cos > 0)
			cos = 1; // very similar
		else
			cos = -1; // very unsimilar
	} else if (fabs(cos) > 1) {

		cout<<"COS_THRESHOLD = " <<COS_THRESHOLD <<", cos = "<<cos<<endl;

		cout<<"got cosine > 1, cosine=" <<cos <<", :";
		v1->toString();
		cout<<", v2:";
		v2->toString() ;
		cout<<"Quitting.";
		cout<<endl;

		exit(0);
	}

	mscosdist = acos(cos);
	massDiffTerm = getAbsPrecursorMassDiff(v1, v2);
	double dist = massDiffTerm + mscosdist;

	return dist;
}

/**
* @param v1
*        the {@link SpectraWithPrecursorMass} over which the keys are defined.
* @param v2
*        the other {@link SpectraWithPrecursorMass} over which the keys are defined.
*/
double CMSMSMetric::getCosine(CTandemSpectra* one , CTandemSpectra* two)
{
	double *v1 = one->getSpectra();
	double *v2= two->getSpectra();
	int v1Length=one->getLen();
	int v2Length=two->getLen();
	return getInnerProduct(v1,v1Length, v2,v2Length) / (getMagnitude(v1Length) * getMagnitude(v2Length));

	// smriti - 30sep
	/*
	* double numer = getInnerProduct(v1, v2) ; double denom = getMagnitude(v1) *
	* getMagnitude(v2); double denom = getMagnitude(v1) * getMagnitude(v1); double d2 =
	* getMagnitude(v2) * getMagnitude(v2); denom = (denom + d2) / 2; return numer/denom;
	*/
}

/**
* Computes inner product within a certain tolerance-- "fuzzy" inner product.
*/
int CMSMSMetric::getInnerProduct(double* v1,int v1Length, double* v2,int v2Length)
{
	int dist = 0;
	int i = 0, j = 0;
	double val1;
	double val2;

	// System.out.println("TOL = " + tol);
	while (i < v1Length && j < v2Length) {
		val1 = v1[i];
		val2 = v2[j];
		// new version -- gets rid of Math.abs()
		if (val1 <= val2) {
			if (val2 - val1 <= tol) {
				dist++;
				i++;
				j++;
			} else {
				i++;
			}
		} else {
			if (val1 - val2 <= tol) {
				dist++;
				i++;
				j++;
			} else {
				j++;
			}
		}

		/*
		* old code if (Math.abs(val1 - val2) <= tol) { dist++; i++; j++; } else { if (val1 <
		* val2) i++; else j++; }
		*/
		// debug
		// System.out.println("i = " + i + ", j = " + j);
	}
	// System.out.println("INNER PROD = " + dist);
	return dist;
}


/**
* Computes the magnitude of the double array by treating it as a *sparse* binary vector (store
* only 1's)
* 
* @param length
* @return
*/
double CMSMSMetric::getMagnitude(int length)
{
	return sqrt(length);
}
/**
* Returns absolute difference between precursor masses - within a tolerance
*/
double   CMSMSMetric::getAbsPrecursorMassDiff(CTandemSpectra* v1,CTandemSpectra* v2)
{
	double m1 = v1->getPrecursorMass();
	double m2 = v2->getPrecursorMass();

	absMassDiff = abs(m1 - m2);

	if (absMassDiff < COS_THRESHOLD)
		absMassDiff = 0.0;

	if (absMassDiff <= MS_PRECURSOR_TOLERANCE)
		return 0.0;
	else
		return (absMassDiff);
}




CMSMSMetric::~CMSMSMetric(void)
{
}
const double CMSMSMetric::MS_PRECURSOR_TOLERANCE=2.0;
const double CMSMSMetric::MS_TOLERANCE=0.2;