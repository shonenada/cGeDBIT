#include "../../HeaderFiles/query/RangeQuery.h"

CRangeQuery::CRangeQuery(double _r, CIndexObject* _q):CQuery(_q)
{
	r = _r;
}

CRangeQuery::CRangeQuery(CIndexObject *_q):CQuery(_q)
{
}

CRangeQuery::~CRangeQuery(void)
{
}

void CRangeQuery::setRadius(double _r)
{
	r = _r;
}

double CRangeQuery::getRadius() const
{
	return r;
}