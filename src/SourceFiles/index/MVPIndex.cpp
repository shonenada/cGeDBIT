#include "../../HeaderFiles/index/MVPIndex.h"

/**In the whole process of building a mvp-tree, some same openration like creating a internal node or leaf node could be encapsulated as a task and then put the task into a queue or stack for the iteration of building child trees*/
class Task 
{
public:
	/**give class CMVPIndex a friend right to access the data information of a Task instance in the process of building a mvp tree*/
	friend CMVPIndex;

	/**move the pivot to the end of a object list partition
	 * @param pivotIndex a vector contains all the indices of pivots in the whole objects list
	*/
	void groupPivotsAtEnd(vector<int> &pivotIndex);

	/**get the index of current node in the parent's children list*/
	int getMyIndex();

	/**get the height of current node. the height of root is 0*/
	int getMyHeight();

	/**get the parent node's address*/
	CIndexNode* getParentIndex();

	/**get all the pivots that belongs to current node
	 * @param pivots a vector to store the address of pivot.This vector is initialized with none elements, all the elements is generated through calling thing function. This parameter is used to return the address of all pivot to the caller of this funciton.
	*/
	void getPivots(vector<CIndexObject*>&pivots);

	/**get all the objects that belong to the tree rooted at this node*/
	void getDataPoints(vector<CIndexObject*>&dataList);

private:
	/**constructor*/
	Task():dataList(*(new vector<CIndexObject*>))
	{
        /*vector<CIndexObject*> ve;
        this->dataList=ve;*/
		this->parentIndex=0;
		this->myIndex=0;
		this->myHeight=0;

		this->size=0;
		this->start=0;


		this->numPivots=0;

	};

/**constructor with six parameters
 *	@param dataList a vector contains the address of all the obejct over which this mvp tree is built
 *	@param parentIndex address of parent node
 *	@param start the start index of current object partition in the whole data list
 *	@param end the last object index of current object partition
 *	@param myIndex current node index in parent node's children list
 *	@param myHeight height of current node
*/
	Task(vector<CIndexObject*> &dataList,CIndexNode *parentIndex,int start,int end,int myIndex,int myHeight):dataList(dataList)
	{
		//this->dataList=dataList;
		this->parentIndex=parentIndex;
		this->myIndex=myIndex;

		this->myHeight=myHeight;


		this->start=start;
		this->size=end-start;

		numPivots=0;

	};

	/**a vector contains the address of all the obejct over which this mvp tree is built*/
	vector<CIndexObject*> &dataList;

	/**address of parent node*/
	CIndexNode* parentIndex;

	/**current node index in parent node's children list*/
	int myIndex;

	/**height of current node*/
	int myHeight;

	/**the start index of current object partition in the whole objects list*/
	int start;

	/**number of objects in current object partition*/
	int size;

	/**number of pivots in a node of mvp tree*/
	int numPivots;
};

/**move the pivot to the end of a object list partition
 * @param pivotIndex a vector contains all the indices of pivots in the whole objects list
*/
void Task::groupPivotsAtEnd(vector<int> &pivotIndex)
{
	/**initialize numPivots*/
	this->numPivots=pivotIndex.size();


	if(this->numPivots<this->size)
	{
		/**new address of pivots, the first pivots will be moved to the end of current object partition, start+this->size-1 is the end index of current object partition*/
		int newPivotAddressInCompressedData=start+this->size-1;

		/**move every pivot to the end of partiton*/
		for(int i=0;i<this->numPivots;i++)
		{
			CIndexObject* temp;

			/**exchange the address content of pivot element unit and the target element unit*/
			temp=this->dataList[pivotIndex[i]];
			dataList[pivotIndex[i]]=dataList[newPivotAddressInCompressedData];
			dataList[newPivotAddressInCompressedData]=temp;

			/**judge if the content of target elment unit is the address of another pivot whose index is also in the pivot index list, exchange the index the result of judge is true*/
			for(int j=i+1;j<this->numPivots;j++)
				if(pivotIndex[j]==newPivotAddressInCompressedData)
					pivotIndex[j]=pivotIndex[i];

			/**the first pivot move the end of partition, the next pivot is always moved to one element unit before the previous moved pivots*/
			newPivotAddressInCompressedData--;
		}
	}

}

/**get the index of current node in the parent's children list*/
CIndexNode* Task::getParentIndex()
{
	return this->parentIndex;
}

/**get the index of current node in the parent's children list*/
int Task::getMyIndex()
{
	return this->myIndex;
}

/**get the height of current node. the height of root is 0*/
int Task::getMyHeight()
{
	return this->myHeight;
}

