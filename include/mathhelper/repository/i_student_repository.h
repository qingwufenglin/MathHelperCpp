#ifndef MATHHELPER_REPOSITORY_I_STUDENT_REPOSITORY_H
#define MATHHELPER_REPOSITORY_I_STUDENT_REPOSITORY_H

#include <vector>

#include "mathhelper/model/student.h"

namespace mathhelper {

class IStudentRepository {
public:
    virtual ~IStudentRepository() = default;

    virtual std::vector<Student> LoadAll() = 0;
    virtual void SaveAll(const std::vector<Student>& students) = 0;
};

}  // namespace mathhelper

#endif
