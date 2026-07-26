#include <cassert>
#include <cmath>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "mathengine/mathengine.hpp"

int main() {
    using namespace mathengine;

    Vector3 a(1.0, 2.0, 3.0);
    Vector3 b(4.0, 5.0, 6.0);

    assert((a + b) == Vector3(5.0, 7.0, 9.0));
    assert((a - b) == Vector3(-3.0, -3.0, -3.0));
    assert((-a) == Vector3(-1.0, -2.0, -3.0));
    assert(a != b);
    assert(!(a == b));

    Vector3 sum_assign = a;
    sum_assign += b;
    assert(sum_assign == Vector3(5.0, 7.0, 9.0));

    Vector3 diff_assign = a;
    diff_assign -= b;
    assert(diff_assign == Vector3(-3.0, -3.0, -3.0));

    Vector3 scalar_mul = a;
    scalar_mul *= 2.0;
    assert(scalar_mul == Vector3(2.0, 4.0, 6.0));

    Vector3 scalar_div = a;
    scalar_div /= 2.0;
    assert(scalar_div == Vector3(0.5, 1.0, 1.5));

    assert((a * 2.0) == Vector3(2.0, 4.0, 6.0));
    assert((2.0 * a) == Vector3(2.0, 4.0, 6.0));
    assert((a / 2.0) == Vector3(0.5, 1.0, 1.5));
    assert(a.dot(b) == 32.0);
    assert(a.cross(b) == Vector3(-3.0, 6.0, -3.0));
    assert(a.magnitude() > 3.0);
    assert(a.squaredMagnitude() == 14.0);
    assert(a.normalized().magnitude() > 0.99 && a.normalized().magnitude() < 1.01);
    Vector3 normalized_copy = a.normalized();
    assert(normalized_copy.magnitude() > 0.99 && normalized_copy.magnitude() < 1.01);
    assert(std::fabs(a.distanceTo(b) - std::sqrt(27.0)) < 1e-12);
    assert(std::fabs(Vector3::unitX().angleTo(Vector3::unitY()) - (3.141592653589793 / 2.0)) < 1e-6);
    assert((a.lerp(b, 0.5)) == Vector3(2.5, 3.5, 4.5));
    assert(Vector3::zero() == Vector3(0.0, 0.0, 0.0));
    assert(Vector3::unitX() == Vector3(1.0, 0.0, 0.0));
    assert(Vector3::unitY() == Vector3(0.0, 1.0, 0.0));
    assert(Vector3::unitZ() == Vector3(0.0, 0.0, 1.0));

    std::ostringstream stream;
    stream << a;
    assert(stream.str() == "(1.000, 2.000, 3.000)");

    std::istringstream input("4 5 6");
    Vector3 read_back;
    input >> read_back;
    assert(read_back == Vector3(4.0, 5.0, 6.0));

    Matrix m1{{1.0, 2.0}, {3.0, 4.0}};
    Matrix m2{{5.0, 6.0}, {7.0, 8.0}};
    Matrix sum = m1 + m2;
    assert(sum(0, 0) == 6.0);
    assert(sum(1, 1) == 12.0);

    Matrix diff = m2 - m1;
    assert(diff(0, 0) == 4.0);
    assert(diff(1, 1) == 4.0);

    Matrix product = m1 * m2;
    assert(product(0, 0) == 19.0);
    assert(product(0, 1) == 22.0);
    assert(product(1, 0) == 43.0);
    assert(product(1, 1) == 50.0);

    Matrix scaled = m1 * 2.0;
    assert(scaled(0, 1) == 4.0);

    Matrix divided = m1 / 2.0;
    assert(divided(0, 0) == 0.5);

    Matrix identity = Matrix::identity(3);
    assert(identity(0, 0) == 1.0);
    assert(identity(2, 2) == 1.0);

    Matrix transposed = m1.transpose();
    assert(transposed(0, 1) == 3.0);
    assert(transposed(1, 0) == 2.0);

    Matrix copied = m1;
    copied(0, 0) = 9.0;
    assert(m1(0, 0) == 1.0);
    assert(copied(0, 0) == 9.0);

    Matrix moved = std::move(copied);
    assert(moved(0, 0) == 9.0);

    std::vector<double> row = m2.row(0);
    assert(row.size() == 2u);
    assert(row[0] == 5.0 && row[1] == 6.0);

    std::vector<double> col = m2.column(1);
    assert(col.size() == 2u);
    assert(col[0] == 6.0 && col[1] == 8.0);

    Matrix vec_matrix{{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}};
    Vector3 vector(1.0, 2.0, 3.0);
    Vector3 transformed = vec_matrix * vector;
    assert(transformed == vector);

    bool caught_bounds = false;
    try {
        (void)m1.at(2, 0);
    } catch (const std::out_of_range&) {
        caught_bounds = true;
    }
    assert(caught_bounds);

    bool caught_dimension_again = false;
    try {
        Matrix invalid(2, 2);
        Matrix other(3, 3);
        (void)(invalid * other);
    } catch (const std::invalid_argument&) {
        caught_dimension_again = true;
    }
    assert(caught_dimension_again);

    bool caught_zero_division = false;
    try {
        Matrix zero(2, 2);
        (void)(zero / 0.0);
    } catch (const std::invalid_argument&) {
        caught_zero_division = true;
    }
    assert(caught_zero_division);

    std::ostringstream output;
    output << identity;
    assert(output.str().find("[") != std::string::npos);

    bool caught_normalize = false;
    try {
        Vector3 zero = Vector3::zero();
        zero.normalize();
    } catch (const std::runtime_error&) {
        caught_normalize = true;
    }
    assert(caught_normalize);

    bool caught_dimension = false;
    try {
        Matrix invalid(2, 2);
        Matrix other(3, 3);
        (void)(invalid * other);
    } catch (const std::invalid_argument&) {
        caught_dimension = true;
    }
    assert(caught_dimension);

    std::cout << "MathEngine tests passed.\n";
    return 0;
}
