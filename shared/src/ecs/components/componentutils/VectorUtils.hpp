/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** VectorUtils
*/

#pragma once

#include "../Vector2.hpp"
#include <cmath>

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

// Dot product
inline float dot(const Vector2& a, const Vector2& b) {
    return a.x * b.x + a.y * b.y;
}

// Magnitude (length) of a vector
inline float magnitude(const Vector2& v) {
    return std::sqrt(v.x * v.x + v.y * v.y);
}

// Squared magnitude (avoids sqrt for performance)
inline float magnitudeSquared(const Vector2& v) {
    return v.x * v.x + v.y * v.y;
}

// Normalize a vector (returns unit vector in same direction)
inline Vector2 normalize(const Vector2& v) {
    float mag = magnitude(v);
    if (mag == 0.0f) {
        return { 0.0f, 0.0f };
    }
    return v / mag;
}

// Calculate direction vector from point A to point B
inline Vector2 direction(const Vector2& from, const Vector2& to) {
    return normalize(to - from);
}

// Distance between two points
inline float distance(const Vector2& a, const Vector2& b) {
    return magnitude(b - a);
}
