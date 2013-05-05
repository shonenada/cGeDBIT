#ifndef MVPINDEX_H
#define MAPINDEX_H

#include "../indexalgorithm/PartitionMethod.h"
#include "../indexalgorithm/PivotSelectionMethod.h"

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/vector.hpp>


#include "Index.h"
#include "MVPInternalNode.h"
#include "MVPLeafNode.h"

class Task;

/**@defgroup MVPINDEX SQAI:MVPINDEX
* @author Fuli Lei, Qiaozhi Li, Yuxuan Qiu, Yaoda Liu
* @version 2013-4-18
*
*/
/**
this class is to build a mvp-tree and give out a search method, users can use the given search method to find out what they want quickly from the mvp-tree
*/
class CMVPIndex:public CIndex
{
public:
/*Null parameter constructor */
	CMVPIndex();

/**constructor with eight parameters.
 * @param data: the whole object set above which the mvp-tree will be built
 * @param metric: it is a distance function, we can use it to work out the distance of two objects
 * @param psm: the name of this parameter means pivot-selection-method, it is a vantage points selection method
 * @param pm: pm means partition-method, it is used to partition a given data set into several small partitions, we can get a partition list throught it
 * @param numPivot: the maxmum number of pivots in a single mvp-tree node
 * @param singlePivotFanout: the maxmum number of partitions related to a single pivots
 * @param maxLeafSize: the maxmum number of objects in a leaf node of mvp-tree
 * @param maxPathLength: the maxmum number of distance values stored in a single leaf node, each distance value represent the distance from the object in every leaf node to the pivots in the parent node of the mvp-tree
*/
	CMVPIndex(vector<CIndexObject*> &dataObjects,CMetric *metric,CPivotSelectionMethod* psm,CPartitionMethod* pm,int numPivot,int singlePivotFanout,int maxLeafSize,int maxPathLength);

	/** destructor */
	~CMVPIndex();

	/**this is the function to build a mvp-tree 
   	 * @parameter objectList: the whole object set above which the mvp-tree will be built 
	 */
	void bulkLoad(vector<CIndexObject*> &objectList);

	/*this is a search method, users can use it to find out all the objects in demand
	* @param q: this object is given by users for the search operation, distance from every object in the results to q.centerObject is not greater than r(the search radius)
	* @return return a vector contains all the objects that comform to the requirment that distance of each object to q.centerObject is not greater than r
	*/
	vector<CIndexObject*> search(CQuery* q);

	/** fucntion to create an root of mvp-tree
	 * @param task encapsulated the information which will be used to create the root node of mvp tree
	 * @return return the address of the mvp-tree root
	*/
	CIndexNode* createRoot(Task* task);

	/**function to create an internal node
	 * @param task encapsulated the information which will be used to create a internal node of mvp tree
	 */
	void createInternalNode(Task* task);

	/**function to create a leaf node
	 * @param task encapsulated the information which will be used to create a leaf node of mvp tree
	 */
	void createLeafNode(Task* task);

	/** write an object to external memory
	 *  @param out
	 */
	void write(text_oarchive & out);

	/** read an object from external memory
	 * @param in
	 */
	void read(text_iarchive & in);


	/**return the number of data objects that make up the index struct*/
	int size();

	/**return all data objects that make up the index struct*/
	vector<CIndexObject*> getAllDataPoints();

	/**return the root of mvp-tree*/
	CIndexNode* getMVPTreeRoot();

	/**return the member varible metric,which is used to calculate the distance of two data objects*/
	CMetric* getMetric();

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialization(Archive &ar,const unsigned int version)
	{
		ar & boost::serialization::base_object<CIndex>(*this);
		ar & root;
		ar & metric;
		ar & numPivots;
		ar & psm;
		ar & pm;

		ar & singlePivotFanout;
		ar & totalSize;
		ar & maxLeafSize;
		ar & maxPathLength;
		ar & numLeaf;
		ar & numInternalNodes;
	}

	/** the root of mvp-tree */
	CIndexNode* root;


	/** the metric over which this index will be built */
	CMetric *metric;

	/** the task list contains all the tasks that creating a internal or leaf node*/
	vector<Task*> taskList;

	/** the total number of pivots to use */
	int numPivots;

	/** the method,used when building a index structure,to select a pivot(or pivots) */
	CPivotSelectionMethod *psm;

	/** the method,used when building a index structure,to partition the data set */
	CPartitionMethod *pm;

	/** the fanout of a single pivot. the total fanout is singlePivotFanout*numPivot*/
	int singlePivotFanout;

	/** the total number of data objects over which the index is built */
	int totalSize;

	/** the maximum number of data objects in a leaf node */
	int maxLeafSize;

	/**the number of distances which are calculated previously and stored in a leaf node*/
	int maxPathLength;

	/** the number of leaf nodes */
	int numLeaf;

	/** the number of internal nodes of the index */
	int numInternalNodes;

};
#endif
