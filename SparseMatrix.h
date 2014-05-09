#ifndef SPARSEMATRIX_H
#define SPARSEMATRIX_H
#include "SparseArray.h"
#include "SparseIterator.h"
#include <string.h>
#include <vector>
#include <iostream>

typedef std::vector<SparseArray*> Table;
typedef SparseArray* SparseArrayPointer;

class SparseMatrix {
    friend Row& operator *(Row&, SparseMatrix&);
private:
    Table table;
    size_t size;
public:
    SparseMatrix(const SparseMatrix&);
    SparseMatrix(std::string, std::string);
    SparseMatrix(Table& _table): table(_table), size(_table.size()) {}
    //~SparseMatrix();
    SparseArrayPointer& operator [] (size_t pos) {
        return table[pos];
    }
    const real& operator () (size_t, size_t);
    size_t getSize() {
        return size;
    }
    void printMatrix() {
        for (size_t i = 0; i < size; ++i) {
            for(SparseIterator it(table[i]->begin()), end(table[i]->end()); it != end; ++it) {
                std::cout << *it << " ";
            }
            std::cout << std:: endl;
            //int r; std::cin >> r;
        }
    }
    SparseMatrix& transposition();
};
#endif // SPARSEMATRIX_H
