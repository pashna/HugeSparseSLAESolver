#ifndef PRECONDITIONS_H
#define PRECONDITIONS_H
#include "ProfileMatrix.h"
#include "SparseArray.h"
#include "SparseMatrix.h"
#include "LevelMatrix.h"
#include <iostream>

class Preconditions {
public:
    // ILU(0)
    // version 1: с использованием AjizJennings / + diagonallyCompensatedReduction
    static void ilu0(SparseMatrix& a) { // a - изменяемая копия матрицы a / матрица, полученная из a с помощью diagonallyCompensatedReduction
        size_t size(a.getSize());
        Row crnt;

        for (size_t i = 0; i < size; ++i) {
            SparseIterator it(a[i]->begin(false)), end(a[i]->end());
            size_t first_num = it.getNumber();
            crnt.resize(i + 1 - first_num);
            for ( ; it != end; ++it) {
                size_t a_num = it.getNumber(); // глобальный номер, по матрице a
                size_t crnt_num = a_num - first_num; // локальный номер, по массиву crnt (с учетом first_num)
                crnt[crnt_num] = *it;
                if (a_num < i) {
                    SparseIterator it_k(--a[a_num]->end()), end_k(a[a_num]->begin(false));
                    real a_crntnum_crntnum = *it_k;
                    while (it_k != end_k) {
                        it_k.prev();
                        int it_k_crnt_number(it_k.getNumber() - first_num);
                        if (it_k_crnt_number >= 0) {
                            crnt[crnt_num] -= crnt[it_k_crnt_number] * *it_k;
                        }
                    }
                    crnt[crnt_num] /= a_crntnum_crntnum;
                    if (!*it) {
                        AjizJennings(a, abs(crnt[crnt_num]), i, a_num);
                        crnt[crnt_num] = 0;
                    }
                }
                else {
                    for (size_t k = 0; k < i - first_num; ++k) {
                        crnt[i - first_num] -= crnt[k] * crnt[k];
                    }
                    crnt[i - first_num] = sqrt(crnt[i - first_num]);
                }
            }

            // отладка
            /*for (size_t zu = 0; zu < crnt.size(); ++zu) {
                std::cout << crnt[zu] << " " << i << std::endl;
            }*/
            // конец отладки

            SparseArray *a_i = new SparseArray(crnt, first_num, false);
            delete a[i];
            a[i] = a_i;
            crnt.clear();
        }
    }

