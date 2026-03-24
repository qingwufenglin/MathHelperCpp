#include "mathhelper/repository/file_student_repository.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

namespace mathhelper {

FileStudentRepository::FileStudentRepository(std::filesystem::path file_path)
    : file_path_(std::move(file_path)) {}

std::vector<Student> FileStudentRepository::LoadAll() {
    if (!std::filesystem::exists(file_path_)) {
        std::ofstream create_file(file_path_);
        if (!create_file) {
            throw std::runtime_error("无法创建学生数据文件: " + file_path_.string());
        }
    }

    std::ifstream input(file_path_);
    if (!input) {
        throw std::runtime_error("无法读取学生数据文件: " + file_path_.string());
    }

    std::vector<Student> students;
    std::string line;
    while (std::getline(input, line)) {
        if (line.empty()) {
            continue;
        }

        std::stringstream stream(line);
        std::string name;
        std::string grade;
        std::string score_text;
        if (!std::getline(stream, name, '\t')) {
            continue;
        }
        if (!std::getline(stream, grade, '\t')) {
            continue;
        }
        if (!std::getline(stream, score_text)) {
            continue;
        }

        try {
            Student student;
            student.name = name;
            student.grade = grade;
            student.score = std::stoi(score_text);
            students.push_back(std::move(student));
        } catch (const std::exception&) {
            continue;
        }
    }

    return students;
}

void FileStudentRepository::SaveAll(const std::vector<Student>& students) {
    std::ofstream output(file_path_, std::ios::trunc);
    if (!output) {
        throw std::runtime_error("无法写入学生数据文件: " + file_path_.string());
    }

    for (const auto& student : students) {
        output << student.name << '\t' << student.grade << '\t' << student.score << '\n';
    }
}

}  // namespace mathhelper
