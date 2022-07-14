#pragma once

struct IntVector2
{
    union
    {
        struct
        {
            Int32 X;
            Int32 Y;
        };

        Int32 XY[2];
    };

    IntVector2() : IntVector2(0, 0) {}
    IntVector2(const Int32 x, const Int32 y) : X(x), Y(y) {}
    IntVector2(const Uint32 x, const Uint32 y) : X((Int32)x), Y((Int32)y) {}
    IntVector2(const Float x, const Float y) : IntVector2(ROUND(x), ROUND(y)) {}

    inline IntVector2 operator+(const IntVector2& v)
    {
        return IntVector2(X + v.X, Y + v.Y);
    }

    inline IntVector2 operator-(const IntVector2& v)
    {
        return IntVector2(X - v.X, Y - v.Y);
    }

    inline IntVector2 operator*(const IntVector2& v)
    {
        return IntVector2(X * v.X, Y * v.Y);
    }

    inline IntVector2 operator+(const Int32 scalar)
    {
        return IntVector2(X + scalar, Y + scalar);
    }

    inline IntVector2 operator-(const Int32 scalar)
    {
        return IntVector2(X - scalar, Y - scalar);
    }

    inline IntVector2 operator*(const Int32 scalar)
    {
        return IntVector2(X * scalar, Y * scalar);
    }

    inline IntVector2 operator*(const Int32 scalar) const
    {
        return IntVector2(X * scalar, Y * scalar);
    }

    inline IntVector2 operator/(const Int32 scalar)
    {
        return IntVector2(X / scalar, Y / scalar);
    }

    inline IntVector2& operator+=(const IntVector2& v)
    {
        X += v.X;
        Y += v.Y;
        return *this;
    }

    inline IntVector2& operator-=(const IntVector2& v)
    {
        X -= v.X;
        Y -= v.Y;
        return *this;
    }

    inline IntVector2& operator*=(const Int32 scalar)
    {
        X *= scalar;
        Y *= scalar;
        return *this;
    }

    inline IntVector2& operator/=(const Int32 scalar)
    {
        X /= scalar;
        Y /= scalar;
        return *this;
    }

    inline IntVector2 Zero()
    {
        return IntVector2(0, 0);
    }

    inline IntVector2 One()
    {
        return IntVector2(1, 1);
    }
};