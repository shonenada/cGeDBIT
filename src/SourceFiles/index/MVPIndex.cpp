#include "../../HeaderFiles/index/MVPIndex.h"
#include "../../HeaderFiles/objects/DoubleVector.h"


/** @file MVPIndex.cpp
* @classes about mvp tree index structure
* @author Fuli Lei,Qiaozhi Li
* @version 2013-6-20
*/

/**
* @class Task
* @brief class about tasks that should be completed during the process of building a mvp-tree
* @author Fuli Lei
*
* In the whole process of building a mvp-tree, some same openration like creating a internal node or leaf node could be encapsulated as a task and then put the task into a queue or stack for the iteration of building child trees
*/
class Task 
{
public:
    /**give class CMVPIndex a friend right to access the data information of a Task instance in the process of building a mvp tree*/
    friend CMVPIndex;

	/**@brief constructor with six parameters
    *	@param dataList a vector contains the address of all the obejct over which this mvp tree is built
    *	@param parentIndex address of parent node
    *	@param start the start index of current object partition in the whole data list
    *	@param end the last object index of current object partition
    *	@param myIndex current node index in parent node's children list
    *	@param myHeight height of current node
    */
    Task(vector< shared_ptr<CIndexObject> > &dataList,shared_ptr<CIndexNode> parentIndex,int start,int end,int myIndex,int myHeight):dataList(dataList)
    {
        this->parentIndex=parentIndex;
        this->myIndex=myIndex;

        this->myHeight=myHeight;


        this->start=start;
        this->size=end-start;

        numPivots=0;

        this->node=0;

    };

    ~Task()
    {        
        //cout<<"task will be destroyed !"<<endl;
		//node.reset();
    }

    /**@brief move the pivot to the end of a object list partition
    * @param pivotIndex a vector contains all the indices of pivots in the whole objects list
    */
    void groupPivotsAtEnd(vector<int> &pivotIndex);

    /**@brief get the index of current node in the parent's children list*/
    int getMyIndex();

    /**@brief get the height of current node. the height of root is 0*/
    int getMyHeight();

    /**@brief get the parent node's address*/
    shared_ptr<CIndexNode> getParentIndex();

    /**@brief get all the pivots that belongs to current node
    * @param pivots a vector to store the address of pivot.This vector is initialized with none elements, all the elements is generated through calling thing function. This parameter is used to return the address of all pivot to the caller of this funciton.
    */
    void getPivots(vector< shared_ptr<CIndexObject> >&pivots);

    /**@brief get all the objects that belong to the tree rooted at this node*/
    void getDataPoints(vector< shared_ptr<CIndexObject> >&dataList);

	vector<shared_ptr<CIndexObject>>& getDataList();
	long getSize();
	int getPivotNumber();
	long getStart();

    void setNode(shared_ptr<CIndexNode> node)
    {
        this->node = node;
    }
    shared_ptr<CIndexNode> getNode()
    {
        return node;
    }
private:
    /**constructor*/
    Task():dataList(*(new vector< shared_ptr<CIndexObject> >))
    {

        this->parentIndex=0;
        this->myIndex=0;
        this->myHeight=0;

        this->size=0;
        this->start=0;


        this->numPivots=0;

        this->node=0;

    };

    

    

    /**a vector contains the address of all the obejct over which this mvp tree is built*/
    vector< shared_ptr<CIndexObject> > &dataList;

    /**address of parent node*/
    shared_ptr<CIndexNode> parentIndex;

    shared_ptr<CIndexNode> node;

