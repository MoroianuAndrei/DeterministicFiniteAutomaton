#include "PolishForm.h"
#include "DeterministicFiniteAutomaton.h"
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

    DeterministicFiniteAutomaton dfa;
    dfa = nfa.ConvertToDFA();

    dfa.PrintAutomaton();

    return 0;
}