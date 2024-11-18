#pragma once
#include <iostream>
#include <set>
#include <map>

class DeterministicFiniteAutomaton {
public:
    using State = std::string;
    using Symbol = char;
    using Transition = std::map<std::pair<State, Symbol>, State>;

    DeterministicFiniteAutomaton(const std::set<State>& states, const std::set<Symbol>& alphabet,
        const Transition& transitions, const State& initialState,
        const std::set<State>& finalStates);

    bool VerifyAutomaton() const;

    void PrintAutomaton() const;
    //
    bool CheckWord(const std::string& word) const;

private:
    std::set<State> Q;
    std::set<Symbol> Sigma;
    Transition delta;
    State q0;
    std::set<State> F;
};