﻿#include "NondeterministicFiniteAutomaton.h"

int NondeterministicFiniteAutomaton::stateCounter = 0;

void NondeterministicFiniteAutomaton::AddState(const std::string& state, bool isInitial, bool isFinal) 
{
    states.insert(state);
    if (isInitial) 
    {
        initialState = state;
    }
    if (isFinal) 
    {
        finalStates.insert(state);
    }
}

void NondeterministicFiniteAutomaton::AddTransition(const std::string& from, char symbol, const std::string& to) 
{
    transitions[{from, symbol}].insert(to);
    alphabet.insert(symbol);
}

void NondeterministicFiniteAutomaton::AddLambdaTransition(const std::string& from, const std::string& to)
{
    transitions[{from, '\0'}].insert(to);
}

void NondeterministicFiniteAutomaton::PrintAutomaton() const 
{
    std::cout << "Stari: ";
    for (const auto& state : states) 
    {
        std::cout << state << " ";
    }
    std::cout << std::endl << "Alfabet: ";
    for (const auto& symbol : alphabet) 
    {
        std::cout << symbol << " ";
    }
    std::cout << std::endl << "Tranzitii:" << std::endl;
    for (const auto& transition : transitions) 
    {
        std::cout << transition.first.first << " --"
            << (transition.first.second == '\0' ? "(lambda)" : std::string(1, transition.first.second))
            << "--> { ";
        for (const auto& target : transition.second) 
        {
            std::cout << target << " ";
        }
        std::cout << "}" << std::endl;
    }
    std::cout << "Starea initiala: " << initialState << std::endl << "Starile finale: ";
    for (const auto& state : finalStates) 
    {
        std::cout << state << " ";
    }
    std::cout << std::endl;
}


NondeterministicFiniteAutomaton NondeterministicFiniteAutomaton::CreateBasicAutomaton(char symbol)
{
    NondeterministicFiniteAutomaton automaton;
    std::string start = "q" + std::to_string(stateCounter++);
    std::string end = "q" + std::to_string(stateCounter++);
    automaton.AddState(start, true, false);
    automaton.AddState(end, false, true);
    automaton.AddTransition(start, symbol, end);
    automaton.alphabet.insert(symbol);
    return automaton;
}

NondeterministicFiniteAutomaton NondeterministicFiniteAutomaton::Concatenate(const NondeterministicFiniteAutomaton& a, const NondeterministicFiniteAutomaton& b)
{
    NondeterministicFiniteAutomaton result = a;
    for (const auto& finalState : a.finalStates) 
    {
        result.AddLambdaTransition(finalState, b.initialState);
    }
    result.states.insert(b.states.begin(), b.states.end());
    result.finalStates = b.finalStates;
    result.transitions.insert(b.transitions.begin(), b.transitions.end());
    result.alphabet.insert(b.alphabet.begin(), b.alphabet.end());
    return result;
}

NondeterministicFiniteAutomaton NondeterministicFiniteAutomaton::Alternate(const NondeterministicFiniteAutomaton& a, const NondeterministicFiniteAutomaton& b) 
{
    NondeterministicFiniteAutomaton result;
    std::string newStart = "q" + std::to_string(stateCounter++);
    std::string newEnd = "q" + std::to_string(stateCounter++);
    result.AddState(newStart, true, false);
    result.AddState(newEnd, false, true);
    result.AddLambdaTransition(newStart, a.initialState);
    result.AddLambdaTransition(newStart, b.initialState);
    for (const auto& finalState : a.finalStates) 
    {
        result.AddLambdaTransition(finalState, newEnd);
    }
    for (const auto& finalState : b.finalStates) 
    {
        result.AddLambdaTransition(finalState, newEnd);
    }
    result.states.insert(a.states.begin(), a.states.end());
    result.states.insert(b.states.begin(), b.states.end());
    result.states.insert({ newStart, newEnd });
    result.transitions.insert(a.transitions.begin(), a.transitions.end());
    result.transitions.insert(b.transitions.begin(), b.transitions.end());
    result.alphabet.insert(a.alphabet.begin(), a.alphabet.end());
    result.alphabet.insert(b.alphabet.begin(), b.alphabet.end());
    return result;
}

NondeterministicFiniteAutomaton NondeterministicFiniteAutomaton::KleeneStar(const NondeterministicFiniteAutomaton& a)
{
    NondeterministicFiniteAutomaton result;
    std::string newStart = "q" + std::to_string(stateCounter++);
    std::string newEnd = "q" + std::to_string(stateCounter++);
    result.AddState(newStart, true, false);
    result.AddState(newEnd, false, true);
    result.AddLambdaTransition(newStart, a.initialState);
    result.AddLambdaTransition(newStart, newEnd);
    for (const auto& finalState : a.finalStates) 
    {
        result.AddLambdaTransition(finalState, newEnd);
        result.AddLambdaTransition(finalState, a.initialState);
    }
    result.states.insert(a.states.begin(), a.states.end());
    result.states.insert({ newStart, newEnd });
    result.transitions.insert(a.transitions.begin(), a.transitions.end());
    result.alphabet.insert(a.alphabet.begin(), a.alphabet.end());
    return result;
}

