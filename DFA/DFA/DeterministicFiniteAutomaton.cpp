#include "DeterministicFiniteAutomaton.h"

DeterministicFiniteAutomaton::DeterministicFiniteAutomaton(
    const std::set<std::string>& states,
    const std::set<char>& alphabet,
    const std::map<std::pair<std::string, char>, std::string>& transitions,
    const std::string& initialState,
    const std::set<std::string>& finalStates
)
    : Q(states), Sigma(alphabet), delta(transitions), q0(initialState), F(finalStates) {
}


bool DeterministicFiniteAutomaton::VerifyAutomaton() const {
    if (Q.find(q0) == Q.end()) {
        std::cerr << "Starea initiala nu apartine multimii de stari.\n";
        return false;
    }
    for (const auto& finalState : F) {
        if (Q.find(finalState) == Q.end()) {
            std::cerr << "O stare finala nu apartine multimii de stari.\n";
            return false;
        }
    }
    for (const auto& transition : delta) {
        if (Q.find(transition.first.first) == Q.end() ||
            Sigma.find(transition.first.second) == Sigma.end() ||
            Q.find(transition.second) == Q.end()) {
            std::cerr << "O tranzitie nu este valida.\n";
            return false;
        }
    }
    return true;
}

void DeterministicFiniteAutomaton::PrintAutomaton() const {
    std::cout << "Stari: ";
    for (const auto& state : Q) {
        std::cout << state << " ";
    }
    std::cout << "\nAlfabet: ";
    for (const auto& symbol : Sigma) {
        std::cout << symbol << " ";
    }
    std::cout << "\nStare initiala: " << q0 << "\nStari finale: ";
    for (const auto& finalState : F) {
        std::cout << finalState << " ";
    }
    std::cout << "\nTranzitii:\n";
    for (const auto& transition : delta) {
        std::cout << "(" << transition.first.first << ", " << transition.first.second
            << ") -> " << transition.second << "\n";
    }
}

bool DeterministicFiniteAutomaton::CheckWord(const std::string& word) const {
    std::string currentState = q0;
    for (const auto& symbol : word) {
        if (Sigma.find(symbol) == Sigma.end()) {
            std::cerr << "Simbolul '" << symbol << "' nu este în alfabet.\n";
            return false;
        }
        auto it = delta.find({ currentState, symbol });
        if (it == delta.end()) {
            std::cerr << "Nu există tranziție din starea '" << currentState << "' cu simbolul '" << symbol << "'.\n";
            return false;
        }
        currentState = it->second;
    }
    return F.find(currentState) != F.end();
}

void DeterministicFiniteAutomaton::renameQs()
{
    int stateCounter = 0;
    for (auto transition : delta)
    {
        
    }
}
