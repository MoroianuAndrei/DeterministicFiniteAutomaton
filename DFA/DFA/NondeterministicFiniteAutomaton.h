#pragma once
#pragma once
#include <iostream>
#include <set>
#include <map>
#include <string>
#include <vector>
#include <stack>

class NondeterministicFiniteAutomaton {
public:
    using State = std::string;
    using Symbol = char;
    using Transition = std::multimap<std::pair<State, Symbol>, State>;

    NondeterministicFiniteAutomaton() = default;

    // Funcție pentru a adăuga stări și tranziții
    void AddState(const State& state, bool isInitial = false, bool isFinal = false);
    void AddTransition(const State& from, Symbol symbol, const State& to);
    void AddLambdaTransition(const State& from, const State& to);

    // Funcții de utilitate
    void PrintAutomaton() const;

    // Funcția de construcție a AFN-ului dintr-un regex
    NondeterministicFiniteAutomaton FromRegex(const std::string& regex);

private:
    std::set<State> states;
    std::set<Symbol> alphabet;
    Transition transitions;
    State initialState;
    std::set<State> finalStates;
    int stateCounter = 0;  // pentru generarea numelor unice de stare

    // Funcții helper pentru construcție
    NondeterministicFiniteAutomaton CreateBasicAutomaton(Symbol symbol);
    NondeterministicFiniteAutomaton Concatenate(const NondeterministicFiniteAutomaton& a, const NondeterministicFiniteAutomaton& b);
    NondeterministicFiniteAutomaton Alternate(const NondeterministicFiniteAutomaton& a, const NondeterministicFiniteAutomaton& b);
    NondeterministicFiniteAutomaton KleeneStar(const NondeterministicFiniteAutomaton& a);

};