/**get all the pivots that belongs to current node
 * @param pivots a vector to store the address of pivot.This vector is initialized with none elements, all the elements is generated through calling thing function. This parameter is used to return the address of all pivot to the caller of this funciton.
*/
void Task::getPivots(vector<CIndexObject*> &pivots)
{
	/**start should be the first pivot index in current partition*/ 
	int start=this->start+this->size-this->numPivots;


	for(int i=0;i<this->numPivots;i++)
	{
		pivots.push_back(this->dataList[start]);

		start++;
	}

}

/**get all the objects that belong to the tree rooted at this node*/
void Task::getDataPoints(vector<CIndexObject*> &objectList)
{
	int mySize=0;

	/**if the number of pivot is less 0,return all the objects in current partitin*/
	if(numPivots==-1)
		mySize=this->size;
	else
		mySize=this->size-this->numPivots;

	for(int i=start;i<start+mySize;i++)
	{
		objectList.push_back(this->dataList[i]);
	}
}

/**Null parameter constructor */
CMVPIndex::CMVPIndex()
{
	root=0;

	metric=0;

	psm=0;
	pm=0;

	singlePivotFanout=0;

	maxLeafSize=0;

	numPivots=0;
	numLeaf=0;
	numInternalNodes=0;
}

/**
 * constructor with eight parameters.
 *
 * @param data: the whole object set above which the mvp-tree will be built
 * @param metric: it is a distance function, we can use it to work out the distance of two objects
 * @param psm: the name of this parameter means pivot-selection-method, it is a vantage points selection method
 * @param pm: pm means partition-method, it is used to partition a given data set into several small partitions, we can get a partition list throught it
 * @param numPivot: the maxmum number of pivots in a single mvp-tree node
 * @param singlePivotFanout: the maxmum number of partitions related to a single pivots
 * @param maxLeafSize: the maxmum number of objects in a leaf node of mvp-tree
 * @param maxPathLength: the maxmum number of distance values stored in a single leaf node, each distance value represent the distance from the object in every leaf node to the pivots in the parent node of the mvp-tree
*/
CMVPIndex::CMVPIndex(vector<CIndexObject*> &dataObjects,CMetric *metric,CPivotSelectionMethod* psm,CPartitionMethod* pm,int numPivots,int singlePivotFanout,int maxLeafSize)
{
	this->root=0;
	this->metric=metric;
	this->numPivots=numPivots;
	this->psm=psm;
	this->pm=pm;
	this->singlePivotFanout=singlePivotFanout;
	this->totalSize=dataObjects.size();
	this->maxLeafSize=maxLeafSize;

	numLeaf=0;
	numInternalNodes=0;
}

/**destructor*/
CMVPIndex::~CMVPIndex()
{

}

/**function to create an internal node
 * @param task encapsulated the information which will be used to create a internal node of mvp tree
 */
void CMVPIndex::createInternalNode(Task *task)
{
	this->numInternalNodes ++;
	cout<<"internalNode:"<<numInternalNodes<<endl;

	/**get pivots of current node*/
	vector<CIndexObject*> pivots;
	task->getPivots(pivots);

	/**partition current partition into several smaller child partitons,the selected pivots are not in the section that will be partitioned in child partitions*/
	CPartitionResults pr=this->pm->partition(this->metric,pivots,task->dataList,task->start,task->size-task->numPivots,this->singlePivotFanout,this->maxLeafSize);

	int childrenNumber=pr.partitionSize();

	/**create an internal node and assign its addres to the children list of parent node*/
	vector<CIndexNode*> *subTreeNode=new vector<CIndexNode*>(childrenNumber-1);
	CMVPInternalNode *node=new CMVPInternalNode(pivots,pr.getLowerBounds(),pr.getUpperBounds(),*subTreeNode,task->getMyHeight()+1);
    
    if(this->root==0)
        this->root=node;
    else
	    (((CMVPInternalNode*)(task->getParentIndex()))->getSubTree())[task->myIndex-1]=node;

	/**create several tasks base on each of the child partitions created before and then push these tasks to the global variable queue for the iteration of building child trees*/
	for(int i=childrenNumber-1;i>0;i--)
	{
		Task *newTask=0;

		///**for the last child partition of current partition, the end index is the end index of current partiton, otherwise the end index is the start of next child partiton*/
		/*if(i==childrenNumber-1)
		{
			cout<<"start-end:"<<pr.getPartition(i-1)<<"-"<<task->start+task->size-task->numPivots<<endl;
			cout<<"the object list size of sub-tree is:"<<task->start+task->size-task->numPivots-pr.getPartition(i-1)<<endl;
		    newTask = new Task(task->dataList,child,pr.getPartition(i-1),pr.getPartition(i),i,task->getMyHeight()+1);
		}
		else
		{
			cout<<"start-end:"<<pr.getPartition(i-1)<<"-"<<pr.getPartition(i)<<endl;
			cout<<"the object list size of sub-tree is:"<<pr.getPartition(i)-pr.getPartition(i-1)<<endl;*/
		newTask = new Task(task->dataList,node,pr.getPartition(i-1),pr.getPartition(i),i,task->getMyHeight()+1);
		//}

		this->taskList.push_back(newTask);

	}

}

