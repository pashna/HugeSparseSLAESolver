#ifndef SPARSEARRAY_H
#define SPARSEARRAY_H
#include "SparseIterator.h"
#include <vector>
#include <iostream>
#include <stdlib.h>

typedef double real;
typedef std::pair<size_t, real*> Link;
typedef std::vector <std::pair<size_t, real*> > Links;
typedef std::vector<real> Row;
class SparseIterator;

class SparseArray{
    friend class SparseIterator;
public:
    SparseArray(const SparseArray&);
    SparseArray(const Row &, const size_t first_num = 0, const bool correct = true); //Должен конструировать полностью доступный для чтения разреженый массив
    size_t toVector(Row& val, const bool null = true) const; //перегоняет (null ? все : все, начиная с первого ненулевого) данные в вектор val
    SparseIterator begin(const bool null = true); //Итератор, ссылающийся на первый (!null ? ненулевой) элемент
    SparseIterator end(); //Итератор, ссылающийся на элемент, следующий за последним
    real& operator[](size_t pos) const;

private:
    size_t sparseLen(Link* it) {
        return (it != link.data() + link.size() - 1) ? ((it + 1)->first - it->first) : (length - it->first);
    }
    static real zero;
    Row array;
    size_t length;
    Links link;
};

#endif // SPARSEARRAY_H
