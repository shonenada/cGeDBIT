#include "../../HeaderFiles/objects/Peptide.h"
/** @addtogroup CPeptide
 *  @{
 */

/** A  static data set stored Peptide symbols.  */
CAminoAcid CPeptide::AminoAcids[AMINOACIDNUMBER] = {
	{0, 'A', "Ala", "Alanine"}, {1, 'R', "Arg", "Arginine"},
	{2, 'N', "Asn", "Asparagine"}, {3, 'D', "Aspartic acid"},
	{4, 'C', "Cys", "Cysteine"}, {5, 'Q', "Gln", "Glutamine"},
	{6, 'E', "Glu", "Glutamic acid"}, {7, 'G', "Gly", "Glycine"},
	{8, 'H', "His", "Histidine"}, {9, 'I', "Ile", "Isoleucine"},
	{10, 'L', "Leu", "Leucine"}, {11, 'K', "Lys", "Lysine"},
	{12, 'M', "Met", "Methionine"}, {13, 'F', "Phe", "Phenylalanine"},
	{14, 'P', "Pro", "Proline"}, {15, 'S', "Ser", "Serine"},
	{16, 'T', "Thr", "Threonine"}, {17, 'W', "Trp", "Tryptophan"},
	{18, 'Y', "Tyr", "Tyrosine"}, {19, 'V', "Val", "Valine"},
	{20, 'U', "Unk", "Unknown"}, {20, 'Z', "Unk", "Unknown"},
	{20, 'X', "Unk", "Unknown"}
};

/**
 * A constructor
 *  This constructor initial Peptide sequence.
 *  @param string sid          Identity string of this Peptide sequence.
 *  @param string sequence     Sequence of this Peptide.
 *  Details:
 *  - The first string parameter will be assigned to _sequenceid.
 *  - The second string parameter will be assigned to _sequence.
 *  - Property _size will be assigned by _sequence's length.
 *  - Each characters in sequence will translate into amino acid id
 *    and stored in _AminoAcidIDs.
*/
CPeptide::CPeptide(string sid, string sequence)
{
    this->_sequenceid = sid;
    this->_sequence = sequence;
    int i;
	int temp;
	this->_size = this->_sequence.size();
	for(i=0;i<this->_size;i++){
		temp = CPeptide::getAminoAcidID(this->_sequence.at(i));
		this->_AminoAcidIDs.push_back(temp);
	}
}

/**
 * @override
 * Return the length of this Peptide sequence.
*/
int CPeptide::getSize()const{
	return this->_size;
}

/**
 * Return a integer vector contain amino acid id of this Peptide sequence.
*/
vector<int> CPeptide::getAminoAcidIDs()const{
	return this->_AminoAcidIDs;
}

/**
 * A static method to return amino acid's ID.
 * Get the amino acid's id according to input amino acid character.
 * This method required a input param which is existed in AminoAcids data set.
 * @param char AminoAcid    A amino acid waiting to get its ID.
 * @return int              Return a int ID which is stand for input char.
*/
int CPeptide::getAminoAcidID(char AminoAcid)
{
    int i;
    for(i=0;i<AMINOACIDNUMBER;i++)
        if(AminoAcid == AminoAcids[i].abbr)
            return AminoAcids[i].sid;
	return -1;
}


/**
 * A static method to load data from a file.
 * This method will get data from a format file, which contain some Peptide
 * informations, and then save as a CPeptide type and store in a vector.
 * Details:
 *  - Firstly, load each peptide sequence according to the characters from the file,
 *    the char '>' in the format files, stand for the beginning of Peptide,
 *    if the length of total characters more than maxSize the method will
 *    stop loadding.
 *  - Then, split Peptide sequenct into many pieces, each piece's length is fragmentLength.
 *  - Finally, save all pieces in a vector, and return this vector.
 * A object definded shared_ptr<T>(a kind of smart pointer, existed in <memory>) will count
 *  how much pointer point to it, and this counting called reference counting.
 *  When reference counting reduces to 0, this object will destory automatically.
 * By using shared_ptr<T>, we can ensure no memory leak.
 * @return vector<shared_ptr<CPeptide>>
 *   - return a vector stored Peptide fragments.
 * 
*/
vector<shared_ptr<CPeptide>> CPeptide::loadData(string filename, int maxSize, int fragmentLength){
	vector<shared_ptr<CPeptide>> data;
	
	ifstream infile(filename, ios::in);
	vector<shared_ptr<CPeptide>> peptides;
	
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
	char buffer[36000];	// max length of format line is 35707

	while(!infile.eof() && counter < maxSize && sequenceLengthCounter < maxSize)
	{
		infile.getline(buffer, 36000);
		line = string(buffer);
		if(line.size() >= 1){
			if(line.at(0) == '>'){
				if(currentSequence.size() > 0)
				{
					shared_ptr<CPeptide> temp(new CPeptide(ident, currentSequence));
					peptides.push_back(temp);
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
		shared_ptr<CPeptide> temp(new CPeptide(ident, currentSequence));
		peptides.push_back(temp);
	}
	for(i=0;i<peptides.size();i++){
		CPeptide* peptide = peptides[i].get();
		int totalSize = peptide->getSize() - fragmentLength;
		string seq_id = peptide->_sequenceid;
		string seq = peptide->_sequence;
		for(int j=0;j<totalSize;j++){
			shared_ptr<CPeptide> temp(new CPeptide(seq_id, seq.substr(j, fragmentLength)));
			data.push_back(temp);
		}
	}
	return data;
}

/**
 * @}  //CPeptide
*/