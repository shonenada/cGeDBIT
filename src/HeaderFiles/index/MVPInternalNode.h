#ifndef MVPINTERNALNODE_H
#define MVPINTERNALNODE_H

#include "IndexNode.h"

#include <boost/serialization/base_object.hpp>

/**@defgroup MVPINTERNALNODE SQAI:MVPINTERNALNODE
 * @author Fuli Lei
 * @version 2013-4-16
 * @{
 */

/*
 *this class packages the basic information of an internal node of mvp-tree.
 */
class CMVPInternalNode:public CIndexNode
{
public:
	/**none parameter constructor*/
	CMVPInternalNode();
	
	/**constructor with parameters parameters
	   @para pivots a vector contains all the memory address of pivots
	   @para lower this is a two dimension vector contains the maximum distance value from every pivot to the object in each sub-tree. the total number of rows is equal to the number of all pivots in this node while the number of column is equal to the number of child nodes
	   @para upper this is a two dimension vector contains the maximum distance value from every pivot to the object in each sub-tree. the total number of rows is equal to the number of all pivots in this node while the number of column is equal to the number of child nodes
	   @para child a sub-tree address vector
	   @para myHeight the layer of this node
	*/
	CMVPInternalNode(vector<CIndexObject*> pivots,vector<vector<double>> lower,vector<vector<double>> upper,vector<CIndexNode*> child,int myHeight);
	
	/**destructor*/
	~CMVPInternalNode();

	/**get the toltal number of child
	   @return return the toltal number of child
	 */
	int numChildren();

	/**get address of a child according the child index
		@return the root address of a child
	*/
	CIndexNode* getChildAddress(int childIndex);

	/**get all the address of sub-trees
	   @return a sub-tree address container
	  */
	vector<CIndexNode*>& getSubTree();


	/**get the lower bound value of every distance range, each element of the contain represent the distance from every object in sub-tree to every pivots
		@para pivotIndex the index of pivot in the pivot list
		@return the double value list of lower bound distance
	*/
	vector<double>& getChildLowerRange(int pivotIndex);
	/**get the upper band value of every distance range, each element of the contain represent the distance from every object in sub-tree to every pivots
		@para pivotIndex the index of pivot in the pivot list
		@return the double value list of upper bound distance
	*/
	vector<double>& getChildUpperRange(int pivotIndex);
	
	/**traversal from this internal node to its every sub-tree root to search out the proper objects base on the parameters
		@para q this object is given by the user according which kind of query does the user want to do later, there is some basic information packaged in the object  like the search radius if the user want do a range query search latter.
		@para metric this object will be use to calculate the distance of two objects
		@return the proper objects address list through a vector
	*/
	virtual vector<CIndexObject*>  search(CRangeQuery &q,CMetric &metric);

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar,const unsigned int version)
	{
		ar & boost::serialization::base_object<CIndexNode>(*this);

		ar & child;

		ar & upper;
		ar & lower;
	}

	/**this is a sub-tree root addresses list*/
	vector<CIndexNode*> child;
	/**In this two dimension vector contains the minimum distance value of data objects in each sub-partition to every given pivots. The total  number of rows of this vector is equal to the number of all pivots in this node while the number of column is equal to the number of child nodes in this node*/
	vector<vector<double>> upper;
	/**In this two dimension vector contains the maximum distance value of data objects in each sub-partition to every given pivots. The total  number of rows of this vector is equal to the number of all pivots in this node while the number of column is equal to the number of child nodes in this node*/
	vector<vector<double>> lower;

};
/**@}*/
#endif