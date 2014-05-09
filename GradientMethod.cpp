#include "GradientMethod.h"
#include "CholeskyReversal.h"

// ОТЛАДКА! УДАЛИТЬ!
void print(Column& col) {
    for (size_t i=0; i<col.size(); ++i) {
        std::cout << col[i] << std::endl;
    }
    std::cout << "================" <<std::endl;
}

void printN(Column& col, int n) {
    for (size_t i=0; i<col.size(); i+=n) {
        std::cout << col[i] << std::endl;
    }
    std::cout << "================" <<std::endl;
}

void Gradient::clearResolution() {
    Column::iterator it_u(Unext.begin()),
            it_end(Unext.end());
    for (; it_u!=it_end; ++it_u) {
        *it_u = 0;
    }
}


void Gradient::solveWithCount(size_t count) {
    clearResolution();
    Column r = R;
    Column p = r;
    Column r_next;
    Column p_next;
    r_next.resize(Unext.size());
    p_next.resize(Unext.size());
    if (verify(r)) {
        for (size_t i = 0; i < count; ++i) {
            real a = solveAlpha(r, p); //
            solveUnext(p, a);
            solveRnext(r_next, r, p, a);
            solvePnext(p_next, r_next, p, solveB(r, r_next));
        }
    }
    print(Unext);
}


void Gradient::solveWithPrecision(real e) {
    clearResolution();
    Column r = R;
    Column p = r;
    Column r_next;
    Column p_next;
    //std::cout << "r=" << r.size() << "   R=" << R.size() << "   p=" << p.size() << "\n";
    real normR = solveNorma(R);
    r_next.resize(Unext.size());
    p_next.resize(Unext.size());
    size_t i=0;
    if (verify(r)&&(normR != 0)) {
        do {
            real a = solveAlpha(r, p);
            //std::cout << "a = " << a << " \n";
            solveUnext(p, a);
            solveRnext(r_next, r, p, a);
            solvePnext(p_next, r_next, p, solveB(r, r_next));
            p = p_next;
            r = r_next;
            if (i%100 == 0)
                std::cout << "iteration n" << i << "\n";
            ++i;
        } while (solveNorma(r)/normR > e); //r ИЛИ r_next??
    }
    std::cout << "\n ВЫПОЛНЕНО ЗА " << i << " ИТЕРАЦИЙ" << std::endl;
}

real Gradient::solveNorma(Column &col) {
    Column::iterator it(col.begin()),
            end(col.end());
    real norma = 0;
    for (;it != end; ++it)
        norma += (*it) * (*it);
    return sqrt(norma);
}

bool Gradient::verify(Column& r) {
    for (Column::iterator it(r.begin()), end(r.end());
                                    it != end; it++)
        if (*it != 0) return true;
    return false;
}

real Gradient::solveAlpha(Column &r, Column &p) {
    Column mult = p*K;
    Column::iterator it_r(r.begin()),
            r_end(r.end()),
            it_mult(mult.begin()),
            it_p(p.begin());
    real up = 0;
    real down = 0;
    for (;it_r != r_end; ++it_r, ++it_mult, ++it_p) {
        up += (*it_r) * (*it_r);
        down += (*it_mult) * (*it_p);
    }
    if (down == 0) {
        std::cout << "Division by zero when solveAlpha()";
        return 0;
    }
    return up/down;
}

void Gradient::solveUnext(Column &p, real a) {
//    std::cout << "SOLVE UNEXT";
//    std::cout << p.size() << "=psize    " << Unext.size() << "unextsize=  \n";
    Column::iterator it_p(p.begin()),
            p_end(p.end()),
            it_Unext(Unext.begin());
    for (; it_p != p_end; ++it_p, ++it_Unext) {
//        std::cout << *it_Unext << "  " << *it_p << "  " << a << "\n";
        *it_Unext = *it_Unext + a * (*it_p);
    }
}

void Gradient::solveRnext(Column &r_next, Column &r, Column &p, real a) {
    Column mult = p*K;
    Column::iterator it_r(r.begin()),
            r_end(r.end()),
            it_r_next(r_next.begin()),
            it_mult(mult.begin());
    for (;it_r != r_end; ++it_r, ++it_r_next, ++it_mult) {
        *it_r_next = *it_r - a*(*it_mult);
    }
}

real Gradient::solveB(Column &r, Column &r_next) {
    real up = 0;
    real down = 0;
    Column::iterator it_r(r.begin()),
            r_end(r.end()),
            it_r_next(r_next.begin());
    for (;it_r != r_end; ++it_r_next, ++it_r) {
        up += (*it_r_next) * (*it_r_next);
        down += (*it_r) * (*it_r);
    }
    if (down == 0) {
        std::cout << "Division by zero when solveB()";
        return 0;
    }
    return up/down;
}

void Gradient::solvePnext(Column &p_next, Column &r_next, Column &p, real B) {
    Column::iterator it_p_next(p_next.begin()),
            end_p_next(p_next.end()),
            it_r_next(r_next.begin()),
            it_p(p.begin());
    for (; it_p_next != end_p_next; ++it_p, ++it_p_next, ++it_r_next) {
        *it_p_next = (*it_r_next) + B * (*it_p);
    }
}


/*
 *
 *==========================================
 *
 *
 *
 */

