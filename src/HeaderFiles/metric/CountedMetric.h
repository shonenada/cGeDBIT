#pragma once
#include "../objects/IndexObject.h"
#include "Metric.h"

class CountedMetric : public CMetric
{
private:
	CMetric *baseMetric;
	int  counter;
	
public:
	CountedMetric(void);
	~CountedMetric(void);
	CountedMetric(CMetric *baseMetric);
	double getDistance(CIndexObject *one, CIndexObject *two);
	int getCounter();
	void clear();
};

