#include "mathhelper/quiz/quiz_service.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace mathhelper {
namespace {

constexpr double kPi = 3.14;

bool AreClose(double lhs, double rhs, double tolerance) {
    return std::fabs(lhs - rhs) <= tolerance;
}

}  // namespace

QuizService::QuizService(std::uint32_t seed) : rng_(seed) {}

MentalQuestion QuizService::GenerateMentalQuestion(const std::string& grade) {
    if (grade == "一年级") {
        return GenerateGrade1Question();
    }
    if (grade == "二年级" || grade == "三年级") {
        return GenerateGrade2To3Question();
    }
    if (grade == "四年级" || grade == "五年级") {
        return GenerateDecimalQuestion();
    }
    if (grade == "六年级") {
        const int mode = RandomInt(0, 2);
        if (mode == 0) {
            return GenerateDecimalQuestion();
        }
        if (mode == 1) {
            return GeneratePowerQuestion();
        }
        return GenerateModuloQuestion();
    }
    if (grade == "七年级") {
        const int mode = RandomInt(0, 4);
        if (mode == 0 || mode == 4) {
            return GenerateDecimalQuestion();
        }
        if (mode == 1) {
            return GeneratePowerQuestion();
        }
        if (mode == 2) {
            return GenerateModuloQuestion();
        }
        return GenerateComplexQuestion();
    }
    throw std::invalid_argument("当前年级不支持口算训练");
}

GeometryProblem QuizService::GenerateGeometryProblem(GeometryShape shape) {
    GeometryProblem problem;
    problem.shape = shape;
    problem.tolerance = 0.01;

    if (shape == GeometryShape::Square) {
        const int side = RandomInt(1, 24);
        problem.expected_perimeter = 4.0 * side;
        problem.expected_area = 1.0 * side * side;
        problem.prompt = "请计算边长为" + std::to_string(side) + "的正方形周长和面积。";
        return problem;
    }

    if (shape == GeometryShape::Rectangle) {
        int length = RandomInt(1, 24);
        int width = RandomInt(1, 19);
        if (length < width) {
            std::swap(length, width);
        }
        problem.expected_perimeter = 2.0 * (length + width);
        problem.expected_area = 1.0 * length * width;
        problem.prompt = "请计算长为" + std::to_string(length) + "、宽为" + std::to_string(width) +
                         "的长方形周长和面积。";
        return problem;
    }

    if (shape == GeometryShape::Triangle) {
        int a = 0;
        int b = 0;
        int c = 0;
        do {
            a = RandomInt(1, 9);
            b = RandomInt(1, 9);
            c = RandomInt(1, 9);
        } while (a + b <= c || a + c <= b || b + c <= a);

        const double semi = (a + b + c) / 2.0;
        const double area = std::sqrt(semi * (semi - a) * (semi - b) * (semi - c));
        problem.expected_perimeter = static_cast<double>(a + b + c);
        problem.expected_area = RoundTo(area, 2);
        problem.prompt = "请计算三边长为" + std::to_string(a) + "、" + std::to_string(b) + "、" +
                         std::to_string(c) + "的三角形周长和面积（面积保留两位小数）。";
        return problem;
    }

    const int radius = RandomInt(1, 14);
    problem.expected_perimeter = RoundTo(2.0 * kPi * radius, 2);
    problem.expected_area = RoundTo(kPi * radius * radius, 2);
    problem.prompt = "请计算半径为" + std::to_string(radius) +
                     "的圆周长和面积（取 PI=3.14，结果保留两位小数）。";
    return problem;
}

bool QuizService::CheckMentalAnswer(const MentalQuestion& question, const std::string& raw_input) const {
    if (std::holds_alternative<NumberQuestion>(question)) {
        const auto& number_question = std::get<NumberQuestion>(question);
        double parsed = 0.0;
        if (!TryParseDouble(raw_input, &parsed)) {
            return false;
        }
        return AreClose(parsed, number_question.expected_value, number_question.tolerance);
    }

    const auto& complex_question = std::get<ComplexQuestion>(question);
    std::string normalized = raw_input;
    for (char& ch : normalized) {
        if (ch == ',') {
            ch = ' ';
        }
    }

    std::stringstream stream(normalized);
    double real = 0.0;
    double imag = 0.0;
    stream >> real >> imag;
    if (stream.fail()) {
        return false;
    }
    stream >> std::ws;
    if (!stream.eof()) {
        return false;
    }

    return AreClose(real, complex_question.expected_real, complex_question.tolerance) &&
           AreClose(imag, complex_question.expected_imag, complex_question.tolerance);
}

