#include "../../HeaderFiles/objects/DNA.h"

double CDNA::EditDistanceMatrix[15][15] = 
{
    {  0,  1,  1,  1,0.5,  1,0.5,  1,0.5,  1,  1,0.5,0.5,0.5,0.5}, // A Adenine 
    {  1,  0,  1,  1,  1,0.5,0.5,  1,  1,0.5,0.5,  1,0.5,0.5,0.5}, // C Cytosine
    {  1,  1,  0,  1,0.5,  1,  1,0.5,  1,0.5,0.5,0.5,  1,0.5,0.5}, // G Guanine  
    {  1,  1,  1,  0,  1,0.5,  1,0.5,0.5,  1,0.5,0.5,0.5,  1,0.5}, // T Thymine   
    {0.5,  1,0.5,  1,  0,  1,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5}, // R Purine (A or G)  
    {  1,0.5,  1,0.5,  1,  0,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5}, // Y Pyrimidine (C, T, or U)      
    {0.5,0.5,  1,  1,0.5,0.5,  0,  1,0.5,0.5,0.5,0.5,0.5,0.5,0.5}, // M C or A  
    {  1,  1,0.5,0.5,0.5,0.5,  1,  0,0.5,0.5,0.5,0.5,0.5,0.5,0.5}, // K T, U, or G  
    {0.5,  1,  1,0.5,0.5,0.5,0.5,0.5,  0,  1,0.5,0.5,0.5,0.5,0.5}, // W T, U, or A   
    {  1,0.5,0.5,  1,0.5,0.5,0.5,0.5,  1,  0,0.5,0.5,0.5,0.5,0.5}, // S C or G 
    {  1,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,  0,0.5,0.5,0.5,0.5}, // B C, T, U, or G (not A)
    {0.5,  1,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,  0,0.5,0.5,0.5}, // D A, T, U, or G (not C)
    {0.5,0.5,  1,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,  0,0.5,0.5}, // H A, T, U, or C (not G) 
    {0.5,0.5,0.5,  1,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,  0,0.5}, // V A, C, or G (not T, not U)
    {0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,  0}  // N Anybase (A,C,G,T,or U) 
};

CDNASymbol CDNA::Symbols[DNASYMBOLNUMBER] = {
	{0, 'A', "Adenine"}, {1, 'C', "Cytosine"}, {2, 'G', "Guanie"}, {3, 'T', "Thymine"},
	{4, 'R', "Purine"}, {5, 'Y', "Pyrimidine"}, {6, 'M', "C or A"}, {7, 'K', "T, U, or G"},
	{8, 'W', "T, U or A"}, {9, 'S', "C or G"}, {10, 'G', "not A"}, {11, 'D', "not C"},
	{12, 'H', "not G"}, {13, 'V', "not T, U"}, {14, 'N', "Any base"}
};

CDNA::CDNA(string _sid, string _sequence)
{
    sid = _sid;
    sequence = _sequence;
}

int CDNA::getSymbolByte(char symbol)
{
    int i;
    for(i=0;i<DNASYMBOLNUMBER;i++)
        if(symbol == Symbols[i].abbr)
            return Symbols[i].byte;
	return -1;
}

vector<int> CDNA::getBytes(){
	int i;
	int sequenceSize = sequence.size();
	vector<int> bytes;
	int temp;
	for(i=0;i<sequenceSize;i++){
		temp = getSymbolByte(sequence.at(i));
		bytes.push_back(temp);
	}
	return bytes;
}

int CDNA::getSize(){
	return getBytes().size();
}

vector<shared_ptr<CDNA>> CDNA::tear(CDNA* dna, int fragmentLength){
	int totalSize = dna->getSize() - fragmentLength;
	string seq_id = dna->sid;
	string seq = dna->sequence;
	vector<shared_ptr<CDNA>> data;
	for(int i=0;i<totalSize;i++){
		shared_ptr<CDNA> temp(new CDNA(seq_id, seq.substr(i, fragmentLength)));
		data.push_back(temp);
	}
	return data;
}

vector<shared_ptr<CDNA>> CDNA::loadData(string filename, int maxSize, int fragmentLength){
	vector<shared_ptr<CDNA>> data;
	
	ifstream infile(filename, ios::in);
	vector<shared_ptr<CDNA>> dnas;
	
	if(!infile.is_open()){
		cerr << "Stop! Cannot open the file." << endl;
		return data;
	}

	string ident = "";
	int i;
	int counter = 0;
	int sequenceLengthCounter = 0;
	string currentSequence;
	string line;
	char buffer[65];

	while(!infile.eof() && counter < maxSize && sequenceLengthCounter < maxSize)
	{
		infile.getline(buffer, 65);
		line = string(buffer);
		if(line.size() >= 1){
			if(line.at(0) == '>'){
				if(currentSequence.size() > 0)
				{
					shared_ptr<CDNA> temp(new CDNA(ident, currentSequence));
					dnas.push_back(temp);
					counter += currentSequence.size();
					currentSequence = "";
				}
				ident = line;
			}else{
				currentSequence = currentSequence + line;
				sequenceLengthCounter = currentSequence.size();
			}
		}
	}
	if(currentSequence.size() > 0){
		shared_ptr<CDNA> temp(new CDNA(ident, currentSequence));
		dnas.push_back(temp);
	}

	CDNA* dna = dnas[0].get();
	int totalSize = dna->getSize() - fragmentLength;
	string seq_id = dna->sid;
	string seq = dna->sequence;

	for(int i=0;i<totalSize;i++){
		shared_ptr<CDNA> temp(new CDNA(seq_id, seq.substr(i, fragmentLength)));
		data.push_back(temp);
	}
	return data;
	/**
	for(i=0;i<dnas.size();i++)
	{
		vector<CDNA> tmp;
		tmp = tear(dnas[i], fragmentLength);
		for(int j=0;j<tmp.size();j++){
			data.push_back(tmp[j]);
		}
	}
	return data;
	*/
}
