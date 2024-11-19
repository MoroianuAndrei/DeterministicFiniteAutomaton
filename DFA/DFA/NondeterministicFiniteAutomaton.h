#include <iostream>
#include <string>
#include <set>
#include <map>
#include <queue>
#include <stack>
#include "DeterministicFiniteAutomaton.h"

class NondeterministicFiniteAutomaton {
public:

    static int stateCounter; // Variabila statică pentru contorul stărilor

    NondeterministicFiniteAutomaton() = default;

    void AddState(const std::string& state, bool isInitial = false, bool isFinal = false);
    void AddTransition(const std::string& from, char symbol, const std::string& to);
    void AddLambdaTransition(const std::string& from, const std::string& to);

    void PrintAutomaton() const;
    NondeterministicFiniteAutomaton CreateBasicAutomaton(char symbol);
    NondeterministicFiniteAutomaton Concatenate(const NondeterministicFiniteAutomaton& a, const NondeterministicFiniteAutomaton& b);
    NondeterministicFiniteAutomaton Alternate(const NondeterministicFiniteAutomaton& a, const NondeterministicFiniteAutomaton& b);
    NondeterministicFiniteAutomaton KleeneStar(const NondeterministicFiniteAutomaton& a);
    NondeterministicFiniteAutomaton FromRegex(const std::string& regex);

    std::set<std::string> LambdaClosure(const std::string& state) const;
    std::set<std::string> LambdaClosure(const std::set<std::string>& states) const;
    DeterministicFiniteAutomaton ConvertToDFA() const;

private:
    std::set<std::string> states;
    std::set<std::string> finalStates;
    std::string initialState;
    std::map<std::pair<std::string, char>, std::string> transitions;
    std::set<char> alphabet;
};