NondeterministicFiniteAutomaton NondeterministicFiniteAutomaton::FromRegex(const std::string& regex) 
{
    std::stack<NondeterministicFiniteAutomaton> automatonStack;
    for (char symbol : regex) 
    {
        if (isalnum(symbol)) // Dacă este un caracter valid (literă sau număr)
        {
            automatonStack.push(CreateBasicAutomaton(symbol));
        }
        else if (symbol == '.') // Concatenare
        {
            NondeterministicFiniteAutomaton b = automatonStack.top(); automatonStack.pop();
            NondeterministicFiniteAutomaton a = automatonStack.top(); automatonStack.pop();
            automatonStack.push(Concatenate(a, b));
        }
        else if (symbol == '|') // Alternanță
        {
            NondeterministicFiniteAutomaton b = automatonStack.top(); automatonStack.pop();
            NondeterministicFiniteAutomaton a = automatonStack.top(); automatonStack.pop();
            automatonStack.push(Alternate(a, b));
        }
        else if (symbol == '*') // Kleene Star
        {
            NondeterministicFiniteAutomaton a = automatonStack.top(); automatonStack.pop();
            automatonStack.push(KleeneStar(a));
        }
        else if (symbol == '+') // Adăugat suport pentru "+"
        {
            NondeterministicFiniteAutomaton a = automatonStack.top(); automatonStack.pop();
            NondeterministicFiniteAutomaton starAutomaton = KleeneStar(a); // Creăm automatul pentru a*
            automatonStack.push(Concatenate(starAutomaton, a)); // Concatenăm a* cu a pentru a obține a+
        }
    }
    return automatonStack.top();
}

std::set<std::string> NondeterministicFiniteAutomaton::LambdaClosure(const std::string& state) const 
{
    std::set<std::string> closure;
    std::queue<std::string> toProcess;
    closure.insert(state);
    toProcess.push(state);

    while (!toProcess.empty()) 
    {
        std::string current = toProcess.front();
        toProcess.pop();

        auto range = transitions.equal_range({ current, '\0' });
        for (const auto& transition : transitions) {
            if (transition.first.first == current && transition.first.second == '\0')
            {
                for (const auto& target : transition.second) 
                {
                    if (closure.find(target) == closure.end()) 
                    {
                        closure.insert(target);
                        toProcess.push(target);
                    }
                }
            }
        }
    }
    return closure;
}

std::set<std::string> NondeterministicFiniteAutomaton::LambdaClosure(const std::set<std::string>& states) const
{
    std::set<std::string> closure;
    for (const auto& state : states)
    {
        std::set<std::string> singleClosure = LambdaClosure(state);
        closure.insert(singleClosure.begin(), singleClosure.end());
    }
    return closure;
}

DeterministicFiniteAutomaton NondeterministicFiniteAutomaton::ConvertToDFA() const
{
    std::set<std::string> dfaStates;
    std::set<char> dfaAlphabet = alphabet;
    std::map<std::pair<std::string, char>, std::string> dfaTransitions;
    std::set<std::string> dfaFinalStates;
    std::map<std::set<std::string>, std::string> stateMapping;
    std::queue<std::set<std::string>> toProcess;

    std::set<std::string> startClosure = LambdaClosure(initialState);
    std::string startState = "{" + initialState + "}";
    stateMapping[startClosure] = startState;
    dfaStates.insert(startState);
    toProcess.push(startClosure);

    while (!toProcess.empty()) 
    {
        std::set<std::string> currentSet = toProcess.front();
        toProcess.pop();
        std::string currentStateName = stateMapping[currentSet];

        for (const auto& symbol : dfaAlphabet) 
        {
            if (symbol == '\0') continue;

            std::set<std::string> moveSet;
            for (const auto& state : currentSet) 
            {
                for (const auto& transition : transitions)
                {
                    if (transition.first.first == state && transition.first.second == symbol) 
                    {
                        moveSet.insert(transition.second.begin(), transition.second.end());
                    }
                }
            }

            std::set<std::string> closure = LambdaClosure(moveSet);
            if (!closure.empty() && stateMapping.find(closure) == stateMapping.end())
            {
                std::string newStateName = "{";
                for (const auto& s : closure)
                {
                    newStateName += s + ",";
                }
                newStateName.back() = '}';
                stateMapping[closure] = newStateName;
                dfaStates.insert(newStateName);
                toProcess.push(closure);
            }

            if (!closure.empty()) 
            {
                dfaTransitions[{currentStateName, symbol}] = stateMapping[closure];
            }
        }

        for (const auto& state : currentSet) 
        {
            if (finalStates.find(state) != finalStates.end()) 
            {
                dfaFinalStates.insert(currentStateName);
                break;
            }
        }
    }

    return DeterministicFiniteAutomaton(dfaStates, dfaAlphabet, dfaTransitions, startState, dfaFinalStates);
}