#ifndef LEVELARRAY_H
#define LEVELARRAY_H
#include <vector>
#include <stdio.h>
#include <stdlib.h>

typedef std::vector<size_t> LevelRow;

class LevelArray {
public:
    LevelArray(LevelRow& _array, size_t _first_num) : array(_array), first_num(_first_num) {}
    size_t& operator [] (size_t num) {
        return array[num - first_num];
    }
    size_t getFirstNum() {
        return first_num;
    }

private:
    LevelRow array;
    size_t first_num;
};

#endif // LEVELARRAY_H
