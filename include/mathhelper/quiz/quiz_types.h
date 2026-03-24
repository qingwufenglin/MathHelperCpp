#ifndef MATHHELPER_QUIZ_QUIZ_TYPES_H
#define MATHHELPER_QUIZ_QUIZ_TYPES_H

#include <string>
#include <variant>

namespace mathhelper {

enum class GeometryShape {
    Square = 0,
    Rectangle = 1,
    Triangle = 2,
    Circle = 3
};

struct NumberQuestion {
    std::string prompt;
    double expected_value = 0.0;
    double tolerance = 1e-8;
    bool expects_integer = false;
};

struct ComplexQuestion {
    std::string prompt;
    double expected_real = 0.0;
    double expected_imag = 0.0;
    double tolerance = 0.1;
};

using MentalQuestion = std::variant<NumberQuestion, ComplexQuestion>;

struct GeometryProblem {
    GeometryShape shape = GeometryShape::Square;
    std::string prompt;
    double expected_perimeter = 0.0;
    double expected_area = 0.0;
    double tolerance = 0.01;
};

}  // namespace mathhelper

#endif
