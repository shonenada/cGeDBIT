#ifndef METRIC_H
#define METRIC_H
#include "../objects/IndexObject.h"

/** @defgroup METRIC SQAI:METRIC
 *	@author  Fuli Lei
 *  @version 2012-12-16
 *  @{
 */

/** this is a class type.
 *	class name is CMetric, this class descripe the
 *  basic information of how to get the distance of two object
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

};


/** @}*/

#endif