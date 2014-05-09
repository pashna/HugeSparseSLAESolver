#include "SparseIterator.h"
#include <iostream>

const real& SparseIterator::operator *(void) {
    if (!element)
        return SparseArray::zero;
    else
        return *element;
}

SparseIterator& SparseIterator::operator ++(void) {
    if (++number == pos->first + len) {
        ++pos;
        len = (pos + 1)->first - number;
        element = pos->second;
    }
    else
        if (element)
            ++element;
    return *this;
}

SparseIterator& SparseIterator::operator --(void) {
    if (--number == pos->first - 1) {
        --pos;
        len = number + 1 - pos->first;
        if (pos->second)
            element = pos->second + len - 1;
        else
            element = NULL;
    }
    else
        if (element)
            --element;
    return *this;
}

SparseIterator& SparseIterator::prev(void) {
    if (!pos->second) {
        number = pos->first;
        if (number) {
            --number;
            len = number + 1 - (--pos)->first;
            element = pos->second + len - 1;
        }
    }
    else {
        if (--number == pos->first - 1) {
            if ((--pos)->first != 0) {
                number = pos->first - 1;
                len = number + 1 - (--pos)->first;
                element = pos->second + len - 1;
            }
            else {
                number = 0;
                element = pos->second;
            }
        }
        else
            --element;
    }
    return *this;
}
