#ifndef HEX_H
#define HEX_H

template<typename Number, int w>
struct _hex
{
    union
    {
        Number V[3];
        struct
        {
            Number Q;
            Number R;
            Number S;
        };

        struct
        {
            Number X;
            Number Y;
            Number Z;
        };
    };

    _hex() : V{0, 0, 0} {};

    _hex(Number _Q, Number _R) : V{_Q, _R, - _Q - _R}
    {
        assert(Q + R + S == 0);
    }

    _hex(Number _Q, Number _R, Number _S) : V{_Q, _R, _S}
    {
        assert(Q + R + S == 0);
    }
};

typedef _hex<s32, 1> hex;
typedef _hex<s32, 0> hex_difference;
typedef _hex<f32, 1> fractional_hex;
typedef _hex<f32, 0> fractional_hex_difference;

bool operator==(hex A, hex B);
bool operator!=(hex A, hex B);
hex operator+(const hex& A, const hex& B);
hex operator-(const hex& A, const hex& B);
hex operator*(s32 I, const hex& A);
hex operator*(const hex& A, s32 I);
u32 Abs(s32 Value);
u32 Length(hex A);
u32 Distance(hex A, hex B);
hex Neighbour(hex A, u32 Direction);

#endif /* HEX_H */
