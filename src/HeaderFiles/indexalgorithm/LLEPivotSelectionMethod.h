#ifndef LLTPIVOTSELECTIONMETHODS_H
#define LLTPIVOTSELECTIONMETHODS_H

#include <vector>
#include "PivotSelectionMethod.h"
#include <gsl/gsl_matrix_double.h>
#include <gsl/gsl_vector_double.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_cblas.h>
#include <gsl/gsl_eigen.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_sort_vector.h>
#include <gsl/gsl_sf.h>
/**
  * @file LLEPivotSelectionMethod.h
  * @brief Select pivots from data set with LLE(Locally Linear Embedding)
  * @author Yaoda Liu(2011150337@email.szu.edu.cn), Guohang Zend
  * @version 2013-12-23
  */

/**
  * @class CLLEPivotSelectionMethod
  * @brief Select pivots from data set with LLE
  *
  * Select pivots from the given data list, using LLE.
  */
class CLLEPivotSelectionMethod : public CPivotSelectionMethod {

private:
    const int nearestNeighbors = 18;
    const double regularizationParameter = 0.001;
    int numOfPivot;
    int numOfRow;
    int numOfColumn;
    gsl_matrix* lleMatrix;

public:
    CLLEPivotSelectionMethod();

    ~CLLEPivotSelectionMethod();

    gsl_matrix* viewDice (gsl_matrix* matrix);

    gsl_matrix* invertMatrix (gsl_matrix* matrix);

    gsl_matrix* pairWiseDistance (CMetric* metric, std::vector<shared_ptr<CIndexObject> > &data);

    std::vector<double> euclideanNorm(gsl_matrix* m);

    void runLLE(gsl_matrix* matrix);

    std::vector<int> selectFromResult(gsl_matrix* matrix, gsl_matrix* distance);

    std::vector<int> selectPivots(CMetric* metric, std::vector<shared_ptr<CIndexObject> > &data, int numPivots);

    std::vector<int> selectPivots(CMetric *metric, std::vector<shared_ptr<CIndexObject> > &data, int first, int dataSize, int numPivots);

    std::vector<int> selectPivots(CMetric *metric, std::vector<shared_ptr<CIndexObject> > &data, int first, int dataSize, int numPivots, int firstPivot);

};

#endif
