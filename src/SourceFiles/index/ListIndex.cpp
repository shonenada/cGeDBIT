#include "../../HeaderFiles/index/ListIndex.h"

/**constructor*/
CListIndex::CListIndex()
{
	this->metric=0;
}

/**constructor with a parameter
 * @param metric used for calculating the distance of two objects
*/
CListIndex::CListIndex(CMetric *metric)
{
	this->metric=metric;
}
/**destructor*/
CListIndex::~CListIndex()
{
}

/**A index struct can be built through this function
 * @param objectList this is a vector contains all the objects of the index structure built later
*/
void CListIndex::bulkLoad(vector<CIndexObject*> &_data)
{
    objectList = _data;
}

/**get all the objects resides in the index structure built before
 * @return return a vector contains all the object address
*/
vector<CIndexObject*> CListIndex::getAllDataPoints()
{
    return objectList;
}

/**get the number of objects in the index structure
 * @return int the size of objectList
*/
int CListIndex::size()
{
	return objectList.size();
}

/**function for serialiaztion. 
   write index structure to the file stored in the external memory
*/
void CListIndex::write(text_oarchive &out)
{
    out << *this;
}

/**function for serialization. 
   return index structure from the external memory file to main memory
*/
void CListIndex::read(text_iarchive &in)
{
    in >> *this;
}

/**get the object which is used to calculate the distance of two objects when constructing or search in the index structure
 * @return return the address of the distance object
*/
CMetric* CListIndex::getMetric()
{
    return metric;
}

/**Find out the proper object from the index structure built before
 * @param q this object is given by users for the search operation, distance from every object in the results to q.centerObject is not greater than r(the search radius)
 * @return return a vector contains all the objects that comform to the requirment that distance of each object to q.centerObject is not greater than r
*/
vector<CIndexObject*> CListIndex::search(CQuery* q)
{
	CRangeQuery *rq=(CRangeQuery*)q;
    vector<CIndexObject*> rs;

	for (vector<CIndexObject*>::size_type i=0;i!=objectList.size();i++)
    {
		if(metric->getDistance(objectList.at(i),rq->getQueryObject())<=rq->getRadius())
			rs.push_back(objectList.at(i));
    }
    return rs;
}
