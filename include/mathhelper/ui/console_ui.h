#ifndef MATHHELPER_UI_CONSOLE_UI_H
#define MATHHELPER_UI_CONSOLE_UI_H

#include <string>

namespace mathhelper {

class ConsoleUi {
public:
    void ClearScreen() const;
    void Pause() const;
    void PrintHeader(const std::string& subtitle) const;
    void PrintLine(const std::string& text) const;

    std::string ReadLine(const std::string& prompt) const;
    int ReadMenuChoice(const std::string& prompt, int min_choice, int max_choice) const;
    double ReadDouble(const std::string& prompt) const;

    static std::string CurrentTimeText();
};

}  // namespace mathhelper

#endif
