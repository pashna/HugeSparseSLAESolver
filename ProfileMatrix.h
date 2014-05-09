#ifndef PROFILEMATRIX_H
#define PROFILEMATRIX_H

#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>

typedef double real;

/*

  1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
  ^ ^ ^   ^     ^            ^        ^
  1 1 2   3     5            3
*/
class ProfileMatrix
{
    ProfileMatrix(const ProfileMatrix&);
    ProfileMatrix& operator =(const ProfileMatrix&);
public:
    typedef std::vector<real> RawContainer;
    typedef std::vector<real*> ColumnContainer;
    typedef std::vector<size_t> SizeVector;

    ProfileMatrix(const SizeVector& sizes = SizeVector());

    real* operator[] (size_t pos) { return col[pos]; }
    real operator()(size_t r, size_t c) const
    {
        return r > c ?
                    this->operator ()(c,r) :
                    (col[c] + c - r < col[c + 1] ?
                        col[c][c - r] :
                        0);
    }

    size_t size() const { return pointers.size() - 1; }

    const RawContainer& rawData() const { return data; }
    const ColumnContainer& columns() const { return pointers; }

    friend std::ifstream& operator >> (std::ifstream&, ProfileMatrix&);
    friend std::ofstream& operator << (std::ofstream&, const ProfileMatrix&);
private:
    RawContainer data;//обеспечивает хранение самих чисел
    ColumnContainer pointers;//обеспечивает хранение указателей
    real** col;//адреса первых элементов столбцов
};

std::ostream& operator << (std::ostream&, const ProfileMatrix&);

#endif // PROFILEMATRIX_H
