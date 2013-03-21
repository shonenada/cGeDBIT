#ifndef RNAMETRIC_H
#define RNAMETRIC_H

#include <iostream>
#include "Metric.h"
#include "../objects/RNA.h"

using namespace std;

class CRNAMetric:
	public CMetric
{
public:
	int distWidth;
	static double EditDistanceMatrix[RNASYMBOLNUMBER][RNASYMBOLNUMBER];
	CRNAMetric();

	virtual double getDistance(CIndexObject* one, CIndexObject* two);
	double getDistance(CRNA* one, CRNA* two);

};

#endif
