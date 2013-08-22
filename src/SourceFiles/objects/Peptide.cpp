/**@file Peptide.h
 * @brief This is a Index object of Peptide.
 * @author Yadi Cai
 * @date 2013/5/30
 *
 * This class define Peptide index object. It load data from a format file.
*/
#include "../../HeaderFiles/objects/Peptide.h"

/** A  static data set stored peptide symbols.  */
CPeptideSymbol CPeptide::PeptideSymbols[PeptideSYMBOLNUMBER]=
{
    {0, 'A', "Alanine"},
    {1, 'R', "Arginine"}, 
    {2, 'N', "Asparagine"}, 
    {3, 'D', "Aspartic acid"}, 
    {4, 'C', "Cysteine"}, 
    {5, 'Q', "Glutamine"}, 
    {6, 'E', "Glutamic acid"}, 
    {7, 'G', "Glycine"}, 
    {8, 'H', "Histidine"}, 
    {9, 'I', "Isoleucine"}, 
    {10, 'L', "Leucine"}, 
    {11, 'K', "Lysine"}, 
    {12, 'M', "Methionine"}, 
    {13, 'F', "Phenylalanine"},
    {14, 'P', "Proline"}, 
    {15, 'S', "Serine"}, 
    {16, 'T', "Threonine"}, 
    {17, 'W', "Tryptophan"}, 
    {18, 'Y', "Tyrosine"}, 
    {19, 'V', "Valine"}, 
    {20, 'B', "Unknown"}, 
    {20, 'Z', "Unknown"}, 
    {20, 'U', "Unknown"}, 
    {20, 'X', "Unknown"}
};

/**
 * @brief This constructor initial Peptide sequence.
 * @param sid          Identity string of this Peptide sequence.
 * @param sequence     Sequence of this Peptide.
 *
 * @note
 * The first string parameter will be assigned to _sequenceid.
 * The second string parameter will be assigned to _sequence.
 * Property _size will be assigned by _sequence's length.
 * Each characters in sequence will translate into symbol id and stored in _symbolIDs.
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

/** 
 * A destructor, do nothing. 
 */
CPeptide::CPeptide()
{
}

/*
 * a destructor, do nothing.  
 */
CPeptide::~CPeptide()
{
}

/**
 * @return Return the size of Peptide sequence.
*/
int CPeptide::getSize()const
{
    return this->_size;
}

/**
 * @brief  Get the sysbolID list.
 * @return Return a integer vector contain symbols of Peptide sequence.
*/
vector<int> CPeptide::getSymbolIDs()const
{
    return this->_symbolIDs;
}

/**
 * @brief A static function to return symbol's ID.
 *        Get the symbol's id according to input symbol character.
 *        This function required a input param which is existed in PeptideSymbol data set.
 * @param symbol    A symbol waiting to get its ID.
 * @return  Return a int ID which is stand for input char.
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
 * @brief    A static function to load data from a file.
 * This function will get data from a format file, which contain some Peptide informations,
 *  and then save as a CPeptide type and store in a vector.
 *
 * @note
 * Firstly, load each Peptide sequence according to the characters from the file,
 *  the char '>' in the format files, stand for the beginning of Peptide,
 *  if the length of total characters more than maxSize the function will
 *  stop loadding.
 * Then, split Peptide sequenct into many pieces, each piece's length is fragmentLength.
 * Finally, save all pieces in a vector, and return this vector.
 * A object defined shared_ptr<T>(a kind of smart pointer, existed in <memory>) will count
 *  how much pointer point to it, and this counting called reference counting.
 *  When reference counting reduces to 0, this object will destory automatically.
 * By using shared_ptr<T>, we can ensure no memory leak.
 * 
 * @param filename           The file path of input data.
 * @param maxSize            The max size of loaded data.
 * @param fragmentLength     The length of each Peptide fragment.
 * @return return a vector stored Peptide fragments.
 */
