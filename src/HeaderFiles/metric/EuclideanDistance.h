#ifndef EUCLIDEANDISTANCE_H
#define EUCLIDEANDISTANCE_H
#include "Metric.h"

#include <boost/serialization/base_object.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
/** @file EuclideanDistance.h
 * @classes about metirc methods
 * @author Fuli Lei
 * @version 2012-12-09
*/

/**
* @class CEuclideanDistance
* @abstract class about metric methods
* @author Fuli Lei
* this class will generate the distance of two object
*/

class CEuclideanDistance :
	public CMetric
{
public:
	/** no parameter constructor function*/
	CEuclideanDistance();
	/** destructor function*/
	~CEuclideanDistance();

	/** return distance of two CIndexObject object*/
	virtual double getDistance(CIndexObject*,CIndexObject*);
	/** return distance of two double array*/
	virtual double getDistance(double *,double *,int);
private:
	/**@defgroup EULICEANDISTANCESERIALIZATION
	* @{
	*/
	/** to allow the class of boost to access private members of this class  */			
	friend class boost::serialization::access;
	/**@name function-template a template for serialization
	* @{
	*/ 
	/** a template declaration */
	template<typename Archive>
	/** function to serialize a object of this class */
	void serialize(Archive & ar,const unsigned int version)
	{
		ar & boost::serialization::base_object<CMetric>(*this);
	}
	/**@}*/
	/**@}*/
};
/**@}*/
#endif


