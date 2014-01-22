#include "../../HeaderFiles/index/ListIndex.h"
/** @file ListIndex.cpp
 * @classes about index
 * @author Fuli Lei
 * @version 2012-12-09
*/


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
void CListIndex::bulkLoad(vector<shared_ptr<CIndexObject> > &_data,int runMode)
{
    objectList = _data;
}


void CListIndex::bulkLoad(vector<shared_ptr<CIndexObject> > &_dataList)
{
	objectList=_dataList;
}

void CListIndex::writeExternal(string fileName)
{ 
	ofstream out(fileName,ios::out|ios::binary);
	
	if(!out)
		cout<<"open file "<<fileName<<" failed when writing index"<<endl;

	long size=objectList.size();
	
	out.write((char*)(&size),sizeof(long));

	for(int i=0;i<size;i++)
		objectList[i]->writeExternal(out);

	out.close();
}

void CListIndex::readExternal(string fileName)
{
	ifstream in(fileName,ios::in|ios::binary);
	if(!in)
		cout<<"open file "<<fileName<<" failed when reading index"<<endl;

	long size=0;
	
	in.read((char*)(&size),sizeof(long));

	for(int i=0;i<size;i++)
	{
		shared_ptr<CIndexObject> temp(new CDoubleVector());
		temp->readExternal(in);

		objectList.push_back(temp);
	}
	
	in.close();

}
/**get all the objects resides in the index structure built before
 * @return return a vector contains all the object address
*/
vector<shared_ptr<CIndexObject> > CListIndex::getAllDataPoints()
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


/**get the object which is used to calculate the distance of two objects when constructing or search in the index structure
   @return return the address of the distance metric object
*/
CMetric* CListIndex::getMetric()
{
    return metric;
}

/**Find out the proper object from the index structure built before
	   @param q this object is given by users for the search operation, distance from every object in the results to q.centerObject is not greater than r(the search radius)
	   @return return a vector contains all the objects that comform to the requirment that distance of each object to q.centerObject is not greater than r
*/
vector<shared_ptr<CIndexObject> >* CListIndex::search(CQuery* q)
{
	CRangeQuery *rq=(CRangeQuery*)q;
	vector<shared_ptr<CIndexObject> > *rs=new vector<shared_ptr<CIndexObject> >;
	double objectDistance = 0;
	double radius = 0;
	for (vector<CIndexObject*>::size_type i=0;i!=objectList.size();i++)
    {
        objectDistance = metric->getDistance(objectList.at(i).get(),rq->getQueryObject().get());

        radius = rq->getRadius();
		if(objectDistance<=radius)
			rs->push_back(objectList.at(i));
    }
    return rs;
}
