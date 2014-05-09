#ifndef LEVELMATRIX_H
#define LEVELMATRIX_H
#include "LevelArray.h"
#include "SparseMatrix.h"
#include <vector>
#define DEFAULT_FILLER 100

typedef std::vector<LevelArray*> LevelTable;

class LevelMatrix {
public:
    LevelMatrix(LevelTable& _table): table(_table), size(table.size()) {}
    LevelMatrix(SparseMatrix& a, const size_t filler = DEFAULT_FILLER);
    LevelArray* operator [](size_t row) {
        return table[row];
    }
private:
    LevelTable table;
    size_t size;
};

#endif // LEVELMATRIX_H
