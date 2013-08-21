#include "../../HeaderFiles/index/MVPInternalNode.h"



#include <Windows.h>
#include <psapi.h>
#pragma comment(lib,"psapi.lib")  


void showMemoryInfo2(char * resultsFileName,char* type)  
{  
    char resultFile[100]="./data/";
    strcat(resultFile,resultsFileName);
    ofstream output(resultFile,ofstream::app);
    HANDLE handle=GetCurrentProcess();  
    PROCESS_MEMORY_COUNTERS pmc;  
    GetProcessMemoryInfo(handle,&pmc,sizeof(pmc));  
    output<<type<<endl;
    output<<"内存使用:"<<pmc.WorkingSetSize/1024<<" KB / 峰值内存使用:"<<pmc.PeakWorkingSetSize/1024<<" KB "<<endl;
    output<<"虚拟内存使用:"<<pmc.PagefileUsage/1024<<"KB / 峰值虚拟内存使用  "<<pmc.PeakPagefileUsage/1024<<" KB "<<endl;
    output<<endl;
}







/** @file MVPInternalNode.cpp
* @classes about index structure
* @author Fuli Lei
* @version 2012-12-09
*/
/**none parameter constructor*/
CMVPInternalNode::CMVPInternalNode()
{
    /*pivots.clear();*/
}

/**constructor with parameters parameters
@param pivots a vector contains all the memory address of pivots
@param lower this is a two dimension vector contains the maximum distance value from every pivot to the object in each sub-tree. the total number of rows is equal to the number of all pivots in this node while the number of column is equal to the number of child nodes
@param upper this is a two dimension vector contains the maximum distance value from every pivot to the object in each sub-tree. the total number of rows is equal to the number of all pivots in this node while the number of column is equal to the number of child nodes
@param child a sub-tree address vector
@param myHeight the layer of this node
*/
CMVPInternalNode::CMVPInternalNode(vector<shared_ptr<CIndexObject> > pivots,vector<vector<double> > lower,vector<vector<double> > upper,vector<shared_ptr<CIndexNode> > child,int myHeight):CIndexNode(pivots,myHeight)
{

    this->child=child;

    this->upper=upper;
    this->lower=lower;

}

/**destructor*/
CMVPInternalNode::~CMVPInternalNode()
{
    delete (childAddress);
}

/**get address of a child according the child index
* @return the root address of a child
*/
vector<shared_ptr<CIndexNode> >& CMVPInternalNode::getSubTree()
{
    return this->child;
}


/**get the toltal number of child
* @return return the toltal number of child
*/
int CMVPInternalNode::numChildren()
{
    return child.size();
}

/**get address of a child according the child index
* @return the root address of a child
*/
shared_ptr<CIndexNode> CMVPInternalNode::getChildAddress(int subTreeRootAddress)
{
    return child[subTreeRootAddress];
}

/**get the lower bound value of every distance range, each element of the contain represent the distance from every object in sub-tree to every pivots
* @param pivotIndex the index of pivot in the pivot list
* @return the double value list of lower bound distance
*/
vector<double>& CMVPInternalNode::getChildLowerRange(int pivotIndex)
{
    return lower[pivotIndex];
}

/**get the upper band value of every distance range, each element of the contain represent the distance from every object in sub-tree to every pivots
* @param pivotIndex the index of pivot in the pivot list
* @return the double value list of upper bound distance
*/
vector<double>& CMVPInternalNode::getChildUpperRange(int pivotIndex)
{
    return upper[pivotIndex];
}

