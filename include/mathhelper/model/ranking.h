#ifndef MATHHELPER_MODEL_RANKING_H
#define MATHHELPER_MODEL_RANKING_H

#include <vector>

#include "mathhelper/model/student.h"

namespace mathhelper {

std::vector<Student> BuildLeaderboard(const std::vector<Student>& students);

}  // namespace mathhelper

#endif
