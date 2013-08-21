#pragma once

#include "Index.h"
#include "IndexNode.h"

#include "../indexalgorithm/PartitionMethod.h"
#include "../indexalgorithm/PivotSelectionMethod.h"

#include "MVPInternalNode.h"
#include "MVPLeafNode.h"


#include <vector>
#include <memory>

/** @file 
* @brief A Forest built by EcludedMiddlePartitionMethod
* @author Qiaozhi LI
* @date 2013.5.26
* @version 1.0.0
* @note
* Include the em-forest and a task class to build an em-forest
*/


/**
* @class ForestTask
* @brief class about tasks that should completed during the process of building a em-forest
* @author Qiaozhi Li
* @note
* In the whole process of building a em-Forest, some same openration like creating a internal node or leaf node and necessary information could be encapsulated as a task and then put the task into a queue or stack for the iteration of building child trees
*/
class ForestTask;

class CEcludedMiddleForest
{
public:
    CEcludedMiddleForest(void);

    /**
    * @brief constructor with 10 parameters.
    * @param data the whole object set above which the mvp-tree will be built
    * @param metric it is a distance function, we can use it to work out the distance of two objects
    * @param psm the name of this parameter means pivot-selection-method, it is a vantage points selection method
    * @param pm pm means partition-method, it is used to partition a given data set into several small partitions, we can get a partition list throught it
    * @param numPivot the maxmum number of pivots in a single mvp-tree node
    * @param singlePivotFanout the maxmum number of partitions related to a single pivots
    * @param maxLeafSize the maxmum number of objects in a leaf node of mvp-tree
    * @param maxPathLength the maxmum number of distance values stored in a single leaf node, each distance value represent the distance from the object in every leaf node to the pivots in the parent node of the mvp-tree
    * @param maxRadius the exclusion width of middle partition 
    * @param middlePorprotion the porprotion of middle partition in each dataset which will be partitioned
    * @note
    *
    */
    CEcludedMiddleForest(vector<CIndexObject*> &dataObjects,CMetric *metric,CPivotSelectionMethod* psm,CPartitionMethod* pm,int numPivot,int singlePivotFanout,int maxLeafSize, int maxPathLength,double maxRadius,double middlePorprotion);
    ~CEcludedMiddleForest(void);


    /**
    * @brief build an em-forest 
    * @auther Qiaozhi Li
    * @param objectList data which will be used to build an em-forest
    * @note
    *
    */
    void bulkLoad(vector<CIndexObject*> &objectList ,int runMode);
    /**
    * @brief Create a tree of em-forest
    * @param objectList data which will be used to build a tree of em-forest
    * @param treeIndex the index of the tree will be created in the em-forest
    * @return The datalist ecluded by partitionmethod
    * @note
    *
    */
    vector<CIndexObject*> createTree(vector<CIndexObject*> &objectList,int treeIndex);

    /**
    * @brief fucntion to create an root of em-forest
    * @param task a ForestTask encapsulated necessary information to create a node
    * @note
    *
    */
    CIndexNode* createRoot(ForestTask *task,vector<CIndexObject*>& ecludedData);

    /**
    * @brief fucntion to create an internalnode
    * @param task a ForestTask encapsulated necessary information to create a node
    * @note
    *
    */
    void createInternalNode(ForestTask *task,vector<CIndexObject*>& ecludedData);

    /**
    * @brief fucntion to create a leafNode
    * @param task a ForestTask encapsulated necessary information to create a node
    * @note
    *
    */
    void createLeafNode(ForestTask *task,vector<CIndexObject*>& ecludedData);

    /*
    * @brief this is a search method, users can use it to find out all the objects in demand
    * @param CQuery this object is given by users, all the result of this function are found out based on this object
    * @return search result
    * @note
    *
    */
    vector<CIndexObject*> search(CQuery* q);




private:

    vector<CIndexNode*> roots;


    CPartitionMethod *pm;
    CPivotSelectionMethod *psm;
    CMetric *metric;
    vector<ForestTask*> forestTaskList;

    int numPivots;
    int singlePivotFanout;
    int total_size;
    int numLeaf;
    int numInternalNodes;
    int maxLeafSize;
    int maxPathLength;

    double middlePorprotion;
    double maxRadius;





};

