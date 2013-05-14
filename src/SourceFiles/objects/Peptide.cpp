#include "../../HeaderFiles/objects/Peptide.h"

/** @addtogroup CPeptide
 *  @{
 */

/** A  static data set stored peptide symbols.  */
CPeptideSymbol CPeptide::PeptideSymbols[PeptideSYMBOLNUMBER]=
{
	{0,'A',"Alanine"},
	{1,'R', "Arginine"}, 
	{2,'N', "Asparagine"}, 
	{3,'D', "Aspartic acid"}, 
	{4,'C', "Cysteine"}, 
	{5,'Q', "Glutamine"}, 
	{6,'E', "Glutamic acid"}, 
	{7,'G',"Glycine"}, 
	{8,'H', "Histidine"}, 
	{9,'I',"Isoleucine"}, 
	{10,'L',"Leucine"}, 
	{11,'K',"Lysine"}, 
	{12,'M', "Methionine"}, 
	{13,'F', "Phenylalanine"},
	{14,'P', "Proline"}, 
	{15,'S',"Serine"}, 
	{16,'T', "Threonine"}, 
	{17,'W', "Tryptophan"}, 
	{18,'Y', "Tyrosine"}, 
	{19,'V',"Valine"}, 
	{20,'B', "Unknown"}, 
	{21,'Z',"Unknown"}, 
	{22,'U', "Unknown"}, 
	{23,'X', "Unknown"}
};

/**
 * A constructor
 *  This constructor initial peptide sequence.
 *  @param string sid          Identity string of this peptide sequence.
 *  @param string sequence     Sequence of this peptide.
 *  Details:
 *  - The first string parameter will be assigned to _sequenceid.
 *  - The second string parameter will be assigned to _sequence.
 *  - Property _size will be assigned by _sequence's size.
 *  - Each characters in sequence will translate into symbol id and stored in _symbolIDs.
*/
CPeptide::CPeptide(string sid, string sequence)
{
    this->_sequenceid = sid;
    this->_sequence = sequence;
    int i;
	int temp;
	this->_size = this->_sequence.size();
	for(i=0;i<this->_size;i++)
	{
		temp = CPeptide::getSymbolID(this->_sequence.at(i));
		this->_symbolIDs.push_back(temp);
	}
}

/** a no parameter construction, do nothing.  */
CPeptide::CPeptide()
{
}

/** a destructor, do nothing.  */
CPeptide::~CPeptide()
{
}

/**
 * @override
 * Return the size of peptide sequence.
*/
int CPeptide::getSize()const
{
	return this->_size;
}

/**
 * Return a integer vector contain symbols of peptide sequence.
*/
vector<int> CPeptide::getSymbolIDs()const
{
	return this->_symbolIDs;
}

/**
 * A static function to return symbol's ID.
 * Get the symbol's id according to input symbol character.
 * This function required a input param which is existed in PeptideSymbol data set.
 * @param char symbol    A symbol waiting to get its ID.
 * @return int           A int ID which is stand for input char.
*/
int CPeptide::getSymbolID(char symbol)
{
    int i;
    for(i=0;i<PeptideSYMBOLNUMBER;i++)
        if(symbol == PeptideSymbols[i].abbr)
            return PeptideSymbols[i].sid;
	return -1;
}

/**
 * A static function to load data from a file.
 * This function will get data from a format file, which contain some peptide informations,
 *  and then save as a peptide type and store in a vector.
 * Details:
 *  - Firstly, load each peptide sequence according to the characters from the file,
 *    the char '>' in the format files, stand for the beginning of peptide,
 *    if the length of total characters more than maxSize the function will
 *    stop loadding.
 *  - Then, split peptide sequenct into many pieces, each piece's length is fragmentLength.
 *  - Finally, save all pieces in a vector, and return this vector.
 * A object definded shared_ptr<T>(a kind of smart pointer, existed in <memory>) will count
 *  how much pointer point to it, and this counting called reference counting.
 *  When reference counting reduces to 0, this object will destory automatically.
 * By using shared_ptr<T>, we can ensure no memory leak.
 * @return vector<shared_ptr<CPeptide>>
 *   - return a vector stored Peptide fragments.
 * 
*/
vector<shared_ptr<CPeptide>> CPeptide::loadData(string filename, int maxSize, int fragmentLength)
{
	vector<shared_ptr<CPeptide>> data;
	 
	ifstream infile(filename, ios::in);
	vector<shared_ptr<CPeptide>> peptides;
	
	if(!infile.is_open())
	{
		cerr << "Stop! Cannot open the file." << endl;
		return data;
	}

	string ident = "";
	int i;
	int counter = 0;
	int sequenceLengthCounter = 0;
	string currentSequence;
	string line;
	char buffer[36000];
	
	while(!infile.eof() && counter < maxSize && sequenceLengthCounter < maxSize)
	{
		infile.getline(buffer, 36000);
		line = string(buffer);
		if(line.size() >= 1)
		{
			if(line.at(0) == '>')
			{	
				if(currentSequence.size() > 0)
				{
					shared_ptr<CPeptide> temp(new CPeptide(ident, currentSequence));
					peptides.push_back(temp);
					counter += currentSequence.size();
					currentSequence = "";
				}
				ident = line;
			}
			else
			{
				currentSequence = currentSequence + line;
				sequenceLengthCounter = currentSequence.size();
			}
		}
	}
	
	if(currentSequence.size() > 0)
	{
		shared_ptr<CPeptide> temp(new CPeptide(ident, currentSequence)); 
		peptides.push_back(temp);
	}
	for(i=0;i<peptides.size();i++)
	{
		CPeptide* peptide = peptides[i].get();
		int totalSize = peptide->getSize() - fragmentLength;
		string seq_id = peptide->_sequenceid;
		string seq = peptide->_sequence; 
		for(int j=0;j<totalSize;j++)
		{
			shared_ptr<CPeptide> temp(new CPeptide(seq_id , seq.substr(j,fragmentLength)));
			data.push_back(temp);
		}
	}
	return data;
}
/**
 * @}  //CPeptide
*/