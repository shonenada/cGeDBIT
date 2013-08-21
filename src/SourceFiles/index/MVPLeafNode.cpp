#include "../../HeaderFiles/index/MVPLeafNode.h"


#include <Windows.h>
#include <psapi.h>
#pragma comment(lib,"psapi.lib")  


void showMemoryInfo3(char * resultsFileName,char* type)  
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

/** @file MVPLeafNode.cpp
* @classes about index structure
* @author Fuli Lei
* @version 2012-12-09
*/
/**constructor with no parameters*/
CMVPLeafNode::CMVPLeafNode()
{
}

/**constructor with four parameters.
* @param pivots a vector contains all the address of pivots
* @param dataobjects address list of all the objects composed in this node
* @param distance contains all the distance values from every pivots to each of the object in the object list of this node.the number of rows in this vector is euqual to the number of pivots in this node while the number of column is equal to the length of dataObjects, which is the second parameter of this function
* @param the height of current node
*/
CMVPLeafNode::CMVPLeafNode(vector<shared_ptr<CIndexObject> > &pivots,vector<shared_ptr<CIndexObject> > &dataObjects, vector<vector<double> > &distance,int height):CIndexNode(pivots,height)
{
    this->dataObjects=dataObjects;
    this->distance=distance;
}

/**destructor*/
CMVPLeafNode::~CMVPLeafNode()
{
    /*dataObjects.clear();
    pivots.clear();*/
}

/**return the distance value list according to the pivot index in the parameter of this function
* @param pivotIndex the index of pivot in the pivot list
*/
vector<double>& CMVPLeafNode::getDataPointPivotDistance(int pivotNumber)
{
    return distance[pivotNumber];
}

/**return the number of objects in this node*/
int CMVPLeafNode::numChildren()
{
    return dataObjects.size();
}

/**return the number of objects in this node*/
shared_ptr<CIndexObject>  CMVPLeafNode::getObject(int childIndex)
{
    return dataObjects[childIndex];
}

/**return all the address of object in this node trough a vector*/
vector<shared_ptr<CIndexObject> >& CMVPLeafNode::getObjects()
{
    return dataObjects;
}

/**traversal from this internal node to its every child to search out the proper objects base on the parameters
@param q this object is given by the user according which kind of query does the user want to do later, there is some basic information packaged in the object  like the search radius if the user want do a range query search latter.
@param metric this object will be use to calculate the distance of two objects
@return the proper objects address list through a vector
*/
vector<CIndexObject*> CMVPLeafNode::search(CRangeQuery &q,CMetric &metric)
{
    vector<CIndexObject*> rs;
    int numpivots = pivots.size();


    int i,j,p=-1;
    double* tempd = new double[numpivots];
    double r = q.getRadius();

    for(i=0;i<numpivots;i++)
    {
        tempd[i] = metric.getDistance(pivots[i].get(),q.getQueryObject().get());
        if (tempd[i]<=r)
        {
            rs.push_back(pivots.at(i).get());
            if (tempd[i]==0)
                p = i;
        }

    }

    if (p>=0&&tempd[p]==0)
    {
        for(j=0;j<dataObjects.size();j++)
        {
            if(distance.at(p).at(j)<=r)
                rs.push_back(dataObjects.at(j).get());

        }

        return rs;
    }




    for(i=0;i<dataObjects.size();i++)
    {			
        for(j=0;j<numpivots;j++)
        {           

            if(abs(tempd[j]-distance.at(j).at(i))>r)
            {
                /*dataObjects.at(i).reset();*/
                break;    
            }
            else if(metric.getDistance(dataObjects.at(i).get(),q.getQueryObject().get())<=r)
            {
                rs.push_back(dataObjects.at(i).get());
                break;
            }
            else
            {
               /* dataObjects.at(i).reset()*/;
            }

        }
    }

    delete(tempd);

    return rs;
}

void CMVPLeafNode::getAlldata(vector<shared_ptr<CIndexObject> > &result)
{
    int i;

    for (i=0;i<pivots.size();i++)
    {
        result.push_back(pivots.at(i));
    }

    for (i=0;i<dataObjects.size();i++)
    {
        result.push_back(dataObjects.at(i));
    }


}

void CMVPLeafNode::travelSearch(CRangeQuery &q,CMetric &metric,vector<shared_ptr<CIndexObject> > &result)
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

    for(j=0;j<dataObjects.size();j++)
    {

        if(metric.getDistance(dataObjects.at(j).get(),q.getQueryObject().get())<=r)
            result.push_back(dataObjects.at(j));
    }



}

int CMVPLeafNode ::writeExternal(ofstream &out)
{
    int size=0;
    int i,j,tempsize;    

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


    //write distance
    int lineSize = distance.size();
    int rowSize = distance.at(0).size();

    out.write((char*)(&lineSize),sizeof(int));
    out.write((char*)(&rowSize),sizeof(int));
    size+=2*sizeof(int);

    for(i=0;i<lineSize;i++)
    {
        for(j=0;j<rowSize;j++)
        {
            out.write((char*)(&distance.at(i).at(j)),sizeof(double));           
            size+=sizeof(double);
        }

    }


    //write objects
    int dataSize=dataObjects.size();
    out.write((char*)&dataSize,sizeof(int));
    size+=sizeof(int);

    for(i=0;i<dataSize;i++)
    {
        size+=dataObjects.at(i)->writeExternal(out);
    }

    return size;
}

