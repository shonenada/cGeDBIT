#ifndef RANGEQUERY_H
#define RANGEQUERY_H

#include "query.h"

class CRangeQuery :
	public CQuery
{
public:
	CRangeQuery(double, CIndexObject*);
	CRangeQuery(CIndexObject*);
	
	~CRangeQuery(void);

	void setRadius(double);
	double getRadius() const;
private:
	double r;
};
#endif