vector<shared_ptr<CIndexObject> >* CPeptide::loadData(string filename, int maxSize, int fragmentLength)
{
    vector<shared_ptr<CIndexObject> >* data = new vector<shared_ptr<CIndexObject> >;
    vector<shared_ptr<CPeptide> > peptides;
     
    ifstream infile(filename, ios::in);
    
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
                if(currentSequence.size() != 0)
                {
                    shared_ptr<CPeptide> temp(new CPeptide(ident, currentSequence));
                    peptides.push_back(temp);
                    counter += currentSequence.size();
                    currentSequence.clear();
                }
                ident = line;
            }
            else
            {
                currentSequence = currentSequence.append(line);
                sequenceLengthCounter = currentSequence.size();
            }
        }
    }
    
    if(currentSequence.size() != 0)
    {
        shared_ptr<CPeptide> temp(new CPeptide(ident, currentSequence));
        peptides.push_back(temp);
    }
    for(i=0;i<peptides.size();i++)
    {
        shared_ptr<CPeptide> peptide = peptides.at(i);
        int totalSize = (peptide->getSize() - fragmentLength) > maxSize ? maxSize : (peptide->getSize() - fragmentLength);
        string seq_id = peptide->_sequenceid;
        string seq = peptide->_sequence; 
        for(int j=0;j<totalSize;j++)
        {
            shared_ptr<CPeptide> temp(new CPeptide(seq_id , seq.substr(j,fragmentLength)));
            data->push_back(temp);
        }
    }
    
    if (data->size() > maxSize){
        data->resize(maxSize);
    }
    
    return data;
}


/**
 * @brief return the name of a instance of this class
 * @return return the name of a instance of this class
*/
CreateFuntion CPeptide::getConstructor()
{
    return &CreateInstance;
}

void* CPeptide::CreateInstance()
{
    return new CPeptide();
}


/**
 * @brief Write this CPeptide instance into the hard disk.
 * The format of file:
 *  - The first line is an integer show how long the sequenceid is.
    - The second line is sequenceid.
    - The third line is the size of sequence.
    - The forth line is sequence.
    - The fifth lines is the data of sysbom IDs.
 * @param A file output stream, which used to write the information of this instance into hard disk.
 * @return return an integer refers to the size of information has been wrote into the hard disk.
 */
 int CPeptide::writeExternal(ofstream &out){
    int size = 0;
    int temp;

    temp = _sequenceid.size() + 1;
    out.write((char*) (&(temp)), sizeof(int));
    size += sizeof(int);

    out.write((char*) (_sequenceid.c_str()), temp * sizeof(char));
    size += (temp+1) * sizeof(char);

    temp = _sequence.size() + 1;
    out.write((char*) (&(temp)), sizeof(int));
    size += sizeof(int);

    out.write((char*) (_sequence.c_str()), temp * sizeof(char));
    size += sizeof(char) * (_size+1);

    temp = _symbolIDs.size();
    out.write((char*) (&(temp)), sizeof(int));
    size += temp;

    for (int i=0; i<_symbolIDs.size(); ++i){
        out.write((char*) ((&_symbolIDs.at(i))), sizeof(int));
        size += sizeof(int);
    }

    return size;
 }

/**
 * @brief Read this CPeptide instance from the hard disk.
 * The format of file:
 *  - The first line is an integer show how long the sequenceid is.
    - The second line is sequenceid.
    - The third line is the size of sequence.
    - The forth line is sequence.
    - The fifth lines is the data of sysbom IDs.
 * @param A file input stream, which used to read the information from hard disk.
 * @return return an integer refers to the size of information has been read from the hard disk.
 */
 int CPeptide::readExternal(ifstream &in){

    _sequence.clear();
    _sequenceid.clear();
    _symbolIDs.clear();

    int size = 0;
    int tempSize;
    int tempSymbol;

    in.read((char*) (&tempSize), sizeof(int));
    size += sizeof(int);

    char* tempSequenceid = new char[tempSize];
    in.read(tempSequenceid, sizeof(char) * tempSize);
    size += sizeof(char) * tempSize;
    _sequenceid = string(tempSequenceid);
    delete [] tempSequenceid;

    in.read((char*) (&tempSize), sizeof(int));
    size += sizeof(int);

    char* tempSequence = new char[tempSize];
    in.read(tempSequence, sizeof(char) * tempSize);
    size += sizeof(char) * tempSize;
    _sequence = string(tempSequence);
    _size = _sequence.size();
    delete [] tempSequence;

    in.read((char*) (&tempSize), sizeof(int));
    size += sizeof(int);

    for (int i=0; i<tempSize; ++i){
        in.read((char*) (&tempSymbol), sizeof(int));
        _symbolIDs.push_back(tempSymbol);
        size += sizeof(int);
    }

    return size;
 }


 string CPeptide::getSequence(){
     return this->_sequence;
 }