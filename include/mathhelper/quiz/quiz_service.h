#ifndef MATHHELPER_QUIZ_QUIZ_SERVICE_H
#define MATHHELPER_QUIZ_QUIZ_SERVICE_H

#include <cstdint>
#include <random>
#include <string>

#include "mathhelper/quiz/quiz_types.h"

namespace mathhelper {

class QuizService {
public:
    explicit QuizService(std::uint32_t seed = std::random_device{}());

    MentalQuestion GenerateMentalQuestion(const std::string& grade);
    GeometryProblem GenerateGeometryProblem(GeometryShape shape);

    bool CheckMentalAnswer(const MentalQuestion& question, const std::string& raw_input) const;
    bool CheckGeometryAnswer(const GeometryProblem& problem, double perimeter, double area) const;

    std::string ExpectedAnswerText(const MentalQuestion& question) const;
    static bool IsSupportedMentalGrade(const std::string& grade);

private:
    NumberQuestion GenerateGrade1Question();
    NumberQuestion GenerateGrade2To3Question();
    NumberQuestion GenerateDecimalQuestion();
    NumberQuestion GeneratePowerQuestion();
    NumberQuestion GenerateModuloQuestion();
    ComplexQuestion GenerateComplexQuestion();

    int RandomInt(int min_inclusive, int max_inclusive);
    double RandomOneDecimal(double min_inclusive, double max_inclusive);
    static double RoundTo(double value, int decimals);
    static bool TryParseDouble(const std::string& text, double* value);

    std::mt19937 rng_;
};

}  // namespace mathhelper

#endif