/**traversal from this internal node to its every sub-tree root to search out the proper objects base on the parameters
@param q this object is given by the user according which kind of query does the user want to do later, there is some basic information packaged in the object  like the search radius if the user want do a range query search latter.
@param metric this object will be use to calculate the distance of two objects
@return the proper objects address list through a vector
*/
vector<CIndexObject*>  CMVPInternalNode::search(CRangeQuery &q,CMetric &metric)
{

    vector<CIndexObject*> rs;

    vector<CIndexObject*> temprs;


    int numpivot = pivots.size();

    double r=q.getRadius();

    double* tempd=new double[numpivot];

    int shouldBeSearched = 1;



    int i,j;


    for(i=0;i<numpivot;i++)
    {
        tempd[i] = metric.getDistance(q.getQueryObject().get(),CIndexNode::getPivot(i).get());
        //tempd[i]: the distance between query and pivot[i]
        if(tempd[i]<=r)
        {
            rs.push_back(pivots[i].get());
        }

    }




    for(j=0;j<child.size();j++)
    {

        shouldBeSearched = 1;

        for(i=0;i<numpivot;i++)
        {


            if(tempd[i]-lower.at(i).at(j)<r&&tempd[i]+r>=upper.at(i).at(j))
            {
                shouldBeSearched=-1;
            }
            else
            {
                shouldBeSearched=1;
            }




            if((tempd[i]+r)<lower.at(i).at(j)||(tempd[i]-r)>upper.at(i).at(j))
            {
                shouldBeSearched =0;
                break;

            }
        }



        if(shouldBeSearched == -1)
        {
            /*child.at(j)->getAlldata(rs);*/
        }
        else if(shouldBeSearched==1)
        {
            temprs = getChildAddress(j)->search(q,metric);
            rs.insert(rs.begin(),temprs.begin(),temprs.end());
        }

    }


    delete(tempd);

    return rs;

}

void CMVPInternalNode::getAlldata(vector<shared_ptr<CIndexObject> > &result)
{
    int i;

    for (i=0;i<pivots.size();i++)
    {
        result.push_back(pivots.at(i));
    }

    for (i=0;i<child.size();i++)
    {
        child.at(i)->getAlldata(result);
    }


}

void CMVPInternalNode::travelSearch(CRangeQuery &q,CMetric &metric,vector<shared_ptr<CIndexObject> > &result)
{

    int numpivot = pivots.size();

    double r=q.getRadius();


    int i,j;


    for(i=0;i<numpivot;i++)
    {

        if(metric.getDistance(q.getQueryObject().get(),CIndexNode::getPivot(i).get())<=r)
        {
            result.push_back(pivots[i]);
        }


    }

    for(j=0;j<child.size();j++)
    {

        child.at(j)->travelSearch(q,metric,result);
    }



}

int CMVPInternalNode ::writeExternal(ofstream &out)
{
    int size=0;
    int i,j,tempsize;





    //write children
    out.write((char*)(&childSize),sizeof(int));
    size+=sizeof(int);

    out.write((char*)childAddress,childSize*sizeof(long));
    size+=childSize*sizeof(long);

    //write height
    out.write((char*)(&height),sizeof(int));
    size+=sizeof(int);


    //write pivots
    tempsize = pivots.size();
    out.write((char*)(&tempsize),sizeof(int));
    size+=sizeof(int);

    for(i=0;i<pivots.size();i++)
    {
        size+=pivots.at(i)->writeExternal(out);
    }


    //write upper and lower
    int lineSize = upper.size();
    int rowSize = upper.at(0).size();

    out.write((char*)(&lineSize),sizeof(int));
    out.write((char*)(&rowSize),sizeof(int));
    size+=2*sizeof(int);


    for(i=0;i<lineSize;i++)
    {     

        for(j=0;j<rowSize;j++)
        {
            out.write((char*)(&lower.at(i).at(j)),sizeof(double));
            out.write((char*)(&upper.at(i).at(j)),sizeof(double));
            size+=2*sizeof(double);
        }

    }

    return size;

}

int CMVPInternalNode ::readExternal(ifstream &in,string &objectType)
{
    //showMemoryInfo2("testInternal.txt","readBegin");

    int size=0;
    int tempSize=0;
    int lineSize=0,rowSize=0;

    int i,j;







    //read children
    in.read((char*)(&childSize),sizeof(int));
    size+=sizeof(int);

    childAddress = new long[childSize];
    in.read((char*)childAddress,childSize*sizeof(long));
    size+=childSize*sizeof(long);

    //read height
    in.read((char*)(&height),sizeof(int));
    size+=sizeof(int);

    //read pivots
    in.read((char*)(&tempSize),sizeof(int));
    size+=sizeof(int);

    for(i=0;i<tempSize;i++)
    {
        shared_ptr<CIndexObject> pivot;

        //CIndexObject *obj=(CIndexObject*)CObjectFactory::getClassByName(objectType);
        pivot.reset((CIndexObject*)CObjectFactory::getClassByName(objectType));
        pivots.push_back(pivot);
        size+=pivots.at(i)->readExternal(in);
    }


    //read upper and lower
    in.read((char*)(&lineSize),sizeof(int));
    in.read((char*)(&rowSize),sizeof(int));
    size+=2*sizeof(int);

    for(i=0;i<lineSize;i++)
    {   
        vector<double> line(rowSize);
        upper.push_back(line);
        lower.push_back(line);

        for(j=0;j<rowSize;j++)
        {
            in.read((char*)(&lower.at(i).at(j)),sizeof(double));
            in.read((char*)(&upper.at(i).at(j)),sizeof(double));
            size+=2*sizeof(double);
        }

    }

    //showMemoryInfo2("testInternal.txt","readEnd");

    return size;
}