    long myIndexAdress;

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

/**@brief move the pivot to the end of a object list partition
* @param pivotIndex a vector contains all the indices of pivots in the whole objects list
*/
void Task::groupPivotsAtEnd(vector<int> &pivotIndex)
{
    /*initialize numPivots*/
    this->numPivots=pivotIndex.size();


    if(this->numPivots<this->size)
    {
        /*new address of pivots, the first pivots will be moved to the end of current object partition, start+this->size-1 is the end index of current object partition*/
        int newPivotAddressInCompressedData=start+this->size-1;

        /*move every pivot to the end of partiton*/
        for(int i=0;i<this->numPivots;i++)
        {
            shared_ptr<CIndexObject> temp;

            /*exchange the address content of pivot element unit and the target element unit*/
            temp=this->dataList[pivotIndex[i]];
            dataList[pivotIndex[i]]=dataList[newPivotAddressInCompressedData];
            dataList[newPivotAddressInCompressedData]=temp;

            /*judge if the content of target elment unit is the address of another pivot whose index is also in the pivot index list, exchange the index the result of judge is true*/
            for(int j=i+1;j<this->numPivots;j++)
                if(pivotIndex[j]==newPivotAddressInCompressedData)
                    pivotIndex[j]=pivotIndex[i];

            /*the first pivot move the end of partition, the next pivot is always moved to one element unit before the previous moved pivots*/
            newPivotAddressInCompressedData--;
        }
    }

}

/**@brief get the index of current node in the parent's children list*/
shared_ptr<CIndexNode> Task::getParentIndex()
{
    return this->parentIndex;
}

/**@brief get the index of current node in the parent's children list*/
int Task::getMyIndex()
{
    return this->myIndex;
}

/**@brief get the height of current node. the height of root is 0*/
int Task::getMyHeight()
{
    return this->myHeight;
}

/**@brief get all the pivots that belongs to current node
* @param pivots a vector to store the address of pivot.This vector is initialized with none elements, all the elements is generated through calling thing function. This parameter is used to return the address of all pivot to the caller of this funciton.
*/
void Task::getPivots(vector< shared_ptr<CIndexObject> > &pivots)
{
    /*start should be the first pivot index in current partition*/ 
    int start=this->start+this->size-this->numPivots;


    for(int i=0;i<this->numPivots;i++)
    {
        pivots.push_back(this->dataList[start]);

        start++;
    }

}

/**@brief get all the objects that belong to the tree rooted at this node*/
void Task::getDataPoints(vector< shared_ptr<CIndexObject> > &objectList)
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

vector<shared_ptr<CIndexObject> >& Task::getDataList()
{
	return dataList;
}

long Task::getSize()
{
	return size;
}

int Task::getPivotNumber()
{
	return numPivots;
}

long Task::getStart()
{
	return start;
}

/////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////



/**@brief None parameter constructor */
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

void CMVPIndex::setMetric(CMetric *objMetric)
{
    metric = objMetric;
}

/**@brief constructor with eight parameters.
* @param data the whole object set above which the mvp-tree will be built
* @param metric it is a distance function, we can use it to work out the distance of two objects
* @param psm the name of this parameter means pivot-selection-method, it is a vantage points selection method
* @param pm pm means partition-method, it is used to partition a given data set into several small partitions, we can get a partition list throught it
* @param numPivot the maxmum number of pivots in a single mvp-tree node
* @param singlePivotFanout the maxmum number of partitions related to a single pivots
* @param maxLeafSize the maxmum number of objects in a leaf node of mvp-tree
* @param maxPathLength the maxmum number of distance values stored in a single leaf node, each distance value represent the distance from the object in every leaf node to the pivots in the parent node of the mvp-tree
*/
CMVPIndex::CMVPIndex(vector<shared_ptr<CIndexObject> > &dataObjects,CMetric *metric,CPivotSelectionMethod* psm,CPartitionMethod* pm,int numPivots,int singlePivotFanout,int maxLeafSize)
{
    this->root=0;
    this->metric=metric;
    this->numPivots=numPivots;
    this->psm=psm;
    this->pm=pm;
    this->singlePivotFanout=singlePivotFanout;
    this->totalSize=dataObjects.size();
    this->maxLeafSize=maxLeafSize;

	filePointer=0;
    numLeaf=0;
    numInternalNodes=0;
}

/**@brief destructor*/
CMVPIndex::~CMVPIndex()
{

}

////////////////////////////////////////////////////////////////

void CMVPIndex::bulkLoad(vector<shared_ptr<CIndexObject> > &data,int indexMode)
{
    if (indexMode == 0)
        loadLeafToRoot(data);
	else if(indexMode == 1)
        loadRootToLeaf(data);
	else 
		//parallelLoadLeafToRoot(data);
		parallelBuildingMVPtree(data);
}

///////////////////////////////////////////////////////////////

/**@brief function to create an internal node
* @param task encapsulated the information which will be used to create a internal node of mvp tree
*/
//void CMVPIndex::createInternalNode(Task *task)
//{
//	this->numInternalNodes ++;
//	//cout<<"internalNode:"<<numInternalNodes<<endl;
//	/*get pivots of current node*/
//	vector<shared_ptr<CIndexObject> > pivots;
//	task->getPivots(pivots);
//
//	/*partition current partition into several smaller child partitons,the selected pivots are not in the section that will be partitioned in child partitions*/
//	CPartitionResults pr=this->pm->partition(this->metric,pivots,task->dataList,task->start,task->size-task->numPivots,this->singlePivotFanout,this->maxLeafSize);
//
//	int childrenNumber=pr.partitionSize();
//
//	/*create an internal node and assign its addres to the children list of parent node*/
//    vector<shared_ptr<CIndexNode> > *subTreeNode = new vector<shared_ptr<CIndexNode> >(childrenNumber-1);
//	shared_ptr<CMVPInternalNode> node(new CMVPInternalNode(pivots,pr.getLowerBounds(),pr.getUpperBounds(),*subTreeNode,task->getMyHeight()+1));
//
//	if(this->root==0)
//		this->root=node;
//	else
//		(((CMVPInternalNode*)(task->getParentIndex()))->getSubTree())[task->myIndex-1]=node;
//
//	/*create several tasks base on each of the child partitions created before and then push these tasks to the global variable queue for the iteration of building child trees*/
//	for(int i=childrenNumber-1;i>0;i--)
//	{
//		Task *newTask=0;
//
//		newTask = new Task(task->dataList,node,pr.getPartition(i-1),pr.getPartition(i),i,task->getMyHeight()+1);
//
//
//		this->taskList.push_back(newTask);
//
//
//
//		this->taskList.insert(this->taskList.begin(),newTask);
//		//this->taskList.push_back(newTask);
//
//	}
//
//}

/**@brief function to create a leaf node
* @param task encapsulated the information which will be used to create a leaf node of mvp tree
*/
//void CMVPIndex::createLeafNode(Task* task)
//{
//	this->numLeaf++;
//	//cout<<"leafNode:"<<numLeaf<<endl;
//	/*get all the objects of current partition*/
//	vector<CIndexObject*> children;
//	task->getDataPoints(children);
//
//	/*get all the pivots of current node*/
//	vector<CIndexObject*> pivots;
//	task->getPivots(pivots);
//
//	vector<vector<double>> distance;
//
//	/*calcualte the distance from each of the objects of current parition to every pivots*/
//	for(vector<CIndexObject*>::size_type i=0;i<pivots.size();i++)
//	{
//		vector<double> ve;
//
//		for(vector<CIndexObject*>::size_type j=0;j<children.size();j++)
//		{
//			ve.push_back(this->metric->getDistance(children[j],pivots[i]));
//		}
//
//		distance.push_back(ve);
//	}
//
//	/*create a leaf node and assign its memory address to the child list of parent node*/
//	CMVPLeafNode *mvpLeafNode=new CMVPLeafNode(pivots,children,distance,task->getMyHeight()+1);
//
//	if(this->root==0)
//		this->root=mvpLeafNode;
//	else
//		(((CMVPInternalNode*)(task->getParentIndex()))->getSubTree())[task->myIndex-1]=mvpLeafNode;
//
//}

/**@brief this is the function to build a mvp-tree 
*@param dataObjects the whole object set above which the mvp-tree will be built
*/




vector<shared_ptr<CIndexObject> > CMVPIndex::travelSearch(CQuery* q)
{
    vector<shared_ptr<CIndexObject> > re;

    CRangeQuery *rq=(CRangeQuery*)q;
    root->travelSearch(*rq,*metric,re);
    return re;
}

/**@brief get the number of data objects that make up the index struct
*@return the number of data objects that make up the index struct
*/
int CMVPIndex::size()
{
    return totalSize;
}

int traversingMVPTree(shared_ptr<CIndexNode> node, vector<shared_ptr<CIndexObject> > &data)
{
    if(node==NULL)
        return 0;

    shared_ptr<CMVPInternalNode> interNode(dynamic_cast<CMVPInternalNode*>(node.get()));
    if(interNode!=0)
    {
        for(int i=0;i<interNode->getPivots().size();i++)
        {
            data.push_back(interNode->getPivot(i));
        }
        int childNum=interNode->numChildren();
        for(int i=0;i<childNum;i++)
            traversingMVPTree(interNode->getChildAddress(i),data);
        return 0;
    }

    shared_ptr<CMVPLeafNode> leafNode(dynamic_cast<CMVPLeafNode*>(node.get()));
    if(leafNode!=0)
    {
        for(int i=0;i<leafNode->getPivots().size();i++)
        {
            data.push_back(leafNode->getPivot(i));
        }
        for(int i=0;i<leafNode->numChildren();i++)
        {
            data.push_back(leafNode->getObject(i));
        }
    }
}

/**@brief get the object list contains all the objects that make up the index structure
*@return all data objects that make up the index structure
*/
vector<shared_ptr<CIndexObject> > CMVPIndex::getAllDataPoints()
{
    vector<shared_ptr<CIndexObject> > data;
    traversingMVPTree(this->root,data);
    return data;
}

/**@brief get the root of a mvptree built before
*@return the root of mvp-tree
*/
shared_ptr<CIndexNode> CMVPIndex::getMVPTreeRoot()
{
    return this->root;
}

/**@brief get the metric object of a mvptree
*@return the member varible metric,which is used to calculate the distance of two data objects
*/
CMetric* CMVPIndex::getMetric()
{
    return metric;
}





void CMVPIndex::setDataType(string dataType)
{
    this->dataType = dataType;
}

string CMVPIndex::getDataType()
{
    return this->dataType;
}

void CMVPIndex::setFileName(string fileName)
{
    this->fileName=fileName;
}

string CMVPIndex::getFileName()
{
    return fileName;
}

/*@brief this is a search method, users can use it to find out all the objects in demand
* @param q: this object is given by users for the search operation, distance from every object in the results to q.centerObject is not greater than r(the search radius)
* @return return a vector contains all the objects that comform to the requirment that distance of each object to q.centerObject is not greater than r
*/
vector<shared_ptr<CIndexObject> >* CMVPIndex::search(CQuery* q)
{
    //CMemMonitor::updateMem();

   
    CRangeQuery *rq=(CRangeQuery*)q;


    filePointer = 0;

    ifstream in(fileName,ios::binary);

    in.seekg(-(long)sizeof(long),ios::end);

    in.read((char*)(&rootAddress),sizeof(long));


    in.seekg(rootAddress,ios::beg);


    shared_ptr<CIndexNode> root;
    char* type = new char[6];
    in.read(type,6*sizeof(char));
    if(!strcmp(type,"INODE"))
    {
        root = dynamic_pointer_cast<CMVPInternalNode> (root);
        root.reset(new CMVPInternalNode());
        /*root = new CMVPInternalNode();*/
    }
    else
    {
        root = dynamic_pointer_cast<CMVPLeafNode> (root);
        root.reset( new CMVPLeafNode());
        //root = new CMVPLeafNode();
    }
    rootAddress += 6*sizeof(char);
    delete(type);	
    root->searchIndex(*rq,rootAddress,in,*metric,re,dataType);

   // CMemMonitor::updateMem();
    in.close();
   // CMemMonitor::updateMem();

    return &re;

}

/*DATA STRUCT FOR BOTH WINDOWS AND C++11 STANDARD VERSION OF MULTI-THREAD CODING*/
struct parallelBlock
{
	/*constuctor*/
	//parallelBlock(){cout<<"building a parallel block"<<endl;}
	/*desturctor*/
	//~parallelBlock(){cout<<"rootTask.useCount:"<<rootTask.use_count()<<" destroying a parallel block"<<endl;}	
	
