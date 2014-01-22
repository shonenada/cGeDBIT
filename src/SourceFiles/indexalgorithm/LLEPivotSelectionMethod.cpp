#include "../../HeaderFiles/indexalgorithm/LLEPivotSelectionMethod.h"

#include <gsl/gsl_matrix_double.h>
#include <gsl/gsl_vector_double.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_cblas.h>
#include <gsl/gsl_eigen.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_sort.h>
#include <gsl/gsl_sort_vector.h>
#include <gsl/gsl_sf.h>

CLLEPivotSelectionMethod::CLLEPivotSelectionMethod ()
{

}

CLLEPivotSelectionMethod::~CLLEPivotSelectionMethod ()
{
    
}

/**
 * @param matrix The matrix waiting for transpose
 * @return Return transpose of input matrix
 */
gsl_matrix* CLLEPivotSelectionMethod::viewDice (gsl_matrix* matrix)
{
    int row = matrix->size1;
    int column = matrix->size2;
    gsl_matrix* output = gsl_matrix_alloc(column, row);
    for (int i=0; i<row; ++i){
        for (int j=0; j<column; ++j){
            double element = gsl_matrix_get(matrix, j, i);
            gsl_matrix_set(output, i, j, element);
        }
    }
    return output;
}

/**
 * @param matrix The matrix waiting for calculating invert matrix
 * @return Return the invert matrix of input matrix
 */
gsl_matrix* CLLEPivotSelectionMethod::invertMatrix (gsl_matrix* matrix)
{
    int row = matrix->size1;
    int column = matrix->size2;
    int s;
    if (row != column){
        return NULL;
    }
    gsl_matrix * inverse = gsl_matrix_alloc (row, column);
    gsl_permutation * perm = gsl_permutation_alloc (row);
    gsl_linalg_LU_decomp (matrix, perm, &s);
    gsl_linalg_LU_invert (matrix, perm, inverse);
    return inverse
}

/**
 * 对阵距离矩阵
 */
gsl_matrix* CLLEPivotSelectionMethod::pairWiseDistance (CMetric* metric,
                                                 std::vector<shared_ptr<CIndexObject> > &data)
{
    int i, j;
    int dataSize = data.size();
    gsl_matrix* distance = gsl_matrix_alloc(dataSize, dataSize);
    for (i=0; i<dataSize; ++i) {
        gsl_matrix_set(distance, i, i, 0);
        for (j=0; j<i; ++j) {
            gsl_matrix_set(distance, i, j, metric->getDistance(data[i].get(), data[j].get()));
            gsl_matrix_set(distance, j, i, gsl_matrix_get(distance, i, i));
        }
    }
    return distance;
}

