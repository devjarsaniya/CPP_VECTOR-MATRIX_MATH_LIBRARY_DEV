#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iomanip>
#include <initializer_list>
#include <iostream>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "mathengine/vector3.hpp"

namespace mathengine {

class Matrix {
public:
    Matrix() noexcept = default;
    Matrix(std::size_t rows, std::size_t cols, double fill = 0.0)
        : rows_(rows), cols_(cols), data_(rows * cols, fill) {}

    Matrix(std::initializer_list<std::initializer_list<double>> values) {
        if (values.size() == 0) {
            rows_ = 0;
            cols_ = 0;
            return;
        }

        const std::size_t expected_cols = values.begin()->size();
        rows_ = values.size();
        cols_ = expected_cols;
        data_.reserve(rows_ * cols_);

        for (const auto& row : values) {
            if (row.size() != expected_cols) {
                throw std::invalid_argument("All rows in a matrix initializer list must have the same size.");
            }
            data_.insert(data_.end(), row.begin(), row.end());
        }
    }

    Matrix(const Matrix&) = default;
    Matrix(Matrix&&) noexcept = default;
    Matrix& operator=(const Matrix&) = default;
    Matrix& operator=(Matrix&&) noexcept = default;
    ~Matrix() noexcept = default;

    std::size_t rows() const noexcept { return rows_; }
    std::size_t cols() const noexcept { return cols_; }
    bool empty() const noexcept { return rows_ == 0 || cols_ == 0; }

    void resize(std::size_t rows, std::size_t cols, double fill = 0.0) {
        const std::size_t new_size = rows * cols;
        std::vector<double> new_data(new_size, fill);
        const std::size_t copy_count = std::min(data_.size(), new_size);
        for (std::size_t index = 0; index < copy_count; ++index) {
            new_data[index] = data_[index];
        }
        data_.swap(new_data);
        rows_ = rows;
        cols_ = cols;
    }

    void fill(double value) noexcept { std::fill(data_.begin(), data_.end(), value); }

    double at(std::size_t row, std::size_t col) const {
        validateIndex(row, col);
        return data_[row * cols_ + col];
    }

    double& at(std::size_t row, std::size_t col) {
        validateIndex(row, col);
        return data_[row * cols_ + col];
    }

    double& operator()(std::size_t row, std::size_t col) { return at(row, col); }
    double operator()(std::size_t row, std::size_t col) const { return at(row, col); }

    std::vector<double> row(std::size_t row) const {
        validateIndex(row, 0);
        std::vector<double> values(cols_);
        for (std::size_t col = 0; col < cols_; ++col) {
            values[col] = at(row, col);
        }
        return values;
    }

    std::vector<double> column(std::size_t col) const {
        validateIndex(0, col);
        std::vector<double> values(rows_);
        for (std::size_t row = 0; row < rows_; ++row) {
            values[row] = at(row, col);
        }
        return values;
    }

    Matrix& operator+=(const Matrix& rhs) {
        requireSameDimensions(rhs, "for addition");
        for (std::size_t index = 0; index < data_.size(); ++index) {
            data_[index] += rhs.data_[index];
        }
        return *this;
    }

    Matrix& operator-=(const Matrix& rhs) {
        requireSameDimensions(rhs, "for subtraction");
        for (std::size_t index = 0; index < data_.size(); ++index) {
            data_[index] -= rhs.data_[index];
        }
        return *this;
    }

    Matrix& operator*=(double scalar) noexcept {
        for (double& value : data_) {
            value *= scalar;
        }
        return *this;
    }

    Matrix& operator/=(double scalar) {
        if (std::fabs(scalar) < 1e-12) {
            throw std::invalid_argument("Cannot divide a matrix by zero.");
        }
        for (double& value : data_) {
            value /= scalar;
        }
        return *this;
    }

    Matrix operator+(const Matrix& rhs) const {
        Matrix result(*this);
        result += rhs;
        return result;
    }

    Matrix operator-(const Matrix& rhs) const {
        Matrix result(*this);
        result -= rhs;
        return result;
    }

