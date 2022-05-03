#ifndef LAYOUT_H
#define LAYOUT_H

struct orientation
{
    const f32 f0, f1, f2, f3;
    const f32 b0, b1, b2, b3;
    const f32 StartAngle; // in multiples of 60°

    orientation(double f0_, double f1_, double f2_, double f3_,
                double b0_, double b1_, double b2_, double b3_,
                double StartAngle_)
    : f0(f0_), f1(f1_), f2(f2_), f3(f3_),
    b0(b0_), b1(b1_), b2(b2_), b3(b3_),
    StartAngle(StartAngle_) {}

    static orientation Flat()
    {
        return orientation(3.0 / 2.0, 0.0, sqrt(3.0) / 2.0, sqrt(3.0),
                           2.0 / 3.0, 0.0, -1.0 / 3.0, sqrt(3.0) / 3.0,
                           0.0);
    }
};

struct layout
{
    const orientation Orientation;
    v2 Size;
    v2 Origin;

    layout(orientation Orientation_, v2 Size_, v2 Origin_)
        : Orientation(Orientation_), Size(Size_), Origin(Origin_) {}

    static layout& Get()
    {
        static layout Layout(orientation::Flat(), V2(45, 45), V2(0, 0));
        return Layout;
    }
};

std::vector<v2> PolygonCorners(layout Layout, hex Hex);
#endif /* LAYOUT_H */
