#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "mathengine/mathengine.hpp"

namespace {

using mathengine::Matrix;
using mathengine::Vector3;

void printHeader(const std::string& title) {
    std::cout << "\n========================================\n";
    std::cout << title << "\n";
    std::cout << "========================================\n";
}

bool readInt(const std::string& prompt, int& value) {
    while (true) {
        std::cout << prompt;
        std::string line;
        if (!std::getline(std::cin, line)) {
            return false;
        }

        std::stringstream stream(line);
        int parsed = 0;
        if (stream >> parsed >> std::ws && stream.eof()) {
            value = parsed;
            return true;
        }

        std::cout << "Invalid input. Please enter a whole number.\n";
    }
}

bool readDouble(const std::string& prompt, double& value) {
    while (true) {
        std::cout << prompt;
        std::string line;
        if (!std::getline(std::cin, line)) {
            return false;
        }

        std::stringstream stream(line);
        double parsed = 0.0;
        if (stream >> parsed >> std::ws && stream.eof()) {
            value = parsed;
            return true;
        }

        std::cout << "Invalid input. Please enter a numeric value.\n";
    }
}

bool readVector(const std::string& prompt, Vector3& vector) {
    while (true) {
        std::cout << prompt;
        std::string line;
        if (!std::getline(std::cin, line)) {
            return false;
        }

        std::stringstream stream(line);
        double x = 0.0;
        double y = 0.0;
        double z = 0.0;
        if (stream >> x >> y >> z >> std::ws && stream.eof()) {
            vector = Vector3{x, y, z};
            return true;
        }

        std::cout << "Invalid vector input. Please enter three values in the form: x y z\n";
    }
}

bool readMatrix(const std::string& label, Matrix& matrix) {
    int rows = 0;
    int cols = 0;

    while (true) {
        if (!readInt("Number of rows for " + label + ": ", rows) || rows <= 0) {
            if (std::cin.fail()) {
                std::cin.clear();
            }
            std::cout << "Rows must be a positive integer.\n";
            continue;
        }
        break;
    }

    while (true) {
        if (!readInt("Number of columns for " + label + ": ", cols) || cols <= 0) {
            if (std::cin.fail()) {
                std::cin.clear();
            }
            std::cout << "Columns must be a positive integer.\n";
            continue;
        }
        break;
    }

    Matrix temp(rows, cols, 0.0);
    for (int row = 0; row < rows; ++row) {
        while (true) {
            std::cout << "Enter row " << (row + 1) << " of " << label << " (" << cols << " values separated by spaces): ";
            std::string line;
            if (!std::getline(std::cin, line)) {
                return false;
            }

            std::stringstream stream(line);
            std::vector<double> values;
            values.reserve(static_cast<std::size_t>(cols));
            bool valid_row = true;
            for (int col = 0; col < cols; ++col) {
                double value = 0.0;
                if (!(stream >> value)) {
                    valid_row = false;
                    break;
                }
                values.push_back(value);
            }

            if (valid_row) {
                stream >> std::ws;
                if (values.size() == static_cast<std::size_t>(cols) && stream.eof()) {
                    for (int col = 0; col < cols; ++col) {
                        temp(row, col) = values[static_cast<std::size_t>(col)];
                    }
                    break;
                }
            }

            std::cout << "Invalid row input. Please enter exactly " << cols << " numeric values.\n";
        }
    }

    matrix = temp;
    return true;
}

std::string formatVector(const Vector3& vector) {
    std::ostringstream stream;
    stream << vector;
    return stream.str();
}

std::string formatMatrix(const Matrix& matrix) {
    std::ostringstream stream;
    stream << matrix;
    return stream.str();
}

void printVectorDemo(const std::string& title,
                     const std::string& purpose,
                     const std::string& input,
                     const std::string& operation,
                     const std::string& output,
                     const std::string& explanation) {
    std::cout << "\n" << title << "\n";
    std::cout << "----------------------------------------\n";
    std::cout << std::left << std::setw(12) << "Purpose" << ": " << purpose << "\n";
    std::cout << std::left << std::setw(12) << "Input" << ":\n" << input << "\n";
    std::cout << std::left << std::setw(12) << "Operation" << ":\n" << operation << "\n";
    std::cout << std::left << std::setw(12) << "Output" << ":\n" << output << "\n";
    std::cout << std::left << std::setw(12) << "Explanation" << ": " << explanation << "\n";
}

void printMatrixDemo(const std::string& title,
                     const std::string& purpose,
                     const std::string& input,
                     const std::string& operation,
                     const std::string& output,
                     const std::string& explanation) {
    std::cout << "\n" << title << "\n";
    std::cout << "----------------------------------------\n";
    std::cout << std::left << std::setw(12) << "Purpose" << ": " << purpose << "\n";
    std::cout << std::left << std::setw(12) << "Input" << ":\n" << input << "\n";
    std::cout << std::left << std::setw(12) << "Operation" << ":\n" << operation << "\n";
    std::cout << std::left << std::setw(12) << "Output" << ":\n" << output << "\n";
    std::cout << std::left << std::setw(12) << "Explanation" << ": " << explanation << "\n";
}

void showAboutProject() {
    printHeader("About Project");
    std::cout << "Project Name : MathEngine\n";
    std::cout << "Version      : 0.1.0\n";
    std::cout << "Description  : A lightweight C++17 library for vector and matrix mathematics.\n";
    std::cout << "Build System : CMake\n";
    std::cout << "Focus        : Vectors, matrices, operator overloading, and demonstrations.\n";
}

void runVectorMenu() {
    using namespace mathengine;

    Vector3 a;
    Vector3 b;
    if (!readVector("Enter Vector A (x y z): ", a)) {
        std::cout << "Input stream closed. Returning to main menu.\n";
        return;
    }
    if (!readVector("Enter Vector B (x y z): ", b)) {
        std::cout << "Input stream closed. Returning to main menu.\n";
        return;
    }

    while (true) {
        printHeader("Vector Operations");
        std::cout << "Choose Operation\n";
        std::cout << "1. Addition\n";
        std::cout << "2. Subtraction\n";
        std::cout << "3. Dot Product\n";
        std::cout << "4. Cross Product\n";
        std::cout << "5. Magnitude of Vector A\n";
        std::cout << "6. Magnitude of Vector B\n";
        std::cout << "7. Normalize Vector A\n";
        std::cout << "8. Normalize Vector B\n";
        std::cout << "9. Distance Between Vectors\n";
        std::cout << "10. Angle Between Vectors\n";
        std::cout << "11. Linear Interpolation (Lerp)\n";
        std::cout << "12. Run All Vector Operations\n";
        std::cout << "13. Enter New Vectors\n";
        std::cout << "14. Back to Main Menu\n";
        std::cout << "----------------------------------------\n";

        int choice = 0;
        if (!readInt("Select an option: ", choice)) {
            std::cout << "Input stream closed. Returning to main menu.\n";
            return;
        }

        switch (choice) {
            case 1: {
                Vector3 result = a + b;
                printVectorDemo("Vector Addition",
                                 "Add two vectors.",
                                 "A = " + formatVector(a) + "\nB = " + formatVector(b),
                                 "A + B",
                                 formatVector(result),
                                 "Returns a new vector by adding corresponding components.");
                break;
            }
            case 2: {
                Vector3 result = a - b;
                printVectorDemo("Vector Subtraction",
                                 "Subtract one vector from another.",
                                 "A = " + formatVector(a) + "\nB = " + formatVector(b),
                                 "A - B",
                                 formatVector(result),
                                 "Returns a new vector by subtracting corresponding components.");
                break;
            }
            case 3: {
                double result = a.dot(b);
                printVectorDemo("Dot Product",
                                 "Measure the projection similarity of two vectors.",
                                 "A = " + formatVector(a) + "\nB = " + formatVector(b),
                                 "dot(A, B)",
                                 std::to_string(result),
                                 "Produces a scalar value that reflects vector alignment.");
                break;
            }
            case 4: {
                Vector3 result = a.cross(b);
                printVectorDemo("Cross Product",
                                 "Compute a perpendicular vector.",
                                 "A = " + formatVector(a) + "\nB = " + formatVector(b),
                                 "cross(A, B)",
                                 formatVector(result),
                                 "Returns a vector orthogonal to both input vectors.");
                break;
            }
            case 5: {
                double result = a.magnitude();
                printVectorDemo("Magnitude of Vector A",
                                 "Find the length of Vector A.",
                                 "A = " + formatVector(a),
                                 "magnitude(A)",
                                 std::to_string(result),
                                 "Calculates the Euclidean length of the vector.");
                break;
            }
            case 6: {
                double result = b.magnitude();
                printVectorDemo("Magnitude of Vector B",
                                 "Find the length of Vector B.",
                                 "B = " + formatVector(b),
                                 "magnitude(B)",
                                 std::to_string(result),
                                 "Calculates the Euclidean length of the vector.");
                break;
            }
            case 7: {
                Vector3 result = a.normalized();
                printVectorDemo("Normalize Vector A",
                                 "Create a unit vector from Vector A.",
                                 "A = " + formatVector(a),
                                 "normalize(A)",
                                 formatVector(result),
                                 "Produces a direction vector with unit length.");
                break;
            }
            case 8: {
                Vector3 result = b.normalized();
                printVectorDemo("Normalize Vector B",
                                 "Create a unit vector from Vector B.",
                                 "B = " + formatVector(b),
                                 "normalize(B)",
                                 formatVector(result),
                                 "Produces a direction vector with unit length.");
                break;
            }
            case 9: {
                double result = a.distanceTo(b);
                printVectorDemo("Distance Between Vectors",
                                 "Measure the straight-line distance between the vectors.",
                                 "A = " + formatVector(a) + "\nB = " + formatVector(b),
                                 "distance(A, B)",
                                 std::to_string(result),
                                 "Calculates the length of the difference between the vectors.");
                break;
            }
            case 10: {
                double result = a.angleTo(b);
                printVectorDemo("Angle Between Vectors",
                                 "Measure the angle between the vectors.",
                                 "A = " + formatVector(a) + "\nB = " + formatVector(b),
                                 "angle(A, B)",
                                 std::to_string(result),
                                 "Computes the angle in radians using the dot product.");
                break;
            }
            case 11: {
                double t = 0.0;
                if (!readDouble("Enter interpolation value t in [0, 1]: ", t)) {
                    std::cout << "Input stream closed. Returning to main menu.\n";
                    return;
                }
                if (t < 0.0 || t > 1.0) {
                    std::cout << "Interpolation value t must be between 0 and 1.\n";
                    break;
                }
                Vector3 result = a.lerp(b, t);
                printVectorDemo("Linear Interpolation",
                                 "Blend two vectors smoothly.",
                                 "A = " + formatVector(a) + "\nB = " + formatVector(b) + "\nT = " + std::to_string(t),
                                 "lerp(A, B, t)",
                                 formatVector(result),
                                 "Interpolates between the two vectors based on the parameter t.");
                break;
            }
            case 12: {
                printVectorDemo("Vector Addition",
                                 "Add two vectors.",
                                 "A = " + formatVector(a) + "\nB = " + formatVector(b),
                                 "A + B",
                                 formatVector(a + b),
                                 "Returns a new vector by adding corresponding components.");
                printVectorDemo("Vector Subtraction",
                                 "Subtract one vector from another.",
                                 "A = " + formatVector(a) + "\nB = " + formatVector(b),
                                 "A - B",
                                 formatVector(a - b),
                                 "Returns a new vector by subtracting corresponding components.");
                printVectorDemo("Dot Product",
                                 "Measure the projection similarity of two vectors.",
                                 "A = " + formatVector(a) + "\nB = " + formatVector(b),
                                 "dot(A, B)",
                                 std::to_string(a.dot(b)),
                                 "Produces a scalar value that reflects vector alignment.");
                printVectorDemo("Cross Product",
                                 "Compute a perpendicular vector.",
                                 "A = " + formatVector(a) + "\nB = " + formatVector(b),
                                 "cross(A, B)",
                                 formatVector(a.cross(b)),
                                 "Returns a vector orthogonal to both input vectors.");
                printVectorDemo("Magnitude of Vector A",
                                 "Find the length of Vector A.",
                                 "A = " + formatVector(a),
                                 "magnitude(A)",
                                 std::to_string(a.magnitude()),
                                 "Calculates the Euclidean length of the vector.");
                printVectorDemo("Magnitude of Vector B",
                                 "Find the length of Vector B.",
                                 "B = " + formatVector(b),
                                 "magnitude(B)",
                                 std::to_string(b.magnitude()),
                                 "Calculates the Euclidean length of the vector.");
                printVectorDemo("Normalize Vector A",
                                 "Create a unit vector from Vector A.",
                                 "A = " + formatVector(a),
                                 "normalize(A)",
                                 formatVector(a.normalized()),
                                 "Produces a direction vector with unit length.");
                printVectorDemo("Normalize Vector B",
                                 "Create a unit vector from Vector B.",
                                 "B = " + formatVector(b),
                                 "normalize(B)",
                                 formatVector(b.normalized()),
                                 "Produces a direction vector with unit length.");
                printVectorDemo("Distance Between Vectors",
                                 "Measure the straight-line distance between the vectors.",
                                 "A = " + formatVector(a) + "\nB = " + formatVector(b),
                                 "distance(A, B)",
                                 std::to_string(a.distanceTo(b)),
                                 "Calculates the length of the difference between the vectors.");
                printVectorDemo("Angle Between Vectors",
                                 "Measure the angle between the vectors.",
                                 "A = " + formatVector(a) + "\nB = " + formatVector(b),
                                 "angle(A, B)",
                                 std::to_string(a.angleTo(b)),
                                 "Computes the angle in radians using the dot product.");
                break;
            }
            case 13: {
                if (!readVector("Enter Vector A (x y z): ", a)) {
                    std::cout << "Input stream closed. Returning to main menu.\n";
                    return;
                }
                if (!readVector("Enter Vector B (x y z): ", b)) {
                    std::cout << "Input stream closed. Returning to main menu.\n";
                    return;
                }
                std::cout << "Vectors updated successfully.\n";
                break;
            }
            case 14: {
                return;
            }
            default:
                std::cout << "Invalid option. Please select a valid menu item.\n";
                break;
        }
    }
}

void runMatrixMenu() {
    using namespace mathengine;

    Matrix a;
    Matrix b;
    Matrix identity;
    Vector3 vector;
    bool haveMatrixA = false;
    bool haveMatrixB = false;

    while (true) {
        printHeader("Matrix Operations");
        std::cout << "Choose Operation\n";
        std::cout << "1. Matrix Addition\n";
        std::cout << "2. Matrix Multiplication\n";
        std::cout << "3. Matrix × Vector\n";
        std::cout << "4. Identity Matrix\n";
        std::cout << "5. Transpose\n";
        std::cout << "6. Run All Matrix Operations\n";
        std::cout << "7. Enter New Matrices\n";
        std::cout << "8. Back to Main Menu\n";
        std::cout << "----------------------------------------\n";

        int choice = 0;
        if (!readInt("Select an option: ", choice)) {
            std::cout << "Input stream closed. Returning to main menu.\n";
            return;
        }

        if (choice >= 1 && choice <= 6 && !haveMatrixA) {
            if (!readMatrix("Matrix A", a)) {
                std::cout << "Input stream closed. Returning to main menu.\n";
                return;
            }
            haveMatrixA = true;
        }

        if ((choice == 1 || choice == 2 || choice == 6) && !haveMatrixB) {
            if (!readMatrix("Matrix B", b)) {
                std::cout << "Input stream closed. Returning to main menu.\n";
                return;
            }
            haveMatrixB = true;
        }

        if (choice == 3 && !haveMatrixA) {
            if (!readVector("Enter Vector for Matrix × Vector (x y z): ", vector)) {
                std::cout << "Input stream closed. Returning to main menu.\n";
                return;
            }
        }

        switch (choice) {
            case 1: {
                if (a.rows() != b.rows() || a.cols() != b.cols()) {
                    std::cout << "Matrix addition requires both matrices to have the same dimensions.\n";
                    break;
                }
                Matrix result = a + b;
                printMatrixDemo("Matrix Addition",
                                 "Add two matrices element by element.",
                                 "A = \n" + formatMatrix(a) + "\nB = \n" + formatMatrix(b),
                                 "A + B",
                                 formatMatrix(result),
                                 "Combines matching positions from both matrices.");
                break;
            }
            case 2: {
                if (a.cols() != b.rows()) {
                    std::cout << "Matrix multiplication requires the number of columns in Matrix A to match the number of rows in Matrix B.\n";
                    break;
                }
                Matrix result = a * b;
                printMatrixDemo("Matrix Multiplication",
                                 "Multiply two matrices using standard row-by-column rules.",
                                 "A = \n" + formatMatrix(a) + "\nB = \n" + formatMatrix(b),
                                 "A * B",
                                 formatMatrix(result),
                                 "Produces a new matrix by combining rows and columns.");
                break;
            }
            case 3: {
                if (!readVector("Enter Vector for Matrix × Vector (x y z): ", vector)) {
                    std::cout << "Input stream closed. Returning to main menu.\n";
                    return;
                }
                if (a.rows() != 3 || a.cols() != 3) {
                    std::cout << "Matrix × Vector requires a 3x3 matrix.\n";
                    break;
                }
                Vector3 result = a * vector;
                printMatrixDemo("Matrix × Vector",
                                 "Apply a matrix to a vector.",
                                 "A = \n" + formatMatrix(a) + "\nV = " + formatVector(vector),
                                 "A * V",
                                 formatVector(result),
                                 "Transforms the vector using the matrix rows and columns.");
                break;
            }
            case 4: {
                int size = 0;
                if (!readInt("Enter identity matrix size: ", size) || size <= 0) {
                    std::cout << "Identity size must be a positive integer.\n";
                    break;
                }
                Matrix result = Matrix::identity(static_cast<std::size_t>(size));
                printMatrixDemo("Identity Matrix",
                                 "Create an identity matrix.",
                                 "Size = " + std::to_string(size),
                                 "identity(size)",
                                 formatMatrix(result),
                                 "Produces a diagonal matrix with ones on the main diagonal.");
                break;
            }
            case 5: {
                Matrix result = a.transpose();
                printMatrixDemo("Transpose",
                                 "Swap rows and columns of a matrix.",
                                 "A = \n" + formatMatrix(a),
                                 "transpose(A)",
                                 formatMatrix(result),
                                 "Reorients the matrix by reflecting it across its main diagonal.");
                break;
            }
            case 6: {
                printMatrixDemo("Matrix Addition",
                                 "Add two matrices element by element.",
                                 "A = \n" + formatMatrix(a) + "\nB = \n" + formatMatrix(b),
                                 "A + B",
                                 formatMatrix(a + b),
                                 "Combines matching positions from both matrices.");
                printMatrixDemo("Matrix Multiplication",
                                 "Multiply two matrices using standard row-by-column rules.",
                                 "A = \n" + formatMatrix(a) + "\nB = \n" + formatMatrix(b),
                                 "A * B",
                                 formatMatrix(a * b),
                                 "Produces a new matrix by combining rows and columns.");
                printMatrixDemo("Matrix × Vector",
                                 "Apply a matrix to a vector.",
                                 "A = \n" + formatMatrix(a) + "\nV = " + formatVector(vector),
                                 "A * V",
                                 formatVector(a * vector),
                                 "Transforms the vector using the matrix rows and columns.");
                printMatrixDemo("Identity Matrix",
                                 "Create an identity matrix.",
                                 "Size = 3",
                                 "identity(3)",
                                 formatMatrix(Matrix::identity(3)),
                                 "Produces a diagonal matrix with ones on the main diagonal.");
                printMatrixDemo("Transpose",
                                 "Swap rows and columns of a matrix.",
                                 "A = \n" + formatMatrix(a),
                                 "transpose(A)",
                                 formatMatrix(a.transpose()),
                                 "Reorients the matrix by reflecting it across its main diagonal.");
                break;
            }
            case 7: {
                if (!readMatrix("Matrix A", a)) {
                    std::cout << "Input stream closed. Returning to main menu.\n";
                    return;
                }
                if (!readMatrix("Matrix B", b)) {
                    std::cout << "Input stream closed. Returning to main menu.\n";
                    return;
                }
                haveMatrixA = true;
                haveMatrixB = true;
                std::cout << "Matrices updated successfully.\n";
                break;
            }
            case 8: {
                return;
            }
            default:
                std::cout << "Invalid option. Please select a valid menu item.\n";
                break;
        }
    }
}

}  // namespace

int main() {
    using namespace mathengine;

    std::cout << std::fixed << std::setprecision(3) << std::boolalpha;

    while (true) {
        printHeader("MathEngine Demo");
        std::cout << "1. Vector Operations\n";
        std::cout << "2. Matrix Operations\n";
        std::cout << "3. About Project\n";
        std::cout << "4. Exit\n";
        std::cout << "----------------------------------------\n";

        int choice = 0;
        if (!readInt("Select an option: ", choice)) {
            std::cout << "Input stream closed. Exiting.\n";
            break;
        }

        switch (choice) {
            case 1:
                runVectorMenu();
                break;
            case 2:
                runMatrixMenu();
                break;
            case 3:
                showAboutProject();
                break;
            case 4:
                std::cout << "Exiting MathEngine Demo. Thank you.\n";
                return 0;
            default:
                std::cout << "Invalid option. Please select a valid menu item.\n";
                break;
        }
    }

    return 0;
}
