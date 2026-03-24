#include "mathhelper/app/session_service.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "mathhelper/model/ranking.h"

namespace mathhelper {
namespace {

std::string NormalizeGradeInput(const std::string& raw_input) {
    if (raw_input == "1") {
        return "一年级";
    }
    if (raw_input == "2") {
        return "二年级";
    }
    if (raw_input == "3") {
        return "三年级";
    }
    if (raw_input == "4") {
        return "四年级";
    }
    if (raw_input == "5") {
        return "五年级";
    }
    if (raw_input == "6") {
        return "六年级";
    }
    if (raw_input == "7") {
        return "七年级";
    }
    return raw_input;
}

std::string FormatStudentLine(std::size_t rank, const Student& student) {
    std::stringstream stream;
    stream << "| " << std::setw(2) << rank << "  " << std::setw(10) << std::left << student.name << "  "
           << std::setw(10) << std::left << student.grade << "  " << std::setw(4) << std::right
           << student.score << " |";
    return stream.str();
}

}  // namespace

SessionService::SessionService(IStudentRepository& repository, QuizService& quiz_service, ConsoleUi& ui)
    : repository_(repository), quiz_service_(quiz_service), ui_(ui) {}

void SessionService::Run() {
    LoadStudents();
    LoginFlow();
    MainMenuLoop();
}

void SessionService::LoadStudents() {
    students_ = repository_.LoadAll();
}

void SessionService::SaveStudents() {
    repository_.SaveAll(students_);
}

void SessionService::LoginFlow() {
    while (true) {
        ui_.ClearScreen();
        ui_.PrintHeader("|------------欢迎来到数学小帮手------------|");
        const std::string name = ui_.ReadLine("请输入您的姓名: ");
        if (name.empty()) {
            ui_.PrintLine("姓名不能为空。");
            ui_.Pause();
            continue;
        }

        auto iterator = std::find_if(students_.begin(), students_.end(),
                                     [&name](const Student& student) { return student.name == name; });

        if (iterator != students_.end()) {
            current_index_ = static_cast<std::size_t>(std::distance(students_.begin(), iterator));
        } else {
            std::string grade;
            while (grade.empty()) {
                const std::string raw_grade =
                    ui_.ReadLine("请输入您的年级（可输入 1~7 或 中文，如 六年级）: ");
                grade = NormalizeGradeInput(raw_grade);
                if (grade.empty()) {
                    ui_.PrintLine("年级不能为空。");
                }
            }

            Student student;
            student.name = name;
            student.grade = grade;
            student.score = 0;
            students_.push_back(student);
            current_index_ = students_.size() - 1;
            SaveStudents();
        }

        ui_.ClearScreen();
        ui_.PrintHeader("|--------------欢--迎--回--来--------------|");
        ui_.PrintLine("|----------------" + CurrentStudent().name + " 同学----------------|");
        ui_.Pause();
        return;
    }
}

void SessionService::MainMenuLoop() {
    while (true) {
        ui_.ClearScreen();
        ui_.PrintHeader("|----------------主菜单--------------------|");
        ui_.PrintLine("|当前用户: " + CurrentStudent().name + "  年级: " + CurrentStudent().grade + "|");
        ui_.PrintLine("|累计对题数: " + std::to_string(CurrentStudent().score) + "|");
        ui_.PrintLine("|------------------------------------------|");
        ui_.PrintLine("|-------------<0>-退出系统-----------------|");
        ui_.PrintLine("|-------------<1>-口算题训练---------------|");
        ui_.PrintLine("|-------------<2>-应用题训练---------------|");
        ui_.PrintLine("|-------------<3>-排行榜-------------------|");
        ui_.PrintLine("|------------------------------------------|");

        const int choice = ui_.ReadMenuChoice("请输入(0~3): ", 0, 3);
        if (choice == 0) {
            return;
        }
        if (choice == 1) {
            RunMentalTraining();
            continue;
        }
        if (choice == 2) {
            RunApplicationTraining();
            continue;
        }
        ShowLeaderboard();
    }
}

void SessionService::RunMentalTraining() {
    if (!IsMentalTrainingAllowed(CurrentStudent().grade)) {
        ui_.ClearScreen();
        ui_.PrintHeader("|--------------访问限制提示----------------|");
        ui_.PrintLine("|-------本栏目仅对一~七年级同学开放!-------|");
        ui_.PrintLine("|当前年级信息: " + CurrentStudent().grade + "|");
        ui_.Pause();
        return;
    }

    bool continue_training = true;
    while (continue_training) {
        int correct_count = 0;
        ui_.ClearScreen();
        ui_.PrintHeader("|------------欢迎来到口算题训练------------|");
        ui_.PrintLine("|当前同学: " + CurrentStudent().grade + " " + CurrentStudent().name + "|");
        ui_.PrintLine("|目前累计做对: " + std::to_string(CurrentStudent().score) + " 题|");
        ui_.Pause();

        for (int index = 1; index <= 10; ++index) {
            const auto question = quiz_service_.GenerateMentalQuestion(CurrentStudent().grade);
            ui_.ClearScreen();
            ui_.PrintHeader("|----------------口算训练中----------------|");
            ui_.PrintLine("第 " + std::to_string(index) + " 题:");
            if (std::holds_alternative<NumberQuestion>(question)) {
                ui_.PrintLine(std::get<NumberQuestion>(question).prompt);
            } else {
                ui_.PrintLine(std::get<ComplexQuestion>(question).prompt);
            }

            const std::string answer = ui_.ReadLine("你的答案: ");
            if (quiz_service_.CheckMentalAnswer(question, answer)) {
                ++correct_count;
                ++CurrentStudent().score;
                ui_.PrintLine("回答正确!");
            } else {
                ui_.PrintLine("回答错误，正确答案为: " + quiz_service_.ExpectedAnswerText(question));
            }
            if (index != 10) {
                ui_.Pause();
            }
        }

        SaveStudents();

        ui_.ClearScreen();
        ui_.PrintHeader("|----------------训练结果------------------|");
        ui_.PrintLine("|本次正确率: " + std::to_string(correct_count * 10) + "%|");
        ui_.PrintLine("|累计对题数: " + std::to_string(CurrentStudent().score) + "|");
        continue_training = (AskBackOrContinue() == 1);
    }
}

void SessionService::RunApplicationTraining() {
    if (CurrentStudent().grade != "六年级") {
        ui_.ClearScreen();
        ui_.PrintHeader("|--------------访问限制提示----------------|");
        ui_.PrintLine("|---------本栏目仅对六年级同学开放!--------|");
        ui_.PrintLine("|当前年级信息: " + CurrentStudent().grade + "|");
        ui_.Pause();
        return;
    }

    bool continue_training = true;
    while (continue_training) {
        ui_.ClearScreen();
        ui_.PrintHeader("|------------欢迎来到应用题训练------------|");
        ui_.PrintLine("|------<0> 正方形----------<1> 长方形------|");
        ui_.PrintLine("|------<2> 三角形----------<3> 圆形--------|");
        const int shape_choice = ui_.ReadMenuChoice("请输入(0~3): ", 0, 3);
        const auto problem =
            quiz_service_.GenerateGeometryProblem(static_cast<GeometryShape>(shape_choice));

        ui_.ClearScreen();
        ui_.PrintHeader("|----------------应用题练习----------------|");
        ui_.PrintLine(problem.prompt);
        const double perimeter = ui_.ReadDouble("请输入周长: ");
        const double area = ui_.ReadDouble("请输入面积: ");

        std::stringstream expected;
        expected << std::fixed << std::setprecision(2) << "周长: " << problem.expected_perimeter
                 << "，面积: " << problem.expected_area;
        if (quiz_service_.CheckGeometryAnswer(problem, perimeter, area)) {
            ui_.PrintLine("恭喜你，回答正确!");
        } else {
            ui_.PrintLine("很遗憾，回答错误。正确答案为: " + expected.str());
        }

        continue_training = (AskBackOrContinue() == 1);
    }
}

void SessionService::ShowLeaderboard() {
    if (students_.empty()) {
        ui_.ClearScreen();
        ui_.PrintHeader("|----------------排行榜--------------------|");
        ui_.PrintLine("当前暂无学生数据。");
        ui_.Pause();
        return;
    }

    const auto leaderboard = BuildLeaderboard(students_);
    constexpr std::size_t page_size = 5;
    std::size_t page_index = 0;
    while (true) {
        ui_.ClearScreen();
        ui_.PrintHeader("|----------------排行榜--------------------|");
        ui_.PrintLine("|-------排名----姓名------年级------对题数--|");

        const std::size_t begin = page_index * page_size;
        const std::size_t end = std::min(begin + page_size, leaderboard.size());
        for (std::size_t i = begin; i < end; ++i) {
            ui_.PrintLine(FormatStudentLine(i + 1, leaderboard[i]));
        }

        if (end >= leaderboard.size()) {
            ui_.PrintLine("*------<0> 返回主页------*");
            ui_.ReadMenuChoice("请输入(0): ", 0, 0);
            return;
        }

        ui_.PrintLine("*------<0> 返回主页------<1> 下一页------*");
        const int choice = ui_.ReadMenuChoice("请输入(0~1): ", 0, 1);
        if (choice == 0) {
            return;
        }
        ++page_index;
    }
}

bool SessionService::IsMentalTrainingAllowed(const std::string& grade) const {
    return QuizService::IsSupportedMentalGrade(grade);
}

int SessionService::AskBackOrContinue() const {
    ui_.PrintLine("|--------------<0> 返回主页----------------|");
    ui_.PrintLine("|--------------<1> 继续练习----------------|");
    return ui_.ReadMenuChoice("请输入(0~1): ", 0, 1);
}

Student& SessionService::CurrentStudent() {
    if (students_.empty()) {
        throw std::runtime_error("当前无登录学生");
    }
    return students_.at(current_index_);
}

const Student& SessionService::CurrentStudent() const {
    if (students_.empty()) {
        throw std::runtime_error("当前无登录学生");
    }
    return students_.at(current_index_);
}

}  // namespace mathhelper
