#include "NondeterministicFiniteAutomaton.h"

// Funcție pentru a adăuga o stare
void NondeterministicFiniteAutomaton::AddState(const State& state, bool isInitial, bool isFinal) {
    states.insert(state);
    if (isInitial) {
        initialState = state;
    }
    if (isFinal) {
        finalStates.insert(state);
    }
}

// Funcție pentru a adăuga o tranziție
void NondeterministicFiniteAutomaton::AddTransition(const State& from, Symbol symbol, const State& to) {
    transitions.insert({ {from, symbol}, to });
}

// Funcție pentru a adăuga o λ-tranziție
void NondeterministicFiniteAutomaton::AddLambdaTransition(const State& from, const State& to) {
    transitions.insert({ {from, '\0'}, to });  // λ-tranziția este simbolizată prin '\0'
}

// Funcție pentru a afișa automatul
void NondeterministicFiniteAutomaton::PrintAutomaton() const {
    std::cout << "Stări: ";
    for (const auto& state : states) {
        std::cout << state << " ";
    }
    std::cout << "\nAlfabet: ";
    for (const auto& symbol : alphabet) {
        std::cout << symbol << " ";
    }
    std::cout << "\nTranziții:\n";
    for (const auto& transition : transitions) {
        std::cout << transition.first.first << " --"
            << (transition.first.second == '\0' ? "λ" : std::string(1, transition.first.second))
            << "--> " << transition.second << "\n";
    }
    std::cout << "Starea inițială: " << initialState << "\nStările finale: ";
    for (const auto& state : finalStates) {
        std::cout << state << " ";
    }
    std::cout << "\n";
}

// Funcție pentru a crea un AFN de bază pentru un simbol
NondeterministicFiniteAutomaton NondeterministicFiniteAutomaton::CreateBasicAutomaton(Symbol symbol) {
    NondeterministicFiniteAutomaton automaton;
    State start = "q" + std::to_string(stateCounter++); // Folosește doar variabile statice sau locale
    State end = "q" + std::to_string(stateCounter++);
    automaton.AddState(start, true, false);
    automaton.AddState(end, false, true);
    automaton.AddTransition(start, symbol, end);
    automaton.alphabet.insert(symbol);
    return automaton;
}

// Funcție pentru concatenare
NondeterministicFiniteAutomaton NondeterministicFiniteAutomaton::Concatenate(const NondeterministicFiniteAutomaton& a, const NondeterministicFiniteAutomaton& b) {
    NondeterministicFiniteAutomaton result = a;
    for (const auto& finalState : a.finalStates) {
        result.AddLambdaTransition(finalState, b.initialState);
    }
    result.states.insert(b.states.begin(), b.states.end());
    result.finalStates = b.finalStates;
    result.transitions.insert(b.transitions.begin(), b.transitions.end());
    result.alphabet.insert(b.alphabet.begin(), b.alphabet.end());
    return result;
}

// Funcție pentru alternare (sau)
NondeterministicFiniteAutomaton NondeterministicFiniteAutomaton::Alternate(const NondeterministicFiniteAutomaton& a, const NondeterministicFiniteAutomaton& b) {
    NondeterministicFiniteAutomaton result;
    State newStart = "q" + std::to_string(stateCounter++);
    State newEnd = "q" + std::to_string(stateCounter++);
    result.AddState(newStart, true, false);
    result.AddState(newEnd, false, true);
    result.AddLambdaTransition(newStart, a.initialState);
    result.AddLambdaTransition(newStart, b.initialState);
    for (const auto& finalState : a.finalStates) {
        result.AddLambdaTransition(finalState, newEnd);
    }
    for (const auto& finalState : b.finalStates) {
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

// Funcție pentru închiderea Kleene
NondeterministicFiniteAutomaton NondeterministicFiniteAutomaton::KleeneStar(const NondeterministicFiniteAutomaton& a) {
    NondeterministicFiniteAutomaton result;
    State newStart = "q" + std::to_string(stateCounter++);
    State newEnd = "q" + std::to_string(stateCounter++);
    result.AddState(newStart, true, false);
    result.AddState(newEnd, false, true);
    result.AddLambdaTransition(newStart, a.initialState);
    result.AddLambdaTransition(newStart, newEnd);
    for (const auto& finalState : a.finalStates) {
        result.AddLambdaTransition(finalState, newEnd);
        result.AddLambdaTransition(finalState, a.initialState);
    }
    result.states.insert(a.states.begin(), a.states.end());
    result.states.insert({ newStart, newEnd });
    result.transitions.insert(a.transitions.begin(), a.transitions.end());
    result.alphabet.insert(a.alphabet.begin(), a.alphabet.end());
    return result;
}

// Funcție pentru a construi un AFN dintr-un regex
NondeterministicFiniteAutomaton NondeterministicFiniteAutomaton::FromRegex(const std::string& regex) {
    std::stack<NondeterministicFiniteAutomaton> automatonStack;
    for (char symbol : regex) {
        if (isalnum(symbol)) {
            automatonStack.push(CreateBasicAutomaton(symbol));
        }
        else if (symbol == '.') {
            NondeterministicFiniteAutomaton b = automatonStack.top(); automatonStack.pop();
            NondeterministicFiniteAutomaton a = automatonStack.top(); automatonStack.pop();
            automatonStack.push(Concatenate(a, b));
        }
        else if (symbol == '|') {
            NondeterministicFiniteAutomaton b = automatonStack.top(); automatonStack.pop();
            NondeterministicFiniteAutomaton a = automatonStack.top(); automatonStack.pop();
            automatonStack.push(Alternate(a, b));
        }
        else if (symbol == '*') {
            NondeterministicFiniteAutomaton a = automatonStack.top(); automatonStack.pop();
            automatonStack.push(KleeneStar(a));
        }
    }
    return automatonStack.top();
}