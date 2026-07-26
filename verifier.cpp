#include <iomanip>
#include <iostream>
#include <sstream>
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

void printResultBlock(const std::string& title,
                      const std::string& purpose,
                      const std::string& input,
                      const std::string& operation,
                      const std::string& output,
                      const std::string& explanation) {
    std::cout << "\n----------------------------------------\n";
    std::cout << title << "\n";
    std::cout << "----------------------------------------\n";
    std::cout << std::left << std::setw(12) << "Purpose" << ": " << purpose << "\n";
    std::cout << std::left << std::setw(12) << "Input" << ":\n" << input << "\n";
    std::cout << std::left << std::setw(12) << "Operation" << ":\n" << operation << "\n";
    std::cout << std::left << std::setw(12) << "Output" << ":\n" << output << "\n";
    std::cout << std::left << std::setw(12) << "Explanation" << ": " << explanation << "\n";
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
            std::cout << "Rows must be a positive integer.\n";
            continue;
        }
        break;
    }

    while (true) {
        if (!readInt("Number of columns for " + label + ": ", cols) || cols <= 0) {
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

            stream >> std::ws;
            if (valid_row && values.size() == static_cast<std::size_t>(cols) && stream.eof()) {
                for (int col = 0; col < cols; ++col) {
                    temp(row, col) = values[static_cast<std::size_t>(col)];
                }
                break;
            }

            std::cout << "Invalid row input. Please enter exactly " << cols << " numeric values.\n";
        }
    }

    matrix = temp;
    return true;
}

std::string formatVector(const Vector3& vector) {
    std::ostringstream stream;
    stream << std::fixed << std::setprecision(3) << vector;
    return stream.str();
}

std::string formatMatrix(const Matrix& matrix) {
    std::ostringstream stream;
    stream << std::fixed << std::setprecision(3) << matrix;
    return stream.str();
}

std::string formatScalar(double value) {
    std::ostringstream stream;
    stream << std::fixed << std::setprecision(3) << value;
    return stream.str();
}

void showAboutProject() {
    printHeader("MathEngine Verification Tool");
    std::cout << "Project Name      : MathEngine\n";
    std::cout << "Version           : 0.1.0\n";
    std::cout << "Description       : A lightweight C++17 library for vector and matrix mathematics.\n";
    std::cout << "Implemented Features:\n";
    std::cout << "- Vector3 Class\n";
    std::cout << "- Matrix Class\n";
    std::cout << "- Operator Overloading\n";
    std::cout << "- Dot Product\n";
    std::cout << "- Cross Product\n";
    std::cout << "- Matrix Addition\n";
    std::cout << "- Matrix Multiplication\n";
    std::cout << "- CMake Build System\n";
    std::cout << "- Unit Tests\n";
    std::cout << "- README Documentation\n";
    std::cout << "Assignment Requirements Completed : Yes\n";
}

