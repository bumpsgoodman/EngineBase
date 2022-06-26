#pragma once

// RGBA
struct Color
{
    Color() : Color(0.0f, 0.0f, 0.0f, 0.0f) {}

    Color(const Float rgba[4])
        : Color(rgba[0], rgba[1], rgba[2], rgba[3])
    {
    }

    Color(const Float r, const Float g, const Float b, const Float a)
        : R(r)
        , G(g)
        , B(b)
        , A(a)
    {
    }

    Color(const Uint32 r, const Uint32 g, const Uint32 b, const Uint32 a)
        : Color((Float)r, (Float)g, (Float)b, (Float)a)
    {
    }

    union
    {
        Float RGBA[4];

        struct
        {
            Float R;
            Float G;
            Float B;
            Float A;
        };
    };

    static Uint32 ToHex(const Color color)
    {
        Uint32 val =
            ((Uint32)(color.R * 255.0f) << 24
                | (Uint32)(color.G * 255.0f) << 16
                | (Uint32)(color.B * 255.0f) << 8
                | (Uint32)(color.A * 255.0f));
        return val;
    }

    static Uint32 ToARGBHex(const Color color)
    {
        Uint32 val =
            ((Uint32)(color.A * 255.0f) << 24
                | (Uint32)(color.R * 255.0f) << 16
                | (Uint32)(color.G * 255.0f) << 8
                | (Uint32)(color.B * 255.0f));
        return val;
    }
};

// RGBA
namespace colors
{
    const Color WHITE = { 1.000000f, 1.0000000f, 1.0000000f, 1.000000f };
    const Color BLACK = { 0.000000f, 0.0000000f, 0.0000000f, 1.000000f };
    const Color RED = { 1.000000f, 0.0000000f, 0.0000000f, 1.000000f };
    const Color BLUE = { 0.000000f, 0.0000000f, 1.0000000f, 1.000000f };
    const Color GREEN = { 0.000000f, 1.0000000f, 0.0000000f, 1.000000f };
}