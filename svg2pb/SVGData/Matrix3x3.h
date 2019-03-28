//
//  Matrix3x3.h
//  SVGViewDemo
//
//  Created by Evgen Bodunov on 3/28/19.
//

#ifndef Matrix3x3_h
#define Matrix3x3_h

struct Matrix3x3 {
    double m[3][3];
};

inline Matrix3x3 identMatrix()
{
    return (Matrix3x3)
    {1,0,0,
     0,1,0,
     0,0,1};
}

inline Matrix3x3 rotateMatrix(double angle) {
    angle = angle * M_PI / 180.0;
    return (Matrix3x3)
    {cos(angle), -sin(angle), 0,
     sin(angle), cos(angle) , 0,
     0,0,1};
}

inline Matrix3x3 translateMatrix(double tx, double ty) {
    return (Matrix3x3)
        {1, 0, tx,
         0, 1, ty,
         0, 0, 1};
}

inline Matrix3x3 operator *(Matrix3x3 a, Matrix3x3 b) {
    Matrix3x3 mult = {};
    for (int i=0; i<3; i++)
        for (int j=0; j<3; j++)
            for (int k=0; k<3; k++)
                mult.m[i][j] += a.m[i][k] * b.m[k][j];
    return mult;
}

#endif /* Matrix3x3_h */
