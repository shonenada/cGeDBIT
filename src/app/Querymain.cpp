/*
Date: 02/03/2011 - Ludvik Jerabek - Initial Release
Version: 1.0
Comment: Sample Application using Getopt for Microsoft C\C++
License: LGPL

Revisions:

02/03/2011 - Ludvik Jerabek - Initial Release

**DISCLAIMER**
THIS MATERIAL IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING, BUT Not LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE, OR NON-INFRINGEMENT. SOME JURISDICTIONS DO NOT ALLOW THE
EXCLUSION OF IMPLIED WARRANTIES, SO THE ABOVE EXCLUSION MAY NOT
APPLY TO YOU. IN NO EVENT WILL I BE LIABLE TO ANY PARTY FOR ANY
DIRECT, INDIRECT, SPECIAL OR OTHER CONSEQUENTIAL DAMAGES FOR ANY
USE OF THIS MATERIAL INCLUDING, WITHOUT LIMITATION, ANY LOST
PROFITS, BUSINESS INTERRUPTION, LOSS OF PROGRAMS OR OTHER DATA ON
YOUR INFORMATION HANDLING SYSTEM OR OTHERWISE, EVEN If WE ARE
EXPRESSLY ADVISED OF THE POSSIBILITY OF SUCH DAMAGES. 
*/

#include "../HeaderFiles/objects/DoubleVector.h"
#include "../HeaderFiles/metric/Metric.h"
#include "../HeaderFiles/metric/EuclideanDistance.h"
#include "../HeaderFiles/index/index.h"
#include "../HeaderFiles/index/MVPIndex.h"
#include "../HeaderFiles/index/ListIndex.h"
#include "../HeaderFiles/indexalgorithm/PartitionMethod.h"
#include "../HeaderFiles/indexalgorithm/PivotSelectionMethod.h"
#include "../HeaderFiles/indexalgorithm/FFTPivotSelectionMethod.h"
#include "../HeaderFiles/indexalgorithm/BalancePartitionMethod.h"
#include "../HeaderFiles/query/Query.h"
#include "../HeaderFiles/query/RangeQuery.h"
#include "../HeaderFiles/util/getopt.h"

#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>


using namespace std;




vector<CIndexObject*> Queryi(string _IndexDataFileName, string _QueryDataFileName, double _radius)
{

	vector<CIndexObject*> indexData = CDoubleVector::loadData(_IndexDataFileName);
	vector<CIndexObject*> queryData = CDoubleVector::loadData(_QueryDataFileName);

	CRangeQuery* rangeQuery = new CRangeQuery(_radius,queryData[0]);

	CMetric *m = new CEuclideanDistance;
	CListIndex* listIndex = new CListIndex(m);
	

	listIndex->bulkLoad(indexData);
	
	vector<CIndexObject*> results = listIndex->search(rangeQuery);
	
	cout<<"results size:"<<results.size()<<endl;
	for(vector<CIndexObject*>::size_type ix=0;ix!=results.size();++ix)
	{
		CIndexObject *ob=results[ix];
		CDoubleVector *cd=(CDoubleVector*)ob;
		
		for(int i=0;i<cd->getLen();i++)
		{
			if(i==0)
				cout<<"(";
			if(i!=cd->getLen()-1)
				cout<<(cd->getData())[i]<<",";
			else
				cout<<(cd->getData())[i]<<")";
		}

		cout<<"\n";
	}

	return results;
}



template<typename type>
type stringToNumber(const char str[])
{
	istringstream iss(str);

	type data;

	iss>>data;

	return data;
}


/**
 * This is a utility class to query a VPIndex. It, taking command line
 * parameters, runs a set of query on the given index and compute the average
 * performance.
 * 
 * main function to evaluate the query performance of an {@link Index} The
 * eveluation is done by run a set of query on the Index, and compute the
 * average performance statistics, such as number of distance calculation,
 * number of index node visited (#I/O), and search time. The command line
 * interface to bulkload one {@link Index}, or a series of {@link Index}es for
 * scalability study.
 * 
 * -d [name of index, should be a prefix of the actual file name containing serialized database] 
 * -q [query file name] 
 * -f [offset of first query to be used in the query file, start from 0, inclusive, default 0] 
 * -l [offset of
 * last query to be used in the query file, exclusive, default 1] 
 * -i [minimum search radius, default 0] 
 * -a [maximum search radius, default 10] 
 * -s [step size for search radii, default 1] 
 * -t [data type, "vector", "protein", "dna", "image", "mass"] 
 * -p [length of the path distance list] 
 * -v [1 if search results are to be verified against a linear scan and 0 otherwise, default 0]
 * -g [debug level, default 0] 
 * -frag [fragment length, only meaningful for {@link Sequence}s] 
 * -dim [dimension of vector data to load, only meaningful for {@link DoubleVector}s] 
 * -res [output results to the given filename]
 * 
 * The {@link Metric} is hardcoded for each give data type.
 * 
 * @author Rui Mao, Willard
 * @version 2006.06.27
 */
