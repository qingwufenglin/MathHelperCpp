#ifndef MATHHELPER_MODEL_STUDENT_H
#define MATHHELPER_MODEL_STUDENT_H

#include <string>

namespace mathhelper {

struct Student {
    std::string name;
    std::string grade;
    int score = 0;
};

}  // namespace mathhelper

#endif