void CLLEPivotSelectionMethod::runLLE(gsl_matrix* matrix)
{
    int i, j, row, Qi;
    gsl_matrix* weightMatrix;
    gsl_matrix* E;
    gsl_matrix* diffMatrix;
    gsl_vector* sumOfSquareVector;
    gsl_permutation* sortedIndex;
    gsl_matrix* sortedMatrix;
    gsl_matrix* sortedMatrixDice;
    gsl_matrix* Q;
    gsl_matrix* invertQ;
    gsl_matrix* wTemp;
    gsl_vector* w;
    gsl_matrix* weightMatrixDice;
    gsl_matrix* M;
    gsl_vector* eval;
    gsl_matrix* evec;
    gsl_matrix* evec2;
    gsl_matrix* output;

    /* 为权值矩阵申请内存空间 */
    weightMatrix = gsl_matrix_alloc(numOfRow, numOfColumn);
    for (i=0; i<numOfRow; ++i){
        for (j=0; j<numOfColumn; ++j){
            gsl_matrix_set(weightMatrix, i, j, 0);
        }
    }

    E = gsl_matrix_alloc(nearestNeighbors, nearestNeighbors);
    for (i=0; i<nearestNeighbors; ++i) {
        gsl_matrix_set(E, i, i, 1);
    }

    for (row=0; row<numOfRow; ++row) {
        /* Step 1. */
        diffMatrix = gsl_matrix_alloc(numOfRow, numOfColumn);

        for (i=0; i<numOfColumn; ++i) {
            double element = gsl_matrix_get(matrix, row, i);
            for (j=0; j <numOfRow; ++j) {
                double diff = gsl_matrix_get(matrix, i, j) - element;
                double diffSquare = diff * diff;
                gsl_matrix_set(diffMatrix, j, i, diffSquare);
            }
        }

        sumOfSquareVector = gsl_vector_alloc(numOfRow);
        sortedIndex = gsl_permutation_alloc(numOfRow);
        gsl_permutation_init(sortedIndex);

        for (i=0; i<numOfRow; ++i){
            double element = 0;
            for (j=0; j<numOfColumn; ++j){
                double diff = gsl_matrix_get(diffMatrix, i, j);
                double diffSquare = diff * diff;
                element = element + diffSquare;
            }
            gsl_vector_set(sumOfSquareVector, i, element);
        }
        
        gsl_sort_vector_index(sortedIndex, sumOfSquareVector);
		gsl_sort_vector(sumOfSquareVector);

        sortedMatrix = gsl_matrix_alloc(nearestNeighbors, numOfColumn);
        for (i=1; i<=nearestNeighbors; ++i){
            int rowTemp = gsl_permutation_get(sortedIndex, i);
            gsl_vector* tempVector = gsl_vector_alloc(numOfColumn);
            gsl_matrix_get_row(tempVector, diffMatrix, rowTemp);
            gsl_matrix_set_row(sortedMatrix, rowTemp, tempVector);
            gsl_vector_free(tempVector);
        }

        /* 步骤2: 重建矩阵 */
        sortedMatrixDice = viewDice(sortedMatrix);
        Q = gsl_matrix_alloc(nearestNeighbors, nearestNeighbors);
        
        /* 下面的语句是计算 Q = sortedMatrix x sortedMatrixDice */
        gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, sortedMatrix, sortedMatrixDice, 0.0, Q);

        double matrixTrace = 0;
        for (i=0; i<nearestNeighbors; ++i){
            matrixTrace = matrixTrace + gsl_matrix_get(Q, i, i);
        }

        double base = regularizationParameter * matrixTrace;
        for (Qi=0; Qi<nearestNeighbors; ++Qi){
            double element = gsl_matrix_get(Q, Qi, Qi);
            gsl_matrix_set(Q, Qi, Qi, element + base);
        }
        invertQ = invertMatrix(Q);
        wTemp = gsl_matrix_alloc(nearestNeighbors, nearestNeighbors);
        gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, invertQ, E, 0.0, wTemp);

        w = gsl_vector_alloc(nearestNeighbors);
        gsl_matrix_get_col(w, wTemp, 0);
        double wSum = 0;
        for (i=0;i<nearestNeighbors;++i){
            wSum += gsl_vector_get(w, i);
        }

        for (i=0;i<w->size;++i){
            double temp = gsl_vector_get(w, i) / wSum;
            gsl_vector_set(w, i, temp);
        }

        for (i=0;i<nearestNeighbors;++i){
            double temp = gsl_vector_get(w, i);
            int index = (int) gsl_permutation_get(sortedIndex, i+1);
            gsl_matrix_set(weightMatrix, index, row, temp);
        }
    }

    /* 步骤3：映射到低维空间中 */
    for (i=0;i<weightMatrix->size1;++i){
        double newVal = gsl_matrix_get(weightMatrix, i, i) - 1;
        gsl_matrix_set(weightMatrix, i, i, newVal);
    }

    weightMatrixDice = viewDice(weightMatrix);
    M = gsl_matrix_alloc(weightMatrix->size1, weightMatrix->size2);

    gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, weightMatrix, weightMatrixDice, 0.0, M);

    for (int i=0;i<M->size2;++i){
        double element = gsl_matrix_get(M, i, i) + 0.1;
        gsl_matrix_set(M, i, i, element);
    }
    
    lleMatrix = gsl_matrix_alloc (numOfRow, numOfPivot);
    
    /* eval 存储 N 个特征值 */
    /* evec 为由 M 矩阵的 N 的特征值向量构成的 N 阶方阵 */
    eval = gsl_vector_alloc(numOfRow);
    evec = gsl_matrix_alloc(numOfRow, numOfRow);

    /* 申请工作空间 */
    gsl_eigen_symmv_workspace *ws = gsl_eigen_symmv_alloc(numOfRow); 
    /* eval，evec 赋值 */
    gsl_eigen_symmv (matrix, eval, evec, ws);

    /* eval 升序排序，对应 eval 的顺序给 evec 重新赋值 */
    gsl_eigen_symmv_sort(eval, evec, GSL_EIGEN_SORT_ABS_ASC);

    /* 需要转置矩阵evec使得行列正确对应 */
    evec2 = viewDice(evec);
    int i, j;
    for(i=0;i<numOfRow;i++){
        for(j=1;j<numOfPivot+1;j++){
            double temp = gsl_matrix_get(evec2, i, j);
            gsl_matrix_set(lleMatrix, i, j-1, temp);
        }
    }

    gsl_matrix_free(diffMatrix);
    gsl_matrix_free(sortedMatrix);
    gsl_matrix_free(sortedMatrixDice);
    gsl_matrix_free(Q);
    gsl_vector_free(sumOfSquareVector);
    gsl_permutation_free(sortedIndex);
    gsl_matrix_free(invertQ);
    gsl_matrix_free(wTemp);
    gsl_vector_free(w);
    gsl_eigen_symmv_free(ws);
    gsl_vector_free(eval);
    gsl_matrix_free(evec);
    gsl_matrix_free(evec2);
    gsl_matrix_free(E);

}

