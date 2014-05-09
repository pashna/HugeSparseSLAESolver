#include "CholeskyReversal.h"
#include "GradientMethod.h"
#include "Preconditions.h"
#include "ProfileMatrix.h"
#include "SparseArray.h"
#include "SparseIterator.h"
#include "GradientMethod.h"
#include <iostream>
#include <string>

bool operator ==(const Row& t1, const Row& t2) {
    if (t1.size() != t2.size())
        return false;
    for (size_t i(0); i != t1.size(); ++i)
        if (t1.at(i) != t2.at(i))
            return false;
    return true;
}

int main() {
    struct timespec start, end;
    double diff1;
    clock_gettime (CLOCK_MONOTONIC, &start);

    std::string lv("/home/popka/kursach/simple1_out_model/LV.bin");
    std::ifstream lv_stream;
    lv_stream.open(lv.c_str(), std::ios::binary);
    if (lv_stream.bad())
        std::cout << "wrong file name " << lv;

    lv_stream.seekg(0, std::ios_base::end);
    const size_t count_of_elements(lv_stream.tellg() / sizeof(real));
    lv_stream.seekg(0, std::ios_base::beg);
    std::vector<real> R(count_of_elements);

    real* buf(new real[count_of_elements]);
    lv_stream.read(static_cast<char*>(static_cast<void*>(buf)), count_of_elements * sizeof(real));
    lv_stream.close();
    for (size_t i(0); i != R.size(); ++i) {
        R[i] = buf[i];
    }
    delete[] buf;

    std::string rv("/home/popka/kursach/simple1_out_model/RV.bin");
    std::ifstream rv_stream;
    rv_stream.open(rv.c_str(), std::ios::binary);
    if (rv_stream.bad())
        std::cout << "wrong file name " << rv;

    rv_stream.seekg(0, std::ios_base::end);
    const size_t count_of_rv(rv_stream.tellg() / sizeof(real));
    rv_stream.seekg(0, std::ios_base::beg);
    std::vector<real> Rv(count_of_rv);

    real* buf1(new real[count_of_rv]);
    rv_stream.read(static_cast<char*>(static_cast<void*>(buf1)), count_of_rv * sizeof(real));

    for (size_t i(0); i != Rv.size(); ++i) {
        Rv[i] = buf1[i];
    }
    delete[] buf1;

    SparseMatrix matrix("/home/popka/kursach/simple1_out_model/ENV.bin",
        "/home/popka/kursach/simple1_out_model/STIF.bin");
    Gradient gradient(matrix, R);

    std::cout << "Алгоритм без предобуславлевания запущен" << std::endl;
    clock_gettime (CLOCK_MONOTONIC, &start);

    gradient.solveWithPrecision(1e-8);

    clock_gettime (CLOCK_MONOTONIC, &end);
    diff1 = (end.tv_sec + end.tv_nsec / 1e9) - (start.tv_sec + start.tv_nsec / 1e9);
    std::cout << "Время выполнения алгоритма: " << diff1 << "c\n";

    Column Res = gradient.getResolution();

    for (size_t i=0; i<Res.size(); ++i) {
        if (abs(Res[i] - Rv[i]) > 1e-5) std::cout << "Error: " << Res[i] << "!=" << Rv[i] << std::endl;
    }

    std::cout << "Алгоритм c предобуславлеванием запущен" << std::endl;
    clock_gettime (CLOCK_MONOTONIC, &start);

    SparseMatrix& PreconditionMatrix = Preconditions::diagonallyCompensatedReduction(matrix);
    Preconditions::ilu0(PreconditionMatrix);

    gradient.solveWithPrecondition(1e-8, PreconditionMatrix);
    clock_gettime (CLOCK_MONOTONIC, &end);
    diff1 = (end.tv_sec + end.tv_nsec / 1e9) - (start.tv_sec + start.tv_nsec / 1e9);

    Res = gradient.getResolution();

    for (size_t i=0; i<Res.size(); ++i) {
        if (abs(Res[i] - Rv[i]) > 1e-5) std::cout << "Error: " << Res[i] << "!=" << Rv[i];
    }

    std::cout << "\n Время выполнения алгоритма: " << diff1 << "c" << std::endl;
}


