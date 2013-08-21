#ifndef LINFINITYDISTANCE_H
#define LINFINITYDISTANCE_H
#include "Metric.h"

#include <boost/serialization/base_object.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

/** @defgroup LINFINITYDISTANCE SQAI:LINFINITYDISTANCE
 *  @author weipeng zhang
 *  @version 2013-7-18
 *  @{
 */

/** this class will generate the distance of two object*/
class CLInfinityDistance :
        public CMetric
{
public:
		/** no parameter constructor function*/
        CLInfinityDistance();
		/** destructor function*/
        ~CLInfinityDistance();

		/** return distance of two CIndexObject object*/
        virtual double getDistance(CIndexObject*,CIndexObject*);
		/** return distance of two double array*/
		virtual double getDistance(double *,double *,int);
private:
	    /**@defgroup LINFINITYDISTANCESERIALIZATION
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


