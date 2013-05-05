#include "../../HeaderFiles/query/Query.h"


CQuery::CQuery(CIndexObject* _q)
{
	q = _q;
}


CQuery::~CQuery(void)
{
}

void CQuery::setQueryObject(CIndexObject* _q)
{
	q=_q;
}
CIndexObject* CQuery::getQueryObject() const
{
	return q;
}