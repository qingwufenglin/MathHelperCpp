#include "mathhelper/ui/console_ui.h"

#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string_view>

#ifdef _WIN32
#include <windows.h>
#endif

namespace mathhelper {
namespace {

std::string Trim(const std::string& text) {
    const auto begin = text.find_first_not_of(" \t\r\n");
    if (begin == std::string::npos) {
        return {};
    }
    const auto end = text.find_last_not_of(" \t\r\n");
    return text.substr(begin, end - begin + 1);
}

bool TryParseInt(const std::string& text, int* value) {
    std::stringstream stream(text);
    int parsed = 0;
    stream >> parsed;
    if (stream.fail()) {
        return false;
    }
    stream >> std::ws;
    if (!stream.eof()) {
        return false;
    }
    *value = parsed;
    return true;
}

bool TryParseDouble(const std::string& text, double* value) {
    std::stringstream stream(text);
    double parsed = 0.0;
    stream >> parsed;
    if (stream.fail()) {
        return false;
    }
    stream >> std::ws;
    if (!stream.eof()) {
        return false;
    }
    *value = parsed;
    return true;
}

std::wstring Utf8ToWide(const std::string_view utf8_text) {
#ifdef _WIN32
    if (utf8_text.empty()) {
        return {};
    }

    const int wide_size = MultiByteToWideChar(CP_UTF8, 0, utf8_text.data(),
                                              static_cast<int>(utf8_text.size()), nullptr, 0);
    if (wide_size <= 0) {
        return {};
    }

    std::wstring wide_text(static_cast<std::size_t>(wide_size), L'\0');
    const int converted = MultiByteToWideChar(CP_UTF8, 0, utf8_text.data(),
                                              static_cast<int>(utf8_text.size()), wide_text.data(),
                                              wide_size);
    if (converted <= 0) {
        return {};
    }
    return wide_text;
#else
    (void)utf8_text;
    return {};
#endif
}

void WriteUtf8(const std::string& text) {
#ifdef _WIN32
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    if (output != INVALID_HANDLE_VALUE && GetConsoleMode(output, &mode)) {
        const std::wstring wide_text = Utf8ToWide(text);
        if (!wide_text.empty()) {
            DWORD written = 0;
            WriteConsoleW(output, wide_text.c_str(), static_cast<DWORD>(wide_text.size()),
                          &written, nullptr);
            return;
        }
    }
#endif
    std::cout << text;
}

}  // namespace

void ConsoleUi::ClearScreen() const {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

void ConsoleUi::Pause() const {
    ReadLine("按回车键继续...");
}

void ConsoleUi::PrintHeader(const std::string& subtitle) const {
    WriteUtf8("*--------------【数学小帮手】--------------*\n");
    WriteUtf8("|------------当前时间--" + CurrentTimeText() + "------------|\n");
    WriteUtf8("|------------------------------------------|\n");
    if (!subtitle.empty()) {
        WriteUtf8(subtitle + "\n");
        WriteUtf8("|------------------------------------------|\n");
    }
}

void ConsoleUi::PrintLine(const std::string& text) const {
    WriteUtf8(text + "\n");
}

std::string ConsoleUi::ReadLine(const std::string& prompt) const {
    WriteUtf8(prompt);
    std::string input;
    std::getline(std::cin, input);
    return Trim(input);
}

int ConsoleUi::ReadMenuChoice(const std::string& prompt, int min_choice, int max_choice) const {
    while (true) {
        const std::string raw = ReadLine(prompt);
        int value = 0;
        if (TryParseInt(raw, &value) && value >= min_choice && value <= max_choice) {
            return value;
        }
        WriteUtf8("输入错误，请重新输入。\n");
    }
}

double ConsoleUi::ReadDouble(const std::string& prompt) const {
    while (true) {
        const std::string raw = ReadLine(prompt);
        double value = 0.0;
        if (TryParseDouble(raw, &value)) {
            return value;
        }
        WriteUtf8("输入格式错误，请输入数字。\n");
    }
}

std::string ConsoleUi::CurrentTimeText() {
    using clock = std::chrono::system_clock;
    const auto now = clock::to_time_t(clock::now());
    std::tm time_info{};
#ifdef _WIN32
    localtime_s(&time_info, &now);
#else
    localtime_r(&now, &time_info);
#endif
    std::stringstream stream;
    stream << std::put_time(&time_info, "%H:%M:%S");
    return stream.str();
}

}  // namespace mathhelper
