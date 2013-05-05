#include "../../HeaderFiles/index/MVPInternalNode.h"

CMVPInternalNode::CMVPInternalNode()
{
}

CMVPInternalNode::CMVPInternalNode(vector<CIndexObject*> pivots,vector<vector<double> > lower,vector<vector<double> > upper,vector<CIndexNode*> child,int myHeight):CIndexNode(pivots,myHeight)
{

	this->child=child;

	this->upper=upper;
	this->lower=lower;

}

CMVPInternalNode::~CMVPInternalNode()
{

}

vector<CIndexNode*> CMVPInternalNode::getSubTree()
{
	return this->child;
}



int CMVPInternalNode::numChildren()
{
	return child.size();
}

CIndexNode* CMVPInternalNode::getChildAddress(int subTreeRootAddress)
{
	return child[subTreeRootAddress];
}

vector<double> CMVPInternalNode::getChildLowerRange(int pivotIndex)
{
	return lower[pivotIndex];
}

vector<double> CMVPInternalNode::getChildUpperRange(int pivotIndex)
{
	return upper[pivotIndex];
}

vector<CIndexObject*>  CMVPInternalNode::search(CRangeQuery &q,CMetric &metric)
{
	
	vector<CIndexObject*> rs;
	vector<CIndexObject*>* temprs;

    vector<CIndexObject*> pivots = CIndexNode::getPivots();
    int numpivot = CIndexNode::getPivots().size();

    int numpartition = child.size();
	int numperlayerpartition = lower.at(0).size();

	double r=q.getRadius();

	double tempd;

	int lowest=0;
    //the lower index of the range which will be searched
    int highest=upper.at(0).size()-1;
    //the upper index of the range which will be searched

	int i,j;
	bool isIncluded=false;

	for(i=0;i<numpivot;i++)
	{
		tempd = metric.getDistance(q.getQueryObject(),CIndexNode::getPivot(i));
        //tempd: the distance between query and pivot[i]
        if(tempd<=r)
        {
            rs.push_back(pivots[i]);
        }

		for (;lowest<=highest;lowest++)
		{
			if (abs(tempd-r)>=lower.at(i).at(lowest))
			{				
                for(;highest>=lowest;highest--)
                    if(upper.at(i).at(highest)<=abs(tempd+r))
						break;
                break;
			}
		}
		lowest = (lowest+1)*numperlayerpartition-1;
		highest = (highest+1)*numperlayerpartition-1;
	}
	for(;lowest<=highest||lowest<child.size();lowest++)
	{
		temprs = &getChildAddress(lowest)->search(q,metric);
		rs.insert(rs.begin(),temprs->begin(),temprs->end());
	}
	return rs;
}
