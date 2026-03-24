#ifndef MATHHELPER_REPOSITORY_FILE_STUDENT_REPOSITORY_H
#define MATHHELPER_REPOSITORY_FILE_STUDENT_REPOSITORY_H

#include <filesystem>
#include <vector>

#include "mathhelper/repository/i_student_repository.h"

namespace mathhelper {

class FileStudentRepository : public IStudentRepository {
public:
    explicit FileStudentRepository(std::filesystem::path file_path);

    std::vector<Student> LoadAll() override;
    void SaveAll(const std::vector<Student>& students) override;

private:
    std::filesystem::path file_path_;
};

}  // namespace mathhelper

#endif
