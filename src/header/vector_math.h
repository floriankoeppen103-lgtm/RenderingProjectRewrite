#ifndef VECTOR_MATH_H
#define VECTOR_MATH_H

#include <cmath>
#include "types.h"

inline struct vector faceCenter(struct face face1) {
    struct vector solution;
    solution.x = (face1.P1.x + face1.P2.x + face1.P3.x) / 3;
    solution.y = (face1.P1.y + face1.P2.y + face1.P3.y) / 3;
    solution.z = (face1.P1.z + face1.P2.z + face1.P3.z) / 3;
    return solution;
}

inline double length(struct vector VECTOR) {
    return sqrt(VECTOR.x * VECTOR.x + VECTOR.y * VECTOR.y + VECTOR.z * VECTOR.z);
}

inline struct vector normalizedVector(struct vector V) {
    struct vector solution;
    double l = length(V);

    if (l <= 1e-12) {
        return {1e-9, 1e-9, 1e-9};
    }

    solution.x = V.x / l;
    solution.y = V.y / l;
    solution.z = V.z / l;

    return solution;
}

inline struct vector matrixMultiply(struct Matrix3x3 m, struct vector v) {
    vector solution;
    solution.x = m.row1.x * v.x + m.row1.y * v.y + m.row1.z * v.z;
    solution.y = m.row2.x * v.x + m.row2.y * v.y + m.row2.z * v.z;
    solution.z = m.row3.x * v.x + m.row3.y * v.y + m.row3.z * v.z;
    return solution;
}

#endif // VECTOR_MATH_H
