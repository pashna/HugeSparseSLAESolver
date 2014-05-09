#ifndef GRADIENTMETHOD_H
#define GRADIENTMETHOD_H
#include "SparseMatrix.h"
#include <vector>
#include <math.h>

typedef std::vector<real> Column;

class Gradient {
public:
    Gradient(SparseMatrix _K, Column& _R): K(_K), R(_R) {Unext.resize(K.getSize(), real(0));} // НАСТОЯЩИЙ
    //Gradient(SparseMatrix &_K, Column& _R): K(_K), R(_R) {Unext.resize(5, real(0));} // ОТЛАДОЧНЫЙ
    void solveWithCount(size_t);
    void solveWithPrecision(real);
    void solveWithPrecondition(real, SparseMatrix &);
    Column& getResolution(){return Unext;}
private:
    SparseMatrix K;
    Column Unext;
    Column& R;
    void clearResolution();
    bool verify(Column&);
    real solveAlpha(Column&, Column&);
    void solveUnext(Column&, real);
    void solveRnext(Column&, Column&, Column&, real);
    real solveB(Column&, Column&);
    void solvePnext(Column&, Column&, Column&, real);
    real solveNorma(Column&);

    //==============

    real solveAlphaPre(Column&,Column& ,Column&);
    real solveBPre(Column&, Column&, Column&, Column&);
    void solvePnextPre(Column&, Column&, Column&, real);

};
#endif // GRADIENTMETHOD_H
