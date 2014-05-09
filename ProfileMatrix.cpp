#include "ProfileMatrix.h"

ProfileMatrix::ProfileMatrix(const SizeVector& sizes)
    : pointers(sizes.size() + 1)
    , col(pointers.data())
{
    size_t size(0);
    for (SizeVector::const_iterator it(sizes.begin()); it != sizes.end(); ++it)
        size += *it;
    data.resize(size);
    real* raw(data.data());
    ColumnContainer::iterator jt(pointers.begin());
    SizeVector::const_iterator it(sizes.begin());
    while (it != sizes.end()) {
        *jt = raw;
        raw += *it;
        ++it;
        ++jt;
    }
    *jt = raw;
}

std::ostream& operator << (std::ostream& out, const ProfileMatrix& m) {
    for (size_t i(0); i != m.size(); ++i) {
        for (size_t j(0); j != m.size(); ++j) {
            out << m(i, j) << '\t';
        }
        out << std::endl << std::endl << std::endl;
    }
    return out;
}