long* CMVPInternalNode::getSubTreeAddress()
{
    return childAddress;
}

long CMVPInternalNode::getSubTreeRootAddress(int subTreeIndex)
{
    return childAddress[subTreeIndex];
}

void CMVPInternalNode::setChildSize(int size)
{
    if(childSize<0)
    {
        childSize = size;
        childAddress = new long[size];
    }
}

void CMVPInternalNode::searchIndex(CRangeQuery &q,long filePointer,ifstream &in,CMetric& metric,vector<shared_ptr<CIndexObject> > &rs,string &dataType)
{
    in.seekg(filePointer,ios::beg);

    this->readExternal(in,dataType);

    int numpivot = pivots.size();

    double r=q.getRadius();

    double* tempd=new double[numpivot];

    bool shouldBeSearched = true;




    int i,j;


    for(i=0;i<pivots.size();i++)
    {
        tempd[i] = metric.getDistance(q.getQueryObject().get(),CIndexNode::getPivot(i).get());
        //tempd[i]: the distance between query and pivot[i]
        if(tempd[i]<=r)
        {
            rs.push_back(pivots[i]);
        }
        else
        {
            /*pivots[i].reset();*/
        }

    }




    for(j=0;j<childSize;j++)
    {

        shouldBeSearched = true;

        for(i=0;i<numpivot;i++)
        {

            if((tempd[i]+r)<lower.at(i).at(j)||(tempd[i]-r)>upper.at(i).at(j))
            {


                if((tempd[i]+upper.at(i).at(j))<=r)
                {
                    shouldBeSearched=true;
                    break;
                }
                else
                {
                    shouldBeSearched = false;
                    break;
                }
            }
        }

        if(shouldBeSearched)
        {
            shared_ptr<CIndexNode> child;

            in.seekg(childAddress[j],ios::beg);

            char *type = new char[6];
            in.read((char*)type,6*sizeof(char));

            if(!strcmp(type,"INODE"))
            {
                child.reset(new CMVPInternalNode());
            }
            else
            {
                child.reset( new CMVPLeafNode());
            }
            CMemMonitor::updateMem();
            delete(type);

            child->searchIndex(q,childAddress[j]+6*sizeof(char),in,metric,rs,dataType);

        }

    }

    CMemMonitor::updateMem();
    delete(tempd);
}

void CMVPInternalNode::searchExternal(CRangeQuery &q,long filePointer,ifstream &in,CMetric &metric,vector<shared_ptr<CIndexObject> > &rs,deque<long> &childrenAddress,string &dataType)
{

    //showMemoryInfo2("testInternal.txt","searchBegin");
    in.seekg(filePointer,ios::beg);

    this->readExternal(in,dataType);

    int numpivot = pivots.size();

    double r=q.getRadius();

    double* tempd=new double[numpivot];

    bool shouldBeSearched = true;




    int i,j;


    for(i=0;i<pivots.size();i++)
    {
        tempd[i] = metric.getDistance(q.getQueryObject().get(),CIndexNode::getPivot(i).get());
        //tempd[i]: the distance between query and pivot[i]
        if(tempd[i]<=r)
        {
            rs.push_back(pivots[i]);
        }
        else
        {
            //pivots[i].reset();
        }

    }




    for(j=0;j<childSize;j++)
    {

        shouldBeSearched = true;

        for(i=0;i<numpivot;i++)
        {

            if((tempd[i]+r)<lower.at(i).at(j)||(tempd[i]-r)>upper.at(i).at(j))
            {


                if((tempd[i]+upper.at(i).at(j))<=r)
                {
                    shouldBeSearched=true;
                    break;
                }
                else
                {
                    shouldBeSearched = false;
                    break;
                }
            }
        }

        if(shouldBeSearched)
        {
            childrenAddress.push_back(childAddress[j]);
        }

    }

    CMemMonitor::updateMem();
    delete(tempd);

    //showMemoryInfo2("testInternal.txt","searchEnd");
}

