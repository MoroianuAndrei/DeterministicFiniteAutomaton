#pragma once
#include <string>
#include <stack>
#include <queue>

class PolishForm {
public:
    static int priority(char c);
    static bool IsLetterOrNumber(const char& c);
    static std::queue<char> convertToPolishForm(std::string& str);
};
