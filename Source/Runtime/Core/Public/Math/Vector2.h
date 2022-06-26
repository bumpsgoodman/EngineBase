#pragma once

struct Vector2
{
    union
    {
        struct
        {
            Float X;
            Float Y;
        };

        Float XY[2];
    };

    Vector2() : Vector2(0.0f, 0.0f) {}
    Vector2(const Int32 x, const Int32 y) : Vector2((Float)x, (Float)y) {}
    Vector2(const Float x, const Float y) : X(x), Y(y) {}

    inline Vector2 operator+(const Vector2& v)
    {
        return Vector2(X + v.X, Y + v.Y);
    }

    inline Vector2 operator-(const Vector2& v)
    {
        return Vector2(X - v.X, Y - v.Y);
    }

    inline Vector2 operator*(const Vector2& v)
    {
        return Vector2(X * v.X, Y * v.Y);
    }

    inline Vector2 operator+(const Float scalar)
    {
        return Vector2(X + scalar, Y + scalar);
    }

    inline Vector2 operator-(const Float scalar)
    {
        return Vector2(X - scalar, Y - scalar);
    }

    inline Vector2 operator*(const Float scalar)
    {
        return Vector2(X * scalar, Y * scalar);
    }

    inline Vector2 operator/(const Float scalar)
    {
        return Vector2(X / scalar, Y / scalar);
    }

    inline Vector2& operator+=(const Vector2& v)
    {
        X += v.X;
        Y += v.Y;
        return *this;
    }

    inline Vector2& operator-=(const Vector2& v)
    {
        X -= v.X;
        Y -= v.Y;
        return *this;
    }

    inline Vector2& operator*=(const Float scalar)
    {
        X *= scalar;
        Y *= scalar;
        return *this;
    }

    inline Vector2& operator/=(const Float scalar)
    {
        X /= scalar;
        Y /= scalar;
        return *this;
    }

    inline Vector2 Zero()
    {
        return Vector2(0, 0);
    }

    inline Vector2 One()
    {
        return Vector2(1, 1);
    }
};