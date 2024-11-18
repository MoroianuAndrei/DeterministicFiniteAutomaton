﻿#include "DeterministicFiniteAutonom.h"

DeterministicFiniteAutomaton::DeterministicFiniteAutomaton(const std::set<State>& states, const std::set<Symbol>& alphabet,
    const Transition& transitions, const State& initialState,
    const std::set<State>& finalStates)
    : Q(states), Sigma(alphabet), delta(transitions), q0(initialState), F(finalStates) {
}

bool DeterministicFiniteAutomaton::VerifyAutomaton() const {
    if (Q.find(q0) == Q.end()) {
        std::cerr << "Starea inițială nu aparține mulțimii de stări.\n";
        return false;
    }
    for (const auto& finalState : F) {
        if (Q.find(finalState) == Q.end()) {
            std::cerr << "O stare finală nu aparține mulțimii de stări.\n";
            return false;
        }
    }
    for (const auto& transition : delta) {
        if (Q.find(transition.first.first) == Q.end() ||
            Sigma.find(transition.first.second) == Sigma.end() ||
            Q.find(transition.second) == Q.end()) {
            std::cerr << "O tranziție nu este validă.\n";
            return false;
        }
    }
    return true;
}

void DeterministicFiniteAutomaton::PrintAutomaton() const {
    std::cout << "Stări: ";
    for (const auto& state : Q) {
        std::cout << state << " ";
    }
    std::cout << "\nAlfabet: ";
    for (const auto& symbol : Sigma) {
        std::cout << symbol << " ";
    }
    std::cout << "\nStare inițială: " << q0 << "\nStări finale: ";
    for (const auto& finalState : F) {
        std::cout << finalState << " ";
    }
    std::cout << "\nTranziții:\n";
    for (const auto& transition : delta) {
        std::cout << "(" << transition.first.first << ", " << transition.first.second
            << ") -> " << transition.second << "\n";
    }
}

bool DeterministicFiniteAutomaton::CheckWord(const std::string& word) const {
    State currentState = q0;
    for (const auto& symbol : word) {
        if (Sigma.find(symbol) == Sigma.end()) {
            return false;
        }
        auto it = delta.find({ currentState, symbol });
        if (it == delta.end()) {
            return false;
        }
        currentState = it->second;
    }
    return F.find(currentState) != F.end();
}
