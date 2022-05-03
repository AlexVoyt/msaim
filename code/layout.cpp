#ifndef LAYOUT_CPP
#define LAYOUT_CPP

#include "layout.h"

v2 HexToPixel(layout Layout, hex Hex)
{
    const orientation& M = Layout.Orientation;
    double X = (M.f0 * Hex.Q + M.f1 * Hex.R) * Layout.Size.x;
    double Y = (M.f2 * Hex.Q + M.f3 * Hex.R) * Layout.Size.y;

    return v2(X + Layout.Origin.x, Y + Layout.Origin.y);
}

v2 HexCornerOffset(layout Layout, u32 Corner)
{
    v2 Size = Layout.Size;
    f32 Angle = 2 * Pi * (Layout.Orientation.StartAngle + Corner) / 6;

    return V2(Size.x * cos(Angle), Size.y * sin(Angle));
}

std::vector<v2> PolygonCorners(layout Layout, hex Hex)
{
    std::vector<v2> Result;
    Result.reserve(6);

    v2 Center = HexToPixel(Layout, Hex);
    for(u32 Index = 0; Index < 6; Index++)
    {
        v2 Offset = HexCornerOffset(Layout, Index);
        Result.push_back(v2(Center.x + Offset.x,
                            Center.y + Offset.y));
    }

    return Result;
}

#if 1
fractional_hex PixelToHex(layout Layout, v2 Point)
{
    const orientation& Orientation = Layout.Orientation;
    v2 pt = V2((Point.x - Layout.Origin.x) / Layout.Size.x,
               (Point.y - Layout.Origin.y) / Layout.Size.y);

    f32 Q = Orientation.b0 * pt.x + Orientation.b1 * pt.y;
    f32 R = Orientation.b2 * pt.x + Orientation.b3 * pt.y;
    return fractional_hex(Q, R, -Q - R);
}
#endif



#endif /* LAYOUT_CPP */
