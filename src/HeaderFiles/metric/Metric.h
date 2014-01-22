#ifndef METRIC_H
#define METRIC_H
#include "../objects/IndexObject.h"

/** @file Metric.h
 * @classes about metirc methods
 * @author Fuli Lei
 * @version 2012-12-09
*/


/**
* @class CMetric
* @brief abstract class about methic methods
* @author Fuli Lei
* @version 2013526
*
* class name is CMetric, this class descripe the basic information of how to get the distance of two object.It specifies a binary function to compute distance
 * between two data points in metric space. The distance should have the metric properties, i.e.
 * non-negativity, symmetry and triangle inequality.
*/

class CMetric
{
public:
	    /**@brief no parameter constructor function*/
        CMetric();
		/**@brief destructor function of this class type for releasing the internal memory space which allocated dynamically
		 */
		~CMetric();

		/**@brief compute the distance of two given CIndexObject(or it's subclass) objects
            @return return the distance of to objects.
        */
        virtual double getDistance(CIndexObject*,CIndexObject*)=0;

};

#endif