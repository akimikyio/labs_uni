#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>

static float randFloat(float min, float max) {                  // функция генерации случайного вещественного числа
    return min + (float)rand() / (float)RAND_MAX * (max - min);
}

#endif
