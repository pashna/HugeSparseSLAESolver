#ifndef SPARSEITERATOR_H
#define SPARSEITERATOR_H
#include "SparseArray.h"
#include <utility>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

class SparseArray;

typedef double real;
typedef std::pair<size_t, real*> Link;

class SparseIterator {
    friend class SparseArray;
public:
    const real& operator *();
    SparseIterator& operator ++(); // Перейти к следующему элементу
    SparseIterator& operator --(); // Перейти к предыдущему элементу
    bool operator ==(const SparseIterator& other) const { // Ссылаются на один и тот же элемент ? true: false;
        return (number == other.number)? true : false;
    }
    bool operator !=(const SparseIterator& other) const { // Ссылаются на различные элементы ? true: false;
        return (number != other.number)? true : false;
    }
   // SparseIterator& next(); // Перейти к следующему ненулевому элементу
    SparseIterator& prev(); // Перейти к предыдущему ненулевому элементу
    size_t getNumber() { // Получить порядковый номер текущего элемента
        return number;
    }

private:
    real* element; // Элемент в массиве значений, которому соответствует текущий элемент
    size_t number; // Порядковый номер элемента
    Link* pos; // Элемент в массиве пар, соответствующий отрезку, которому принадлежит текущий элемент
    size_t len; // Длина текущего элемента в массиве паре, соответствующего отрезку, которому принадлежит текущий элемент
};
#endif // SPARSEITERATOR_H