    Matrix operator*(const Matrix& rhs) const {
        if (cols_ != rhs.rows_) {
            throw std::invalid_argument("Matrix dimensions are incompatible for multiplication.");
        }

        Matrix result(rows_, rhs.cols_);
        for (std::size_t row = 0; row < rows_; ++row) {
            for (std::size_t col = 0; col < rhs.cols_; ++col) {
                double sum = 0.0;
                for (std::size_t inner = 0; inner < cols_; ++inner) {
                    sum += at(row, inner) * rhs.at(inner, col);
                }
                result(row, col) = sum;
            }
        }
        return result;
    }

    Vector3 operator*(const Vector3& vector) const {
        if (cols_ != 3 || rows_ != 3) {
            throw std::invalid_argument("Matrix-vector multiplication requires a 3x3 matrix.");
        }
        return Vector3{
            at(0, 0) * vector.x() + at(0, 1) * vector.y() + at(0, 2) * vector.z(),
            at(1, 0) * vector.x() + at(1, 1) * vector.y() + at(1, 2) * vector.z(),
            at(2, 0) * vector.x() + at(2, 1) * vector.y() + at(2, 2) * vector.z()
        };
    }

    Matrix operator*(double scalar) const {
        Matrix result(*this);
        result *= scalar;
        return result;
    }

    Matrix operator/(double scalar) const {
        Matrix result(*this);
        result /= scalar;
        return result;
    }

    friend Matrix operator*(double scalar, const Matrix& matrix) { return matrix * scalar; }

    Matrix transpose() const {
        Matrix result(cols_, rows_);
        for (std::size_t row = 0; row < rows_; ++row) {
            for (std::size_t col = 0; col < cols_; ++col) {
                result(col, row) = at(row, col);
            }
        }
        return result;
    }

    static Matrix identity(std::size_t size) {
        if (size == 0) {
            throw std::invalid_argument("Identity matrix size must be greater than zero.");
        }
        Matrix result(size, size, 0.0);
        for (std::size_t index = 0; index < size; ++index) {
            result(index, index) = 1.0;
        }
        return result;
    }

    bool operator==(const Matrix& rhs) const noexcept {
        return rows_ == rhs.rows_ && cols_ == rhs.cols_ && data_ == rhs.data_;
    }

    bool operator!=(const Matrix& rhs) const noexcept { return !(*this == rhs); }

    std::string toString() const {
        std::ostringstream stream;
        stream << std::fixed << std::setprecision(3);
        for (std::size_t row = 0; row < rows_; ++row) {
            stream << "[ ";
            for (std::size_t col = 0; col < cols_; ++col) {
                stream << std::setw(7) << at(row, col);
                if (col + 1 < cols_) {
                    stream << ' ';
                }
            }
            stream << " ]\n";
        }
        return stream.str();
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
        os << matrix.toString();
        return os;
    }

private:
    void validateIndex(std::size_t row, std::size_t col) const {
        if (row >= rows_ || col >= cols_) {
            throw std::out_of_range("Matrix index is out of bounds.");
        }
    }

    void requireSameDimensions(const Matrix& rhs, const char* operation) const {
        if (rows_ != rhs.rows_ || cols_ != rhs.cols_) {
            throw std::invalid_argument(std::string("Matrix dimensions must match ") + operation + ".");
        }
    }

    std::size_t rows_ = 0;
    std::size_t cols_ = 0;
    std::vector<double> data_;
};

namespace utility {

inline Matrix randomMatrix(std::size_t rows, std::size_t cols, double min = -1.0, double max = 1.0) {
    static std::mt19937 generator{std::random_device{}()};
    std::uniform_real_distribution<double> distribution(min, max);
    Matrix matrix(rows, cols);
    for (std::size_t row = 0; row < rows; ++row) {
        for (std::size_t col = 0; col < cols; ++col) {
            matrix(row, col) = distribution(generator);
        }
    }
    return matrix;
}

}  // namespace utility

}  // namespace mathengine