/**function to create a leaf node
 * @param task encapsulated the information which will be used to create a leaf node of mvp tree
 */
void CMVPIndex::createLeafNode(Task* task)
{
	this->numLeaf++;
	cout<<"leafNode:"<<numLeaf<<endl;
	/**get all the objects of current partition*/
	vector<CIndexObject*> children;
	task->getDataPoints(children);
    cout<<"child.size:"<<children.size()<<endl;
	/**get all the pivots of current node*/
	vector<CIndexObject*> pivots;
	task->getPivots(pivots);

	vector<vector<double> > distance;

	/**calcualte the distance from each of the objects of current parition to every pivots*/
	for(vector<CIndexObject*>::size_type i=0;i<pivots.size();i++)
	{
		vector<double> ve;
		
		for(vector<CIndexObject*>::size_type j=0;j<children.size();j++)
		{
			ve.push_back(this->metric->getDistance(children[j],pivots[i]));
		}
		
		distance.push_back(ve);
	}
	
	/**create a leaf node and assign its memory address to the child list of parent node*/
	CMVPLeafNode *mvpLeafNode=new CMVPLeafNode(pivots,children,distance,task->getMyHeight()+1);

    if(this->root==0)
        this->root=mvpLeafNode;
    else
	    (((CMVPInternalNode*)(task->getParentIndex()))->getSubTree())[task->myIndex-1]=mvpLeafNode;
	
}

/**this is the function to build a mvp-tree 
 * @parameter dataObjects: the whole object set above which the mvp-tree will be built 
 */
void CMVPIndex::bulkLoad(vector<CIndexObject*> &dataObjects)
{

	/**put the task that create the root node to the task queue*/
	Task *task=new Task(dataObjects,NULL,0,dataObjects.size(),0,0);
	this->taskList.push_back(task);
	
    /**use every task in the queue to create a internal node or leaf node until the the queue is null*/
	while(this->taskList.size()>0)
	{
		vector<Task*>::iterator taskToGet=this->taskList.begin();

		task=*taskToGet;

		/**delete current task from the queue*/
		this->taskList.erase(taskToGet);

		
		/**get the number of pivot*/
		numPivots = (this->numPivots>=task->size) ? task->size : this->numPivots;

		/**selecte several piovt from current partition based on the given parameter*/
		vector<int> pivotsIndex;
		pivotsIndex=this->psm->selectPivots(this->metric,task->dataList,task->start,task->size,numPivots);
			
		/**move the pivot to the end of current partition*/
		task->groupPivotsAtEnd(pivotsIndex);
		cout<<"taskList.size:"<<this->taskList.size()<<endl;
		int remainNodeSize=task->size-task->numPivots;//task->size-task->numPivots
		cout<<"remain node list size:"<<remainNodeSize<<endl;
			
		/**if the size of current partition is greater than the max size of a leaf node then create a internal node, otherwise create a leaf node.the max size of leaf node is given by the builder of the mvp-tree*/
		if(remainNodeSize>maxLeafSize)
			createInternalNode(task);
		else
			createLeafNode(task);
		
	}


}

/*this is a search method, users can use it to find out all the objects in demand
 * @param q: this object is given by users for the search operation, distance from every object in the results to q.centerObject is not greater than r(the search radius)
 * @return return a vector contains all the objects that comform to the requirment that distance of each object to q.centerObject is not greater than r
 */
vector<CIndexObject*> CMVPIndex::search(CQuery* q)
{
	vector<CIndexObject*> re;

	CRangeQuery *rq=(CRangeQuery*)q;
	re=root->search(*rq,*metric);
	return re;
}

/**return the number of data objects that make up the index struct*/
int CMVPIndex::size()
{
	return totalSize;
}

/**return all data objects that make up the index struct*/
vector<CIndexObject*> CMVPIndex::getAllDataPoints()
{
	vector<CIndexObject*> data;
	return data;
}

/**return the root of mvp-tree*/
CIndexNode* CMVPIndex::getMVPTreeRoot()
{
	return this->root;
}

/**return the member varible metric,which is used to calculate the distance of two data objects*/
CMetric* CMVPIndex::getMetric()
{
	return metric;
}

/**function for serialization implemented by the boost library 
 * write an object to external memory 
 *  @param out instance of class text_oarchive, please looking up boost library for detailed infromation if you need
 */
void CMVPIndex::write(text_oarchive &out)
{
	out<<*this;
}

/**function for deserialization implemented by the boost library
 * read an object from external memory
 * @param in instance of class text_iarchive, please looking up boost library for detailed infromation if you need
 */
void CMVPIndex::read(text_iarchive &in)
{
	in >> *this;
}
