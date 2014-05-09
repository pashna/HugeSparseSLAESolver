#include "LevelMatrix.h"
#include <iostream>

LevelMatrix::LevelMatrix(SparseMatrix& a, const size_t filler) {
    size_t a_size(a.getSize());
    size = a_size;
    table.resize(size);

    for (size_t i = 0; i < a_size; ++i) {
        SparseIterator it(a[i]->end()), end(a[i]->begin(false));
        size_t first_num = end.getNumber();
        LevelRow crnt(i + 1 - first_num, filler);
        while (it != end) {
            it.prev();
            if (*it) {
                crnt[it.getNumber() - first_num] = 0;
            }
        }
        /*for (size_t j = 0; j < crnt.size(); ++j)
            std::cout << crnt[j] << std::endl;
        std::cout << std::endl;*/
        LevelArray* table_i = new LevelArray(crnt, first_num);
        table[i] = table_i;
    }
}
