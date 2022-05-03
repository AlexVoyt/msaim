#ifndef MATH_H
#define MATH_H

# if 0
#include "typedefs.h"
template<u32 Dim ,class Type>
struct _v
{
    Type V[Dim];

    _v<Dim, Type> operator+(const _v<Dim, Type>& B)
    {
        _v<Dim, Type> Result;
        for(u32 I = 0; I < Dim; I++)
        {
            Result.V[I] = this->V[I] + B.V[I];
        }

        return Result;
    }

    _v<Dim, Type> operator-(const _v<Dim, Type>& B)
    {
        _v<Dim, Type> Result;
        for(u32 I = 0; I < Dim; I++)
        {
            Result.V[I] = this->V[I] - B.V[I];
        }

        return Result;
    }
};

typedef _v<2, f32> v2;
typedef _v<3, f32> v3;

#else
typedef ImVec2 v2;

ImVec2 operator+(ImVec2 A, ImVec2 B)
{
    ImVec2 Result;
    Result.x = A.x + B.x;
    Result.y = A.y + B.y;

    return Result;
}

ImVec2 V2(f32 X, f32 Y)
{
    return ImVec2(X, Y);
}
#endif

#define Pi           3.14159265358979323846

#endif /* MATH_H */
