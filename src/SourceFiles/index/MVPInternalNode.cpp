#include "../../HeaderFiles/index/MVPInternalNode.h"
/**none parameter constructor*/
CMVPInternalNode::CMVPInternalNode()
{

}

/**constructor with parameters parameters
@para pivots a vector contains all the memory address of pivots
@para lower this is a two dimension vector contains the maximum distance value from every pivot to the object in each sub-tree. the total number of rows is equal to the number of all pivots in this node while the number of column is equal to the number of child nodes
@para upper this is a two dimension vector contains the maximum distance value from every pivot to the object in each sub-tree. the total number of rows is equal to the number of all pivots in this node while the number of column is equal to the number of child nodes
@para child a sub-tree address vector
@para myHeight the layer of this node
*/
CMVPInternalNode::CMVPInternalNode(vector<CIndexObject*> pivots,vector<vector<double> > lower,vector<vector<double> > upper,vector<CIndexNode*> child,int myHeight):CIndexNode(pivots,myHeight)
{

    this->child=child;

    this->upper=upper;
    this->lower=lower;

}

/**destructor*/
CMVPInternalNode::~CMVPInternalNode()
{

}

/**get address of a child according the child index
* @return the root address of a child
*/
vector<CIndexNode*>& CMVPInternalNode::getSubTree()
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
CIndexNode* CMVPInternalNode::getChildAddress(int subTreeRootAddress)
{
    return child[subTreeRootAddress];
}

/**get the lower bound value of every distance range, each element of the contain represent the distance from every object in sub-tree to every pivots
* @para pivotIndex the index of pivot in the pivot list
* @return the double value list of lower bound distance
*/
vector<double>& CMVPInternalNode::getChildLowerRange(int pivotIndex)
{
    return lower[pivotIndex];
}

/**get the upper band value of every distance range, each element of the contain represent the distance from every object in sub-tree to every pivots
* @para pivotIndex the index of pivot in the pivot list
* @return the double value list of upper bound distance
*/
vector<double>& CMVPInternalNode::getChildUpperRange(int pivotIndex)
{
    return upper[pivotIndex];
}

/**traversal from this internal node to its every sub-tree root to search out the proper objects base on the parameters
@para q this object is given by the user according which kind of query does the user want to do later, there is some basic information packaged in the object  like the search radius if the user want do a range query search latter.
@para metric this object will be use to calculate the distance of two objects
@return the proper objects address list through a vector
*/

vector<CIndexObject*>  CMVPInternalNode::search(CRangeQuery &q,CMetric &metric)
{

    vector<CIndexObject*> rs;

    vector<CIndexObject*> temprs;


    vector<CIndexObject*> pivots = CIndexNode::getPivots();
    int numpivot = CIndexNode::getPivots().size();

    double r=q.getRadius();

    double* tempd=new double[numpivot];

    bool shouldBeSearched = true;



    int i,j;


    for(i=0;i<pivots.size();i++)
    {
        tempd[i] = metric.getDistance(q.getQueryObject(),CIndexNode::getPivot(i));
        //tempd[i]: the distance between query and pivot[i]
        if(tempd[i]<=r)
        {
            rs.push_back(pivots[i]);
        }

    }




    for(j=0;j<child.size();j++)
    {

        shouldBeSearched = true;

        for(i=0;i<numpivot;i++)
        {

            if(abs(tempd[i]+r)<lower.at(i).at(j)||abs(tempd[i]-r)>upper.at(i).at(j))
            {


                if(abs(tempd[i]+upper.at(i).at(j))<=r)
                    shouldBeSearched=true;
                else
                {
                    shouldBeSearched = false;
                    break;
                }
            }
        }

        if(shouldBeSearched)
        {
            temprs = getChildAddress(j)->search(q,metric);
            rs.insert(rs.begin(),temprs.begin(),temprs.end());
        }

    }


    delete(tempd);

    return rs;

}
