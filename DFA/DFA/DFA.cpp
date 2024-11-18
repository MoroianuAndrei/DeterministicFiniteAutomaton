//#include "PolishForm.h"
//#include "DeterministicFiniteAutonom.h"
//#include <string>
//#include <fstream>
//#include <iostream>
//
//int main() 
//{
//    std::queue<char> polish;
//    std::string str;
//    std::ifstream file("file.in");
//    file >> str;
//    file.close();
//    polish = PolishForm::convertToPolishForm(str);
//
//    while (!polish.empty()) {
//        std::cout << polish.front();
//        polish.pop();
//    }
//
//    return 0;
//}

#include "PolishForm.h"
#include "NondeterministicFiniteAutomaton.h"
#include <iostream>
#include <fstream>
#include <queue>
#include <string>

int main() {
    std::string regex;
    std::ifstream file("file.in");
    file >> regex;
    file.close();

    // Convertirea la forma poloneză inversată (RPN)
    PolishForm polishConverter;
    std::queue<char> rpnQueue = polishConverter.convertToPolishForm(regex);

    // Transformă forma poloneză inversată într-un string pentru AFN
    std::string rpn;
    while (!rpnQueue.empty()) {
        rpn += rpnQueue.front();
        rpnQueue.pop();
    }

    // Construiește AFN-ul din forma poloneză inversată
    NondeterministicFiniteAutomaton nfa;
    nfa = nfa.FromRegex(rpn);

    // Afișează automatul construit
    nfa.PrintAutomaton();

    return 0;
}
