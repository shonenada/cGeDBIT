#pragma once

#include "Index.h"
#include "IndexNode.h"

#include "../indexalgorithm/PartitionMethod.h"
#include "../indexalgorithm/PivotSelectionMethod.h"

#include "MVPInternalNode.h"
#include "MVPLeafNode.h"


#include <vector>

class ForestTask;

class CEcludedMiddleForest
{
public:
    CEcludedMiddleForest(void);
    CEcludedMiddleForest(vector<CIndexObject*> &dataObjects,CMetric *metric,CPivotSelectionMethod* psm,CPartitionMethod* pm,int numPivot,int singlePivotFanout,int maxLeafSize, int maxPathLength,double maxRadius,double middlePorprotion);
    ~CEcludedMiddleForest(void);

    void bulkLoad(vector<CIndexObject*> &objectList);
     /*
     @return :the first index of subDataList which haven't be partition
    */
    vector<CIndexObject*> createTree(vector<CIndexObject*> &objectList,int treeIndex);
    /*
    */

	/*
		this is a search method, users can use it to find out all the objects in demand

		@parameter CQuery: this object is given by users, all the result of this function are found out based on this object
	*/
	//vector<CIndexObject*> search(CQuery*);

	/* fucntion to create an root of mvp-tree*/
	CIndexNode* createRoot(ForestTask *task,int treeIndex,vector<CIndexObject*>& ecludedData);

	/* function to create an internal node*/
	void createInternalNode(ForestTask *task,int treeIndex,vector<CIndexObject*>& ecludedData);

	/* function to create a leaf node*/
	void createLeafNode(ForestTask *task,int treeIndex,vector<CIndexObject*>& ecludedData);

    vector<CIndexObject*> search(CQuery* q);



    
private:

    vector<CIndexNode*> roots;

    //vector<CMVPIndex*> forest;

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

