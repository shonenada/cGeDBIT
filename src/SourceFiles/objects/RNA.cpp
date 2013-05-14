#include "../../HeaderFiles/objects/RNA.h"
/** @addtogroup CRNA
 *  @{
 */

/** A  static data set stored RNA symbols.  */
CRNASymbol CRNA::RNASymbols[RNASYMBOLNUMBER] = {
	{0, 'A', "Adenine"}, {1, 'C', "Cytosine"}, {2, 'G', "Guanie"}, {3, 'T', "Thymine"},
	{4, 'R', "Purine"}, {5, 'Y', "Pyrimidine"}, {6, 'M', "C or A"}, {7, 'K', "T, U, or G"},
	{8, 'W', "T, U or A"}, {9, 'S', "C or G"}, {10, 'G', "not A"}, {11, 'D', "not C"},
	{12, 'H', "not G"}, {13, 'V', "not T, U"}, {14, 'N', "Any base"}
};

/**
 * A constructor
 *  This constructor initial RNA sequence.
 *  @param string sid          Identity string of this RNA sequence.
 *  @param string sequence     Sequence of this RNA.
 *  Details:
 *  - The first string parameter will be assigned to _sequenceid.
 *  - The second string parameter will be assigned to _sequence.
 *  - Property _size will be assigned by _sequence's size.
 *  - Each characters in sequence will translate into symbol id and stored in _symbolIDs.
*/
CRNA::CRNA(string sid, string sequence)
{
    this->_sequenceid = sid;
    this->_sequence = sequence;
    int i;
	int temp;
	this->_size = this->_sequence.size();
	for(i=0;i<this->_size;i++){
		temp = CRNA::getSymbolID(this->_sequence.at(i));
		this->_symbolIDs.push_back(temp);
	}
}

/** a destructor, do nothing.  */
CRNA::~CRNA()
{

}

/**
 * @override
 * Return the size of RNA sequence.
*/
int CRNA::getSize()const{
	return this->_size;
}

/**
 * Return a integer vector contain symbols of RNA sequence.
*/
vector<int> CRNA::getSymbolIDs()const{
	return this->_symbolIDs;
}

/**
 * A static function to return symbol's ID.
 * Get the symbol's id according to input symbol character.
 * This function required a input param which is existed in RNASymbol data set.
 * @param char symbol    A symbol waiting to get its ID.
 * @return int           A int ID which is stand for input char.
*/
int CRNA::getSymbolID(char symbol)
{
    int i;
    for(i=0;i<RNASYMBOLNUMBER;i++)
        if(symbol == RNASymbols[i].abbr)
            return RNASymbols[i].sid;
	return -1;
}


/**
 * A static function to load data from a file.
 * This function will get data from a format file, which contain some RNA informations,
 *  and then save as a RNA type and store in a vector.
 * Details:
 *  - Firstly, load each RNA sequence according to the characters from the file,
 *    the char '>' in the format files, stand for the beginning of RNA,
 *    if the length of total characters more than maxSize the function will
 *    stop loadding.
 *  - Then, split RNA sequenct into many pieces, each piece's length is fragmentLength.
 *  - Finally, save all pieces in a vector, and return this vector.
 * A object definded shared_ptr<T>(a kind of smart pointer, existed in <memory>) will count
 *  how much pointer point to it, and this counting called reference counting.
 *  When reference counting reduces to 0, this object will destory automatically.
 * By using shared_ptr<T>, we can ensure no memory leak.
 * @return vector<shared_ptr<CRNA>>
 *   - return a vector stored RNA fragments.
 * 
*/
vector<CIndexObject*>* CRNA::loadData(string filename, int maxSize, int fragmentLength){
	
	vector<CIndexObject*> data;
	vector<CRNA*> rnas;
	
	ifstream infile(filename, ios::in);
	
	if(!infile.is_open()){
		cerr << "Stop! Cannot open the file." << endl;
		return &data;
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
					CRNA* temp = new CRNA(ident, currentSequence);
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
		CRNA* temp = new CRNA(ident, currentSequence);
		rnas.push_back(temp);
	}
	for(i=0;i<rnas.size();i++){
		CRNA* rna = rnas[i];
		int totalSize = rna->getSize() - fragmentLength;
		string seq_id = rna->_sequenceid;
		string seq = rna->_sequence;
		for(int j=0;j<totalSize-1;j++){
			CRNA* temp = new CRNA(seq_id, seq.substr(j, fragmentLength));
			data.push_back(temp);
		}
	}
	return &data;
}

/**
 * @}  //CRNA
*/