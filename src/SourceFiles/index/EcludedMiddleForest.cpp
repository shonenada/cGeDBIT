#include "../../HeaderFiles/index/EcludedMiddleForest.h"

class ForestTask 
{
public:
    /**give class CMVPIndex a friend right to access the data information of a ForestTask instance in the process of building a mvp tree*/
    friend CEcludedMiddleForest;

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
    ForestTask():dataList(*(new vector<CIndexObject*>))
    {
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
    ForestTask(vector<CIndexObject*> &dataList,CIndexNode *parentIndex,int start,int end,int myIndex,int myHeight):dataList(dataList)
    {
        this->dataList=dataList;
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
void ForestTask::groupPivotsAtEnd(vector<int> &pivotIndex)
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
CIndexNode* ForestTask::getParentIndex()
{
    return this->parentIndex;
}

/**get the index of current node in the parent's children list*/
int ForestTask::getMyIndex()
{
    return this->myIndex;
}

/**get the height of current node. the height of root is 0*/
int ForestTask::getMyHeight()
{
    return this->myHeight;
}

/**get all the pivots that belongs to current node
* @param pivots a vector to store the address of pivot.This vector is initialized with none elements, all the elements is generated through calling thing function. This parameter is used to return the address of all pivot to the caller of this funciton.
*/
void ForestTask::getPivots(vector<CIndexObject*> &pivots)
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
void ForestTask::getDataPoints(vector<CIndexObject*> &objectList)
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

/***********************************************************************/

CIndexNode* CEcludedMiddleForest::createRoot(ForestTask *forestTask,int treeIndex,vector<CIndexObject*>& ecludedData)
{
    this->numInternalNodes ++;

    /**get pivots of current node*/
    vector<CIndexObject*> pivots;



    forestTask->getPivots(pivots);

    /**partition current partition into several smaller child partitons*/


    CPartitionResults pr=this->pm->partition(this->metric,pivots,forestTask->dataList,0,forestTask->size - forestTask->numPivots,this->maxRadius,this->singlePivotFanout,this->maxLeafSize,this->middlePorprotion);

    int childrenNumber=pr.partitionSize();

    /**create a internal node as root node and assign the address of this node to the root pointer of mvp tree*/
    vector<CIndexNode*> subTreeNode(childrenNumber-2);

    vector<vector<double> >  lowerBounds=pr.getLowerBounds();
    vector<vector<double> >  upperBounds=pr.getUpperBounds();


    for(int i =0;i<numPivots;i++)
    {
        lowerBounds.at(i).pop_back();
        upperBounds.at(i).pop_back();
    }



    CMVPInternalNode *root=new CMVPInternalNode(pivots,lowerBounds,upperBounds,subTreeNode,forestTask->getMyHeight());

    /**create several ForestTasks base on each of the child partitions created before and then push these ForestTasks to the global variable queue for the iteration of building child trees*/
    for(int i=childrenNumber-1;i>0;i--)
    {
        ForestTask* newForestTask = 0;

        /**for the last child partition of current partition, the end index to create a ForestTask is the end index of current partiton, otherwise the end index is the start of next child partiton*/
        if(i==childrenNumber-1)
        {
            cout<< "start-end:" << pr.getPartition(i-1) << "-" << forestTask->start+forestTask->size-forestTask->numPivots << endl;
            cout<<"the object list size of sub-tree is:"<<forestTask->start+forestTask->size-forestTask->numPivots-pr.getPartition(i-1)<<endl;


            //newForestTask=new ForestTask(ForestTask->dataList,root,pr.getPartition(i-1),ForestTask->start + ForestTask->size - ForestTask->numPivots,i,ForestTask->getMyHeight()+1);

            for(int j=pr.getPartition(i-1);j<pr.getPartition(i) - forestTask->numPivots+1;j++)
            {
                ecludedData.push_back(forestTask->dataList.at(j));
            }

            //newForestTask=new ForestTask(forestTask->dataList,root,pr.getPartition(i-1),pr.getPartition(i),i,forestTask->getMyHeight()+1);//!!!!!!!!!

        }
        else
        {
            cout<<"start-end:"<<pr.getPartition(i-1)<<"-"<<pr.getPartition(i)<<endl;
            cout<<"the object list size of sub-tree is:"<<pr.getPartition(i)-pr.getPartition(i-1)<<endl;


            newForestTask=new ForestTask(forestTask->dataList,root,pr.getPartition(i-1),pr.getPartition(i),i,forestTask->getMyHeight()+1);


            this->forestTaskList.push_back(newForestTask);
            /**push ForestTask into the queue*/

        }
    }

    return root;


}

void CEcludedMiddleForest::createInternalNode(ForestTask *forestTask,int treeIndex,vector<CIndexObject*>& ecludedData)
{
    this->numInternalNodes ++;
    cout<<"internalNode:"<<numInternalNodes<<endl;




    /**get pivots of current node*/
    vector<CIndexObject*> pivots;
    forestTask->getPivots(pivots);




    /**partition current partition into several smaller child partitons*/
    CPartitionResults pr=this->pm->partition(this->metric,pivots,forestTask->dataList,forestTask->start,forestTask->size-forestTask->numPivots,this->maxRadius,this->singlePivotFanout,this->maxLeafSize,this->middlePorprotion);




    int childrenNumber=pr.partitionSize();

    vector<vector<double> >  lowerBounds=pr.getLowerBounds();
    vector<vector<double> >  upperBounds=pr.getUpperBounds();

    for(int i =0;i<numPivots;i++)
    {
        lowerBounds.at(i).pop_back();
        upperBounds.at(i).pop_back();
    }



    /**create an internal node and assign its addres to the children list of parent node*/
    vector<CIndexNode*> *subTreeNode=new vector<CIndexNode*>(childrenNumber-2);
    CMVPInternalNode *child=new CMVPInternalNode(pivots,lowerBounds,upperBounds,*subTreeNode,forestTask->getMyHeight()+1);
    (((CMVPInternalNode*)(forestTask->getParentIndex()))->getSubTree())[forestTask->myIndex-1]=child;

    /**create several forestTasks base on each of the child partitions created before and then push these forestTasks to the global variable queue for the iteration of building child trees*/


    for(int i=childrenNumber-1;i>0;i--)
    {
        ForestTask *newforestTask=0;

        /**for the last child partition of current partition, the end index to create a ForestTask is the end index of current partiton, otherwise the end index is the start of next child partiton*/
        if(i==childrenNumber-1)
        {
            cout<<"start-end:"<<pr.getPartition(i-1)<<"-"<<forestTask->start+forestTask->size-forestTask->numPivots<<endl;
            cout<<"the object list size of sub-tree is:"<<forestTask->start+forestTask->size-forestTask->numPivots-pr.getPartition(i-1)<<endl;



            for(int j=pr.getPartition(i-1);j < pr.getPartition(i) - forestTask->numPivots+1;j++)
            {
                ecludedData.push_back(forestTask->dataList.at(j));
            }


        }
        else
        {
            cout<<"start-end:"<<pr.getPartition(i-1)<<"-"<<pr.getPartition(i)<<endl;
            cout<<"the object list size of sub-tree is:"<<pr.getPartition(i)-pr.getPartition(i-1)<<endl;
            newforestTask = new ForestTask(forestTask->dataList,child,pr.getPartition(i-1),pr.getPartition(i),i,forestTask->getMyHeight()+1);

            this->forestTaskList.push_back(newforestTask);
        }



    }
}

void CEcludedMiddleForest::createLeafNode(ForestTask *forestTask,int treeIndex,vector<CIndexObject*>& ecludedData)
{
    this->numLeaf++;
    cout<<"leafNode:"<<numLeaf<<endl;
    /**get all the objects of current partition*/
    vector<CIndexObject*> children;
    forestTask->getDataPoints(children);
    cout<<"child.size:"<<children.size()<<endl;
    /**get all the pivots of current node*/
    vector<CIndexObject*> pivots;
    forestTask->getPivots(pivots);

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
    CMVPLeafNode *mvpLeafNode=new CMVPLeafNode(pivots,children,distance,forestTask->getMyHeight()+1);cout<<"parent.child.size:"<<(((CMVPInternalNode*)(forestTask->getParentIndex()))->getSubTree()).size()<<endl;
    (((CMVPInternalNode*)(forestTask->getParentIndex()))->getSubTree())[forestTask->myIndex-1]=mvpLeafNode;

}


vector<CIndexObject*> CEcludedMiddleForest::createTree(vector<CIndexObject*> &dataObjects,int treeIndex)
{
    vector<CIndexObject*> ecludedData;

    ForestTask *forestTask=new ForestTask(dataObjects,NULL,0,dataObjects.size(),0,0);
    this->forestTaskList.push_back(forestTask);

    /**use every forestTask in the queue to create a internal node or leaf node until the the queue is null*/
    while(this->forestTaskList.size()>0)
    {
        vector<ForestTask*>::iterator ForestTaskToGet=this->forestTaskList.begin();

        forestTask=*ForestTaskToGet;

        /**delete current forestTask from the queue*/
        this->forestTaskList.erase(ForestTaskToGet);

        if(roots.size()<treeIndex+1)
        {
            /**get the number of pivot*/
            numPivots = (this->numPivots>=forestTask->size) ? forestTask->size : this->numPivots;

            /**selecte several piovt from current partition based on the given parameter*/
            vector<int> pivotsIndex;
            pivotsIndex=this->psm->selectPivots(this->metric,forestTask->dataList,0,forestTask->dataList.size(),numPivots);

            /**move the pivot to the end of current partition*/
            forestTask->groupPivotsAtEnd(pivotsIndex);

            /**create the mvp-tree root*/
            this->roots.push_back(createRoot(forestTask,treeIndex,ecludedData));
        }
        else
        {
            /**get the number of pivot*/
            numPivots = (this->numPivots>=forestTask->size) ? forestTask->size : this->numPivots;

            /**selecte several piovt from current partition based on the given parameter*/
            vector<int> pivotsIndex;
            pivotsIndex=this->psm->selectPivots(this->metric,forestTask->dataList,forestTask->start,forestTask->size,numPivots);

            /**move the pivot to the end of current partition*/
            forestTask->groupPivotsAtEnd(pivotsIndex);

            int remainNodeSize=forestTask->size-forestTask->numPivots;//forestTask->size-forestTask->numPivots
            cout<<"remain node list size:"<<remainNodeSize<<endl;

            /**if the size of current partition is greater than the max size of a leaf node then create a internal node, otherwise create a leaf node.the max size of leaf node is given by the builder of the mvp-tree*/
            if(remainNodeSize>maxLeafSize)
                createInternalNode(forestTask,treeIndex,ecludedData);
            else
                createLeafNode(forestTask,treeIndex,ecludedData);
        }


    }


    return ecludedData;

}

void CEcludedMiddleForest::bulkLoad(vector<CIndexObject*> & dataList)
{


    //roots.push_back(newRoot);

    vector<CIndexObject*> formerEcludedData = createTree(dataList,0);
    vector<CIndexObject*>* ecludedData;

    for(int i=1;;i++)
    {
        if(formerEcludedData.size()<=maxLeafSize)
        {
            break;
        }

        formerEcludedData = createTree(formerEcludedData,i);

    }


    vector<CIndexObject*> pivots;
    vector<CIndexObject*> children;

    vector<int> pivotsIndex = this->psm->selectPivots(metric,formerEcludedData,numPivots);
    ForestTask forestTask(formerEcludedData,NULL,0,formerEcludedData.size(),0,0);

    forestTask.groupPivotsAtEnd(pivotsIndex);

    forestTask.getPivots(pivots);
    forestTask.getDataPoints(children);

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

    CIndexNode* newRoot = new CMVPLeafNode(pivots,children,distance,0);
    roots.push_back(newRoot);

}




vector<CIndexObject*> CEcludedMiddleForest:: search(CQuery* q)
{
    vector<CIndexObject*> rs;

    CRangeQuery *rq=(CRangeQuery*)q;
    for(int i=0;i<roots.size();i++)
    {
        vector<CIndexObject*> temprs = roots.at(i)->search(*rq,*metric);

        rs.insert(rs.begin(),temprs.begin(),temprs.end());
    }
    return rs;
}





CEcludedMiddleForest::CEcludedMiddleForest()
{


    metric=0;

    psm=0;

    pm=0;

    singlePivotFanout=0;

    maxLeafSize=0;


    numPivots=0;


}

CEcludedMiddleForest::CEcludedMiddleForest(vector<CIndexObject*> &dataObjects,CMetric *metric,CPivotSelectionMethod* psm,CPartitionMethod* pm,int numPivot,int singlePivotFanout,int maxLeafSize, int maxPathLength,double maxRadius,double middlePorprotion)
{

    this->metric=metric;
    this->numPivots=1;
    this->psm=psm;
    this->pm=pm;
    this->singlePivotFanout=2;
    this->total_size=dataObjects.size();
    this->maxLeafSize=maxLeafSize;
    this->maxPathLength = maxPathLength;
    this->middlePorprotion = middlePorprotion;
    this->maxRadius = maxRadius;


}


CEcludedMiddleForest::~CEcludedMiddleForest(void)
{
}
