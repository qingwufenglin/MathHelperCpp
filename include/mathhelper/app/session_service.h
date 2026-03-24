#ifndef MATHHELPER_APP_SESSION_SERVICE_H
#define MATHHELPER_APP_SESSION_SERVICE_H

#include <cstddef>
#include <vector>

#include "mathhelper/model/student.h"
#include "mathhelper/quiz/quiz_service.h"
#include "mathhelper/repository/i_student_repository.h"
#include "mathhelper/ui/console_ui.h"

namespace mathhelper {

class SessionService {
public:
    SessionService(IStudentRepository& repository, QuizService& quiz_service, ConsoleUi& ui);

    void Run();

private:
    void LoadStudents();
    void SaveStudents();
    void LoginFlow();
    void MainMenuLoop();
    void RunMentalTraining();
    void RunApplicationTraining();
    void ShowLeaderboard();

    bool IsMentalTrainingAllowed(const std::string& grade) const;
    int AskBackOrContinue() const;

    Student& CurrentStudent();
    const Student& CurrentStudent() const;

    IStudentRepository& repository_;
    QuizService& quiz_service_;
    ConsoleUi& ui_;

    std::vector<Student> students_;
    std::size_t current_index_ = 0;
};

}  // namespace mathhelper

#endif
