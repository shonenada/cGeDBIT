#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <ctime>
#include "../../HeaderFiles/objects/IndexObject.h"
#include "../../HeaderFiles/objects/DNA.h"
#include "../../HeaderFiles/objects/RNA.h"
#include "../../HeaderFiles/metric/Metric.h"
#include "../../HeaderFiles/metric/DNAMetric.h"
#include "../../HeaderFiles/metric/RNAMetric.h"

using namespace std;

int main(int argc, char* argv[]){

    string inputFile;
    string outputFile;
    char* type;
    int maxSize = 100;
    int fragmentLength = 5;

    for(int i=0;i<argc;i++)
    {
        //input filepath
        if(!strcmp(argv[i], "-i"))
        {
            inputFile = string(argv[i+1]);
        }
        // output filepath
        if(!strcmp(argv[i], "-o"))
        {
            outputFile = string(argv[i+1]);
        }
        // maxSize
        if(!strcmp(argv[i], "-m"))
        {
            maxSize = atoi(argv[i+1]);
            cout << maxSize << endl;
        }

        // fragment length
        if(!strcmp(argv[i], "-f"))
        {
            fragmentLength = atoi(argv[i+1]);
        }

        // type
        if(!strcmp(argv[i], "-t"))
        {
            type = new char[strlen(argv[i+1]) + 1];
            strcpy(type, argv[i]);
        }

    }

    clock_t start, finish;
    double totalTime;
    start = clock();

    ofstream outfile(outputFile, ios::out);

    cout << "Begin loadding file: " << endl;

    vector<shared_ptr<CIndexObject>>* list;
    CMetric* metric;

    if(!strcmp(type, "dna"))
    {
        (vector<shared_ptr<CDNA>>*) list = &(CDNA::loadData(inputFile, maxSize, fragmentLength));
        metric = new CDNAMetric();
    }

    if(!strcmp(type, "rna"))
    {
        (vector<shared_ptr<CRNA>>*) list = &(CRNA::loadData(inputFile, maxSize, fragmentLength));
        metric = new CRNAMetric();
    }

    double dist;
    for(int i=0;i<list->size()-1;i++){
        dist = metric->getDistance(*(list)[0].get(), *(list)[i+1].get());
        outfile << dist << ".0" <<endl;
    }

    finish = clock();
    totalTime = (double) (finish - start) / CLOCKS_PER_SEC;
    cout << "Execute Time: " << totalTime << " seconds." << endl;

    return 1;
}
