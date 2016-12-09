//
// Created by bahuljain on 12/8/16.
//

#ifndef OPENGLRENDER_VECTOR_H
#define OPENGLRENDER_VECTOR_H

#include <assert.h>
#include <iostream>
#include <math.h>

class Vector {
public:
    float i, j, k;

    Vector() {
        this->i = this->j = this->k = 0;
    };

    Vector(float i, float j, float k) {
        this->i = i;
        this->j = j;
        this->k = k;
    };

    inline float mag() const {
        return sqrtf(powf(i, 2.0) + powf(j, 2.0) + powf(k, 2.0));
    };

    inline Vector norm() const {
        float mag = this->mag();
        assert (mag != 0.0);

        return Vector(i / mag, j / mag, k / mag);
    };

    inline Vector cross(const Vector &vec) const {
        return Vector(j * vec.k - k * vec.j,
                      k * vec.i - i * vec.k,
                      i * vec.j - j * vec.i);
    };

    inline Vector plus(const Vector &vec) const {
        return Vector(i + vec.i, j + vec.j, k + vec.k);
    };

    inline void plusEq(const Vector &vec) {
        i += vec.i;
        j += vec.j;
        k += vec.k;
    };

    inline Vector operator-() const {
        return Vector(-i, -j, -k);
    };

    inline bool equals(const Vector vec) const {
        return i == vec.i && j == vec.j && k == vec.k;
    }

    inline void printVector() const {
        std::cout << i << "i + " << j << "j + " << k << "k" << std::endl;
    };
};



#endif //OPENGLRENDER_VECTOR_H
