#ifndef DNA_H
#define DNA_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>

#include "IndexObject.h"

using namespace std;

#define DNASYMBOLNUMBER 15

struct CDNASymbol{
    int byte;
    char abbr;
    string description;
};

class CDNA:
    public CIndexObject
{

public:
    static double EditDistanceMatrix[15][15];
    static CDNASymbol Symbols[DNASYMBOLNUMBER];
    string sid;
    string sequence;

    CDNA(string sid, string sequence);
    void initDNASymbols();

    int getSymbolByte(char symbol);
	vector<int> getBytes();
    
    virtual int getSize();


    static vector<shared_ptr<CDNA>> loadData(string filename, int maxSize, int fragmentLength);
	static vector<shared_ptr<CDNA>> tear(CDNA* dna, int fragmentLength);

};

#endif
