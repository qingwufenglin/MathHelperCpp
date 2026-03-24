#include <clocale>
#include <iostream>
#include <locale>

#ifdef _WIN32
#include <windows.h>
#endif

#include "mathhelper/app/session_service.h"
#include "mathhelper/quiz/quiz_service.h"
#include "mathhelper/repository/file_student_repository.h"
#include "mathhelper/ui/console_ui.h"

int main() {
    try {
#ifdef _WIN32
        SetConsoleCP(CP_UTF8);
        SetConsoleOutputCP(CP_UTF8);
#endif
        std::setlocale(LC_ALL, ".UTF-8");
        try {
            std::locale::global(std::locale(""));
        } catch (const std::exception&) {
            // Ignore locale initialization failures and continue with UTF-8 code page.
        }

        mathhelper::FileStudentRepository repository("student.txt");
        mathhelper::QuizService quiz_service;
        mathhelper::ConsoleUi ui;
        mathhelper::SessionService session(repository, quiz_service, ui);
        session.Run();
    } catch (const std::exception& ex) {
        std::cerr << "程序运行失败: " << ex.what() << '\n';
        return 1;
    }
    return 0;
}
