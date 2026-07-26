#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

namespace mathengine {

class Vector3 {
public:
    using value_type = double;

    // Default and explicit constructors.

    Vector3() noexcept = default;
    Vector3(value_type x, value_type y, value_type z) noexcept : x_(x), y_(y), z_(z) {}
    Vector3(const Vector3&) noexcept = default;
    Vector3(Vector3&&) noexcept = default;
    Vector3& operator=(const Vector3&) noexcept = default;
    Vector3& operator=(Vector3&&) noexcept = default;
    ~Vector3() noexcept = default;

    // Component accessors and mutators.
    value_type x() const noexcept { return x_; }
    value_type y() const noexcept { return y_; }
    value_type z() const noexcept { return z_; }

    void setX(value_type x) noexcept { x_ = x; }
    void setY(value_type y) noexcept { y_ = y; }
    void setZ(value_type z) noexcept { z_ = z; }

    // Compound arithmetic operators.
    Vector3& operator+=(const Vector3& rhs) noexcept {
        x_ += rhs.x_;
        y_ += rhs.y_;
        z_ += rhs.z_;
        return *this;
    }

    Vector3& operator-=(const Vector3& rhs) noexcept {
        x_ -= rhs.x_;
        y_ -= rhs.y_;
        z_ -= rhs.z_;
        return *this;
    }

    Vector3& operator*=(value_type scalar) noexcept {
        x_ *= scalar;
        y_ *= scalar;
        z_ *= scalar;
        return *this;
    }

    Vector3& operator/=(value_type scalar) {
        if (scalar == 0.0) {
            throw std::invalid_argument("Cannot divide a vector by zero.");
        }
        x_ /= scalar;
        y_ /= scalar;
        z_ /= scalar;
        return *this;
    }

    // Binary and unary arithmetic operators.
    Vector3 operator+(const Vector3& rhs) const noexcept { return Vector3{x_ + rhs.x_, y_ + rhs.y_, z_ + rhs.z_}; }
    Vector3 operator-(const Vector3& rhs) const noexcept { return Vector3{x_ - rhs.x_, y_ - rhs.y_, z_ - rhs.z_}; }
    Vector3 operator-() const noexcept { return Vector3{-x_, -y_, -z_}; }

    bool operator==(const Vector3& rhs) const noexcept {
        return std::fabs(x_ - rhs.x_) < 1e-12 && std::fabs(y_ - rhs.y_) < 1e-12 && std::fabs(z_ - rhs.z_) < 1e-12;
    }

    bool operator!=(const Vector3& rhs) const noexcept { return !(*this == rhs); }

    // Vector algebra helpers.
    value_type dot(const Vector3& rhs) const noexcept { return x_ * rhs.x_ + y_ * rhs.y_ + z_ * rhs.z_; }
    Vector3 cross(const Vector3& rhs) const noexcept {
        return Vector3{
            y_ * rhs.z_ - z_ * rhs.y_,
            z_ * rhs.x_ - x_ * rhs.z_,
            x_ * rhs.y_ - y_ * rhs.x_
        };
    }

    value_type magnitude() const noexcept { return std::sqrt(x_ * x_ + y_ * y_ + z_ * z_); }
    value_type squaredMagnitude() const noexcept { return x_ * x_ + y_ * y_ + z_ * z_; }

    // Normalization and geometric utilities.
    Vector3& normalize() {
        const value_type len = magnitude();
        if (len <= 1e-12) {
            throw std::runtime_error("Cannot normalize a zero-length vector.");
        }
        *this /= len;
        return *this;
    }

    Vector3 normalized() const {
        Vector3 copy(*this);
        copy.normalize();
        return copy;
    }

    value_type distanceTo(const Vector3& rhs) const noexcept {
        return (*this - rhs).magnitude();
    }

    value_type angleTo(const Vector3& rhs) const {
        const value_type denom = magnitude() * rhs.magnitude();
        if (denom <= 1e-12) {
            throw std::runtime_error("Cannot compute angle with a zero-length vector.");
        }
        const value_type cosine = dot(rhs) / denom;
        return std::acos(std::max(-1.0, std::min(1.0, cosine)));
    }

    Vector3 lerp(const Vector3& rhs, value_type t) const {
        if (t < 0.0 || t > 1.0) {
            throw std::invalid_argument("Interpolation parameter t must be in the range [0, 1].");
        }
        return Vector3{
            x_ + (rhs.x_ - x_) * t,
            y_ + (rhs.y_ - y_) * t,
            z_ + (rhs.z_ - z_) * t
        };
    }

    std::string toString() const {
        std::ostringstream stream;
        stream << std::fixed << std::setprecision(3) << "(" << x_ << ", " << y_ << ", " << z_ << ")";
        return stream.str();
    }

    // Common basis and zero vectors.
    static Vector3 zero() noexcept { return Vector3{0.0, 0.0, 0.0}; }
    static Vector3 unitX() noexcept { return Vector3{1.0, 0.0, 0.0}; }
    static Vector3 unitY() noexcept { return Vector3{0.0, 1.0, 0.0}; }
    static Vector3 unitZ() noexcept { return Vector3{0.0, 0.0, 1.0}; }

    friend Vector3 operator*(value_type scalar, const Vector3& vector) noexcept {
        return Vector3{vector.x_ * scalar, vector.y_ * scalar, vector.z_ * scalar};
    }

    friend Vector3 operator*(const Vector3& vector, value_type scalar) noexcept {
        return scalar * vector;
    }

    friend Vector3 operator/(const Vector3& vector, value_type scalar) {
        if (scalar == 0.0) {
            throw std::invalid_argument("Cannot divide a vector by zero.");
        }
        return Vector3{vector.x_ / scalar, vector.y_ / scalar, vector.z_ / scalar};
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector3& vector) {
        os << vector.toString();
        return os;
    }

    friend std::istream& operator>>(std::istream& is, Vector3& vector) {
        value_type x = 0.0;
        value_type y = 0.0;
        value_type z = 0.0;
        if (!(is >> x >> y >> z)) {
            throw std::runtime_error("Failed to read Vector3 from input stream.");
        }
        vector = Vector3{x, y, z};
        return is;
    }

private:
    value_type x_ = 0.0;
    value_type y_ = 0.0;
    value_type z_ = 0.0;
};

namespace utility {

inline Vector3 randomVector(double min = -1.0, double max = 1.0) {
    static std::mt19937 generator{std::random_device{}()};
    std::uniform_real_distribution<double> distribution(min, max);
    return Vector3{distribution(generator), distribution(generator), distribution(generator)};
}

}  // namespace utility

}  // namespace mathengine
