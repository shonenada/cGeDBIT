/**@file    
 * @brief   This is a Index object of Image.
 * @author  Arthur Fu
 * @date    2013/05/29
 * @version Bate_0.9
 *
 * detailed description
 * This class define Image index object. 
 * This is the key of Image Object. 
 * This object stores an array of floating numbers representing the key for image object. 
 * It is designed only for MoBIoS image dataset, might not work for other datasets.
*/

int i=0;

#include "../../HeaderFiles/objects/Image.h"

/**
 * @brief A constructor.
 * @author Arthur Fu
 * @return none
 * @note
 * detailed description
 * - The m_Feas_Len will be initialized to '66'.
 * - The m_Feas will be initialized to '0'.
 * - The max_Dist_Len will be initialized to '3'. 
 * - The max_Dist will be initialized to '1.0', '60.0', '1.0'.
*/
CImage::CImage()
{
	m_Feas_Len = 66;
	m_Feas = new float[66];
	for(int i = 0 ; i < 66; i++)
		m_Feas[i] =  0.0 ; 

	max_Dist_Len = 3;
	max_Dist = new double[3];
	max_Dist[0]=1.0;
	max_Dist[1]=60.0;
	max_Dist[2]=1.0;
}

/**
 * @brief A constructor.
 * @author Arthur Fu
 * @param FLen : The length of the feas[]
 * @param feas : An array of floats over which the feature values are defined.
 * @return none
 * @note
 * detailed description
 * - The FLen will be initialized to m_Feas_Len. 
 * - The feas will be initialized to m_Feas.
*/
CImage::CImage(int FLen, float *feas)
{
	int i;

	m_Feas_Len = FLen ; 
    m_Feas = new float[m_Feas_Len];
	for ( i = 0; i < m_Feas_Len; i++)
		m_Feas[i] = feas[i];

	max_Dist_Len = 3;
	max_Dist = new double[3];
	max_Dist[0]=1.0;
	max_Dist[1]=60.0;
	max_Dist[2]=1.0;
}

/**
 * @brief A constructor.
 * @author Arthur Fu
 * @param FLen : The length of the feas[]
 * @param DLen : The length of the maxDist[]
 * @param feas : an array of floats over which the feature values are defined.
 * @param maxDist : an array of floats over which the dis values are defined.
 * @return none
 * @note
 *
 * detailed description
 * - The FLen will be initialized to m_Feas_Len. 
 * - The feas will be initialized to m_Feas.
 * - The DLen will be initialized to max_Dist_Len.
 * - The maxDist will be initialized to max_Dist.
*/
CImage::CImage(int FLen, float *feas, int DLen, double *maxDist)
{  
	int i;
	m_Feas_Len = FLen ; 
	max_Dist_Len = DLen;
	
    m_Feas = new float[m_Feas_Len];
	for (i = 0; i < m_Feas_Len; i++)
		m_Feas[i] = feas[i];

	max_Dist = new double[max_Dist_Len];
	for (i = 0; i < max_Dist_Len; i++)
		max_Dist[i] = maxDist[i];
}

/**
 * @brief A destructor, do nothing.
 * @author Arthur Fu
 * Details:
 * - The m_Feas and max_Dist will be deleted.
*/
CImage::~CImage()
{
	delete[] (m_Feas);
	delete[] (max_Dist);
    i++;
   /* cout<<i<<endl;*/
}

/**
 * @brief The method will return the value of index subscript corresponds in m_Feas.
 * @author Arthur Fu
 * @param index : An index will be used to find a value.
 * @return Return the value of index subscript corresponds in m_Feas.
*/
float CImage::getFeature(int index)
{
	return m_Feas[index];
}

/**
 * @brief The method will return the m_Feas_Len.
 * @author Arthur Fu
 * @return Return the m_Feas_Len.
*/
int CImage::getSize()
{
	return m_Feas_Len;
}

/**
 * @brief The method will compare the two object.
 * @author Arthur Fu
 * @param *oThat : The object of CIndexObject.
 * @return '0', '1', '-1'.
 * @note
 * detailed description
 *  - The CIndexObject types of object will be changed into CImage types.
 *  - Compare the CIndexObject types of object to the CImage types of object.
 *  - If "this" is equal to "that", then returns '0'.
 *  - If the length of "this" is larger than the length of "that", then return '-1'.
 *  - If the length of "that" is larger than the length of "this", then return '1'.
 *  - If their length is the same, then compare the features one by one.
 *  - If the feature of "this" is larger than the feature of "that", then return '-1'.
 *  - If the feature of "that" is larger than the feature of "this", then return '1'.
*/
int CImage::compareTo(CIndexObject *oThat)
{
	CImage  *that = static_cast<CImage *> (oThat);//将CIndexObject对象指针转化为CImage对象指针 

	if (this == that) 
		return 0;

	if (this->m_Feas_Len < that->m_Feas_Len)
		return -1;
	else if (this->m_Feas_Len > that->m_Feas_Len)
		return 1;

	else
	{
		for (int i = 0; i < m_Feas_Len; i++)
		{
			if (this->m_Feas[i] < that->m_Feas[i])
				return -1;
			else if (this->m_Feas[i] > that->m_Feas[i])
				return 1;
		}
		return 0;
	}
}

