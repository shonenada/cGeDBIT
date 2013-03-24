#ifndef SQAI_OBJECTS_PEPTIDE_H
#define SQAI_OBJECTS_PEPTIDE_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>

#include "IndexObject.h"

using namespace std;

/** The total number of amino acids.  */
#define AMINOACIDNUMBER 23

/*
*  @defgroup CAminoAcid
*  @author   Lyd.
*  @version  2013/3/18
*  @date     2012/3/18
*  @{
*/
    
/**
 * A struct of Amino Acid
*/
struct CAminoAcid{

    /** Identity number of the amino acid.  */
    int sid;

    /** Abbreviation of the amino acid  */
    char abbr;

    /** Short description of amino acid  */
    string shortDescription;

    /** Description of amino acids  */
    string description;
};
/**
 * @}   // CAminoAcid
*/


/*
*  @defgroup CPeptide
*  @author   Lyd.
*  @version  2013/3/18
*  @date     2012/3/18
*  @{
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
    vector<int> _AminoAcidIDs;

    /** Length of this Peptide sequence.  */
    int _size;


public:

    /** A  static data set stored all amino acids.  */
    static CAminoAcid AminoAcids[AMINOACIDNUMBER];

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
    CPeptide(string sid, string sequence);

    /**
     * @override
     * Return the length of this Peptide sequence.
    */
    virtual int getSize()const;

    /**
     * Return a integer vector contain amino acid id of this Peptide sequence.
    */
     vector<int> getAminoAcidIDs()const;

    /**
     * A static method to return amino acid's ID.
     * Get the amino acid's id according to input amino acid character.
     * This method required a input param which is existed in AminoAcids data set.
     * @param char AminoAcid    A amino acid waiting to get its ID.
     * @return int              Return a int ID which is stand for input char.
    */
    static int getAminoAcidID(char AminoAcid);
    
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
     * A object defined shared_ptr<T>(a kind of smart pointer, existed in <memory>) will count
     *  how much pointer point to it, and this counting called reference counting.
     *  When reference counting reduces to 0, this object will destory automatically.
     * By using shared_ptr<T>, we can ensure no memory leak.
     * @return vector<shared_ptr<CPeptide>>
     *   - return a vector stored Peptide fragments.
     * 
    */
    static vector<shared_ptr<CPeptide>> loadData(string filename, int maxSize, int fragmentLength);

};
/**
 * @}   // CPeptide
*/
#endif
//SQAI_OBJECTS_PEPTIDE_H
