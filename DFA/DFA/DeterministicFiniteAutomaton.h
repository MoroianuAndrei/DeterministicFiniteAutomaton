#pragma once
#include <iostream>
#include <set>
#include <map>
#include <string>
#include <fstream>

class DeterministicFiniteAutomaton 
{
public:
    using Transition = std::map<std::pair<std::string, char>, std::string>;

    DeterministicFiniteAutomaton() = default;

    DeterministicFiniteAutomaton(
        const std::set<std::string>& states,
        const std::set<char>& alphabet,
        const std::map<std::pair<std::string, char>, std::string>& transitions,
        const std::string& initialState,
        const std::set<std::string>& finalStates
    );

    bool VerifyAutomaton() const;

    void PrintAutomaton() const;

    void PrintAutomatonInFile(std::ofstream& file);
    
    bool CheckWord(const std::string& word) const;

    void RenameStates();

private:
    std::set<std::string> Q;
    std::set<char> Sigma;
    std::map<std::pair<std::string, char>, std::string> delta;
    std::string q0;
    std::set<std::string> F;
};