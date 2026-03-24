#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "mathhelper/model/ranking.h"
#include "mathhelper/quiz/quiz_service.h"
#include "mathhelper/repository/file_student_repository.h"

namespace {

void TestMentalAnswerCheck() {
    mathhelper::QuizService quiz(123);

    mathhelper::NumberQuestion number;
    number.prompt = "10.0 + 2.3 = ?";
    number.expected_value = 12.3;
    number.tolerance = 0.01;
    number.expects_integer = false;

    assert(quiz.CheckMentalAnswer(mathhelper::MentalQuestion(number), "12.30"));
    assert(quiz.CheckMentalAnswer(mathhelper::MentalQuestion(number), "12.295"));
    assert(!quiz.CheckMentalAnswer(mathhelper::MentalQuestion(number), "12.40"));

    mathhelper::ComplexQuestion complex;
    complex.prompt = "complex";
    complex.expected_real = 3.2;
    complex.expected_imag = -1.4;
    complex.tolerance = 0.11;

    assert(quiz.CheckMentalAnswer(mathhelper::MentalQuestion(complex), "3.2 -1.4"));
    assert(!quiz.CheckMentalAnswer(mathhelper::MentalQuestion(complex), "3.6 -1.4"));
}

void TestGradeRoutingCoverage() {
    mathhelper::QuizService quiz(2026);

    bool has_decimal = false;
    bool has_power = false;
    bool has_modulo = false;
    for (int i = 0; i < 300; ++i) {
        const auto q6 = quiz.GenerateMentalQuestion("六年级");
        const std::string prompt =
            std::holds_alternative<mathhelper::NumberQuestion>(q6) ? std::get<mathhelper::NumberQuestion>(q6).prompt
                                                                    : std::get<mathhelper::ComplexQuestion>(q6).prompt;
        if (prompt.find('^') != std::string::npos) {
            has_power = true;
        } else if (prompt.find('%') != std::string::npos) {
            has_modulo = true;
        } else {
            has_decimal = true;
        }
    }

    bool has_complex = false;
    for (int i = 0; i < 500; ++i) {
        const auto q7 = quiz.GenerateMentalQuestion("七年级");
        if (std::holds_alternative<mathhelper::ComplexQuestion>(q7)) {
            has_complex = true;
            break;
        }
    }

    assert(has_decimal);
    assert(has_power);
    assert(has_modulo);
    assert(has_complex);
}

void TestGeometryProblems() {
    mathhelper::QuizService quiz(1);

    const auto square = quiz.GenerateGeometryProblem(mathhelper::GeometryShape::Square);
    assert(quiz.CheckGeometryAnswer(square, square.expected_perimeter, square.expected_area));
    assert(!quiz.CheckGeometryAnswer(square, square.expected_perimeter + 2.0, square.expected_area));

    const auto rectangle = quiz.GenerateGeometryProblem(mathhelper::GeometryShape::Rectangle);
    assert(quiz.CheckGeometryAnswer(rectangle, rectangle.expected_perimeter, rectangle.expected_area));

    const auto triangle = quiz.GenerateGeometryProblem(mathhelper::GeometryShape::Triangle);
    assert(quiz.CheckGeometryAnswer(triangle, triangle.expected_perimeter, triangle.expected_area));

    const auto circle = quiz.GenerateGeometryProblem(mathhelper::GeometryShape::Circle);
    assert(quiz.CheckGeometryAnswer(circle, circle.expected_perimeter, circle.expected_area));
}

void TestRanking() {
    std::vector<mathhelper::Student> students = {
        {"张三", "六年级", 20},
        {"李四", "五年级", 35},
        {"王五", "六年级", 35},
        {"赵六", "七年级", 10},
    };

    const auto ranked = mathhelper::BuildLeaderboard(students);
    assert(ranked.size() == 4);
    assert(ranked[0].name == "李四");
    assert(ranked[1].name == "王五");
    assert(ranked[2].name == "张三");
    assert(ranked[3].name == "赵六");
}

void TestRepositoryCompatibility() {
    const auto temp_file = std::filesystem::temp_directory_path() / "mathhelper_smoke_student.txt";
    {
        std::ofstream output(temp_file, std::ios::trunc);
        output << "张三\t六年级\t12\n";
        output << "李四\t五年级\t30\n";
    }

    mathhelper::FileStudentRepository repository(temp_file);
    auto students = repository.LoadAll();
    assert(students.size() == 2);
    assert(students[0].name == "张三");
    assert(students[0].grade == "六年级");
    assert(students[0].score == 12);

    students[0].score = 40;
    repository.SaveAll(students);

    auto reloaded = repository.LoadAll();
    assert(reloaded[0].score == 40);

    std::filesystem::remove(temp_file);
}

}  // namespace

int main() {
    TestMentalAnswerCheck();
    TestGradeRoutingCoverage();
    TestGeometryProblems();
    TestRanking();
    TestRepositoryCompatibility();
    std::cout << "[mathhelper_smoke] 全部测试通过。\n";
    return 0;
}
