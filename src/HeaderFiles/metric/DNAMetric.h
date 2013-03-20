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
	double* distance;
	CDNAMetric(double*, int distWidth);

	virtual double getDistance(CIndexObject* one, CIndexObject* two);
	double getDistance(CDNA* one, CDNA* two);

};

#endif
