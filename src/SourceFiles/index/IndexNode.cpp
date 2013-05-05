#include "..\..\HeaderFiles\index\IndexNode.h"

/**@addtogroup INDEXNODE
 *@{
 */

/**none parameter constructor. 
   this constructor will be called to create an instance of class CIndexNode
*/
CIndexNode::CIndexNode()
{

}

/**constructor with one parameter pivots. 
	this constructor will be called to create an instance of class CIndexNode
	@para pivots a vector include all the address of pivots belong to single node in the index structor
*/
CIndexNode::CIndexNode(vector<CIndexObject*> pivots,int height)
{
	this->pivots=pivots;
	this->height=height;

}

/**destructor
   the space of this class's instance will be released if the instance is no longer used in the program
*/
CIndexNode::~CIndexNode()
{

}

/**set the height of this node*/
void CIndexNode::setHeight(int height)
{
	this->height=height;
}

/**get the height of this node*/
int CIndexNode::getHeight()
{
	return this->height;
}

/** get the address of a pivot based on the pivot index in the parameter list
	@para pivot pivot index of a pivot in the pivot address vector in the node
	@return a pivot address in the main memory
*/
CIndexObject* CIndexNode::getPivot(int pivotIndex)
{
	return pivots[pivotIndex];
}

vector<CIndexObject*>& CIndexNode::getPivots()
{
    return pivots;
}

/**@}*/