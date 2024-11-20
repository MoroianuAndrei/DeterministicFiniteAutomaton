#include "PolishForm.h"
#include "DeterministicFiniteAutomaton.h"
#include "NondeterministicFiniteAutomaton.h"
#include <iostream>
#include <fstream>
#include <queue>
#include <string>

#include <stack>
#include <cctype>

bool IsValidRegex(const std::string& expresie) {
    std::stack<char> paranteze;       // Stivă pentru a verifica echilibrarea parantezelor
    bool ultimulCaracterOperator = true; // Dacă ultimul caracter era un operator
    char ultimOperator = NULL;
    bool aFostCaracter = false;      // Dacă s-a găsit cel puțin un caracter valid

    for (size_t i = 0; i < expresie.size(); ++i) 
    {
        char c = expresie[i];

        if (std::isalnum(c)) 
        {
            // Caracter valid (literă sau cifră)
            ultimulCaracterOperator = false;
            aFostCaracter = true;
        }
        else if (c == '(') 
        {
            // Deschiderea unei paranteze
            paranteze.push(c);
            ultimulCaracterOperator = true; // Paranteza poate urma după un operator
        }
        else if (c == ')')
        {
            // Închiderea unei paranteze
            if (paranteze.empty()) 
            {
                return false; // Paranteză închisă fără deschidere
            }
            paranteze.pop();
            ultimulCaracterOperator = false; // Nu poate urma imediat un alt operator
        }
        else if (c == '|' || c == '*' || c == '+' || c == '.') 
        {
            // Operator (inclusiv concatenarea '.')
            if (ultimulCaracterOperator && c != '*' && ultimOperator != '*')
            {
                return false; // Nu poate urma un alt operator (cu excepția '*')
            }
            ultimulCaracterOperator = true;
            ultimOperator = c;
        }
        else 
        {
            // Caracter invalid
            return false;
        }
    }

    // Verifică dacă toate parantezele sunt închise
    if (!paranteze.empty())
    {
        return false;
    }

    if (ultimOperator != NULL && ultimOperator == '*')
        ultimulCaracterOperator = false;

    // Expresia trebuie să aibă cel puțin un caracter valid
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

int main() {
    std::string regex;
    std::ifstream file("file.in");
    file >> regex;
    file.close();

    if (!IsValidRegex(regex))
    {
        std::cout << "Expresia regulata nu este valida" << std::endl;
        return 1;
    }

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

    if (dfa.VerifyAutomaton())
    {
        int optiune;
        do 
        {
            std::cout << "\nMeniu:\n";
            std::cout << "1. Afisarea inteligibila a expresiei regulate.\n";
            std::cout << "2. Afisarea automatului finit determinist.\n";
            std::cout << "3. Verificarea unui cuvant in automat.\n";
            std::cout << "0. Iesire.\n";
            std::cout << "Alegeti o optiune: ";
            std::cin >> optiune;
            std::string word;

            switch (optiune) 
            {
            case 1:
                printRegex(regex);
                break;
            case 2:
                dfa.PrintAutomaton();
                break;
            case 3:
                std::cout << "Introduceti cuvantul ce se doreste verificat: ";
                std::cin >> word;
                if (dfa.CheckWord(word))
                {
                    std::cout << "Cuvantul " << word << " este acceptat de automat" << std::endl;
                }
                else
                {
                    std::cout << "Cuvantul " << word << " nu este acceptat de automat" << std::endl;
                }
                break;
            case 0:
                std::cout << "Iesire din program.\n";
                break;
            default:
                std::cout << "Optiune invalida. Încercati din nou.\n";
            }
        } while (optiune != 0);
    }
    else
    {
        std::cout << "Expresia regulata nu este valida" << std::endl;
    }

    return 0;
}