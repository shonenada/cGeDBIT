#include "../../HeaderFiles/indexalgorithm/PartitionResults.h"


CPartitionResults::CPartitionResults(vector<int> &offsets,vector<vector<double>> &lowerBounds,vector<vector<double>> &upperBounds)
{
	this->offsets=offsets;

	this->lowerBounds=lowerBounds;
	this->upperBounds=upperBounds;

}

CPartitionResults::CPartitionResults()
{
}

CPartitionResults::~CPartitionResults()
{

}

int CPartitionResults::partitionSize()
{
	return offsets.size();
}

int CPartitionResults::getPartition(int index)
{
	return offsets.at(index);
}

vector<vector<double>>& CPartitionResults::getLowerBounds()
{
	return lowerBounds;
}

vector<vector<double>>& CPartitionResults::getUpperBounds()
{
	return upperBounds;
}
