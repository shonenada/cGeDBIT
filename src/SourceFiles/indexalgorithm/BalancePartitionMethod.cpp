#include "../../HeaderFiles/indexalgorithm/BalancePartitionMethod.h"
#include "../../HeaderFiles/type/DoubleIndexObjectPair.h"
#include "../../HeaderFiles/index/MVPInternalNode.h"

#include <cmath>

/** @addtogroup CBalancePartitionMethod
*  @{
*/

CBalancePartitionMethod::CBalancePartitionMethod()
{

}

CBalancePartitionMethod::~CBalancePartitionMethod()
{

}

/** Sort DoubleIndexObjectPair array where element's index belongs [fromIndex, toIndex) */
void CBalancePartitionMethod::sort(vector<CDoubleIndexObjectPair>& arr, int fromIndex, int toIndex)
{
	int i, j;
	int low = fromIndex;
	int high = toIndex;
	CDoubleIndexObjectPair temp;
	if ( low >= high )
	{
		return ;
	}
	i = low;
	j = high-1;
	temp = arr[i];
	while( i < j )
	{
		while( i<j && temp.getDouble() < arr[j].getDouble() )
		{
			j --;
		}
		if (i<j){
			arr[i++] = arr[j];
		}
		while( i<j && arr[i].getDouble() <= temp.getDouble() )
		{
			i++;
		}
		if (i<j)
		{
			arr[j--] = arr[i];
		}
	}
	arr[i] = temp;
	sort(arr, low, --j);
	sort(arr, ++i, high);
}

CPartitionResults CBalancePartitionMethod::partition(CMetric *metric, const vector<CIndexObject*> &pivots,vector<CIndexObject*> &data,int first,int size,int maxRadius,int numPartitions,int maxLeafSize,double middleProportion)
{
	return partition( metric,  pivots , data, first,  size,  numPartitions, maxLeafSize);
}

