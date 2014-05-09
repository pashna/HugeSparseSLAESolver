#include "SparseMatrix.h"
#include "SparseIterator.h"
#include "SparseArray.h"
#include <string>
#include <fstream>
#include <iostream>

SparseMatrix::SparseMatrix(const SparseMatrix& _a): size(_a.size) {
    Table _table = *(new Table(size));
    for (size_t i = 0 ; i < size; ++i) {
        _table[i] = new SparseArray(*(_a.table[i]));
    }
    table = _table;
}

SparseMatrix::SparseMatrix(std::string env, std::string stif) {
//    std::string filename("/home/popka/kursach/simple1_out_model/ENV.bin");
    std::ifstream env_stream;
    env_stream.open(env.c_str(), std::ios::binary);
    if (env_stream.bad())
        std::cout << "wrong file name " << env;

    env_stream.seekg(0, std::ios_base::end);
    const size_t count_of_elements(env_stream.tellg() / sizeof(int));
    env_stream.seekg(0, std::ios_base::beg);
    std::vector<size_t> sizes(count_of_elements);

    int* buf(new int[count_of_elements]);
    env_stream.read(static_cast<char*>(static_cast<void*>(buf)), count_of_elements * sizeof(int));
    env_stream.close();
    for (size_t i(0); i != sizes.size(); ++i) {
        sizes[i] = buf[i];
    }
    delete[] buf;
    table.resize(sizes.size());
    size = sizes.size();

    std::ifstream stif_stream;
    stif_stream.open(stif.c_str(), std::ios::binary);
    size_t fullSize(0);
    stif_stream.seekg(0, std::ios_base::end);
    fullSize = stif_stream.tellg();
    stif_stream.seekg(0, std::ios_base::beg);
    real* buf_m(new real[fullSize]);
    stif_stream.read(static_cast<char*>(static_cast<void*>(buf_m)), fullSize);
    stif_stream.close();
    size_t sum(0);
    for (size_t i=0 ;i<sizes.size(); ++i) {
        std::vector<real> tmpl(sizes[i]);
        for (size_t j=0; j<sizes[i]; ++j) {
            tmpl[sizes[i]-j-1] = buf_m[sum+j];
            if (abs(buf_m[sum+j]) >1e+40) std::cout << "BIG MATRIX    ";
        }
/*        size_t k = i- (sizes[i] - 1);
        std::cout << "\n sizes[i] = " << sizes[i] << "  i=" << i <<"    первый ненулевой" << k << std::endl;
        std::cout << "\n SparseArray \n ============= \n ";
        for (size_t m=0; m<tmpl.size(); m++) {
            std::cout << tmpl[m] << "   ";
        }
        std::cout << "\n Buf \n ============= \n ";
        for (size_t m=0; m<tmpl.size(); m++) {
            std::cout << buf_m[sum+m] << "   ";
        }
        int as;
        std::cin >> as;*/
        table[i] = new SparseArray(tmpl, i - (sizes[i] - 1));
        sum+=sizes[i];
    }
    delete[] buf_m;
}
/*
SparseMatrix::~SparseMatrix() {
    for (size_t i(0); i<size; ++i)
        delete table[i];
    delete[] &table;
}
*/
const real& SparseMatrix::operator () (size_t row, size_t col) {
    return row < col ? (*table[col])[row] : (*table[row])[col];
}

Row& operator *(Row& row, SparseMatrix& m) {
    size_t size = m.getSize();
    Row* ret = new Row(size, 0);
    for (size_t i = 0; i < size; ++i) {
        SparseIterator it(m[i]->end()), end(m[i]->begin(false));
        real sum = 0;
        while(it != end) {
            it.prev();
            size_t crnt_num = it.getNumber();
            real crnt = *it;
            sum += crnt * row[crnt_num];
            if (crnt_num != i)
                (*ret)[crnt_num] += crnt * row[i];
        }
        (*ret)[i] += sum;
    }
    /*for (size_t i = 0; i < size; ++i) {
        std::cout << i << "\t" << (*ret)[i] << std::endl;
    }*/
    return (*ret);
}

SparseMatrix& SparseMatrix::transposition() {
    Table& t = *(new Table(size));
    Row r;
    bool null;
    size_t first_num;
    real crnt;

    for (size_t i = 0; i < size; ++i) {
        first_num = 0;
        null = true;
        for (size_t j = i; j < size; ++j) {
            crnt = (*this)(i,j);
            if (!null)
                r.push_back(crnt);
            else {
                if (crnt == 0)
                    ++first_num;
                else {
                    null = false;
                    if (first_num == 1) {
                        r.push_back(0);
                        first_num = 0;
                    }
                    else
                        if (first_num == 2) {
                            r.push_back(0);
                            r.push_back(0);
                            first_num = 0;
                        }
                    r.push_back(crnt);
                }
            }
        }
        SparseArray* t_i = new SparseArray(r, first_num);
        t[i] = t_i;
        r.clear();
    }
    return *(new SparseMatrix(t));
}
