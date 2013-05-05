#ifndef LISTINDEX_H
#define LISTINDEX_H
#include "Index.h"
#include "..\query\RangeQuery.h"

#include <string>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/vector.hpp>

/**@defgroup LISTINDEX SQAI:LISTINDEX
 * @author
 * @version 2013-4-18
 * @{
 */

/**A liner index structure can be built through this class and then write structure into the file stored in the external memory*/
class CListIndex :
    public CIndex
{
public:
	/**constructor*/
	CListIndex();

	/**constructor with a parameter
	   @param metric used for calculating the distance of two objects
	*/
	CListIndex(CMetric *metric);

	/**destructor*/
    ~CListIndex();

	/**A index struct can be built through this function
	   @param objectList this is a vector contains all the objects of the index structure built later
	*/
    void bulkLoad(vector<CIndexObject*> &objectList);

	/**Find out the proper object from the index structure built before
	   @param q this object is given by users for the search operation, distance from every object in the results to q.centerObject is not greater than r(the search radius)
	   @return return a vector contains all the objects that comform to the requirment that distance of each object to q.centerObject is not greater than r
	*/
    vector<CIndexObject*> search(CQuery *q);

	/**get all the objects resides in the index structure built before
	   @return return a vector contains all the object address
	*/
    virtual vector<CIndexObject*> getAllDataPoints();

	/**get the object which is used to calculate the distance of two objects when constructing or search in the index structure
	   @return return the address of the distance object
	*/
    virtual CMetric* getMetric();

	/**get the number of objects in the index structure
	   @return int the size of objectList
	*/
	virtual int size();

	/**function for serialiaztion. 
	   write index structure to the file stored in the external memory
	*/
	virtual void write(text_oarchive &out);

    /**function for serialization. 
	   return index structure from the external memory file to main memory
	*/
    virtual void read(text_iarchive &in);

private:
	/**fucntion for serialization*/
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar,const unsigned int version)
    {
		ar & boost::serialization::base_object<CIndex>(*this);		
        ar & objectList;
		ar & metric;
      
    }

	/**this vector represents the linder struct*/
    vector<CIndexObject*> objectList;
	/**An distance object used to calculat the distance of two objects*/
    CMetric *metric;

};
/**@}*/
#endif
