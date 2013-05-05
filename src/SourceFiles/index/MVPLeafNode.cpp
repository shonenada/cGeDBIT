#include "../../HeaderFiles/index/MVPLeafNode.h"

CMVPLeafNode::CMVPLeafNode()
{

}

CMVPLeafNode::CMVPLeafNode(vector<CIndexObject*> pivots,vector<CIndexObject*> dataObjects,vector<vector<double> > distance,int height):CIndexNode(pivots,height)
{
	this->pivots=pivots;
	this->dataObjects=dataObjects;
	this->distance=distance;
}

CMVPLeafNode::~CMVPLeafNode()
{

}


vector<double> CMVPLeafNode::getDataPointPivotDistance(int pivotNumber)
{
	return distance[pivotNumber];
}

int CMVPLeafNode::numChildren()
{
	return dataObjects.size();
}

CIndexObject* CMVPLeafNode::getObject(int childIndex)
{
	return dataObjects[childIndex];
}

vector<CIndexObject*> CMVPLeafNode::getObjects()
{
	return dataObjects;
}

vector<CIndexObject*> CMVPLeafNode::search(CRangeQuery &q,CMetric &metric)
{
	vector<CIndexObject*> rs;
	int numpivots = pivots.size();
		

	int i,j;
	double* tempd = new double[numpivots];
	double r = q.getRadius();

	for(i=0;i<numpivots;i++)
		tempd[i] = metric.getDistance(CIndexNode::getPivot(i),q.getQueryObject());




	for(i=0;i<dataObjects.size();i++)
	{			
		for(j=0;j<numpivots;j++)
		{
			if(abs(tempd[j]+r)<distance.at(i).at(j)||abs(tempd[j]-r)>distance.at(i).at(j))
				break;
			if(metric.getDistance(dataObjects.at(i),q.getQueryObject())<=r)
				rs.push_back(dataObjects.at(i));
		}
	}

	return rs;
}

