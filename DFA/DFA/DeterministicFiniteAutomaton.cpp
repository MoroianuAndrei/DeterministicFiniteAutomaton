#include "DeterministicFiniteAutomaton.h"

DeterministicFiniteAutomaton::DeterministicFiniteAutomaton(
    const std::set<std::string>& states,
    const std::set<char>& alphabet,
    const std::map<std::pair<std::string, char>, std::string>& transitions,
    const std::string& initialState,
    const std::set<std::string>& finalStates
)
    : Q(states), Sigma(alphabet), delta(transitions), q0(initialState), F(finalStates) {}


bool DeterministicFiniteAutomaton::VerifyAutomaton() const
{
    if (Q.find(q0) == Q.end()) 
    {
        std::cerr << "Starea initiala nu apartine multimii de stari." << std::endl;
        return false;
    }
    for (const auto& finalState : F)
    {
        if (Q.find(finalState) == Q.end()) 
        {
            std::cerr << "O stare finala nu apartine multimii de stari." << std::endl;
            return false;
        }
    }
    for (const auto& transition : delta) 
    {
        if (Q.find(transition.first.first) == Q.end() ||
            Sigma.find(transition.first.second) == Sigma.end() ||
            Q.find(transition.second) == Q.end()) 
        {
            std::cerr << "O tranzitie nu este valida." << std::endl;
            return false;
        }
    }
    return true;
}

void DeterministicFiniteAutomaton::PrintAutomaton() const 
{
    std::cout << "Stari: ";
    for (const auto& state : Q) 
    {
        std::cout << state << " ";
    }
    std::cout << std::endl << "Alfabet: ";
    for (const auto& symbol : Sigma) 
    {
        std::cout << symbol << " ";
    }
    std::cout << std::endl << "Stare initiala: " << q0 << std::endl << "Stari finale: ";
    for (const auto& finalState : F) 
    {
        std::cout << finalState << " ";
    }
    std::cout << std::endl << "Tranzitii:" << std::endl;
    for (const auto& transition : delta) 
    {
        std::cout << "(" << transition.first.first << ", " << transition.first.second
            << ") -> " << transition.second << std::endl;
    }
}

void DeterministicFiniteAutomaton::PrintAutomatonInFile(std::ofstream& file)
{
    file << "Stari: ";
    for (const auto& state : Q)
    {
        file << state << " ";
    }
    file << std::endl << "Alfabet: ";
    for (const auto& symbol : Sigma)
    {
        file << symbol << " ";
    }
    file << std::endl << "Stare initiala: " << q0 << std::endl << "Stari finale: ";
    for (const auto& finalState : F)
    {
        file << finalState << " ";
    }
    file << std::endl << "Tranzitii:" << std::endl;
    for (const auto& transition : delta)
    {
        file << "(" << transition.first.first << ", " << transition.first.second
            << ") -> " << transition.second << std::endl;
    }
}


bool DeterministicFiniteAutomaton::CheckWord(const std::string& word) const 
{
    std::string currentState = q0;
    for (const auto& symbol : word) 
    {
        if (Sigma.find(symbol) == Sigma.end()) 
        {
            std::cerr << "Simbolul '" << symbol << "' nu este in alfabet." << std::endl;
            return false;
        }
        auto it = delta.find({ currentState, symbol });
        if (it == delta.end())
        {
            std::cerr << "Nu exista tranzitie din starea '" << currentState << "' cu simbolul '" << symbol << "." << std::endl;
            return false;
        }
        currentState = it->second;
    }
    return F.find(currentState) != F.end();
}

void DeterministicFiniteAutomaton::RenameStates()
{
    std::map<std::string, std::string> stateRenameMap;
    int stateCounter = 0;

    for (const auto& state : Q) 
    {
        stateRenameMap[state] = "q" + std::to_string(stateCounter++) + "'";
    }

    q0 = stateRenameMap[q0];

    std::set<std::string> newFinalStates;
    for (const auto& state : F)
    {
        newFinalStates.insert(stateRenameMap[state]);
    }
    F = std::move(newFinalStates);

    std::map<std::pair<std::string, char>, std::string> newTransitions;
    for (const auto& transition : delta) 
    {
        std::string newFrom = stateRenameMap[transition.first.first];
        char symbol = transition.first.second;
        std::string newTo = stateRenameMap[transition.second];
        newTransitions[{newFrom, symbol}] = newTo;
    }
    delta = std::move(newTransitions);

    std::set<std::string> newStates;
    for (const auto& state : Q)
    {
        newStates.insert(stateRenameMap[state]);
    }
    Q = std::move(newStates);
}