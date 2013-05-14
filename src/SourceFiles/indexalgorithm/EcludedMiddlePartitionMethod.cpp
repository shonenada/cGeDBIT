#include "../../HeaderFiles/indexalgorithm/EcludedMiddlePartitionMethod.h"



void extract(vector<CIndexObject*> &data,vector<double> &distance,int start,int target,int first)
{

    data.insert(data.begin()+start,data.at(target+first));
    data.erase(data.begin()+(target+first+1));
    distance.insert(distance.begin()+start,distance[target]);
    distance.erase(distance.begin()+(target+1));

}



/*==============================================================================================================================*/


CEcludedMiddlePartitionMethod::CEcludedMiddlePartitionMethod(void)
{
}


CEcludedMiddlePartitionMethod::~CEcludedMiddlePartitionMethod(void)
{
}

CPartitionResults CEcludedMiddlePartitionMethod::partition(CMetric *metric, const vector<CIndexObject*> &pivots,vector<CIndexObject*> &data,int first,int size,double maxRadius,int numPartitions,int maxLeafSize,double middleProportion)
{

    int i,j,k;

    const int numPivots = pivots.size();

    //double* distance = new double[size]; 

    vector<double> LowerBound(3);
    vector<double> upperBound(3);

    //CDoubleIndexObjectPair* tempPair;



    vector<double> distance(size);
    //distance from pivot to each dataObject

    for(int i=first; i<first+size; i++)
    {
        distance[i-first]= metric->getDistance(pivots.at(0),data.at(i));

        //tempPair = new CDoubleIndexObjectPair(metric->getDistance(pivots.at(0),data.at(i)),data[i]);
        //wrapper.push_back(tempPair);
    }



    double midRadius = getMid(distance);
    //the 





    vector<int> offsets(4); 
    offsets.at(0)=first;    
    offsets.at(3)=first+size;

    offsets[1]= offsets[0];




    LowerBound[0]=distance.at(getMin(distance,0,distance.size()));
    upperBound[0]=LowerBound[0];

    for(int i=first;i<size+first;i++)
    { 
        if(distance[i-first]<midRadius-maxRadius)
        {



            if(distance[i-first]<=LowerBound[0])
            {
                LowerBound[0]=distance[i-first];
            }
            if(distance[i-first]>=upperBound[0])
            {
                upperBound[0]=distance[i-first];
            }



            swap(data.at(offsets[1]),data.at(i));            
            swap(distance[i-first],distance[offsets[1]-first]);

            offsets[1]++;
        }
    }


    offsets[2]=offsets[1];
    LowerBound[1]=midRadius+maxRadius;
    upperBound[1]=LowerBound[1];



    for(int i=offsets[1];i<size+first;i++)
    { 
        if(distance[i-first]>=midRadius+maxRadius)
        {    


            if(distance[i-first]<=LowerBound[1])
            {
                LowerBound[1]=distance[i-first];
            }
            if(distance[i-first]>=upperBound[1])
            {
                upperBound[1]=distance[i-first];
            }



            swap(data.at(offsets[2]),data.at(i));            
            swap(distance[i-first],distance[offsets[2]-first]);

            offsets[2]++;

        }
    }


    //





    for(i=1;abs(offsets[3]-offsets[2])>middleProportion*size;i=-i)
    {               
        if(i>0)
        {


            j=getMin(distance,offsets[2]-first,offsets[3]-first);

            while (abs(offsets[3]-offsets[2])>1)
            {
                upperBound[0]=distance[j];
                extract(data,distance,offsets[1]-first,j,first);
                offsets[1]++;
                offsets[2]++;

                k=getMin(distance,j+1,offsets[3]-first);

                if(k>=0&&distance[j]==distance[k])
                {
                    j=k;
                }
                else
                {
                    break;
                }


            }


            //upperBound[0]=distance[j];

            //data.insert(data.begin()+offsets[1],data.at(j+first));
            //data.erase(data.begin()+(j+first+1));
            //distance.insert(distance.begin()+offsets[1],distance[j]);
            //distance.erase(distance.begin()+(j+1));


            //offsets[1]++;
            //offsets[2]++;


        }
        if(i<0)
        { 



            j=getMax(distance,offsets[2]-first,offsets[3]-first);

            while (abs(offsets[3]-offsets[2])>1)
            {
                LowerBound[1]=distance[j];
                extract(data,distance,offsets[2]-first,j,first);            
                offsets[2]++;


                k=getMax(distance,j+1,offsets[3]-first);


                if(k>=0&&distance[j]==distance[k])
                {
                    j=k;
                }
                else
                {
                    break;
                }



            }


            //j=getMax(distance,offsets[2],offsets[3]);

            //LowerBound[1]=distance[j]; 

            ///*  data.insert(data.begin()+offsets[2],data.at(j+first));
            //data.erase(data.begin()+(j+first+1));
            //distance.insert(distance.begin()+offsets[2]-first,distance[j]);
            //distance.erase(distance.begin()+(j+1));    */       


            //extract(data,distance,offsets[2],j,first);

            //offsets[2]++;
        }

    }
    //if proportion of middle set bigger than m , decrease its size;
    LowerBound[2]=distance[getMin(distance,offsets[2]-first,offsets[3]-first)];
    upperBound[2]=distance[getMax(distance,offsets[2]-first,offsets[3]-first)];

    /*data.insert(data.end(),data.begin()+offsets[3],data.begin()+offsets[4]+1);
    data.erase(data.begin()+offsets[3],data.begin()+offsets[4]);    */
    //move all ecluded datas to the end of datalist 





    vector<vector<double> > lowerBounds;	
    lowerBounds.push_back(LowerBound);

    vector<vector<double> > upperBounds;
    upperBounds.push_back(upperBound);

    /*  for(int i=0;i<offsets.size();i++)
    {
    offsets.at(i)-=first;
    }*/

    return CPartitionResults(offsets,lowerBounds,upperBounds);
}

int CEcludedMiddlePartitionMethod::getMin(vector<double> &distance,int begin,int end)
{
    if(end<begin||begin>=distance.size())
        return -1;

    int minIndex=begin;

    double min = distance[begin];



    for (int i = begin;i<end;i++)
    {
        if(distance[i]<=min)
        {
            minIndex=i;
            min = distance[i];
        }
    }

    return minIndex;
}

int CEcludedMiddlePartitionMethod::getMax(vector<double> &distance,int begin,int end)
{
   if(end<begin||begin>=distance.size())
        return -1;

    int maxIndex=begin;

    double max = distance[begin];



    for (int i = begin;i<end;i++)
    {
        if(distance[i]>=max)
        {
            maxIndex=i;
            max = distance[i];
        }
    }

    return maxIndex;
}

double CEcludedMiddlePartitionMethod::getMid(vector<double> distance)
{
    nth_element(distance.begin(),distance.begin()+distance.size()/2,distance.end());
    return distance.at(distance.size()/2);



}

CPartitionResults CEcludedMiddlePartitionMethod:: partition(CMetric *metric, const vector<CIndexObject*> &pivots,vector<CIndexObject*> &data, int first, int size, int numPartitions, int maxLeafSize)
{

    return partition(metric, pivots,data, first,size,5/*maxR*/,numPartitions,maxLeafSize,0.3 /*middleProportion*/);
}