	ofstream *fstr;
	int *numPivots;
	CMetric *metric;
	CPivotSelectionMethod *psm;
	CPartitionMethod *pm;

	vector<shared_ptr<Task> > *taskList;
	vector<shared_ptr<Task> > *newTaskList;
	int maxLeafSize;
	int singlePivotFanout;
	int *numInternalNode;
	int *numLeafNode;

	string fileName;
	long *filePointer;
	long *rootAddress;

	shared_ptr<Task> rootTask;
};


/***************************************************WINDOWS VERSION MULTI-THREAD CODE BEGIN*************************************************************/
HANDLE mutexA;
HANDLE mutexB;
HANDLE mutexO;

long threadANum=0;
long threadBNum=0;




int createInternalNodeToWrite(shared_ptr<Task> task,vector<shared_ptr<Task> > *taskList,vector<shared_ptr<Task> >*newTaskList,CMetric *metric,CPartitionMethod *pm,int *numInternalNodes,int start,int size,int singlePivotFanout,int maxLeafSize)
{
	(*numInternalNodes)++;

	/*get pivots of current node*/
    vector<shared_ptr<CIndexObject> > pivots;
    task->getPivots(pivots);


    /*partition current partition into several smaller child partitons,the selected pivots are not in the section that will be partitioned in child partitions*/
	CPartitionResults pr=pm->partition(metric,pivots,task->getDataList(),start,size - task->getPivotNumber(),singlePivotFanout,maxLeafSize);
    int childrenNumber=pr.partitionSize();

    /*create an internal node and assign its addres to the children list of parent node*/
    vector<shared_ptr<CIndexNode> > *subTreeNode = new vector<shared_ptr<CIndexNode> >(childrenNumber-1);
    shared_ptr< CMVPInternalNode> node(new CMVPInternalNode(pivots,pr.getLowerBounds(),pr.getUpperBounds(),*subTreeNode,task->getMyHeight()+1) );
    node->setChildSize(childrenNumber-1);


    task->setNode((shared_ptr<CIndexNode>) node );
	
	//shared_ptr<Task> temp;temp.reset(task);
	newTaskList->push_back(task);
    /*create several tasks base on each of the child partitions created before and then push these tasks to the global variable queue for the iteration of building child trees*/
    for(int i=childrenNumber-1;i>0;i--)
    {
        int j=0;
		shared_ptr<Task> newTask( new Task(task->getDataList(),node,pr.getPartition(i-1),pr.getPartition(i),i,task->getMyHeight()+1));       

        taskList->push_back(newTask);

    }

	return 0;
}

int createAndWriteLeafNode(shared_ptr<Task> task,ofstream *out,vector<shared_ptr<Task> > *taskList,string fileName,long *filePointer,int *numLeaf,CMetric *metric,long *rootAddress)
{

    (*numLeaf)++;

	/*get all the objects of current partition*/
    vector<shared_ptr<CIndexObject> > children;
    task->getDataPoints(children);

    /*get all the pivots of current node*/
    vector<shared_ptr<CIndexObject> > pivots;
    task->getPivots(pivots);

    vector<vector<double> > distance;

    /*calcualte the distance from each of the objects of current parition to every pivots*/
    for(vector<CIndexObject*>::size_type i=0;i<pivots.size();i++)
    {
        vector<double> ve;

        for(vector<CIndexObject*>::size_type j=0;j<children.size();j++)
        {
            ve.push_back(metric->getDistance(children[j].get(),pivots[i].get()));
        }

        distance.push_back(ve);
    }

    /*create a leaf node and assign its memory address to the child list of parent node*/
    shared_ptr<CMVPLeafNode> mvpLeafNode(new CMVPLeafNode(pivots,children,distance,task->getMyHeight()+1));

    if(*rootAddress==-2)
    {        

        task->setNode((shared_ptr<CIndexNode>)mvpLeafNode);
		taskList->push_back(task);
        return 0;
    }
    else
    {
		WaitForSingleObject(mutexO,INFINITE);
		out->open(fileName,ios::binary|ios::_Nocreate);
		out->seekp(*filePointer);
        char type[6] = "LNODE";
        ((CMVPInternalNode*)task->getParentIndex().get())->getSubTreeAddress()[task->getMyIndex()-1]=*filePointer;
        out->write(type,6*sizeof(char));
		*filePointer=*filePointer+(long)mvpLeafNode->writeExternal(*out)+6*sizeof(char);
		out->close();
		ReleaseMutex(mutexO);
		return 0;
    }

    
}

DWORD WINAPI threadOfCreateInternalOrLeafNode(LPVOID lpParameter)
{
	parallelBlock *pb=(parallelBlock*)lpParameter;
	WaitForSingleObject(mutexA,INFINITE);

	shared_ptr<Task> task=NULL;
	if(pb->taskList->size()>0)
	{
		vector<shared_ptr<Task> >::iterator ite=pb->taskList->end();
		ite--;
		task=*ite;
		pb->taskList->erase(ite);
	}
	ReleaseMutex(mutexA);

	if(task!=NULL)
	{
		/*get the number of pivot*/
		(*(pb->numPivots))=((*(pb->numPivots))>=task.get()->getSize())?task.get()->getSize():(*(pb->numPivots));

		/*selecte several piovt from current partition based on the given parameter*/
		vector<int> pivotsIndex;
		pivotsIndex=pb->psm->selectPivots(pb->metric,task.get()->getDataList(),task.get()->getStart(),task.get()->getSize(),*(pb->numPivots));

		/*move the pivot to the end of current partition*/
		task.get()->groupPivotsAtEnd(pivotsIndex);

		int remainDataSize=task.get()->getSize() - *(pb->numPivots);

		if(remainDataSize>pb->maxLeafSize)
		{
			createInternalNodeToWrite(task,pb->taskList,pb->newTaskList,pb->metric,pb->pm,pb->numInternalNode,task.get()->getStart(),task.get()->getSize(),pb->singlePivotFanout,pb->maxLeafSize);
		}
		else
		{
			createAndWriteLeafNode(task,pb->fstr,pb->newTaskList,pb->fileName,pb->filePointer,pb->numLeafNode,pb->metric,pb->rootAddress);
		}

	}

	threadANum--;

	return 0;

}

int writeInternalNode(shared_ptr<Task> task,ofstream *out,string fileName,long *filePointer)
{

	WaitForSingleObject(mutexO,INFINITE);
	out->open(fileName,ios::binary|ios::_Nocreate);

	out->seekp(*filePointer);

	CMVPInternalNode* parentNode = (CMVPInternalNode*)(task->getParentIndex().get());
	parentNode->getSubTreeAddress()[task->getMyIndex()-1]=*filePointer;

	char *type = "INODE";
	out->write(type,6*sizeof(char));
	*filePointer=*filePointer+(long)task->getNode()->writeExternal(*out)+6*sizeof(char);

	out->close();
	ReleaseMutex(mutexO);

	return 0;
}

DWORD WINAPI threadOfWriteInternal(LPVOID lpParameter)
{
	parallelBlock *pb=(parallelBlock*)lpParameter;

	WaitForSingleObject(mutexB,INFINITE);
	shared_ptr<Task> task=NULL;
	if(pb->newTaskList->size()>0)
	{
		vector<shared_ptr<Task> >::iterator ite=pb->newTaskList->end();
		ite--;
		task=*ite;
		pb->newTaskList->erase(ite);
	}
	ReleaseMutex(mutexB);
	if(task!=NULL)
	{
		if(task->getParentIndex()!=NULL)
		{
			writeInternalNode(task,pb->fstr,pb->fileName,pb->filePointer);
			threadBNum--;
		}
		else
		{
			pb->rootTask=task;
			threadBNum--;
		}
	}
	else
	{
		threadBNum--;
	}

	return 0;
}

void CMVPIndex::parallelLoadLeafToRoot(vector<shared_ptr<CIndexObject> >&dataObject)
{

	ofstream out(fileName,ios::out|ios::binary);
	out.close();

	filePointer=0;//the pointer of file
	
	HANDLE tHandle;

	vector<shared_ptr<Task> > newTaskList;

	mutexA=CreateMutex(NULL,0,NULL);
	mutexB=CreateMutex(NULL,0,NULL);
	mutexO=CreateMutex(NULL,0,NULL);

	if(dataObject.size()<=maxLeafSize+numPivots)
		rootAddress=-2;
	else
		rootAddress=-1;


	shared_ptr<Task> task(new Task(dataObject,NULL,0,dataObject.size(),0,0));

	this->taskList.push_back(task);

	parallelBlock pba;
	{
		pba.fstr=&out;
		pba.numPivots=&(this->numPivots);
		pba.metric=this->metric;
		pba.maxLeafSize=this->maxLeafSize;
		pba.taskList=&(this->taskList);
		pba.newTaskList=&newTaskList;
		pba.pm=this->pm;
		pba.psm=this->psm;
		pba.singlePivotFanout=this->singlePivotFanout;
		pba.numInternalNode=&(this->numInternalNodes);
		pba.numLeafNode=&(this->numLeaf);
		pba.fileName=this->fileName;
		pba.filePointer=&(this->filePointer);
		pba.rootAddress=&(this->rootAddress);
		pba.rootTask=NULL;
		
	}

	while(this->taskList.size()>0||threadANum>0)
	{

		
		if(this->taskList.size()>0&&threadANum<4)
		{

			threadANum++;
			tHandle=CreateThread(NULL,0,threadOfCreateInternalOrLeafNode,&pba,0,NULL);
			CloseHandle(tHandle);
		}
		else
		{

			Sleep(2000);
		}

	}

	while(this->taskList.size()>0||newTaskList.size()==0)
		Sleep(5000);

	parallelBlock pbb;
	{
		pbb.fstr=&out;
		pbb.numPivots=&(this->numPivots);
		pbb.metric=this->metric;
		pbb.maxLeafSize=this->maxLeafSize;
		pbb.taskList=&(this->taskList);
		pbb.newTaskList=&newTaskList;
		pbb.pm=this->pm;
		pbb.psm=this->psm;
		pbb.singlePivotFanout=this->singlePivotFanout;
		pbb.numInternalNode=&(this->numInternalNodes);
		pbb.numLeafNode=&(this->numLeaf);
		pbb.fileName=this->fileName;
		pbb.filePointer=&(this->filePointer);
		pbb.rootAddress=&(this->rootAddress);
		pbb.rootTask=NULL;
	}

	while(pbb.newTaskList->size()>0||threadBNum>0)
	{
		if(pbb.newTaskList->size()>0&&threadBNum<4)
		{
			threadBNum++;
			tHandle=CreateThread(NULL,0,threadOfWriteInternal,&pbb,0,NULL);
			CloseHandle(tHandle);
		}
		else 
		{
			Sleep(2000);
		}
	}

	while(pbb.newTaskList->size()>0||threadBNum>0||this->taskList.size()>0||threadANum>0)
		Sleep(5000);

	writeRoot(pbb.rootTask,out);

}
/***************************************************WINDOWS VERSION MULTI-THREAD CODE END*************************************************************/
/***************************************************C++11 STANDARD VERSION MULTI-THREAD CODE BEGIN***************************************************************/
mutex stMutexA;
mutex stMutexB;
mutex stMutexO;
mutex stMutexN;

int stdThreadNumA=0;
int stdThreadNumB=0;

int standardCreateInternalNodeToWrite(shared_ptr<Task> task,vector<shared_ptr<Task> > *taskList,vector<shared_ptr<Task> >*newTaskList,CMetric *metric,CPartitionMethod *pm,int *numInternalNodes,int start,int size,int singlePivotFanout,int maxLeafSize)
{
	(*numInternalNodes)++;

	/*get pivots of current node*/
    vector<shared_ptr<CIndexObject> > pivots;
    task->getPivots(pivots);


    /*partition current partition into several smaller child partitons,the selected pivots are not in the section that will be partitioned in child partitions*/
	CPartitionResults pr=pm->partition(metric,pivots,task->getDataList(),start,size - task->getPivotNumber(),singlePivotFanout,maxLeafSize);
    int childrenNumber=pr.partitionSize();

    /*create an internal node and assign its addres to the children list of parent node*/
    vector<shared_ptr<CIndexNode> > *subTreeNode = new vector<shared_ptr<CIndexNode> >(childrenNumber-1);
    shared_ptr< CMVPInternalNode> node(new CMVPInternalNode(pivots,pr.getLowerBounds(),pr.getUpperBounds(),*subTreeNode,task->getMyHeight()+1) );
    node->setChildSize(childrenNumber-1);


    task->setNode((shared_ptr<CIndexNode>) node );
	
	//shared_ptr<Task> temp;temp.reset(task);
	stMutexB.lock();
	newTaskList->push_back(task);
	stMutexB.unlock();
    /*create several tasks base on each of the child partitions created before and then push these tasks to the global variable queue for the iteration of building child trees*/
    for(int i=childrenNumber-1;i>0;i--)
    {
        int j=0;
		shared_ptr<Task> newTask( new Task(task->getDataList(),node,pr.getPartition(i-1),pr.getPartition(i),i,task->getMyHeight()+1));       
		stMutexA.lock();
        taskList->push_back(newTask);
		stMutexA.unlock();
    }

	return 0;
}

int standardCreateAndWriteLeafNode(shared_ptr<Task> task,ofstream *out,vector<shared_ptr<Task> > *taskList,string fileName,long *filePointer,int *numLeaf,CMetric *metric,long *rootAddress)
{

    (*numLeaf)++;

	/*get all the objects of current partition*/
    vector<shared_ptr<CIndexObject> > children;
    task->getDataPoints(children);

    /*get all the pivots of current node*/
    vector<shared_ptr<CIndexObject> > pivots;
    task->getPivots(pivots);

    vector<vector<double> > distance;

    /*calcualte the distance from each of the objects of current parition to every pivots*/
    for(vector<CIndexObject*>::size_type i=0;i<pivots.size();i++)
    {
        vector<double> ve;

        for(vector<CIndexObject*>::size_type j=0;j<children.size();j++)
        {
            ve.push_back(metric->getDistance(children[j].get(),pivots[i].get()));
        }

        distance.push_back(ve);
    }

    /*create a leaf node and assign its memory address to the child list of parent node*/
    shared_ptr<CMVPLeafNode> mvpLeafNode(new CMVPLeafNode(pivots,children,distance,task->getMyHeight()+1));

    if(*rootAddress==-2)
    {        

        task->setNode((shared_ptr<CIndexNode>)mvpLeafNode);
		taskList->push_back(task);
        return 0;
    }
    else
    {
		stMutexO.lock();
		//out->open(fileName,ios::binary|ios::_Nocreate);
		//out->seekp(*filePointer);
        char type[6] = "LNODE";
        ((CMVPInternalNode*)task->getParentIndex().get())->getSubTreeAddress()[task->getMyIndex()-1]=*filePointer;
        out->write(type,6*sizeof(char));
		*filePointer=*filePointer+(long)mvpLeafNode->writeExternal(*out)+6*sizeof(char);
		//out->close();
		stMutexO.unlock();
		return 0;
    }

    
}

void standardThreadOfCreateInternalOrLeafNode(parallelBlock *pb)
{
	
	stMutexA.lock();
	shared_ptr<Task> task=NULL;
	if(pb->taskList->size()>0)
	{
		vector<shared_ptr<Task> >::iterator ite=pb->taskList->end();
		ite--;
		task=*ite;
		pb->taskList->erase(ite);
	}
	stMutexA.unlock();

	if(task!=NULL)
	{
		/*get the number of pivot*/
		//(*(pb->numPivots))=((*(pb->numPivots))>=task.get()->getSize())?task.get()->getSize():(*(pb->numPivots));
		int pivotNum=((*(pb->numPivots)>=task.get()->getSize())?task.get()->getSize():(*(pb->numPivots)));

		/*selecte several piovt from current partition based on the given parameter*/
		vector<int> pivotsIndex;
		pivotsIndex=pb->psm->selectPivots(pb->metric,task.get()->getDataList(),task.get()->getStart(),task.get()->getSize(),pivotNum);

		/*move the pivot to the end of current partition*/
		task.get()->groupPivotsAtEnd(pivotsIndex);

		int remainDataSize=task.get()->getSize() - pivotNum;

		if(remainDataSize>pb->maxLeafSize)
		{
			standardCreateInternalNodeToWrite(task,pb->taskList,pb->newTaskList,pb->metric,pb->pm,pb->numInternalNode,task.get()->getStart(),task.get()->getSize(),pb->singlePivotFanout,pb->maxLeafSize);
		}
		else
		{
			standardCreateAndWriteLeafNode(task,pb->fstr,pb->newTaskList,pb->fileName,pb->filePointer,pb->numLeafNode,pb->metric,pb->rootAddress);
		}

	}

	stMutexN.lock();
	stdThreadNumA--;
	stMutexN.unlock();

	return;
}

int standardWriteInternalNode(shared_ptr<Task> task,ofstream *out,string fileName,long *filePointer)
{

	stMutexO.lock();
	//out->open(fileName,ios::binary|ios::_Nocreate);

	//out->seekp(*filePointer);

	CMVPInternalNode* parentNode = (CMVPInternalNode*)(task->getParentIndex().get());
	parentNode->getSubTreeAddress()[task->getMyIndex()-1]=*filePointer;

	char *type = "INODE";
	out->write(type,6*sizeof(char));
	*filePointer=*filePointer+(long)task->getNode()->writeExternal(*out)+6*sizeof(char);

	//out->close();
	stMutexO.unlock();
	return 0;
}

void standardThreadOfWriteInternalNode(parallelBlock *pb)
{

	stMutexB.lock();
	shared_ptr<Task> task=NULL;
	if(pb->newTaskList->size()>0)
	{
		vector<shared_ptr<Task> >::iterator ite=pb->newTaskList->end();
		ite--;
		task=*ite;
		pb->newTaskList->erase(ite);
	}
	stMutexB.unlock();

	if(task!=NULL)
	{
		if(task->getParentIndex()!=NULL)
		{
			standardWriteInternalNode(task,pb->fstr,pb->fileName,pb->filePointer);
			stMutexN.lock();
			stdThreadNumB--;
			stMutexN.unlock();
		}
		else
		{
			pb->rootTask=task;
			stMutexN.lock();
			stdThreadNumB--;
			stMutexN.unlock();
		}
	}
	else
	{
		stMutexN.lock();
		stdThreadNumB--;
		stMutexN.unlock();
	}

	return;
}

void CMVPIndex::parallelBuildingMVPtree(vector<shared_ptr<CIndexObject> > &dataObjectList)
{
	//cout<<"start parallel building"<<endl;
	ofstream outfile(fileName,ios::out|ios::binary);
	

	filePointer=0;

	if(dataObjectList.size()<=maxLeafSize+numPivots)
		rootAddress=-2;
	else
		rootAddress=-1;

	vector<shared_ptr<Task> > taskList;
	vector<shared_ptr<Task> > newTaskList;
	
	shared_ptr<Task> task(new Task(dataObjectList,NULL,0,dataObjectList.size(),0,0));//first task

	taskList.push_back(task);

	parallelBlock pba;
	{
		pba.fileName=fileName;
		pba.filePointer=&filePointer;
		pba.fstr=&outfile;
		pba.maxLeafSize=maxLeafSize;
		pba.metric=metric;
		pba.newTaskList=&newTaskList;
		pba.taskList=&taskList;
		pba.numInternalNode=&numInternalNodes;
		pba.numLeafNode=&numLeaf;
		pba.numPivots=&numPivots;
		pba.pm=pm;
		pba.psm=psm;
		pba.rootAddress=&rootAddress;
		pba.rootTask=NULL;
		pba.singlePivotFanout=singlePivotFanout;
	}

	int dataobjectSize=dataObjectList.size();
	int numThreadNum=pow(pow(singlePivotFanout,numPivots),ceil(log(dataobjectSize)/log(maxLeafSize)));

	while(taskList.size()>0||stdThreadNumA>0)
	{
		if(taskList.size()>0&&stdThreadNumA<numThreadNum)
		//if(taskList.size()>0)
		{
			stMutexN.lock();
			stdThreadNumA++;
			stMutexN.unlock();
			std::thread newThread(standardThreadOfCreateInternalOrLeafNode,&pba);
			newThread.detach();
			//newThread.join();
		}
		else
		{
		#ifdef WIN32
			Sleep(1000);
		#else
			sleep(1);
		#endif
		}
	}

	//cout<<threadANum<<" write leaf node over!"<<endl;

	while(taskList.size()>0||stdThreadNumA>0)
#ifdef WIN32
		Sleep(1000);
#else
		sleep(1);
#endif

/**
	parallelBlock pbb;
	{
		pbb.fstr=&outfile;
		pbb.numPivots=&(this->numPivots);
		pbb.metric=this->metric;
		pbb.maxLeafSize=this->maxLeafSize;
		pbb.taskList=&(this->taskList);
		pbb.newTaskList=&newTaskList;
		pbb.pm=this->pm;
		pbb.psm=this->psm;
		pbb.singlePivotFanout=this->singlePivotFanout;
		pbb.numInternalNode=&(this->numInternalNodes);
		pbb.numLeafNode=&(this->numLeaf);
		pbb.fileName=this->fileName;
		pbb.filePointer=&(this->filePointer);
		pbb.rootAddress=&(this->rootAddress);
		pbb.rootTask=NULL;
	}

	while(newTaskList.size()>0||stdThreadNumB>0)
	{
		if(newTaskList.size()>0&&stdThreadNumB<30)
		//if(newTaskList.size()>0)
		{
			stMutexN.lock();
			stdThreadNumB++;
			stMutexN.unlock();
			std::thread newThread(standardThreadOfWriteInternalNode,&pbb);
			newThread.join();
		}
		else
		{
		#ifdef WIN32
			Sleep(1000);
		#else
			sleep(1);
		#endif
		}
	}

	while(taskList.size()>0||newTaskList.size()>0||stdThreadNumA>0||stdThreadNumB>0)
#ifdef WIN32
		Sleep(1000);
#else
		sleep(1);
#endif

	outfile.close();
	writeRoot(pbb.rootTask,outfile);
**/

/**
TODO
    1. 上面的第二部分是多线程写内部结点，写过程主要是打开文件移动文件指针和关闭文件，没有cpu计算，而且采用多线程需要在设定文件指针锁，有额外开销，所以不用多线程应该会写的快点，这样会加快建树过程
	2. 写内部结点的时候写一个打开以下文件，关闭以下文件，这要消耗时间，应该可以从头到尾只开一次，关闭一次
DONE 20131124  by Fuli Lei
**/
	//cout<<__FILE__<<__LINE__<<endl;
	vector<shared_ptr<Task>>::iterator ite;

	while(newTaskList.size()>1)
	{
		ite=newTaskList.end();
		ite--;
		shared_ptr<Task> temp=*ite;
		
		writeInternalNode(temp,outfile);

		newTaskList.erase(ite);
	}
	outfile.close();

	while(taskList.size()>0||newTaskList.size()>1||stdThreadNumA>0)
#ifdef WIN32
		Sleep(1000);
#else
		sleep(1);
#endif

	writeRoot(newTaskList.at(0),outfile);


}

/***************************************************C++11 STANDARD VERSION MULTI-THREAD CODE END*************************************************************/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CMVPIndex::loadLeafToRoot(vector<shared_ptr<CIndexObject> > &dataObjects)
{

    //PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("log.properties"));
    //Logger logger = Logger::getRoot();

    ofstream out;

    out.open(fileName,ios::binary);
    out.close();

    filePointer = 0;
	if(dataObjects.size()<=maxLeafSize+numPivots)
    {
        rootAddress = -2;
    }
    else
    {
        rootAddress = -1;
    }

    vector<shared_ptr<Task> >::iterator taskToGet;

    /*put the task that create the root node to the task queue*/
    shared_ptr<Task> task(new Task(dataObjects,NULL,0,dataObjects.size(),0,0));

    this->taskList.push_back(task);
    int c=0;
    /*use every task in the queue to create a internal node or leaf node until the the queue is null*/
    while(this->taskList.size()>0)
    {

        taskToGet=this->taskList.end();
        int remainNodeSize;


        taskToGet--;

        task = *taskToGet;


        if(task->getNode()==0)
        {

            /*get the number of pivot*/
            numPivots = (this->numPivots>=task->size) ? task->size : this->numPivots;

            /*selecte several piovt from current partition based on the given parameter*/
            vector<int> pivotsIndex;
            pivotsIndex=this->psm->selectPivots(this->metric,task->dataList,task->start,task->size,numPivots);

            /*move the pivot to the end of current partition*/
            task->groupPivotsAtEnd(pivotsIndex);

            remainNodeSize=task->size - task->numPivots;//task->size-task->numPivots
            //LOG4CPLUS_TRACE(logspace::logToFile(),"remain size:"<<remainNodeSize);
            /*if the size of current partition is greater than the max size of a leaf node then create a internal node, otherwise create a leaf node.the max size of leaf node is given by the builder of the mvp-tree*/
            if(remainNodeSize>maxLeafSize)
                createInternalNodeToWrite(task);
            else
            {
                createAndWriteLeafNode(task,out);

                if(rootAddress!=-2)
                {
                    taskList.erase(taskToGet);
                    task.reset();
                }
                if(taskList.size()==1)
                    break;
            }
        }
        else
        {
			out.open(fileName,ios::binary|ios::_Nocreate);
            writeInternalNode(task,out);
			out.close();
            taskList.erase(taskToGet);
            task.reset();
            if(taskList.size()==1)
                break;
        }
    }

    writeRoot(taskList.at(0),out);
    /*   out.close();*/

}

void CMVPIndex::loadRootToLeaf(vector<shared_ptr<CIndexObject> > &dataObjects)
{
    ofstream out;

    out.open(fileName,ios::binary);
    out.close();

    filePointer = 0;
	if(dataObjects.size()<=maxLeafSize+numPivots)
    {
        rootAddress = -2;
    }
    else
    {
        rootAddress = -1;
    }

    vector<shared_ptr <Task> >::iterator taskToGet;

    /*put the task that create the root node to the task queue*/
    shared_ptr<Task> task(new Task(dataObjects,NULL,0,dataObjects.size(),0,0));
    task->myIndexAdress = -1;

    this->taskList.push_back(task);



    /*use every task in the queue to create a internal node or leaf node until the the queue is null*/
    while(this->taskList.size()>0)
    {

        taskToGet=this->taskList.end();
        int remainNodeSize;


        taskToGet--;

        task = *taskToGet;

        /*get the number of pivot*/
        numPivots = (this->numPivots>=task->size) ? task->size : this->numPivots;

        /*selecte several piovt from current partition based on the given parameter*/
        vector<int> pivotsIndex;
        pivotsIndex=this->psm->selectPivots(this->metric,task->dataList,task->start,task->size,numPivots);

        /*move the pivot to the end of current partition*/
        task->groupPivotsAtEnd(pivotsIndex);

        remainNodeSize=task->size - task->numPivots;//task->size-task->numPivots

        /*if the size of current partition is greater than the max size of a leaf node then create a internal node, otherwise create a leaf node.the max size of leaf node is given by the builder of the mvp-tree*/
        if(remainNodeSize>maxLeafSize)
        {
            createAndWriteInternalNode(task,out,taskToGet);
            task.reset();
        }
        else
        {
            createAndWriteLeafNodeLast(task,out);
            if(rootAddress!=-2)
            {
                taskList.erase(taskToGet);
                task.reset();
            }
            else 
            {
                writeRoot(taskList.at(0),out);
                task.reset();
                break;
            }
        }
    }

    rootAddress = 0;

    out.open(fileName,ios::binary|ios::_Nocreate);
    out.seekp(-(long)sizeof(long),ios::end);
    out.write((char*)&rootAddress,sizeof(long));
    out.close();


    /*   out.close();*/


}


void CMVPIndex::createAndWriteLeafNode(shared_ptr <Task> task,ofstream &out)
{

    out.open(fileName,ios::binary|ios::_Nocreate);
    out.seekp(filePointer);

    this->numLeaf++;
    //cout<<"leafNode:"<<numLeaf<<endl;
    /*get all the objects of current partition*/
    vector<shared_ptr<CIndexObject> > children;
    task->getDataPoints(children);
	//cout<<"childsize:"<<children.size()<<endl;
    /*get all the pivots of current node*/
    vector<shared_ptr<CIndexObject> > pivots;
    task->getPivots(pivots);

    vector<vector<double> > distance;

    /*calcualte the distance from each of the objects of current parition to every pivots*/
    for(vector<CIndexObject*>::size_type i=0;i<pivots.size();i++)
    {
        vector<double> ve;

        for(vector<CIndexObject*>::size_type j=0;j<children.size();j++)
        {
            ve.push_back(this->metric->getDistance(children[j].get(),pivots[i].get()));
        }

        distance.push_back(ve);
    }

    /*create a leaf node and assign its memory address to the child list of parent node*/
    shared_ptr<CMVPLeafNode> mvpLeafNode(new CMVPLeafNode(pivots,children,distance,task->getMyHeight()+1));

    if(this->rootAddress==-2)
    {        

        task->setNode((shared_ptr<CIndexNode>)mvpLeafNode);

        out.close();
        return;
    }
    else
    {
        char type[6] = "LNODE";
        
		((CMVPInternalNode*)task->getParentIndex().get())->getSubTreeAddress()[task->getMyIndex()-1]=filePointer;
		
        out.write(type,6*sizeof(char));
        filePointer+=(long)mvpLeafNode->writeExternal(out)+6*sizeof(char);
        /*delete(type);*/
    }

    out.close();
}

void CMVPIndex::createInternalNodeToWrite(shared_ptr <Task> task)
{



    this->numInternalNodes ++;
    /*cout<<"internalNode:"<<numInternalNodes<<endl;*/
    /*get pivots of current node*/
    vector<shared_ptr<CIndexObject> > pivots;
    task->getPivots(pivots);


    /*partition current partition into several smaller child partitons,the selected pivots are not in the section that will be partitioned in child partitions*/
    CPartitionResults pr=this->pm->partition(this->metric,pivots,task->dataList,task->start,task->size - task->numPivots,this->singlePivotFanout,this->maxLeafSize);
    int childrenNumber=pr.partitionSize();

    /*create an internal node and assign its addres to the children list of parent node*/
    vector<shared_ptr<CIndexNode> > *subTreeNode = new vector<shared_ptr<CIndexNode> >(childrenNumber-1);
    shared_ptr< CMVPInternalNode> node(new CMVPInternalNode(pivots,pr.getLowerBounds(),pr.getUpperBounds(),*subTreeNode,task->getMyHeight()+1) );
    node->setChildSize(childrenNumber-1);


    task->setNode((shared_ptr<CIndexNode>) node );

    /*create several tasks base on each of the child partitions created before and then push these tasks to the global variable queue for the iteration of building child trees*/
    for(int i=childrenNumber-1;i>0;i--)
    {
        int j=0;
        shared_ptr<Task> newTask( new Task(task->dataList,node,pr.getPartition(i-1),pr.getPartition(i),i,task->getMyHeight()+1) );       

        this->taskList.push_back(newTask);

    }
}

void CMVPIndex::writeInternalNode(shared_ptr <Task>task,ofstream& out)
{
    //out.open(fileName,ios::binary|ios::_Nocreate);

    out.seekp(filePointer);

    CMVPInternalNode* parentNode = (CMVPInternalNode*)(task->getParentIndex().get());
    parentNode->getSubTreeAddress()[task->getMyIndex()-1]=filePointer;

    char *type = "INODE";
    out.write(type,6*sizeof(char));
    filePointer+=(long)task->getNode()->writeExternal(out)+6*sizeof(char);
    /* delete(type);*/

    //out.close();
}

void CMVPIndex::writeRoot(shared_ptr <Task> task,ofstream& out)
{ 
    out.open(fileName,ios::binary|ios::_Nocreate);

    out.seekp(filePointer);

	long tempi=0;
	char *type1="INODE";
    char *type2="LNODE";

    if(rootAddress == -2)
        out.write(type2,6*sizeof(char));
    else
        out.write(type1,6*sizeof(char));

	task->getNode()->writeExternal(out);
    rootAddress = filePointer;
    out.write((char*)(&rootAddress),sizeof(long));

	out.close();
	
}

void CMVPIndex::createAndWriteInternalNode(shared_ptr <Task> task,ofstream& out,vector<shared_ptr<Task> >::iterator &shouldeBeCreate)
{
    this->numInternalNodes ++;
    /*cout<<"internalNode:"<<numInternalNodes<<endl;*/
    /*get pivots of current node*/
    vector<shared_ptr<CIndexObject> > pivots;
    task->getPivots(pivots);

    /*partition current partition into several smaller child partitons,the selected pivots are not in the section that will be partitioned in child partitions*/
    CPartitionResults pr=this->pm->partition(this->metric,pivots,task->dataList,task->start,task->size - task->numPivots,this->singlePivotFanout,this->maxLeafSize);
    int childrenNumber=pr.partitionSize();

    /*create an internal node and assign its addres to the children list of parent node*/
    vector<shared_ptr<CIndexNode> > *subTreeNode = new vector<shared_ptr<CIndexNode> >(childrenNumber-1);
    shared_ptr< CMVPInternalNode> node( new CMVPInternalNode(pivots,pr.getLowerBounds(),pr.getUpperBounds(),*subTreeNode,task->getMyHeight()+1) );
    node->setChildSize(childrenNumber-1);



    if(task->getParentIndex()!=NULL)
    {
        (((CMVPInternalNode*)(task->getParentIndex().get()))->getSubTreeAddress())[task->myIndex-1]=-1;
    }    


    out.open(fileName,ios::binary|ios::_Nocreate);

    out.seekp(filePointer);

    /* CMVPInternalNode* parentNode = (CMVPInternalNode*)task->getParentIndex();*/
    if(task->myIndexAdress!=-1)
    {
        out.seekp(task->myIndexAdress);
        out.write((char*)(&filePointer),sizeof(long));
        out.seekp(filePointer);
    }

    char *type = "INODE";
    out.write(type,6*sizeof(char));
    long tempDelta = node->writeExternal(out)+6*sizeof(char);
    /* delete(type);*/

    out.close();

    taskList.erase(shouldeBeCreate);

    /*create several tasks base on each of the child partitions created before and then push these tasks to the global variable queue for the iteration of building child trees*/
    for(int i=childrenNumber-1;i>0;i--)
    {
        shared_ptr <Task> newTask ( new Task(task->dataList,node,pr.getPartition(i-1),pr.getPartition(i),i,task->getMyHeight()+1));

        newTask->myIndexAdress = (long) (filePointer+(newTask->myIndex-1)*sizeof(long)+sizeof(int))+6*sizeof(char);//上12行为父节点写出的入口，filePoointer（此时此刻的文件指针）+ 孩子在父节点中孩子列表中的序列数*sizeof（long)+孩子个数(个数为int类型，因为父节点在写出的时候是先写出孩子个数然后才写每个孩子的文件指针值（long类型））+上13行输出的字符串所占用的宽度

        this->taskList.push_back(newTask);

    }

    filePointer += tempDelta;
}

void CMVPIndex::createAndWriteLeafNodeLast(shared_ptr <Task> task,ofstream &out)
{

    out.open(fileName,ios::binary|ios::_Nocreate);
    out.seekp(filePointer);

    this->numLeaf++;
    /*cout<<"leafNode:"<<numLeaf<<endl;*/
    /*get all the objects of current partition*/
    vector<shared_ptr<CIndexObject> > children;
    task->getDataPoints(children);

    /*get all the pivots of current node*/
    vector<shared_ptr<CIndexObject> > pivots;
    task->getPivots(pivots);

    vector<vector<double>> distance;

    /*calcualte the distance from each of the objects of current parition to every pivots*/
    for(vector<CIndexObject*>::size_type i=0;i<pivots.size();i++)
    {
        vector<double> ve;

        for(vector<CIndexObject*>::size_type j=0;j<children.size();j++)
        {
            ve.push_back(this->metric->getDistance(children[j].get(),pivots[i].get()));
        }

        distance.push_back(ve);
    }

    /*create a leaf node and assign its memory address to the child list of parent node*/
    shared_ptr< CMVPLeafNode> mvpLeafNode(new CMVPLeafNode(pivots,children,distance,task->getMyHeight()+1));

    if(this->rootAddress==-2)
    {        

        task->setNode((shared_ptr<CIndexNode>)mvpLeafNode);

        out.close();
        return;
    }
    else
    {
        char type[6] = "LNODE";


        out.seekp(task->myIndexAdress);
        out.write((char*)(&filePointer),sizeof(long));
        out.seekp(filePointer);

        out.write(type,6*sizeof(char));
        filePointer+=(long)mvpLeafNode->writeExternal(out)+6*sizeof(char);
        /*delete(type);*/
    }

    out.close();
}
