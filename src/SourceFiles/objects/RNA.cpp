#include "../../HeaderFiles/objects/RNA.h"

CRNASymbol CRNA::Symbols[RNASYMBOLNUMBER] = {
	{0, 'A', "Adenine"}, {1, 'C', "Cytosine"}, {2, 'G', "Guanie"}, {3, 'U', "Uracil"},
	{4, 'R', "Purine"}, {5, 'Y', "Pyrimidine"}, {6, 'M', "C or A"}, {7, 'K', "T, U, or G"},
	{8, 'W', "T, U or A"}, {9, 'S', "C or G"}, {10, 'G', "not A"}, {11, 'D', "not C"},
	{12, 'H', "not G"}, {13, 'V', "not T, U"}, {14, 'N', "Any base"}
};

CRNA::CRNA(string _sid, string _sequence)
{
    sid = _sid;
    sequence = _sequence;
}

int CRNA::getSymbolByte(char symbol)
{
    int i;
    for(i=0;i<RNASYMBOLNUMBER;i++)
        if(symbol == Symbols[i].abbr)
            return Symbols[i].byte;
	return -1;
}

vector<int> CRNA::getBytes(){
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

int CRNA::getSize(){
	return getBytes().size();
}

vector<shared_ptr<CRNA>> CRNA::tear(CRNA* rna, int fragmentLength){
	int totalSize = rna->getSize() - fragmentLength;
	string seq_id = rna->sid;
	string seq = rna->sequence;
	vector<shared_ptr<CRNA>> data;
	for(int i=0;i<totalSize;i++){
		shared_ptr<CRNA> temp(new CRNA(seq_id, seq.substr(i, fragmentLength)));
		data.push_back(temp);
	}
	return data;
}

vector<shared_ptr<CRNA>> CRNA::loadData(string filename, int maxSize, int fragmentLength){
	vector<shared_ptr<CRNA>> data;
	
	ifstream infile(filename, ios::in);
	vector<shared_ptr<CRNA>> rnas;
	
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
					shared_ptr<CRNA> temp(new CRNA(ident, currentSequence));
					rnas.push_back(temp);
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
		shared_ptr<CRNA> temp(new CRNA(ident, currentSequence));
		rnas.push_back(temp);
	}

	CRNA* rna = rnas[0].get();
	int totalSize = rna->getSize() - fragmentLength;
	string seq_id = rna->sid;
	string seq = rna->sequence;

	for(int i=0;i<totalSize;i++){
		shared_ptr<CRNA> temp(new CRNA(seq_id, seq.substr(i, fragmentLength)));
		data.push_back(temp);
	}
	return data;
}
