#ifndef DNAMETRIC_H
#define DNAMETRIC_H

#include <iostream>
#include "Metric.h"
#include "../objects/DNA.h"

using namespace std;

class CDNAMetric:
	public CMetric
{
public:
	int distWidth;
	static double EditDistanceMatrix[DNASYMBOLNUMBER][DNASYMBOLNUMBER];
	CDNAMetric();

	virtual double getDistance(CIndexObject* one, CIndexObject* two);
	double getDistance(CDNA* one, CDNA* two);

};

#endif
