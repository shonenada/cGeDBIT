#ifndef SQAI_OBJECTS_DNA_H
#define SQAI_OBJECTS_DNA_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "IndexObject.h"

using namespace std;

/** The total number of DNA symbols.  */
#define DNASYMBOLNUMBER 15

/**
 * @defgroup CDNASymbol
 * @author   Lyd.
 * @version  2013/3/18
 * @date     2012/3/18
 * @{
 * 
 * This is a struct defined for DNA symbols.
 *  - sid            The unique identity of each DNA symbols.
 *  - abbr           Abbreviation of each symbols.
 *  - description    Full name of each symbols.
*/
struct CDNASymbol{

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
 * @defgroup CDNA
 * @author   Lyd. (shonenada at gmail dot com)
 * @version  2013/3/18
 * @date     2012/3/18
 * @{
 *
 * The CDNA class define a DNA tpye to contain DNA information.
 *  - the method loadData will load data from a file.
*/
class CDNA:
    public CIndexObject
{
private:
    /** Record the identity of DNA sequence.  */
    string _sequenceid;

    /** The sequence of the DNA.  */
    string _sequence;

    /** A integer list stored identity numbers of this DNA sequence's symbol.  */
    vector<int> _symbolIDs;

    /** Length of this DNA sequence.  */
    int _size;


public:

    /** A  static data set stored DNA symbols.  */
    static CDNASymbol DNASymbols[DNASYMBOLNUMBER];

    /**
     * A constructor
     *  This constructor initial DNA sequence.
     *  @param string sid          Identity string of this DNA sequence.
     *  @param string sequence     Sequence of this DNA.
     *  Details:
     *  - The first string parameter will be assigned to _sequenceid.
     *  - The second string parameter will be assigned to _sequence.
     *  - Property _size will be assigned by _sequence's length.
     *  - Each characters in sequence will translate into symbol id and stored in _symbolIDs.
    */
    CDNA(string sid, string sequence);

    /** a destructor, do nothing.  */
    ~CDNA();

    /**
     * @override
     * Return the length of this DNA sequence.
    */
    virtual int getSize()const;

    /**
     * Return a integer vector contain symbols of this DNA sequence.
    */
     vector<int> getSymbolIDs()const;

    /**
     * A static function to return symbol's ID.
     * Get the symbol's id according to input symbol character.
     * This function required a input param which is existed in DNASymbol data set.
     * @param char symbol    A symbol waiting to get its ID.
     * @return int           Return a int ID which is stand for input char.
    */
    static int getSymbolID(char symbol);
    
    /**
     * A static function to load data from a file.
     * This function will get data from a format file, which contain some DNA informations,
     *  and then save as a CDNA type and store in a vector.
     * Details:
     *  - Firstly, load each DNA sequence according to the characters from the file,
     *    the char '>' in the format files, stand for the beginning of DNA,
     *    if the length of total characters more than maxSize the function will
     *    stop loadding.
     *  - Then, split DNA sequenct into many pieces, each piece's length is fragmentLength.
     *  - Finally, save all pieces in a vector, and return this vector.
     * A object defined shared_ptr<T>(a kind of smart pointer, existed in <memory>) will count
     *  how much pointer point to it, and this counting called reference counting.
     *  When reference counting reduces to 0, this object will destory automatically.
     * By using shared_ptr<T>, we can ensure no memory leak.
     * @return vector<shared_ptr<CDNA>>
     *   - return a vector stored DNA fragments.
     * 
    */
    static vector<shared_ptr<CDNA>> loadData(string filename, int maxSize, int fragmentLength);

};
/**
 * @}   // CDNA
*/
#endif
//SQAI_OBJECTS_DNA_H
