/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** VectorUtils
*/

#pragma once

#include "../Vector2.hpp"

// Addition
inline Vector2 operator+(const Vector2& a, const Vector2& b) {
    return { a.x + b.x, a.y + b.y };
}

// Subtraction
inline Vector2 operator-(const Vector2& a, const Vector2& b) {
    return { a.x - b.x, a.y - b.y };
}

// Negation
inline Vector2 operator-(const Vector2& v) {
    return { -v.x, -v.y };
}

// Scalar multiplication
inline Vector2 operator*(const Vector2& v, float scalar) {
    return { v.x * scalar, v.y * scalar };
}

inline Vector2 operator*(float scalar, const Vector2& v) {
    return { v.x * scalar, v.y * scalar };
}

// Scalar division
inline Vector2 operator/(const Vector2& v, float scalar) {
    return { v.x / scalar, v.y / scalar };
}

// Component-wise multiplication
inline Vector2 operator*(const Vector2& a, const Vector2& b) {
    return { a.x * b.x, a.y * b.y };
}

// Addition assignment
inline Vector2& operator+=(Vector2& a, const Vector2& b) {
    a.x += b.x;
    a.y += b.y;
    return a;
}

// Subtraction assignment
inline Vector2& operator-=(Vector2& a, const Vector2& b) {
    a.x -= b.x;
    a.y -= b.y;
    return a;
}

// Scalar multiplication assignment
inline Vector2& operator*=(Vector2& v, float scalar) {
    v.x *= scalar;
    v.y *= scalar;
    return v;
}

// Scalar division assignment
inline Vector2& operator/=(Vector2& v, float scalar) {
    v.x /= scalar;
    v.y /= scalar;
    return v;
}

// Equality
inline bool operator==(const Vector2& a, const Vector2& b) {
    return a.x == b.x && a.y == b.y;
}

// Inequality
inline bool operator!=(const Vector2& a, const Vector2& b) {
    return !(a == b);
}
