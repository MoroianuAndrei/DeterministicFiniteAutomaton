#include "PolishForm.h"
#include "DeterministicFiniteAutonom.h"
#include <string>
#include <fstream>
#include <iostream>

int main() 
{
    std::queue<char> polish;
    std::string str;
    std::ifstream file("file.in");
    file >> str;
    file.close();
    polish = PolishForm::convertToPolishForm(str);

    while (!polish.empty()) {
        std::cout << polish.front();
        polish.pop();
    }

    return 0;
}
