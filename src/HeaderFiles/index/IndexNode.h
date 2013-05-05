#ifndef INDEXNODE_H
#define INDEXNODE_H

#include "..\objects\IndexObject.h"
#include "..\query\RangeQuery.h"
#include "..\metric\Metric.h"

#include <boost/serialization/vector.hpp>

/**@defgroup INDEXNODE SQAI:INDEX
 * @author Fuli Lei
 * @version 2013-4-14
 * @{
 */

/**this is a abstract base class which include all the base information of a node in the index, all the derived class of this class will inherite the information descriped in the class*/
class CIndexNode
{
public:
	/**none parameter constructor
		this constructor will be called to create an instance of class CIndexNode
	*/
	CIndexNode();

	/**constructor with one parameter pivots. 
		this constructor will be called to create an instance of class CIndexNode

		@para pivots a vector include all the address of pivots belong to single node in the index structor
		@para height the height of current node
	*/
	CIndexNode(vector<CIndexObject*> pivots, int height);

	/**destructor
		the space of this class's instance will be released if the instance is no longer used in the program
	*/
	~CIndexNode();

	/** get the address of a pivot based on the pivot index in the parameter list
		@para pivot pivot index of a pivot in the pivot address vector in the node
		@return a pivot address in the main memory
	*/
	CIndexObject* getPivot(int pivotIndex);

	/**one can get all the pivots belongs to a single node, which is internal node or leaf node, through this function.
		@return a vector consists of all the pivot address in a single node
	*/
	vector<CIndexObject*>& getPivots();

	/**To get the number of child node in internal node or objects in leaf node. this function must be implemented in advance of being called.
		@return the total number of child node of current node is an internal node, otherwise it will return the total number of objects in a leaf node 
	*/
	virtual int numChildren()=0;

	/**set the height of this node
	   @para degree the value of height of this node
	*/
	void setHeight(int degree);

	/**get the height of this node
	   @return the value of height
	*/
	int getHeight();

	/**this is function will be called when traversal from the root to the leaf doing the search operation
		@para q this object package an some information used to do the search
		@para metric one can get the distance of two objects through the member function of this parameter
		@raturn return a vector containing all the proper objects in this node 
	*/
	virtual vector<CIndexObject*>  search(CRangeQuery &q,CMetric &metric)=0;

protected:
    /**this is a vector containing the address of all the pivots belonging to this node*/
	vector<CIndexObject*> pivots;
	
private:
	friend class boost::serialization::access;
	template<class Archive>
	/**Function for object serialization*/
	void serialize(Archive &ar,const unsigned int version)
	{
		ar & pivots;
		ar & height;
	}

	

	/**the height of current node, the height of mvp-tree root is 0 while the height of every child of the root is equal to root.height+1,so every child node's height is equal to its father node's height+1*/
	int height;

};
/**@}*/
#endif