bool QuizService::CheckGeometryAnswer(const GeometryProblem& problem, double perimeter, double area) const {
    return AreClose(perimeter, problem.expected_perimeter, problem.tolerance) &&
           AreClose(area, problem.expected_area, problem.tolerance);
}

std::string QuizService::ExpectedAnswerText(const MentalQuestion& question) const {
    std::stringstream stream;
    if (std::holds_alternative<NumberQuestion>(question)) {
        const auto& number_question = std::get<NumberQuestion>(question);
        if (number_question.expects_integer) {
            stream << static_cast<long long>(std::llround(number_question.expected_value));
        } else {
            stream << std::fixed << std::setprecision(2) << number_question.expected_value;
        }
        return stream.str();
    }

    const auto& complex_question = std::get<ComplexQuestion>(question);
    stream << std::fixed << std::setprecision(1) << complex_question.expected_real;
    if (complex_question.expected_imag >= 0.0) {
        stream << " + " << complex_question.expected_imag << "i";
    } else {
        stream << " - " << std::fabs(complex_question.expected_imag) << "i";
    }
    stream << "（输入格式：实部 虚部）";
    return stream.str();
}

bool QuizService::IsSupportedMentalGrade(const std::string& grade) {
    static const std::array<const char*, 7> supported = {"一年级", "二年级", "三年级", "四年级",
                                                         "五年级", "六年级", "七年级"};
    return std::any_of(supported.begin(), supported.end(),
                       [&grade](const char* item) { return grade == item; });
}

NumberQuestion QuizService::GenerateGrade1Question() {
    static constexpr std::array<char, 2> operations = {'+', '-'};
    const int a = RandomInt(0, 9);
    const int b = RandomInt(0, 9);
    const char op = operations[RandomInt(0, static_cast<int>(operations.size() - 1))];

    int left = a;
    int right = b;
    int result = 0;
    if (op == '-') {
        if (left < right) {
            std::swap(left, right);
        }
        result = left - right;
    } else {
        result = left + right;
    }

    NumberQuestion question;
    question.prompt = std::to_string(left) + " " + op + " " + std::to_string(right) + " = ?";
    question.expected_value = static_cast<double>(result);
    question.tolerance = 1e-8;
    question.expects_integer = true;
    return question;
}

NumberQuestion QuizService::GenerateGrade2To3Question() {
    static constexpr std::array<char, 4> operations = {'+', '-', '*', '/'};
    const char op = operations[RandomInt(0, static_cast<int>(operations.size() - 1))];

    int left = 0;
    int right = 0;
    int result = 0;
    if (op == '+') {
        left = RandomInt(10, 99);
        right = RandomInt(10, 99);
        result = left + right;
    } else if (op == '-') {
        left = RandomInt(10, 99);
        right = RandomInt(10, 99);
        if (left < right) {
            std::swap(left, right);
        }
        result = left - right;
    } else if (op == '*') {
        left = RandomInt(10, 99);
        right = RandomInt(10, 99);
        result = left * right;
    } else {
        right = RandomInt(10, 19);
        const int quotient = RandomInt(2, 5);
        left = right * quotient;
        result = quotient;
    }

    NumberQuestion question;
    question.prompt = std::to_string(left) + " " + op + " " + std::to_string(right) + " = ?";
    question.expected_value = static_cast<double>(result);
    question.tolerance = 1e-8;
    question.expects_integer = true;
    return question;
}

