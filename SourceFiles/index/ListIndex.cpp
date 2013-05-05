#include "../../HeaderFiles/index/ListIndex.h"

/***/
CListIndex::CListIndex()
{
	this->metric=0;
}

CListIndex::CListIndex(CMetric *metric)
{
	this->metric=metric;
}

CListIndex::~CListIndex()
{
}

void CListIndex::bulkLoad(vector<CIndexObject*> &_data)
{
    objectList = _data;
}

vector<CIndexObject*> CListIndex::getAllDataPoints()
{
    return objectList;
}

int CListIndex::size()
{
	return objectList.size();
}

void CListIndex::write(text_oarchive &out)
{
    out << *this;
}

void CListIndex::read(text_iarchive &in)
{
    in >> *this;
}

CMetric* CListIndex::getMetric()
{
    return metric;
}

vector<CIndexObject*> CListIndex::search(CQuery* q)
{
	CRangeQuery *rq=(CRangeQuery*)q;
    vector<CIndexObject*> rs;

	for (vector<CIndexObject*>::size_type i=0;i!=objectList.size();i++)
    {
		if(metric->getDistance(objectList.at(i),rq->getQueryObject())<rq->getRadius())
			rs.push_back(objectList.at(i));
    }
    return rs;
}