std::vector<double> CLLEPivotSelectionMethod::euclideanNorm(gsl_matrix* m)
{
    int i, j;
    std::vector<double> norm;
    norm.assign(m->size2, 0);
    for (i=0; i< m->size2; ++i) {
        gsl_vector* temp = gsl_vector_alloc(m.size2);
        gsl_matrix_get_col(temp, m, i);
        gsl_vector_mul(temp, temp);
        for (j=0; j<temp->size; ++j) {
            norm[i] += gsl_vector_get(temp, j);
        }
        gsl_vector_free(temp);
    }
    return norm;
}

std::vector<int> CLLEPivotSelectionMethod::selectFromResult(gsl_matrix* lleMatrix, gsl_matrix* distanceMatrix)
{
    int i, j, k;
    std::vector<int> result;
    result.resize(lleMatrix->size2);

    std::vector<double> lleMatrixNorm = euclideanNorm(lleMatrix);
    std::vector<double> distanceMatrixNorm = euclideanNorm(distanceMatrix);
    
	for (i=0; i<lleMatrixNorm.size; ++i) {
        gsl_vector* angle = gsl_vector_alloc(distanceMatrix->size2);
        for (j=0; j<distanceMatrix->size2; ++j) {
            gsl_vector* mVector;
            gsl_vector* dVector;
            gsl_matrix_get_col(mVector, lleMatrix, i);
            gsl_matrix_get_col(dVector, distanceMatrix, j);

            double m = 0, n;
            for (k=0; k<mVector->size; ++k) {
                gsl_vector_mul(mVector, dVector);
                m += gsl_vector_get(mVector, k);
            }
            n = lleMatrixNorm[i] * distanceMatrixNorm[j];
            gsl_vector_set(angle, j, m/n);
            gsl_vector_free(mVector);
            gsl_vector_free(dVector);
        }
        result[i] = gsl_vector_max_index(angle);
        gsl_vector_free(angle);
    }
	gsl_matrix_free(lleMatrix);

    return result;

}

std::vector<int> CLLEPivotSelectionMethod::selectPivots(CMetric* metric, vector<shared_ptr<CIndexObject> > &data, int numPivots)
{

    /* Compute the distance matrix */
    gsl_matrix* distance = pairWiseDistance(metric, data);

    /* Assign parameters */
    this->numOfPivot = numPivots;
    this->numOfRow = distance->size1;
    this->numOfColumn = distance->size2;

    /* Run LLE */
    runLLE(distance);

    /* Select from LLE result */
    return selectFromResult(lleMatrix, distance);
}

std::vector<int> CLLEPivotSelectionMethod::selectPivots(CMetric *metric, vector<shared_ptr<CIndexObject> > &data, int first, int dataSize, int numPivots)
{
    return selectPivots(metric, data, numPivots);
}

std::vector<int> CLLEPivotSelectionMethod::selectPivots(CMetric *metric, vector<shared_ptr<CIndexObject> > &data, int first, int dataSize, int numPivots, int firstPivot)
{
    return selectPivots(metric, data, numPivots);
}
