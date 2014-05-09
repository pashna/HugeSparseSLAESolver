#include "SparseArray.h"
#include <iostream>

real SparseArray::zero = 0;

SparseArray::SparseArray(const SparseArray& _array): length(_array.length) {
    link = *(new Links(_array.link));
    array = *(new Row(_array.array));
}

SparseArray::SparseArray(const Row &_array, const size_t first_num, const bool correct)
    // если first_num != 0 - номер начала первого ненулевого отрезка!
    : length(_array.size() + first_num)
{
    if (!_array.empty()) {
        //link.push_back(Link(0,0));
        bool skipState(!_array[0] && !_array[1] && !_array[2]);//Принадлежит-ли текущий элемент к пропускаемому отрезку...
        size_t i(0);//Порядковый номер в исходном массиве
        size_t size(0); //Сколько элементов попадёт в сокращенный массив
        link.push_back(Link(0,0));
        size_t j(0); // При некорретном вводе first_num: номер начала первого ненулевого отрезка - first_num
        size_t correct_first_num(first_num);
        // Корректировка first_num
        if (first_num) {
            if (!correct) {
                for ( ; j < _array.size() && !_array[j]; ++j);
                //std::cout << j << std::endl << std::endl;
                correct_first_num += j;
                skipState = false;
            }
            link.push_back(Link(correct_first_num, 0));
        }
        for (Row::const_pointer it(_array.data() + j), end(_array.data() + _array.size()); it != end; ++it, ++i) {
            if (skipState) {//если текущий отрезок пропускаемый
                if (*it) {//и встречен ненулевой элемент
                    skipState = false;//заканчиваем отрезок,
                    link.push_back(Link(correct_first_num + i, 0));//вносим информацию о новом (ненулевом) отрезке
                }
            }
            else {//Если текущий отрезок не пропускается
                if (it + 2 < end && !*it && !it[1] && !it[2]) {//Если отрезок кончился
                    skipState = true;//Начинам нулевой отрезок
                    size += i + correct_first_num - (link[link.size() - 1].first);//Увеличиваем количество хранимых элементов на длину отрезка
                    link.push_back(Link(correct_first_num + i, 0));//добавляем информацию о следующем (нулевом) отрезке
                }
            }
        }
        if (!skipState) {//Если исходный массив закончился непропущенным отрезком
            size += _array.size() + first_num - link.at(link.size() - 1).first;
        }
        array.resize(size);

        /*for (i = 0; i < link.size(); ++i) {
            std::cout << link[i].first << " " << link[i].second << std::endl;
        }*/
        Links::iterator it(link.end()), end(link.begin());//Идём по массиву с конца
        const real* border(_array.data() + _array.size());
        while (it != end) {
            --it;
            skipState = !skipState;

            if (skipState) {//Если текущий элемент соответствует ненулевому отрезку
                const real* source(_array.data() + (it->first - first_num));//откуда начать копирование
                //std::cout << it->first << std::endl;
                size -= (border - source);
                //std::cout << size << std::endl;
                real* acceptor(array.data() + size);//куда
                it->second = acceptor;
                while (source != border) {
                    *acceptor = *source;
                    ++acceptor;
                    ++source;
                }
            }
            else {
                int diff = it->first - first_num;
                border = _array.data() + (diff >= 0 ? diff : 0);
            }
        }
    }
    else // if (_array.empty())
        link.push_back(Link(0,0));
    /*for (size_t i = 0; i < array.size(); ++i) {
        std::cout << i << array[i] << std::endl;
    }

        for (i = 0; i < link.size(); ++i) {
            std::cout << link[i].first << " " << link[i].second << std::endl;
        }
*/
    //std::cout << std::endl;
}

size_t SparseArray::toVector(Row& val, const bool null) const {
    size_t first_num = 0;
    Links::const_iterator it(link.end()), end(link.begin());//Идём по массиву с конца
    if (!null && !end->second && link.size() != 1) {
        ++end;
        first_num = end->first;
    }
    val.resize(length - first_num, 0);
    const real* border(array.data() + array.size());
    while (it != end) {
        --it;
        if (it->second) {//Если текущий элемент соответствует ненулевому отрезку
            const real* source(it->second);
            real* acceptor(val.data() + (it->first - first_num));
            //std::cout << it->first << ' ' << it->second << "\n\t" << source << ' ' << border << ' ' << (border - source) << ' ' << (acceptor - val.data()) << std::endl;
            while (source < border) {
                *acceptor = *source;
                ++acceptor;
                ++source;
            }
            border = it->second;
        }
    }
    return first_num;
}

SparseIterator SparseArray::begin(const bool null) {
    SparseIterator ret;
    if (null || link.data()->second || link.size() == 1) {
        ret.pos = link.data();
        ret.len = sparseLen(ret.pos);
        ret.element = ret.pos->second ? array.data() : NULL;
        ret.number = 0;
    }
    else {
        ret.pos = link.data() + 1;
        ret.len = sparseLen(ret.pos);
        ret.element = ret.pos->second ? array.data() : NULL;
        ret.number = ret.pos->first;
    }
    //std::cout << ret.pos << " " << ret.len << " " << ret.element[0] << " " << ret.number << std::endl;
    return ret;
}

SparseIterator SparseArray::end() {
    SparseIterator ret;
    ret.pos = link.data() + link.size() - 1;
    ret.len = sparseLen(ret.pos);
    ret.element = ret.pos->second ? array.data() + array.size() : NULL;
    ret.number = length;
    return ret;
}

real& SparseArray::operator[](size_t pos) const {
    size_t crnt(link.size() / 2);
    size_t s(link.size());
    const Link* d(link.data());
    if (d[crnt].first > pos) {
        crnt = 0;
    }
    while (crnt < s && d[crnt].first <= pos) {
        ++crnt;
    }
    const Link& piece(d[crnt - 1]);
    return piece.second ? piece.second[pos - piece.first] : zero;
}

