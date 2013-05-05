#include "..\..\HeaderFiles\index\MVPLeafNode.h"
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
CMVPLeafNode::CMVPLeafNode(vector<CIndexObject*> &pivots,vector<CIndexObject*> &dataObjects,vector<vector<double>> &distance,int height):CIndexNode(pivots,height)
{
    this->dataObjects=dataObjects;
    this->distance=distance;
}

/**destructor*/
CMVPLeafNode::~CMVPLeafNode()
{

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
CIndexObject* CMVPLeafNode::getObject(int childIndex)
{
    return dataObjects[childIndex];
}

/**return all the address of object in this node trough a vector*/
vector<CIndexObject*>& CMVPLeafNode::getObjects()
{
    return dataObjects;
}

/**traversal from this internal node to its every child to search out the proper objects base on the parameters
* @para q this object is given by the user according which kind of query does the user want to do later, there is some basic information packaged in the object  like the search radius if the user want do a range query search latter.
* @para metric this object will be use to calculate the distance of two objects
* @return the proper objects address list through a vector
*/
//vector<CIndexObject*> CMVPLeafNode::search(CRangeQuery &q,CMetric &metric)
//{
//    vector<CIndexObject*> rs;
//    int numpivots = pivots.size();
//
//
//    int i,j;
//    double* tempd = new double[numpivots];
//    double r = q.getRadius();
//
//    for(i=0;i<numpivots;i++)
//    {
//        tempd[i] = metric.getDistance(pivots[i],q.getQueryObject());
//        if (tempd[i]<=r)
//        {
//            rs.push_back(pivots.at(i));
//        }
//    }
//
//
//    /* for(i=0;i<numpivots;i++)
//    {
//    for(j=0;j<dataObjects.size();j++)
//    {
//    if(abs(tempd[i]+r)<distance.at(i).at(j)||abs(tempd[i]-r)>distance.at(i).at(j))
//    break;
//    if(metric.getDistance(dataObjects.at(j),q.getQueryObject())<=r)
//    rs.push_back(dataObjects.at(j));
//    }
//    }*/
//
//    for(i=0;i<dataObjects.size();i++)
//    {			
//        /*for(j=0;j<numpivots;j++)
//        {
//            if(abs(tempd[j]+r)<distance.at(j).at(i)||abs(tempd[j]-r)>distance.at(j).at(i))
//                break;
//            //else */
//            if(metric.getDistance(dataObjects.at(i),q.getQueryObject())<=r)
//                rs.push_back(dataObjects.at(i));
//        //}
//    }
//
//    return rs;
//}

//vector<CIndexObject*> CMVPLeafNode::search(CRangeQuery &q,CMetric &metric)
//{
//    vector<CIndexObject*> rs;
//    int numpivots = pivots.size();
//
//
//    int i,j;
//    double* tempd = new double[numpivots];
//    double r = q.getRadius();
//
//    for(i=0;i<numpivots;i++)
//    {
//        tempd[i] = metric.getDistance(pivots[i],q.getQueryObject());
//        if (tempd[i]<=r)
//        {
//            rs.push_back(pivots.at(i));
//        }
//        if (tempd[i]==0)
//        {
//             for(j=0;j<dataObjects.size();j++)
//                {
//                    if(distance.at(i).at(j)<=r)
//                        rs.push_back(dataObjects.at(i));
//                }
//             return rs;
//        }
//    }
//
//
//    /* for(i=0;i<numpivots;i++)
//    {
//    for(j=0;j<dataObjects.size();j++)
//    {
//    if(abs(tempd[i]+r)<distance.at(i).at(j)||abs(tempd[i]-r)>distance.at(i).at(j))
//    break;
//    if(metric.getDistance(dataObjects.at(j),q.getQueryObject())<=r)
//    rs.push_back(dataObjects.at(j));
//    }
//    }*/
//
//    for(i=0;i<dataObjects.size();i++)
//    {			
//        for(j=0;j<numpivots;j++)
//        {
//            
//                if(abs(tempd[j]+r)<distance.at(j).at(i)||abs(tempd[j]-r)>distance.at(j).at(i))
//                    break;
//                else if(metric.getDistance(dataObjects.at(i),q.getQueryObject())<=r)
//                    rs.push_back(dataObjects.at(i));
//           
//        }
//    }
//
//    return rs;
//}

vector<CIndexObject*> CMVPLeafNode::search(CRangeQuery &q,CMetric &metric)
{
    vector<CIndexObject*> rs;
    int numpivots = pivots.size();


    int i,j;
    double* tempd = new double[numpivots];
    double r = q.getRadius();

    for(i=0;i<numpivots;i++)
    {
        tempd[i] = metric.getDistance(pivots[i],q.getQueryObject());
        if (tempd[i]<=r)
        {
            rs.push_back(pivots.at(i));
        }
        if (tempd[i]==0)
        {
             for(j=0;j<dataObjects.size();j++)
                {
                    if(distance.at(i).at(j)<=r)
                        rs.push_back(dataObjects.at(i));
                }
             return rs;
        }
    }


    /* for(i=0;i<numpivots;i++)
    {
    for(j=0;j<dataObjects.size();j++)
    {
    if(abs(tempd[i]+r)<distance.at(i).at(j)||abs(tempd[i]-r)>distance.at(i).at(j))
    break;
    if(metric.getDistance(dataObjects.at(j),q.getQueryObject())<=r)
    rs.push_back(dataObjects.at(j));
    }
    }*/

    for(i=0;i<dataObjects.size();i++)
    {			
        for(j=0;j<numpivots;j++)
        {
            
                if(abs(tempd[j]-distance.at(j).at(i))>r)
                    break;
                else if(metric.getDistance(dataObjects.at(i),q.getQueryObject())<=r)
                    rs.push_back(dataObjects.at(i));
           
        }
    }

    return rs;
}