//z_next = CholeskyReversal::reversalEnd(Precondition_t, CholeskyReversal::reversalBegin(Precondition, r_next));
/*
void Gradient::solveWithPrecondition(real e, SparseMatrix &Precondition) {
    Column r = R;
    Column r_next;
    Column p_next;
    Column z;
    Column z_next;
    real normR = solveNorma(R);
    SparseMatrix& Precondition_t = Precondition.transposition();
    r_next.resize(Unext.size());
    p_next.resize(Unext.size());
    z.resize(Unext.size());
    z_next.resize(Unext.size());
    z = CholeskyReversal::reversalEnd(Precondition_t, CholeskyReversal::reversalBegin(Precondition, r));
    int u;
    std::cout << "\n Z\n";
    printN(z,600);
//    std::cin >> u;
    Column p = z;
    size_t i=0;
    if (verify(r)&&(normR != 0)) {
        do {
            real a = solveAlphaPre(r, r, p);

            std::cout << "a = " << a << std::endl;
//            printN(p,1000);
            printN(Unext, 1000);
//            int u;
//            std::cin >> u;

            solveUnext(p, a);
            solveRnext(r_next, r, p, a);

//            std::cout << "\n ZNEXT";
//            printN(z_next,1000);
//            std::cin >> u;

            real B = solveBPre(r_next, r_next, r, r);

//            std::cout << "\n B=" << B << "\n";

            solvePnextPre(p_next, r_next, p, B);

//            std::cout << "\n pnext";
//            printN(p_next,1000);
//            std::cin >> u;

            p = p_next;
            r = r_next;
            z = z_next;
            if (i%100 == 0)
                std::cout << "iteration n" << i << "\n";
            ++i;
        } while (solveNorma(r)/normR > e); //r ИЛИ r_next??
    }
}


real Gradient::solveAlphaPre(Column& z, Column &r, Column &p) {
    Column mult = p*K;
    Column::iterator it_r(r.begin()),
            r_end(r.end()),
            it_mult(mult.begin()),
            it_p(p.begin()),
            it_z(z.begin());
    real up = 0;
    real down = 0;
    for (;it_r != r_end; ++it_r, ++it_mult, ++it_p) {
        up += (*it_r) * (*it_z);
        down += (*it_mult) * (*it_p);
    }
    if (down == 0) {
        std::cout << "Division by zero when solveAlpha()";
        return 0;
    }
    return up/down;
}

real Gradient::solveBPre(Column& z_next, Column& r_next, Column& z, Column& r) {
    real up = 0;
    real down = 0;
    Column::iterator it_z(z.begin()),
            z_end(z.end()),
            it_r_next(r_next.begin()),
            it_z_next(z_next.begin()),
            it_r(r.begin());
    for (;it_z != z_end; ++it_r_next, ++it_r, ++it_z, ++ it_z_next) {
        up += (*it_r_next) * (*it_z_next);
        down += (*it_r) * (*it_z);
    }
    if (down == 0) {
        std::cout << "Division by zero when solveB()";
        return 0;
    }
    return up/down;
}


void Gradient::solvePnextPre(Column& p_next, Column& z_next, Column& p, real B) {
    Column::iterator it_p_next(p_next.begin()),
            end_p_next(p_next.end()),
            it_z_next(z_next.begin()),
            it_p(p.begin());
    for (; it_p_next != end_p_next; ++it_p, ++it_p_next, ++it_z_next) {
        *it_p_next = (*it_z_next) + B * (*it_p);
    }
}
*/

void Gradient::solveWithPrecondition(real e, SparseMatrix &Precondition) {
    clearResolution();
    Column r = R;
    SparseMatrix& Precondition_t = Precondition.transposition();
    Column z = CholeskyReversal::reversalEnd(Precondition_t, CholeskyReversal::reversalBegin(Precondition, r));
    Column p = z;
    Column r_next(r.size(), real(0));
    Column p_next(r.size(), real(0));
    real normR = solveNorma(R);
    real a;
    real B;
    size_t i=0;
    if (normR) {
        do {
            a = solveAlphaPre(z, r, p);
            solveUnext(p,a);
            //std::cout << "a =" << a << std::endl;
            /*std::cout << "Unext\n";
            printN(Unext, 1000);*/
            solveRnext(r_next, r, p, a);
            Column z_next = CholeskyReversal::reversalEnd(Precondition_t, CholeskyReversal::reversalBegin(Precondition, r_next));
            B = solveBPre(z_next, r_next, z, r);
            solvePnextPre(p_next, z_next, p, B);
            /*std::cout << "Pnext\n";
            printN(Unext, 400);*/
            z = z_next;
            r = r_next;
            p = p_next;
            if (i%50 == 0) std::cout << "\n iteration " << i;
            ++i;
        } while(solveNorma(r)/normR > e);
        std::cout << "\n ВЫПОЛНЕНО ЗА " << i << " ИТЕРАЦИЙ" << std::endl;
    }

}

real Gradient::solveAlphaPre(Column &z, Column &r, Column &p) {
    Column mult = p*K;
    real up=0;
    real down=0;
    for (size_t i=0; i<z.size(); i++) {
        up+=z[i]*r[i];
        down+=mult[i]*p[i];
    }
    if (down == 0) {
        std::cout << "Division by zero when solveAlphaPre()";
        return 0;
    }
    return up/down;
}
real Gradient::solveBPre(Column &z_next, Column &r_next, Column &z, Column &r) {
    real up = 0;
    real down = 0;
    for (size_t i=0; i<z_next.size(); ++i) {
        up+=z_next[i]*r_next[i];
        down+=z[i]*r[i];
    }
    if (down == 0) {
        std::cout << "Division by zero when solveBPre()";
        return 0;
    }
    return up/down;
}
void Gradient::solvePnextPre(Column &p_next, Column &z_next, Column &p, real B) {
    for (size_t i=0; i<p_next.size(); ++i) {
        p_next[i] = z_next[i] + B * p[i];
    }
}
