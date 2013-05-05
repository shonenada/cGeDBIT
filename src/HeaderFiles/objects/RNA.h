#ifndef SQAI_OBJECTS_RNA_H
#define SQAI_OBJECTS_RNA_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>

#include "IndexObject.h"

using namespace std;

/** The total number of RNA symbols.  */
#define RNASYMBOLNUMBER 15

/**
 * @defgroup CRNASymbol
 * @author   Lyd., Wuxing Feng.
 * @version  2013/3/18
 * @date     2012/3/18
 * @{
 * 
 * This is a struct defined for RNA symbols.
 *  - sid            The unique identity of each RNA symbols.
 *  - abbr           Abbreviation of each symbols.
 *  - description    Full name of each symbols.
*/
struct CRNASymbol{

    /** Identity number of the symbol  */
    int sid;

    /** Abbreviation of the symbol  */
    char abbr;

    /** Description of symbol  */
    string description;
};
/**
 * @}   // CRNASymbol
*/


/**
 * @defgroup CRNA
 * @author   Lyd. (shonenada at gmail dot com), Wuxing Feng.
 * @version  2013/3/18
 * @date     2012/3/18
 * @{
 *
 * The CRNA class define a RNA tpye to contain RNA information.
 *  - the method loadData will load data from a file.
*/
class CRNA:
    public CIndexObject
{
private:
    /** Record the identity of RNA sequence.  */
    string _sequenceid;

    /** The sequence of the RNA.  */
    string _sequence;

    /** A integer list stored identity numbers of this RNA sequence's symbol.  */
    vector<int> _symbolIDs;

    /** Length of this RNA sequence.  */
    int _size;


public:

    /** A  static data set stored RNA symbols.  */
    static CRNASymbol RNASymbols[RNASYMBOLNUMBER];

    /**
     * A constructor
     *  This constructor initial RNA sequence.
     *  @param string sid          Identity string of this RNA sequence.
     *  @param string sequence     Sequence of this RNA.
     *  Details:
     *  - The first string parameter will be assigned to _sequenceid.
     *  - The second string parameter will be assigned to _sequence.
     *  - Property _size will be assigned by _sequence's length.
     *  - Each characters in sequence will translate into symbol id and stored in _symbolIDs.
    */
    CRNA(string sid, string sequence);

    /** a destructor, do nothing.  */
    ~CRNA();

    /**
     * @override
     * Return the length of this RNA sequence.
    */
    virtual int getSize()const;

    /**
     * Return a integer vector contain symbols of this RNA sequence.
    */
     vector<int> getSymbolIDs()const;

    /**
     * A static function to return symbol's ID.
     * Get the symbol's id according to input symbol character.
     * This function required a input param which is existed in RNASymbol data set.
     * @param char symbol    A symbol waiting to get its ID.
     * @return int           Return a int ID which is stand for input char.
    */
    static int getSymbolID(char symbol);
    
    /**
     * A static function to load data from a file.
     * This function will get data from a format file, which contain some RNA informations,
     *  and then save as a CRNA type and store in a vector.
     * Details:
     *  - Firstly, load each RNA sequence according to the characters from the file,
     *    the char '>' in the format files, stand for the beginning of RNA,
     *    if the length of total characters more than maxSize the function will
     *    stop loadding.
     *  - Then, split RNA sequenct into many pieces, each piece's length is fragmentLength.
     *  - Finally, save all pieces in a vector, and return this vector.
     * A object defined shared_ptr<T>(a kind of smart pointer, existed in <memory>) will count
     *  how much pointer point to it, and this counting called reference counting.
     *  When reference counting reduces to 0, this object will destory automatically.
     * By using shared_ptr<T>, we can ensure no memory leak.
     * @return vector<shared_ptr<CRNA>>
     *   - return a vector stored RNA fragments.
     * 
    */
    static vector<shared_ptr<CRNA>> loadData(string filename, int maxSize, int fragmentLength);

};
/**
 * @}   // CRNA
*/
#endif
//SQAI_OBJECTS_RNA_H
