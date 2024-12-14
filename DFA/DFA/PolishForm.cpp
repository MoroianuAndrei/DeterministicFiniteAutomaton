#include "PolishForm.h"
#include <iostream>
#include <fstream>
#include <regex>

int PolishForm::priority(char c) 
{
    if (c == '(') return 0;
    if (c == '|') return 1;
    if (c == '.') return 2;
    if (c == '*') return 3;
    return -1; // pentru cazuri neașteptate
}

bool PolishForm::IsLetterOrNumber(const char& c) 
{
    auto rule = std::regex("[a-z]|[A-Z]|[0-9]*");
    return std::regex_match(std::string(1, c), rule);
}

std::queue<char> PolishForm::convertToPolishForm(std::string& str) 
{
    std::ifstream file("file.in");
    std::queue<char> polish;
    std::stack<char> op_stack;

    while (!file.eof()) 
    {
        file >> str;
        for (char c : str)
        {
            if (IsLetterOrNumber(c)) 
            {
                polish.push(c);
            }
            else 
            {
                if (c == '(') 
                {
                    op_stack.push(c);
                }
                else if (c == ')')
                {
                    while (!op_stack.empty() && op_stack.top() != '(') 
                    {
                        polish.push(op_stack.top());
                        op_stack.pop();
                    }
                    if (!op_stack.empty()) op_stack.pop();
                }
                else 
                {
                    while (!op_stack.empty() && priority(op_stack.top()) >= priority(c)) 
                    {
                        polish.push(op_stack.top());
                        op_stack.pop();
                    }
                    op_stack.push(c);
                }
            }
        }
    }

    while (!op_stack.empty()) 
    {
        polish.push(op_stack.top());
        op_stack.pop();
    }

    //while (!polish.empty()) 
    // {
    //    std::cout << polish.front();
    //    polish.pop();
    //}

    return polish;

    file.close();
}
