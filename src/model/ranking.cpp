#include "mathhelper/model/ranking.h"

#include <algorithm>

namespace mathhelper {

std::vector<Student> BuildLeaderboard(const std::vector<Student>& students) {
    std::vector<Student> ranked = students;
    std::stable_sort(ranked.begin(), ranked.end(), [](const Student& lhs, const Student& rhs) {
        if (lhs.score != rhs.score) {
            return lhs.score > rhs.score;
        }
        return lhs.name < rhs.name;
    });
    return ranked;
}

}  // namespace mathhelper