/**
 * @brief The method will load the datum from the file, and store them in the vector.
 * @author Arthur Fu
 * @param string fileName : The name of the file containing the datum.
 * @param int cImage_Num : The number of datum.
 * @param int feas_Num : The number of features.
 * @return Return vector stored the datum from the file.
 * @note
 * detailed description
 *  - Open the file containing the datum.
 *  - Store 'feas_Num' datum in a temporary array named 'inFeas'.
 *  - Then using the 'inFeas' to create a new object of 'CImage'.
 *  - Add the object of 'CImage' into the vector.
 *  - Return the vector.
*/
vector< shared_ptr<CIndexObject> >* CImage::loadData(string fileName , int cImage_Num, int  feas_Num)
{
	ifstream in(fileName);
	int i,j;
	string imageName;
	//vector<CIndexObject*> *datas = new vector<CIndexObject*>;
	vector< shared_ptr<CIndexObject> > *datas = new vector< shared_ptr<CIndexObject> >;

	float* inFeas = NULL;
	//CIndexObject* temp = NULL;
	shared_ptr<CIndexObject> temp;

	for(i = 0; i < cImage_Num; i++)
	{
		in>>imageName;
		inFeas = new float[feas_Num];
		for(j = 0; j < feas_Num; j++)
		{
			in>>inFeas[j];
		}
		//temp = new CImage(feas_Num,inFeas);
		temp.reset(new CImage(feas_Num,inFeas));
		datas->push_back(temp);
	}
	return datas;
}

/**
 * @brief The method will load the datum from the file, and store them in the vector.
 * @author Arthur Fu
 * @param string fileName : The name of the file containing the datum.
 * @param int cImage_Num : The number of datum.
 * @param int feas_Num : The number of features.
 * @return Return vector stored the datum from the file.
 * @note
 * detailed description
 *  - Open the file containing the datum.
 *  - Store 'feas_Num' datum in a temporary array named 'inFeas'.
 *  - Then using the 'inFeas' to create a new object of 'CImage'.
 *  - Add the object of 'CImage' into the vector.
 *  - Return the vector.
*/
vector< shared_ptr <CIndexObject> >* CImage::loadData(string fileName , string fileNameMaxInfo, int cImage_Num, int  feas_Num, int maxDist_Num)
{
	ifstream in(fileName);
	ifstream inMaxInfo(fileNameMaxInfo);
	int i,j;
	string imageName;
	//vector<CIndexObject*> *datas = new vector<CIndexObject*>;
	vector< shared_ptr<CIndexObject> > *datas = new vector< shared_ptr<CIndexObject> >;

	float* inFeas = NULL;
	double* inMaxDist = NULL;
	//CIndexObject* temp = NULL;
	shared_ptr<CIndexObject> temp;

	for(i = 0; i < cImage_Num; i++)
	{
		in>>imageName;
		inFeas = new float[feas_Num];
		inMaxDist = new double[maxDist_Num];
		for(j = 0; j < feas_Num; j++)
		{
			in>>inFeas[j];
		}
		for(j = 0; j < maxDist_Num; j++)
		{
			inMaxInfo>>inMaxDist[j];
		}
		//temp = new CImage(feas_Num, inFeas, maxDist_Num, inMaxDist);
		temp.reset(new CImage(feas_Num, inFeas, maxDist_Num, inMaxDist));
		datas->push_back(temp);
	}
	return datas;
}

/**@brief write the instance of this class to hard disk
*@param out out put stream,used to write inforamtion to the hard disk
*@return return the size of information which has been wrote to the hard disk
*/
int CImage::writeExternal(ofstream &out)
{
    int size=0;

    out.write((char*) (&m_Feas_Len),sizeof(int));
    size += sizeof(int);
    
    out.write((char*)m_Feas,m_Feas_Len*sizeof(float));
    size += m_Feas_Len*sizeof(float);

    out.write((char*) (&max_Dist_Len),sizeof(int));
    size += sizeof(int);
    
    out.write((char*)max_Dist,max_Dist_Len*sizeof(double));
    size += max_Dist_Len*sizeof(double);

    return size;

}

/**@load a instance of this class from the file stored in hard disk
*@param in input stream,used to read information from hard disk
*@return return the size of information which has been read from the hard disk
*/
int CImage::readExternal(ifstream &in)
{
    int size=0;

    in.read((char*)(&m_Feas_Len),sizeof(int));    
    size+=sizeof(int);

    //m_Feas = new float[m_Feas_Len];

    in.read((char*)m_Feas,m_Feas_Len*sizeof(float));    
    size+=m_Feas_Len*sizeof(float);


    in.read((char*)(&max_Dist_Len),sizeof(int));    
    size+=sizeof(int);

    //max_Dist = new double[max_Dist_Len];

    in.read((char*)max_Dist,max_Dist_Len*sizeof(double));    
    size+=max_Dist_Len*sizeof(double);
    return size;
    
}

/**@brief return the name of a instance of this class
*@return return the name of a instance of this class
*/
CreateFuntion CImage::getConstructor()
{
    return & CreateInstance;
}

void* CImage:: CreateInstance()
{
    return new CImage();
}