    // version 2: без всего / с использованием diagonallyCompensatedReduction
    static void ilu0_DCR(SparseMatrix& a) { // a - изменяемая копия матрицы a / матрица, полученная из a с помощью diagonallyCompensatedReduction
        size_t size(a.getSize());
        Row crnt;

        for (size_t i = 0; i < size; ++i) {
            SparseIterator it(a[i]->begin(false)), end(a[i]->end());
            size_t first_num = it.getNumber();
            crnt.resize(i + 1 - first_num);

            for (; it != end; ++it) { // it.next();
                if (*it) {
                    size_t a_num = it.getNumber(); // глобальный номер, по матрице a
                    size_t crnt_num = a_num - first_num; // локальный номер, по массиву crnt (с учетом first_num)
                    crnt[crnt_num] = *it;
                    if (a_num < i) {
                        SparseIterator it_k(--a[a_num]->end()), end_k(a[a_num]->begin());
                        real a_crntnum_crntnum = *it_k;
                        while (it_k != end_k) {
                            it_k.prev();
                            int it_k_crnt_number(it_k.getNumber() - first_num);
                            if (it_k_crnt_number >= 0) {
                                crnt[crnt_num] -= crnt[it_k_crnt_number] * *it_k;
                            }
                        }
                        crnt[crnt_num] /= a_crntnum_crntnum;
                    }
                    else {
                        for (size_t k = 0; k < i - first_num; ++k)
                            crnt[i - first_num] -= crnt[k] * crnt[k];
                        crnt[i - first_num] = sqrt(crnt[i - first_num]);
                    }
                }
            }

            // отладка
            /*for (size_t zu = 0; zu < crnt.size(); ++zu) {
                std::cout << crnt[zu] << " " << i << std::endl;
            }*/
            // конец отладки

            SparseArray *a_i = new SparseArray(crnt, first_num, false);
            delete a[i];
            a[i] = a_i;
            crnt.clear();
        }
    }
/*
    // ILU(P)
    // version 1: с использованием AjizJennings / + diagonallyCompensatedReduction
    static void iluP(SparseMatrix& a, size_t p) { // a - изменяемая копия матрицы a / матрица, полученная из a с помощью diagonallyCompensatedReduction
        size_t size(a.getSize());
        Row crnt;
        LevelMatrix lvl(a);
        for (size_t i = 0; i < size; ++i) {
            SparseIterator it(a[i]->begin(false)), end(a[i]->end());
            size_t first_num = it.getNumber();
            crnt.resize(i + 1 - first_num);
            for ( ; it != end; ++it) {
                size_t a_num = it.getNumber(); // глобальный номер, по матрице a
                size_t crnt_num = a_num - first_num; // локальный номер, по массиву crnt (с учетом first_num)
                crnt[crnt_num] = *it;
                if (a_num < i) {
                    SparseIterator it_k(--a[a_num]->end()), end_k(a[a_num]->begin(false));
                    real a_crntnum_crntnum = *it_k;
                    while (it_k != end_k) {
                        it_k.prev();
                        int it_k_crnt_number(it_k.getNumber() - first_num);
                        if (it_k_crnt_number >= 0) {
                            crnt[crnt_num] -= crnt[it_k_crnt_number] * *it_k;
                        }
                    }
                    crnt[crnt_num] /= a_crntnum_crntnum;
                    if (lvl[i][it.getNumber()] > p) {
                        AjizJennings(a, abs(crnt[crnt_num]), i, a_num);
                        crnt[crnt_num] = 0;
                    }
                }
                else {
                    for (size_t k = 0; k < i - first_num; ++k) {
                        crnt[i - first_num] -= crnt[k] * crnt[k];

                    }
                    crnt[i - first_num] = sqrt(crnt[i - first_num]);
                }
            }

            // отладка
            /*for (size_t zu = 0; zu < crnt.size(); ++zu) {
                std::cout << crnt[zu] << " " << i << std::endl;
            }/
            // конец отладки

            SparseArray *a_i = new SparseArray(crnt, first_num, false);
            delete a[i];
            a[i] = a_i;
            crnt.clear();
        }
    }

    // version 2: без всего / с использованием diagonallyCompensatedReduction
    static void iluP_DCR(SparseMatrix& a) { // a - изменяемая копия матрицы a / матрица, полученная из a с помощью diagonallyCompensatedReduction
        size_t size(a.getSize());
        Row crnt;

        for (size_t i = 0; i < size; ++i) {
            SparseIterator it(a[i]->begin(false)), end(a[i]->end());
            size_t first_num = it.getNumber();
            crnt.resize(i + 1 - first_num);

            for (; it != end; ++it) { // it.next();
                if (*it) {
                    size_t a_num = it.getNumber(); // глобальный номер, по матрице a
                    size_t crnt_num = a_num - first_num; // локальный номер, по массиву crnt (с учетом first_num)
                    crnt[crnt_num] = *it;
                    if (a_num < i) {
                        SparseIterator it_k(--a[a_num]->end()), end_k(a[a_num]->begin());
                        real a_crntnum_crntnum = *it_k;
                        while (it_k != end_k) {
                            it_k.prev();
                            int it_k_crnt_number(it_k.getNumber() - first_num);
                            if (it_k_crnt_number >= 0) {
                                crnt[crnt_num] -= crnt[it_k_crnt_number] * *it_k;
                            }
                        }
                        crnt[crnt_num] /= a_crntnum_crntnum;
                    }
                    else {
                        for (size_t k = 0; k < i - first_num; ++k)
                            crnt[i - first_num] -= crnt[k] * crnt[k];
                        crnt[i - first_num] = sqrt(crnt[i - first_num]);
                    }
                }
            }

            // отладка
            /*for (size_t zu = 0; zu < crnt.size(); ++zu) {
                std::cout << crnt[zu] << " " << i << std::endl;
            }/
            // конец отладки

            SparseArray *a_i = new SparseArray(crnt, first_num, false);
            delete a[i];
            a[i] = a_i;
            crnt.clear();
        }
    }
*/
    static SparseMatrix& diagonallyCompensatedReduction(SparseMatrix& a) {
        size_t size(a.getSize());
        size_t crnt_num;
        std::vector<size_t> nums;
        Row tmp;
        Table t = *(new Table(size));

        for (size_t i = 0; i < size; ++i) {
            SparseIterator it(a[i]->end()), end(a[i]->begin(false));
            while (it != end) {
                it.prev();
                if (*it > 0 && (crnt_num = it.getNumber()) != i) {
                    nums.push_back(crnt_num);
                }
            }
            if (!nums.empty()) {
                size_t first_num = a[i]->toVector(tmp, false);
                for (size_t j = 0; j < nums.size(); ++j) {
                    crnt_num = nums[j];
                    real crnt = tmp[crnt_num - first_num];
                    tmp[crnt_num - first_num] = 0;
                    (*t[crnt_num])[crnt_num] += crnt * 2;
                    tmp[i - first_num] += crnt * 2;
                }
                SparseArray* t_i = new SparseArray(tmp, first_num, false);
                t[i] = t_i;
                tmp.clear();
                nums.clear();
            }
            else
                t[i] = new SparseArray(*a[i]);
        }
        return *(new SparseMatrix(t));
    }

private:
    static void AjizJennings(SparseMatrix& a, const real& abs_l_ij, const size_t i, const size_t j) {
        (*a[i])[i] += abs_l_ij;
        (*a[j])[j] += abs_l_ij;
    }
};

#endif // PRECONDITIONS_H