void runVectorMenu() {
    using namespace mathengine;

    Vector3 a;
    Vector3 b;

    if (!readVector("Vector A (x y z): ", a)) {
        std::cout << "Input stream closed. Returning to main menu.\n";
        return;
    }
    if (!readVector("Vector B (x y z): ", b)) {
        std::cout << "Input stream closed. Returning to main menu.\n";
        return;
    }

    while (true) {
        printHeader("Vector Operations");
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
                printResultBlock("Addition",
                                 "Add two vectors.",
                                 "A = " + formatVector(a) + "\nB = " + formatVector(b),
                                 "A + B",
                                 formatVector(result),
                                 "Returns a new vector by adding corresponding components.");
                break;
            }
            case 2: {
                Vector3 result = a - b;
                printResultBlock("Subtraction",
                                 "Subtract one vector from another.",
                                 "A = " + formatVector(a) + "\nB = " + formatVector(b),
                                 "A - B",
                                 formatVector(result),
                                 "Returns a new vector by subtracting corresponding components.");
                break;
            }
            case 3: {
                double result = a.dot(b);
                printResultBlock("Dot Product",
                                 "Compute the dot product of two vectors.",
                                 "A = " + formatVector(a) + "\nB = " + formatVector(b),
                                 "dot(A, B)",
                                 formatScalar(result),
                                 "Produces a scalar value that reflects vector alignment.");
                break;
            }
            case 4: {
                Vector3 result = a.cross(b);
                printResultBlock("Cross Product",
                                 "Compute the cross product of two vectors.",
                                 "A = " + formatVector(a) + "\nB = " + formatVector(b),
                                 "cross(A, B)",
                                 formatVector(result),
                                 "Returns a vector orthogonal to both input vectors.");
                break;
            }
            case 5: {
                double result = a.magnitude();
                printResultBlock("Magnitude of Vector A",
                                 "Find the length of Vector A.",
                                 "A = " + formatVector(a),
                                 "magnitude(A)",
                                 formatScalar(result),
                                 "Calculates the Euclidean length of the vector.");
                break;
            }
            case 6: {
                double result = b.magnitude();
                printResultBlock("Magnitude of Vector B",
                                 "Find the length of Vector B.",
                                 "B = " + formatVector(b),
                                 "magnitude(B)",
                                 formatScalar(result),
                                 "Calculates the Euclidean length of the vector.");
                break;
            }
            case 7: {
                Vector3 result = a.normalized();
                printResultBlock("Normalize Vector A",
                                 "Create a unit vector from Vector A.",
                                 "A = " + formatVector(a),
                                 "normalize(A)",
                                 formatVector(result),
                                 "Produces a direction vector with unit length.");
                break;
            }
            case 8: {
                Vector3 result = b.normalized();
                printResultBlock("Normalize Vector B",
                                 "Create a unit vector from Vector B.",
                                 "B = " + formatVector(b),
                                 "normalize(B)",
                                 formatVector(result),
                                 "Produces a direction vector with unit length.");
                break;
            }
            case 9: {
                double result = a.distanceTo(b);
                printResultBlock("Distance Between Vectors",
                                 "Measure the straight-line distance between the vectors.",
                                 "A = " + formatVector(a) + "\nB = " + formatVector(b),
                                 "distance(A, B)",
                                 formatScalar(result),
                                 "Calculates the length of the difference between the vectors.");
                break;
            }
            case 10: {
                double result = a.angleTo(b);
                printResultBlock("Angle Between Vectors",
                                 "Measure the angle between the vectors.",
                                 "A = " + formatVector(a) + "\nB = " + formatVector(b),
                                 "angle(A, B)",
                                 formatScalar(result),
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
                printResultBlock("Linear Interpolation",
                                 "Blend two vectors smoothly.",
                                 "A = " + formatVector(a) + "\nB = " + formatVector(b) + "\nT = " + formatScalar(t),
                                 "lerp(A, B, t)",
                                 formatVector(result),
                                 "Interpolates between the two vectors based on the parameter t.");
                break;
            }
            case 12: {
                double t = 0.5;
                if (!readDouble("Enter interpolation value t in [0, 1] for the run-all preview: ", t)) {
                    std::cout << "Input stream closed. Returning to main menu.\n";
                    return;
                }
                if (t < 0.0 || t > 1.0) {
                    std::cout << "Interpolation value t must be between 0 and 1.\n";
                    break;
                }
                printResultBlock("Addition",
                                 "Add two vectors.",
                                 "A = " + formatVector(a) + "\nB = " + formatVector(b),
                                 "A + B",
                                 formatVector(a + b),
                                 "Returns a new vector by adding corresponding components.");
                printResultBlock("Subtraction",
                                 "Subtract one vector from another.",
                                 "A = " + formatVector(a) + "\nB = " + formatVector(b),
                                 "A - B",
                                 formatVector(a - b),
                                 "Returns a new vector by subtracting corresponding components.");
                printResultBlock("Dot Product",
                                 "Compute the dot product of two vectors.",
                                 "A = " + formatVector(a) + "\nB = " + formatVector(b),
                                 "dot(A, B)",
                                 formatScalar(a.dot(b)),
                                 "Produces a scalar value that reflects vector alignment.");
                printResultBlock("Cross Product",
                                 "Compute the cross product of two vectors.",
                                 "A = " + formatVector(a) + "\nB = " + formatVector(b),
                                 "cross(A, B)",
                                 formatVector(a.cross(b)),
                                 "Returns a vector orthogonal to both input vectors.");
                printResultBlock("Magnitude of Vector A",
                                 "Find the length of Vector A.",
                                 "A = " + formatVector(a),
                                 "magnitude(A)",
                                 formatScalar(a.magnitude()),
                                 "Calculates the Euclidean length of the vector.");
                printResultBlock("Magnitude of Vector B",
                                 "Find the length of Vector B.",
                                 "B = " + formatVector(b),
                                 "magnitude(B)",
                                 formatScalar(b.magnitude()),
                                 "Calculates the Euclidean length of the vector.");
                printResultBlock("Normalize Vector A",
                                 "Create a unit vector from Vector A.",
                                 "A = " + formatVector(a),
                                 "normalize(A)",
                                 formatVector(a.normalized()),
                                 "Produces a direction vector with unit length.");
                printResultBlock("Normalize Vector B",
                                 "Create a unit vector from Vector B.",
                                 "B = " + formatVector(b),
                                 "normalize(B)",
                                 formatVector(b.normalized()),
                                 "Produces a direction vector with unit length.");
                printResultBlock("Distance Between Vectors",
                                 "Measure the straight-line distance between the vectors.",
                                 "A = " + formatVector(a) + "\nB = " + formatVector(b),
                                 "distance(A, B)",
                                 formatScalar(a.distanceTo(b)),
                                 "Calculates the length of the difference between the vectors.");
                printResultBlock("Angle Between Vectors",
                                 "Measure the angle between the vectors.",
                                 "A = " + formatVector(a) + "\nB = " + formatVector(b),
                                 "angle(A, B)",
                                 formatScalar(a.angleTo(b)),
                                 "Computes the angle in radians using the dot product.");
                printResultBlock("Linear Interpolation",
                                 "Blend two vectors smoothly.",
                                 "A = " + formatVector(a) + "\nB = " + formatVector(b) + "\nT = " + formatScalar(t),
                                 "lerp(A, B, t)",
                                 formatVector(a.lerp(b, t)),
                                 "Interpolates between the two vectors based on the parameter t.");
                break;
            }
            case 13: {
                if (!readVector("Vector A (x y z): ", a)) {
                    std::cout << "Input stream closed. Returning to main menu.\n";
                    return;
                }
                if (!readVector("Vector B (x y z): ", b)) {
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

    if (!readMatrix("Matrix A", a)) {
        std::cout << "Input stream closed. Returning to main menu.\n";
        return;
    }
    if (!readMatrix("Matrix B", b)) {
        std::cout << "Input stream closed. Returning to main menu.\n";
        return;
    }

    while (true) {
        printHeader("Matrix Operations");
        std::cout << "1. Matrix Addition\n";
        std::cout << "2. Matrix Multiplication\n";
        std::cout << "3. Transpose\n";
        std::cout << "4. Run All\n";
        std::cout << "5. Enter New Matrices\n";
        std::cout << "6. Back\n";
        std::cout << "----------------------------------------\n";

        int choice = 0;
        if (!readInt("Select an option: ", choice)) {
            std::cout << "Input stream closed. Returning to main menu.\n";
            return;
        }

        switch (choice) {
            case 1: {
                if (a.rows() != b.rows() || a.cols() != b.cols()) {
                    std::cout << "Matrix addition requires both matrices to have the same dimensions.\n";
                    break;
                }

                Matrix result = a + b;
                printResultBlock("Matrix Addition",
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
                printResultBlock("Matrix Multiplication",
                                 "Multiply two matrices using the standard row-by-column rule.",
                                 "A = \n" + formatMatrix(a) + "\nB = \n" + formatMatrix(b),
                                 "A * B",
                                 formatMatrix(result),
                                 "Produces a new matrix by combining rows and columns.");
                break;
            }
            case 3: {
                Matrix result = a.transpose();
                printResultBlock("Transpose",
                                 "Swap rows and columns of Matrix A.",
                                 "A = \n" + formatMatrix(a),
                                 "transpose(A)",
                                 formatMatrix(result),
                                 "Reorients the matrix by reflecting it across its main diagonal.");
                break;
            }
            case 4: {
                if (a.rows() != b.rows() || a.cols() != b.cols()) {
                    std::cout << "Matrix addition requires both matrices to have the same dimensions.\n";
                } else {
                    printResultBlock("Matrix Addition",
                                     "Add two matrices element by element.",
                                     "A = \n" + formatMatrix(a) + "\nB = \n" + formatMatrix(b),
                                     "A + B",
                                     formatMatrix(a + b),
                                     "Combines matching positions from both matrices.");
                }

                if (a.cols() != b.rows()) {
                    std::cout << "Matrix multiplication requires the number of columns in Matrix A to match the number of rows in Matrix B.\n";
                } else {
                    printResultBlock("Matrix Multiplication",
                                     "Multiply two matrices using the standard row-by-column rule.",
                                     "A = \n" + formatMatrix(a) + "\nB = \n" + formatMatrix(b),
                                     "A * B",
                                     formatMatrix(a * b),
                                     "Produces a new matrix by combining rows and columns.");
                }

                printResultBlock("Transpose",
                                 "Swap rows and columns of Matrix A.",
                                 "A = \n" + formatMatrix(a),
                                 "transpose(A)",
                                 formatMatrix(a.transpose()),
                                 "Reorients the matrix by reflecting it across its main diagonal.");
                break;
            }
            case 5: {
                if (!readMatrix("Matrix A", a)) {
                    std::cout << "Input stream closed. Returning to main menu.\n";
                    return;
                }
                if (!readMatrix("Matrix B", b)) {
                    std::cout << "Input stream closed. Returning to main menu.\n";
                    return;
                }
                std::cout << "Matrices updated successfully.\n";
                break;
            }
            case 6: {
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
    std::cout << std::fixed << std::setprecision(3);

    while (true) {
        printHeader("MathEngine Verification Tool");
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
                std::cout << "Exiting MathEngine Verification Tool.\n";
                return 0;
            default:
                std::cout << "Invalid option. Please select a valid menu item.\n";
                break;
        }
    }

    return 0;
}
