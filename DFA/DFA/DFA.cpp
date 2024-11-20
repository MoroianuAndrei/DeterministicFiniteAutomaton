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
    std::cout << "Automatul Finit Nedeterminist creat din forma poloneza a expresiei regulate" << std::endl;
    nfa.PrintAutomaton();
    for (int i = 0; i < 10; i++)
        std::cout << std::endl;

    DeterministicFiniteAutomaton dfa;
    dfa = nfa.ConvertToDFA();

    std::cout << "Automatul Finit Determinist creat din Automatul Finit Nedeterminist cu lambda tranzitii" << std::endl;
    dfa.PrintAutomaton();
    for (int i = 0; i < 10; i++)
        std::cout << std::endl;

    // Rename
    dfa.RenameStates();

    std::cout << "Automatul Finit Determinist creat dupa redenumirea starilor" << std::endl;
    dfa.PrintAutomaton();

    return 0;
}