int CMVPLeafNode::readExternal(ifstream &in,string &objectType)
{
    //showMemoryInfo3("testLeaf","readBegin");

    int size=0;
    int tempSize=0;
    int lineSize=0,rowSize=0;

    int i,j;




    //read height
    in.read((char*)(&height),sizeof(int));
    size+=sizeof(int);


    //read pivots
    in.read((char*)(&tempSize),sizeof(int));
    size+=sizeof(int);

    for(i=0;i<tempSize;i++)
    {
        shared_ptr<CIndexObject>  pivot;

        pivot.reset((CIndexObject*)CObjectFactory::getClassByName(objectType));
        pivots.push_back(pivot);
        size+=pivots.at(i)->readExternal(in);
    }


    //read distance
    in.read((char*)(&lineSize),sizeof(int));
    in.read((char*)(&rowSize),sizeof(int));
    size+=2*sizeof(int);

    for(i=0;i<lineSize;i++)
    {   
        vector<double> line(rowSize);
        distance.push_back(line);

        for(j=0;j<rowSize;j++)
        {
            in.read((char*)(&distance.at(i).at(j)),sizeof(double));
            size+=sizeof(double);
        }
    }


    //read objects
    int dataSize;
    in.read((char*)&dataSize,sizeof(int));
    size+=sizeof(int);



    for(i=0;i<dataSize;i++)
    {
        shared_ptr<CIndexObject>  data;

        data.reset( (CIndexObject*)CObjectFactory::getClassByName(objectType));
        size+=data->readExternal(in);
        dataObjects.push_back(data);
    }

    //showMemoryInfo3("testLeaf","readEnd");
    return size;
}

void CMVPLeafNode::searchIndex(CRangeQuery &q,long filePointer,ifstream &in,CMetric &metric,vector<shared_ptr<CIndexObject> > &rs,string &dataType)
{
    in.seekg(filePointer,ios::beg);

    this->readExternal(in,dataType);

    int numpivots = pivots.size();

    int i,j,p=-1;
    double* tempd = new double[numpivots];
    double r = q.getRadius();
    bool shouldBeSearched = true;

    for(i=0;i<numpivots;i++)
    {
        tempd[i] = metric.getDistance(pivots[i].get(),q.getQueryObject().get());
        if (tempd[i]<=r)
        {
            rs.push_back(pivots.at(i));
            if (tempd[i]==0)
                p = i;
        }
        else
        {
            /*pivots.at(i).reset();*/
        }

    }

    if (p>=0&&tempd[p]==0)
    {
        for(j=0;j<dataObjects.size();j++)
        {
            if(distance.at(p).at(j)<=r)
                rs.push_back(dataObjects.at(j));
            else
                /*dataObjects.at(j).reset();*/;
        }

        CMemMonitor::updateMem();

        delete(tempd);
        return;
    }





    for(i=0;i<dataObjects.size();i++)
    {			
        for(j=0;j<numpivots;j++)
        {           

            if(abs(tempd[j]-distance.at(j).at(i))>r)
            {
                shouldBeSearched = false;
                break;
            }


        }

        if(shouldBeSearched)
        {
            if(metric.getDistance(dataObjects.at(i).get(),q.getQueryObject().get())<=r)
            {
                rs.push_back(dataObjects.at(i));

            }
        }

        shouldBeSearched = true;

    }
    CMemMonitor::updateMem();
    delete(tempd);

}

void CMVPLeafNode::searchExternal(CRangeQuery &q,long filePointer,ifstream &in,CMetric &metric,vector<shared_ptr<CIndexObject> > &rs,deque<long> &childrenAddress,string &dataType)
{

    //showMemoryInfo3("testLeaf","searchBegin");  


    in.seekg(filePointer,ios::beg);

    this->readExternal(in,dataType);

    int numpivots = pivots.size();

    int i,j,p=-1;
    double* tempd = new double[numpivots];
    double r = q.getRadius();
    bool shouldBeSearched = true;

    for(i=0;i<numpivots;i++)
    {
        tempd[i] = metric.getDistance(pivots[i].get(),q.getQueryObject().get());
        if (tempd[i]<=r)
        {
            rs.push_back(pivots.at(i));
            if (tempd[i]==0)
                p = i;
        }
        /*else
        {
        pivots[i].reset();
        }*/

    }

    if (p>=0&&tempd[p]==0)
    {
        for(j=0;j<dataObjects.size();j++)
        {
            if(distance.at(p).at(j)<=r)
                rs.push_back(dataObjects.at(j));
            /* else
            dataObjects.at(j).reset();*/
        }
        CMemMonitor::updateMem();
        delete(tempd);
        return;
    }





    for(i=0;i<dataObjects.size();i++)
    {			
        for(j=0;j<numpivots;j++)
        {           

            if(abs(tempd[j]-distance.at(j).at(i))>r)
            {
                shouldBeSearched = false;                
                //dataObjects.at(i).reset();

                break;
            }


        }

        if(shouldBeSearched)
        {
            if(metric.getDistance(dataObjects.at(i).get(),q.getQueryObject().get())<=r)
            {

                rs.push_back(dataObjects.at(i));

            }
            /* else
            {
            dataObjects.at(i).reset();
            }*/

        }

        shouldBeSearched = true;

    }
    //showMemoryInfo3("testLeafSearchExternal");
    CMemMonitor::updateMem();
    delete(tempd);

    //showMemoryInfo3("testLeaf","searchEnd");
}