NumberQuestion QuizService::GenerateDecimalQuestion() {
    static constexpr std::array<char, 4> operations = {'+', '-', '*', '/'};
    const char op = operations[RandomInt(0, static_cast<int>(operations.size() - 1))];

    double left = RandomOneDecimal(10.0, 99.9);
    double right = RandomOneDecimal(10.0, 99.9);
    if (op == '/' && left < right) {
        std::swap(left, right);
    }

    double result = 0.0;
    if (op == '+') {
        result = left + right;
    } else if (op == '-') {
        result = left - right;
    } else if (op == '*') {
        result = left * right;
    } else {
        result = left / right;
    }

    NumberQuestion question;
    std::stringstream prompt;
    prompt << std::fixed << std::setprecision(1) << left << " " << op << " " << right << " = ?";
    question.prompt = prompt.str();
    question.expected_value = RoundTo(result, 2);
    question.tolerance = 0.01;
    question.expects_integer = false;
    return question;
}

NumberQuestion QuizService::GeneratePowerQuestion() {
    const int base = RandomInt(1, 9);
    const int exponent = RandomInt(0, 4);
    int value = 1;
    for (int i = 0; i < exponent; ++i) {
        value *= base;
    }

    NumberQuestion question;
    question.prompt = std::to_string(base) + " ^ " + std::to_string(exponent) + " = ?";
    question.expected_value = static_cast<double>(value);
    question.tolerance = 1e-8;
    question.expects_integer = true;
    return question;
}

NumberQuestion QuizService::GenerateModuloQuestion() {
    int left = RandomInt(100, 999);
    int right = RandomInt(10, 499);
    if (left < right) {
        std::swap(left, right);
    }
    if (right == 0) {
        right = 1;
    }

    NumberQuestion question;
    question.prompt = std::to_string(left) + " % " + std::to_string(right) + " = ?";
    question.expected_value = static_cast<double>(left % right);
    question.tolerance = 1e-8;
    question.expects_integer = true;
    return question;
}

ComplexQuestion QuizService::GenerateComplexQuestion() {
    static constexpr std::array<char, 4> operations = {'+', '-', '*', '/'};
    const char op = operations[RandomInt(0, static_cast<int>(operations.size() - 1))];

    const double a = static_cast<double>(RandomInt(0, 9));
    const double b = static_cast<double>(RandomInt(0, 9));
    const double c = static_cast<double>(RandomInt(0, 9));
    double d = static_cast<double>(RandomInt(0, 9));
    if (op == '/' && std::fabs(c) < 1e-8 && std::fabs(d) < 1e-8) {
        d = 1.0;
    }

    double real = 0.0;
    double imag = 0.0;
    if (op == '+') {
        real = a + c;
        imag = b + d;
    } else if (op == '-') {
        real = a - c;
        imag = b - d;
    } else if (op == '*') {
        real = a * c - b * d;
        imag = a * d + b * c;
    } else {
        const double denominator = c * c + d * d;
        real = (a * c + b * d) / denominator;
        imag = (b * c - a * d) / denominator;
    }

    ComplexQuestion question;
    std::stringstream prompt;
    prompt << std::fixed << std::setprecision(1) << "(" << a << " + " << b << "i) " << op << " (" << c
           << " + " << d << "i) = ?（请输入：实部 虚部）";
    question.prompt = prompt.str();
    question.expected_real = RoundTo(real, 1);
    question.expected_imag = RoundTo(imag, 1);
    question.tolerance = 0.11;
    return question;
}

int QuizService::RandomInt(int min_inclusive, int max_inclusive) {
    std::uniform_int_distribution<int> dist(min_inclusive, max_inclusive);
    return dist(rng_);
}

double QuizService::RandomOneDecimal(double min_inclusive, double max_inclusive) {
    const int min_scaled = static_cast<int>(std::llround(min_inclusive * 10.0));
    const int max_scaled = static_cast<int>(std::llround(max_inclusive * 10.0));
    const int sampled = RandomInt(min_scaled, max_scaled);
    return sampled / 10.0;
}

double QuizService::RoundTo(double value, int decimals) {
    const double factor = std::pow(10.0, decimals);
    return std::round(value * factor) / factor;
}

bool QuizService::TryParseDouble(const std::string& text, double* value) {
    std::stringstream stream(text);
    double parsed = 0.0;
    stream >> parsed;
    if (stream.fail()) {
        return false;
    }
    stream >> std::ws;
    if (!stream.eof()) {
        return false;
    }
    *value = parsed;
    return true;
}

}  // namespace mathhelper
