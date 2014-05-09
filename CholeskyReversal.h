#ifndef CHOLESKYREVERSAL_H
#define CHOLESKYREVERSAL_H
#include "SparseMatrix.h"

class CholeskyReversal {
public:
    static Row& reversalBegin(SparseMatrix& l, const Row& b) {
        size_t size = l.getSize();
        Row& y = *(new Row(size));
        real sum;
        bool firstIteration;
        real li;
        for (size_t i = 0; i < size; ++i) {
            sum = 0;
            firstIteration = true;
            SparseIterator it(l[i]->end()), end(l[i]->begin());
            while(it != end) {
                it.prev();
                if (!firstIteration)
                    sum += *it * y[it.getNumber()];
                else {
                    li = *it;
                    firstIteration = false;
                }
            }
            y[i] = (b[i] - sum)/li;
        }
        return y;
    }

    static Row& reversalEnd(SparseMatrix& lt, Row& y) {
        size_t size = lt.getSize();
        Row& x = *(new Row(size));
        real sum;
        size_t crnt_num;
        for (int i = size - 1; i >= 0; --i) {
            sum = 0;
            SparseIterator it(lt[i]->end()), end(lt[i]->begin());
            while (it != end) {
                it.prev();
                if ((crnt_num = it.getNumber()) != 0) {
                    crnt_num += i;
                    sum += *it * x[crnt_num];
                }
                else
                    x[i] = (y[i] - sum) / *it;
            }
        }
        return x;
    }
};

#endif // CHOLESKYREVERSAL_H
