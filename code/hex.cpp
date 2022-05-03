#ifndef HEX_CPP
#define HEX_CPP

#include "hex.h"

static const hex _HexDirections[] =
{
    hex(1, 0, -1), hex(1, -1, 0), hex(0, -1, 1),
    hex(-1, 0, 1), hex(-1, 1, 0), hex(0, 1, -1)
};

bool operator==(hex A, hex B)
{
    return A.Q == B.Q && A.R == B.R && A.S == B.S;
}

bool operator!=(hex A, hex B)
{
    return !(A == B);
}

hex operator+(const hex& A, const hex& B)
{
    return hex(A.Q + B.Q, A.R + B.R, A.S + B.S);
}

hex operator-(const hex& A, const hex& B)
{
    return hex(A.Q - B.Q, A.R - B.R, A.S - B.S);
}

hex operator*(s32 I, const hex& A)
{
    return hex(I * A.Q, I * A.R, I * A.S);
}

hex operator*(const hex& A, s32 I)
{
    return I*A;
}

u32 Abs(s32 Value)
{
    return Value >= 0 ? Value : -Value;
}

u32 Length(hex A)
{
    u32 Result = Abs(A.Q) + Abs(A.R) + Abs(A.S);
    return Result/2;
}

u32 Distance(hex A, hex B)
{
    return Length(A - B);
}

hex HexDirection(u32 Index)
{
    assert(Index >= 0 && Index <= 5);
    return _HexDirections[Index];
}

hex Neighbour(hex A, u32 Direction)
{
    return A + HexDirection(Direction);
}

hex Round(fractional_hex Hex)
{
    s32 Q = s32(round(Hex.Q));
    s32 R = s32(round(Hex.R));
    s32 S = s32(round(Hex.S));

    f32 QDiff = abs(Q - Hex.Q);
    f32 RDiff = abs(R - Hex.R);
    f32 SDiff = abs(S - Hex.S);
    if (QDiff > RDiff and QDiff > SDiff) {
        Q = -R - S;
    } else if (RDiff > SDiff) {
        R = -Q - S;
    } else {
        S = -Q - R;
    }
    return hex(Q, R, S);
}


#endif /* HEX_CPP */