CPartitionResults CBalancePartitionMethod::partition(CMetric *metric, const vector<CIndexObject*> &pivots ,vector<CIndexObject*> &data, int first, int size, int numPartitions, int maxLeafSize)
{

	const int numPivots = pivots.size();
	const int fanout = (int) pow(numPartitions, numPivots);

	/**
	*  The lower and upper bound of distances from each child to each vantage point
	*/

	vector<vector<double> > lower(numPivots);
	vector<vector<double> > upper(numPivots);

	for(int vectorIndex=0;vectorIndex<numPivots;++vectorIndex)
	{
		lower[vectorIndex].resize(fanout);
		upper[vectorIndex].resize(fanout);
	}

	//double* lower = new double[fanout * numPivots];
	//double* upper = new double[fanout * numPivots];

	vector<CDoubleIndexObjectPair> wrapper;

	for(int i=first; i<first+size; i++)
	{
		CDoubleIndexObjectPair temp(0, data.at(i));
		wrapper.push_back(temp);
	}

	/** split data  */

	/** total cluster number when partition based on each vp, SVF ^ i.  */
	int clusterNumber = 1; 
	/** number of final cluster in each of current cluster.  */
	int clusterCardinality = fanout;

	/**
	*  offset of the first point in current cluster in wrapper, the
	*  whole data list
	*  this array has an additional element at the end of value size,
	*  serving as a loop guard
	*  the first element of this array is always 0
	*/
	vector<int> clusterOffset(2);
	clusterOffset[0] = first;                                           //first
	clusterOffset[1] = first+size;

	/** compute distance to the current VP */
	for(int i=0;i<numPivots;i++)
	{
		for(int j=0;j<size;j++)
		{
			CIndexObject* temp = (CIndexObject*) wrapper[j].getObject();
			wrapper[j].setDouble(metric->getDistance(pivots[i], temp));if(wrapper[j].getDouble()==0)cout<<"dis=0:"<<j<<endl;
		}

		/** sort each part.  */
		for(int j=0; j<clusterNumber; j++)
		{
			this->sort(wrapper, clusterOffset[j]-first, clusterOffset[j+1]-first);
		}

		const int nextClusterNumber = clusterNumber * numPartitions;
		vector<int> nextClusterOffset(nextClusterNumber + 1);
		nextClusterOffset[0] = first;                                          //first
		nextClusterOffset[nextClusterNumber] = first+size;

		int nextClusterCardinality = clusterCardinality / numPartitions;

		/** split each current cluster into SVF sub-clusters based on the distance to current VP.  */
		for(int j=0;j<clusterNumber; j++)
		{
			/** size of current cluster (number of points).  */
			const int clusterSize = clusterOffset[j + 1] - clusterOffset[j];

			/** if this cluster is empty, set all its sub-cluster to be empty.  */
			if(clusterSize == 0)
			{
				for(int k=0;k<numPartitions;k++)
				{
					nextClusterOffset[j * numPartitions + k + 1] = clusterOffset[j + 1];
				}

				/** jump to next cluster.  */
				continue;
			}

			/** find the last indices of each distinct distance value in wrapper, which is already sorted.  */
			vector<int> tempIndex;
			vector<double> tempValue;

			/** the distinct disatnce value in check, and the number of points with this distance.  */
			double currentDistance = wrapper.at(clusterOffset[j]-first).getDouble();

			int sum =first;                                                            //first

			for(int k= clusterOffset[j]-first; k<clusterOffset[j + 1]-first;k++)
			{
				const double nextDistance = wrapper[k].getDouble();
				if(nextDistance != currentDistance)
				{
					/** find next  */
					tempIndex.push_back(sum);
					tempValue.push_back(currentDistance);
					currentDistance = nextDistance;
				}
				sum++;
			}
			/** put the last distinct value into the list.  */
			tempIndex.push_back(sum);
			tempValue.push_back(currentDistance);

			const int distinctSize = tempIndex.size();

			/** index of first point with current distinct distance value, this is the offset in current cluster, not the index in wrapper.
			*  distinct distance values
			*/

			int* firstPointWithDistinctDistance = new int[distinctSize + 1];
			double* distinctDistance = new double[distinctSize];
			firstPointWithDistinctDistance[0] = first;                                     //first
			firstPointWithDistinctDistance[distinctSize] = clusterSize+first;
			distinctDistance[0] = wrapper[clusterOffset[j]-first].getDouble();

			for(int k=1; k<distinctSize;k++)
			{
				firstPointWithDistinctDistance[k] = (int) tempIndex.at(k - 1);
				distinctDistance[k] = (double) tempValue.at(k);
			}

			/** assign the total distinctSize set of points with
			* identical distance value
			* to at most SVF sub-clusters, which is actually split
			* current cluster
			*/

			/** number of distinct set that are already been assigned  */
			int startingDistinctSet = 0;                                                 

			/** if distince set number is greater than SVF, assign them,
			* otherwise,
			* just assign one set to each sub-cluster, remain
			* sub-clusters are all empty
			*/
			int k = 0;
			/**  k is the current sub-cluster to assign distinct set to . */

			while((k < numPartitions - 1) && (distinctSize - startingDistinctSet > numPartitions - k))
			{
				/** assign sets based on their cardinality, prefer balance sub-cluster  */
				const int median = (clusterSize+first - firstPointWithDistinctDistance[startingDistinctSet]) / (numPartitions - k);

				/** find the distince set that contains the median point.  */
				int t = startingDistinctSet;
				while( firstPointWithDistinctDistance[t + 1]< median + firstPointWithDistinctDistance[startingDistinctSet])
				{
					t++;
				}

				/** if median falls in the first distinct set, assign this set to current cluster. */

				if(t != startingDistinctSet){
					t = (firstPointWithDistinctDistance[t + 1] - median - firstPointWithDistinctDistance[startingDistinctSet] >= median + firstPointWithDistinctDistance[startingDistinctSet] - firstPointWithDistinctDistance[t]) ? t - 1 : t;
				}

				/** now startingDistinctSet is the index of the first distinct set, and t is the index of the last distinct set, to be assinged to current sub-cluster set the sub-cluster offset, lower, upper bound  */
				nextClusterOffset[j * numPartitions + k + 1] = clusterOffset[j] + firstPointWithDistinctDistance[t + 1]-first;

				const int firstChild = j * clusterCardinality + k * nextClusterCardinality;

				for(int temp=firstChild;temp<firstChild + nextClusterCardinality;temp++)
				{
					lower[i][temp] = distinctDistance[startingDistinctSet];
					upper[i][temp] = distinctDistance[t];
				}

				startingDistinctSet = t + 1;
				k++;
			}
			/** if reaches the last sub-cluster, assign all remain set to it.  */
			if(k == numPartitions - 1)
			{
				/** set the sub-cluster offset, lower, upper bound.  */
				nextClusterOffset[j * numPartitions + k + 1] = clusterOffset[j + 1];

				const int firstChild = j * clusterCardinality + k * nextClusterCardinality;
				for(int temp = firstChild;temp<firstChild;temp++)
				{
					lower[i][temp] = distinctDistance[startingDistinctSet];
					upper[i][temp] = distinctDistance[distinctSize - 1];
				}
			}
			else
			{
				/** remain set number is not greater than remain sub-cluster number, assign one set to each sub-cluster  */
				for(int t=startingDistinctSet; t<distinctSize;t++)
				{
					nextClusterOffset[j * numPartitions + k + 1] = clusterOffset[j] + firstPointWithDistinctDistance[t + 1]-first;
					const int firstChild = j * clusterCardinality + k * nextClusterCardinality;
					for(int temp=firstChild; temp<firstChild + nextClusterCardinality; temp++)
					{
						lower[i][temp] = distinctDistance[t];
						upper[i][temp] = distinctDistance[t];
					}
					k++;
				}

				if (k<numPartitions)
				{
					/** if there are still sub-cluster, set them to be null  */
					for(;k<numPartitions;k++)
					{
						nextClusterOffset[j * numPartitions + k + 1] = clusterOffset[j + 1];
					}
				}
			}
		} /**  end of a loop for each cluster  */

		clusterOffset.resize(nextClusterOffset.size());

		for(int i=0; i<nextClusterNumber + 1;i++)
		{
			clusterOffset[i] = nextClusterOffset[i];
		}

		clusterCardinality = nextClusterCardinality;
		clusterNumber = nextClusterNumber;

	} /** end of loop for each vantage point  */

	/**  compute non-empty cluster number.  */

	int childrenNumber = 0;
	for(int i=0;i<fanout;i++)
	{
		if(clusterOffset[i] < clusterOffset[i + 1])
		{
			childrenNumber++;
		}
	}

	/** if there are some empty cluster, delete them.  */
	if(childrenNumber < fanout)
	{
		vector<vector<double> > newLower(numPivots);
		vector<vector<double> > newUpper(numPivots);

		for(int vectorIndex=0;vectorIndex<numPivots;++vectorIndex)
		{
			newLower.resize(fanout);
			newUpper.resize(fanout);
		}

		vector<int> newOffset(childrenNumber + 1);
		newOffset[childrenNumber] = size;
		int j = 0;
		for(int i=0;i<fanout; i++)
		{
			if(clusterOffset[i] < clusterOffset[i + 1])
			{
				newLower[j] = lower[i];
				newUpper[j] = lower[i];
				newOffset[j] = clusterOffset[i];
				j++;
			}
		}
		lower = newLower;
		upper = newUpper;
		clusterOffset = newOffset;
	}

	/*vector<vector<CIndexObject*>> subDataList;
	for(int i=0;i<childrenNumber;i++)
	{
		vector<CIndexObject*> subList;

		for(int j=clusterOffset[i];j<clusterOffset[i + 1];++j)
		{
			CIndexObject* temp = (CIndexObject*) wrapper[j].getObject();
			subList.push_back(temp);
		}

		if(subList.size() == 0)
		{
			cout << "sub list : " << i << " is empty!" << endl;
		}

		subDataList.push_back(subList);
	}*/

	CPartitionResults *result = new CPartitionResults(clusterOffset, lower, upper);

	return *result;

}

/**
* @}  //CDNA
*/
