#ifndef Peptide_H
#define Peptide_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>

#include "IndexObject.h"

using namespace std;

/** The total number of Peptide symbols.  */
#define PeptideSYMBOLNUMBER 24

/**
 * @defgroup CPeptideSymbol
 * @author   Yadi Cai.
 * @version  2013/4/18
 * @date     2012/4/18
 * @{
 * 
 * This is a struct defined for DNA symbols.
 *  - sid            The unique identity of each DNA symbols.
 *  - abbr           Abbreviation of each symbols.
 *  - description    Full name of each symbols.
*/
struct CPeptideSymbol
{
    /** Identity number of the symbol  */
    int sid;
    /** Abbreviation of the symbol  */
    char abbr;
	/** Description of symbol  */
    string description;
};
/**
 * @}   // CDNASymbol
*/

/**
 * @defgroup CPeptide
 * @author   Yadi Cai.
 * @version  2013/4/18
 * @date     2012/4/18
 * @{
 *
 * The CPeptide class define a DNA tpye to contain DNA information.
 *  - the method loadData will load data from a file.
*/
class CPeptide:
    public CIndexObject
{
	private:
    /** Record the identity of Peptide sequence.  */
    string _sequenceid;

    /** The sequence of the Peptide.  */
    string _sequence;

    /** A integer list stored identity numbers of this Peptide sequence's symbol.  */
    vector<int> _symbolIDs;

    /** Length of this Peptide sequence.  */
    int _size;

public:
    /** A  static data set stored Peptide symbols.  */
    static CPeptideSymbol PeptideSymbols[PeptideSYMBOLNUMBER];

    /**
     * A constructor
     *  This constructor initial DNA sequence.
     *  @param string sid          Identity string of this Peptide sequence.
     *  @param string sequence     Sequence of this Peptide.
     *  Details:
     *  - The first string parameter will be assigned to _sequenceid.
     *  - The second string parameter will be assigned to _sequence.
     *  - Property _size will be assigned by _sequence's length.
     *  - Each characters in sequence will translate into symbol id and stored in _symbolIDs.
    */
    CPeptide(string sid, string sequence);
    
    /**
     * @override
     * Return the length of this Peptide sequence.
    */
    virtual int getSize()const;

	/**
     * Return a integer vector contain symbols of this Peptide sequence.
    */
	vector<int> getSymbolIDs() const;
    
	/**
     * A static function to return symbol's ID.
     * Get the symbol's id according to input symbol character.
     * This function required a input param which is existed in PeptideSymbol data set.
     * @param char symbol    A symbol waiting to get its ID.
     * @return int           Return a int ID which is stand for input char.
    */
     static int getSymbolID(char symbol);

    /**
     * A static function to load data from a file.
     * This function will get data from a format file, which contain some Peptide informations,
     *  and then save as a CPeptide type and store in a vector.
     * Details:
     *  - Firstly, load each Peptide sequence according to the characters from the file,
     *    the char '>' in the format files, stand for the beginning of Peptide,
     *    if the length of total characters more than maxSize the function will
     *    stop loadding.
     *  - Then, split Peptide sequenct into many pieces, each piece's length is fragmentLength.
     *  - Finally, save all pieces in a vector, and return this vector.
     * A object defined shared_ptr<T>(a kind of smart pointer, existed in <memory>) will count
     *  how much pointer point to it, and this counting called reference counting.
     *  When reference counting reduces to 0, this object will destory automatically.
     * By using shared_ptr<T>, we can ensure no memory leak.
     * @return vector<shared_ptr<CPeptide>>
     *   - return a vector stored Peptide fragments.
     * 
    */
    static vector<shared_ptr<CPeptide>> loadData(string filename, int maxSize, int fragmentLength);
	
	/** a no parameter constructor, do nothing.  */
	CPeptide();

	/** a destructor, do nothing.  */
	~CPeptide();
};
/**
 * @}   // CPeptide
*/
#endif
//SQAI_OBJECTS_Peptide_H