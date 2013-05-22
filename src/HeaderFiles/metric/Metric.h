#ifndef METRIC_H
#define METRIC_H
#include "../objects/IndexObject.h"

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
/** @file Metric.h
 * @classes about metirc methods
 * @author Fuli Lei
 * @version 2012-12-09
*/


/**
* @class CMetric
* @this is a class type.
* @abstract class about methic methods
* @author Fuli Lei
* class name is CMetric, this class descripe the
* basic information of how to get the distance of two object
*/

class CMetric
{
public:
	    /** no parameter constructor function*/
        CMetric();
		/** destructor function of this class type for releasing the
		 *  internal memory space which allocated dynamically
		 */
        ~CMetric();

		/** compute the distance of two given CIndexObject(or it's subclass) objects*/
        virtual double getDistance(CIndexObject*,CIndexObject*)=0;

private:
		/** @defgroup METRICSERIALIZATION
		 *  @{
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
        }
		/** @}*/
		/** @}*/
};

#endif