#include "../../HeaderFiles/objects/DNA.h"
/** @addtogroup CDNA
 *  @{
 */

/** A  static data set stored DNA symbols.  */
CDNASymbol CDNA::DNASymbols[DNASYMBOLNUMBER] = {
	{0, 'A', "Adenine"}, {1, 'C', "Cytosine"}, {2, 'G', "Guanie"}, {3, 'T', "Thymine"},
	{4, 'R', "Purine"}, {5, 'Y', "Pyrimidine"}, {6, 'M', "C or A"}, {7, 'K', "T, U, or G"},
	{8, 'W', "T, U or A"}, {9, 'S', "C or G"}, {10, 'G', "not A"}, {11, 'D', "not C"},
	{12, 'H', "not G"}, {13, 'V', "not T, U"}, {14, 'N', "Any base"}
};

/**
 * A constructor
 *  This constructor initial DNA sequence.
 *  @param string sid          Identity string of this DNA sequence.
 *  @param string sequence     Sequence of this DNA.
 *  Details:
 *  - The first string parameter will be assigned to _sequenceid.
 *  - The second string parameter will be assigned to _sequence.
 *  - Property _size will be assigned by _sequence's size.
 *  - Each characters in sequence will translate into symbol id and stored in _symbolIDs.
*/
CDNA::CDNA(string sid, string sequence)
{
    this->_sequenceid = sid;
    this->_sequence = sequence;
    int i;
	int temp;
	this->_size = this->_sequence.size();
	for(i=0;i<this->_size;i++){
		temp = CDNA::getSymbolID(this->_sequence.at(i));
		this->_symbolIDs.push_back(temp);
	}
}

/** a destructor, do nothing.  */
CDNA::~CDNA()
{

}

/**
 * @override
 * Return the size of DNA sequence.
*/
int CDNA::getSize()const{
	return this->_size;
}

/**
 * Return a integer vector contain symbols of DNA sequence.
*/
vector<int> CDNA::getSymbolIDs()const{
	return this->_symbolIDs;
}

/**
 * A static function to return symbol's ID.
 * Get the symbol's id according to input symbol character.
 * This function required a input param which is existed in DNASymbol data set.
 * @param char symbol    A symbol waiting to get its ID.
 * @return int           A int ID which is stand for input char.
*/
int CDNA::getSymbolID(char symbol)
{
    int i;
    for(i=0;i<DNASYMBOLNUMBER;i++)
        if(symbol == DNASymbols[i].abbr)
            return DNASymbols[i].sid;
	return -1;
}


/**
 * A static function to load data from a file.
 * This function will get data from a format file, which contain some DNA informations,
 *  and then save as a DNA type and store in a vector.
 * Details:
 *  - Firstly, load each DNA sequence according to the characters from the file,
 *    the char '>' in the format files, stand for the beginning of DNA,
 *    if the length of total characters more than maxSize the function will
 *    stop loadding.
 *  - Then, split DNA sequenct into many pieces, each piece's length is fragmentLength.
 *  - Finally, save all pieces in a vector, and return this vector.
 * A object definded shared_ptr<T>(a kind of smart pointer, existed in <memory>) will count
 *  how much pointer point to it, and this counting called reference counting.
 *  When reference counting reduces to 0, this object will destory automatically.
 * By using shared_ptr<T>, we can ensure no memory leak.
 * @return vector<shared_ptr<CDNA>>
 *   - return a vector stored DNA fragments.
 * 
*/
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
	for(i=0;i<dnas.size();i++){
		CDNA* dna = dnas[i].get();
		int totalSize = dna->getSize() - fragmentLength;
		string seq_id = dna->_sequenceid;
		string seq = dna->_sequence;
		for(int j=0;j<totalSize;j++){
			shared_ptr<CDNA> temp(new CDNA(seq_id, seq.substr(j, fragmentLength)));
			data.push_back(temp);
		}
	}
	return data;
}

/**
 * @}  //CDNA
*/