int main(int argc, char** argv)
{
	static int verbose_flag;
	int c;

	char *indexName=0;
	//string indexName = "";
	//string queryFileName = "";
    //string forPrint = "";
	char *queryFileName=0;
	char *forPrint=0;
	int lastQuery = 1;
	double minRadius = 0.0;
	double maxRadius = 10.0;
	double step = 1.0;
	char *dataType = "sequence";
	int pathLength = 0;
	bool verify = false;

	int frag = 6;
	int dim = 2;
	//string resultsFileName = NULL;
	char *resultsFileName = NULL;
	

	
	
	while (1)
	{		
		static struct option_a long_options[] =
		{
			{"verbose", ARG_NONE, &verbose_flag, 1},
			{"brief",   ARG_NONE, &verbose_flag, 0},

			{"indexName",    ARG_REQ, 0 , 'd'},
			{"queryFileName",    ARG_REQ, 0 , 'q'},
			{"forPrint",    ARG_REQ, 0 , 'f'},
			{"lastQuery",    ARG_REQ, 0 , 'l'},
			{"minRadius",    ARG_REQ, 0 , 'i'},
			{"maxRadius",    ARG_REQ, 0 , 'a'},
			{"step",    ARG_REQ, 0 , 's'},
			{"dataType",    ARG_REQ, 0 , 't'},
			{"pathLength",    ARG_REQ, 0 , 'p'},
			{"verify",    ARG_REQ, 0 , 'v'},
			{"debug",    ARG_REQ, 0 , 'g'},
			{"frag",    ARG_REQ, 0 , 'r'},
			{"dim",    ARG_REQ, 0 , 'm'},
			{"resultsFileName",    ARG_REQ, 0 , 'F'},
			


			{ ARG_NULL , ARG_NULL , ARG_NULL , ARG_NULL }
		};
		
		int option_index = 0;
		c = getopt_long_a(argc, argv, ("d:q:f:l:i:a:s:t:p:v:g:r:m:F:"), long_options, &option_index);
		
		// Check for end of operation or error
		if (c == -1)
			break;
		
		// Handle options
		switch (c)
		{
		case 0:
			/* If this option set a flag, do nothing else now. */
			if (long_options[option_index].flag != 0)
				break;
			printf (("option %s"), long_options[option_index].name);
			if (optarg_a)
				printf ((" with arg %s"), optarg_a);
			printf (("\n"));
			break;
			
		case ('d'):	
			indexName = new char[strlen(optarg_a)+1];
			strcpy(indexName,optarg_a);
			printf (("option -indexName with value `%s'\n"), indexName);
			break;

		case ('q'):	
			queryFileName = new char[strlen(optarg_a)+1];
			strcpy(queryFileName,optarg_a);
			printf (("option -queryFileName with value `%s'\n"), queryFileName);
			break;

		case ('f'):	
			forPrint= new char[strlen(optarg_a)+1];
			strcpy(forPrint,optarg_a);
			printf (("option -forPrint with value `%s'\n"), forPrint);
			break;

		case ('l'):	
			lastQuery = stringToNumber<int>(optarg_a);
	    	printf (("option -lastQuery with value `%d'\n"), lastQuery);
			break;

		case ('i'):	
			minRadius=stringToNumber<double>(optarg_a);
			printf (("option -minRadius with value `%f'\n"), minRadius);
			break;

		case ('a'):	
			maxRadius=stringToNumber<double>(optarg_a);
			printf (("option -maxRadius with value `%f'\n"), maxRadius);
			break;

		case ('s'):	
			step=stringToNumber<double>(optarg_a);
			printf (("option -step with value `%f'\n"), step);
			break;

		case ('t'):	
			dataType = new char[strlen(optarg_a)+1];
			strcpy(dataType,optarg_a);
			printf (("option -dataType with value `%s'\n"), dataType);
			break;

		case ('p'):	
			pathLength = stringToNumber<int>(optarg_a);
			printf (("option -pathLength with value `%d'\n"), pathLength);
			break;

		case ('v'):	
			verify=stringToNumber<int>(optarg_a)? true : false;;
			printf (("option -verify with value `%d'\n"), verify);
			break;

		case ('g'):	
		 
			//printf (("option -debug with value `%d'\n"), debug);
			break;

		case ('r'):	
			frag = stringToNumber<int>(optarg_a);
			printf (("option -frag with value `%d'\n"), frag);
			break;

		case ('m'):	
			dim = stringToNumber<int>(optarg_a);
			printf (("option -dim with value `%d'\n"), dim);
			break;

		case ('F'):	
			resultsFileName = new char[strlen(optarg_a)+1];
			strcpy(resultsFileName,optarg_a);
			printf (("option -resultsFileName with value `%s'\n"), resultsFileName);
			break;





		case '?':
			/* getopt_long already printed an error message. */
			break;

		default:
			abort();
		}
	}

	if (verbose_flag)
		printf (("verbose flag is set\n"));


	if (optind < argc)
	{
		printf (("non-option ARGV-elements: "));
		while (optind < argc) printf (("%s "), argv[optind++]);
		printf (("\n"));
	}

	Queryi(indexName,queryFileName,maxRadius);
	system("pause");

	return 0;
}
