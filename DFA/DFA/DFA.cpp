#include "PolishForm.h"
#include "DeterministicFiniteAutomaton.h"
#include "NondeterministicFiniteAutomaton.h"
#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include <stack>

bool IsValidRegex(const std::string& expresie) 
{
    std::stack<char> paranteze;
    bool ultimulCaracterOperator = true;
    char ultimOperator = NULL;
    bool aFostCaracter = false;

    for (size_t i = 0; i < expresie.size(); ++i) 
    {
        char c = expresie[i];

        if (std::isalnum(c)) 
        {
            ultimulCaracterOperator = false;
            aFostCaracter = true;
        }
        else if (c == '(') 
        {
            paranteze.push(c);
            ultimulCaracterOperator = true;
        }
        else if (c == ')')
        {
            if (paranteze.empty()) 
            {
                return false;
            }
            paranteze.pop();
            ultimulCaracterOperator = false;
        }
        else if (c == '|' || c == '*' || c == '+' || c == '.') 
        {
            if (ultimulCaracterOperator && c != '*' && ultimOperator != '*')
            {
                return false;
            }
            ultimulCaracterOperator = true;
            ultimOperator = c;
        }
        else 
        {
            return false;
        }
    }

    if (!paranteze.empty())
    {
        return false;
    }

    if (ultimOperator != NULL && ultimOperator == '*')
        ultimulCaracterOperator = false;

    return aFostCaracter && !ultimulCaracterOperator;
}

void printRegex(std::string str)
{
    std::cout << "Expresia regulata este: ";
    for (char c : str)
        if (c != '.')
            std::cout << c;
    std::cout << std::endl;
}

int main() 
{
    std::string regex;
    std::ifstream file("file.in");
    file >> regex;
    file.close();

    if (!IsValidRegex(regex))
    {
        std::cout << "Expresia regulata nu este valida" << std::endl;
        return 1;
    }

    PolishForm polishConverter;
    std::queue<char> rpnQueue = polishConverter.convertToPolishForm(regex);

    std::string rpn;
    while (!rpnQueue.empty()) 
    {
        rpn += rpnQueue.front();
        rpnQueue.pop();
    }

    NondeterministicFiniteAutomaton nfa;
    nfa = nfa.FromRegex(rpn);

    //nfa.PrintAutomaton();
    //std::cout << std::endl;

    DeterministicFiniteAutomaton dfa;
    dfa = nfa.ConvertToDFA();

    dfa.RenameStates();

    if (dfa.VerifyAutomaton())
    {
        int optiune;
        do 
        {
            std::cout << "Meniu:" << std::endl;
            std::cout << "1. Afisarea inteligibila a expresiei regulate." << std::endl;
            std::cout << "2. Afisarea automatului finit determinist." << std::endl;
            std::cout << "3. Verificarea unui cuvant in automat." << std::endl;
            std::cout << "0. Iesire." << std::endl;
            std::cout << "Alegeti o optiune: ";
            std::cin >> optiune;
            std::string word;
            std::ofstream file("file.out");

            switch (optiune)
            {
            case 1:
                printRegex(regex);
                std::cout << std::endl;
                break;
            case 2:
                dfa.PrintAutomaton();
                dfa.PrintAutomatonInFile(file);
                std::cout << std::endl;
                break;
            case 3:
                std::cout << "Introduceti cuvantul ce se doreste verificat: ";
                std::cin >> word;
                if (dfa.CheckWord(word))
                {
                    std::cout << "Cuvantul ---- " << word << " ---- este acceptat de automat" << std::endl;
                }
                else
                {
                    std::cout << "Cuvantul ---- " << word << " ---- nu este acceptat de automat" << std::endl;
                }
                break;
            case 0:
                std::cout << "Iesire din program." << std::endl;
                break;
            default:
                std::cout << "Optiune invalida. Încercati din nou." << std::endl;
            }

            file.close();

        } while (optiune != 0);
    }
    else
    {
        std::cout << "Expresia regulata nu este valida" << std::endl;
    }

    return 0;
}