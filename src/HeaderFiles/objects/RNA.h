#ifndef RNA_H
#define RNA_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>

#include "IndexObject.h"

using namespace std;

#define RNASYMBOLNUMBER 15

struct CRNASymbol{
    int byte;
    char abbr;
    string description;
};

class CRNA:
    public CIndexObject
{

public:
    static CRNASymbol Symbols[RNASYMBOLNUMBER];
    string sid;
    string sequence;

    CRNA(string sid, string sequence);
    void initRNASymbols();

    int getSymbolByte(char symbol);
	vector<int> getBytes();
    
    virtual int getSize();

    static vector<shared_ptr<CRNA>> loadData(string filename, int maxSize, int fragmentLength);
	static vector<shared_ptr<CRNA>> tear(CRNA* dna, int fragmentLength);

};

#endif
