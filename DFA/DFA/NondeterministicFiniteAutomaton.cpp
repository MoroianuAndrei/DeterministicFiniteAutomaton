#include "NondeterministicFiniteAutomaton.h"

int NondeterministicFiniteAutomaton::stateCounter = 0;

void NondeterministicFiniteAutomaton::AddState(const std::string& state, bool isInitial, bool isFinal) {
    states.insert(state);
    if (isInitial) {
        initialState = state;
    }
    if (isFinal) {
        finalStates.insert(state);
    }
}

void NondeterministicFiniteAutomaton::AddTransition(const std::string& from, char symbol, const std::string& to) {
    transitions.insert({ {from, symbol}, to });
    alphabet.insert(symbol); // Adăugăm simbolul în alfabet
}

void NondeterministicFiniteAutomaton::AddLambdaTransition(const std::string& from, const std::string& to) {
    transitions.insert({ {from, '\0'}, to });
}

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

NondeterministicFiniteAutomaton NondeterministicFiniteAutomaton::CreateBasicAutomaton(char symbol) {
    NondeterministicFiniteAutomaton automaton;
    std::string start = "q" + std::to_string(stateCounter++); // Folosește doar variabile statice sau locale
    std::string end = "q" + std::to_string(stateCounter++);
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
    std::string newStart = "q" + std::to_string(stateCounter++);
    std::string newEnd = "q" + std::to_string(stateCounter++);
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
    std::string newStart = "q" + std::to_string(stateCounter++);
    std::string newEnd = "q" + std::to_string(stateCounter++);
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

// Implementare funcție LambdaClosure pentru o stare
std::set<std::string> NondeterministicFiniteAutomaton::LambdaClosure(const std::string& state) const {
    std::set<std::string> closure;
    std::queue<std::string> toProcess;
    closure.insert(state);
    toProcess.push(state);

    while (!toProcess.empty()) {
        std::string current = toProcess.front();
        toProcess.pop();
        for (const auto& transition : transitions) {
            if (transition.first.first == current && transition.first.second == '\0') { // λ-tranziție
                if (closure.find(transition.second) == closure.end()) {
                    closure.insert(transition.second);
                    toProcess.push(transition.second);
                }
            }
        }
    }
    return closure;
}

// Implementare funcție LambdaClosure pentru o mulțime de stări
std::set<std::string> NondeterministicFiniteAutomaton::LambdaClosure(const std::set<std::string>& states) const {
    std::set<std::string> closure;
    for (const auto& state : states) {
        std::set<std::string> singleClosure = LambdaClosure(state);
        closure.insert(singleClosure.begin(), singleClosure.end());
    }
    return closure;
}
// Implementare funcție ConvertToDFA pentru conversia într-un AFD
DeterministicFiniteAutomaton NondeterministicFiniteAutomaton::ConvertToDFA() const {
    std::set<std::string> dfaStates;
    std::set<char> dfaAlphabet = alphabet; // Alfabetul DFA
    std::map<std::pair<std::string, char>, std::string> dfaTransitions;
    std::set<std::string> dfaFinalStates;
    std::map<std::set<std::string>, std::string> stateMapping;
    std::queue<std::set<std::string>> toProcess;

    // Calculăm ε-închiderea pentru starea inițială
    std::set<std::string> startClosure = LambdaClosure(initialState);
    std::string startState = "{" + initialState + "}"; // Numele stării inițiale în DFA
    stateMapping[startClosure] = startState;
    dfaStates.insert(startState);
    toProcess.push(startClosure);

    while (!toProcess.empty()) {
        std::set<std::string> currentSet = toProcess.front();
        toProcess.pop();
        std::string currentStateName = stateMapping[currentSet];

        // Iterăm prin fiecare simbol din alfabet
        for (const auto& symbol : dfaAlphabet) {
            if (symbol == '\0') continue; // Ignorăm λ

            std::set<std::string> moveSet;
            // Calculăm setul de stări atinse prin simbolul curent
            for (const auto& state : currentSet) {
                for (const auto& transition : transitions) {
                    if (transition.first.first == state && transition.first.second == symbol) {
                        moveSet.insert(transition.second);
                    }
                }
            }

            // Calculăm ε-închiderea pentru setul rezultat
            std::set<std::string> closure = LambdaClosure(moveSet);
            if (!closure.empty() && stateMapping.find(closure) == stateMapping.end()) {
                std::string newStateName = "{"; // Generăm un nume pentru noua stare
                for (const auto& s : closure) {
                    newStateName += s + ",";
                }
                newStateName.back() = '}'; // Închidem acolada
                stateMapping[closure] = newStateName;
                dfaStates.insert(newStateName);
                toProcess.push(closure); // Actualizăm toProcess cu noul set de stări
            }

            // Adăugăm tranziția în AFD
            if (!closure.empty()) {
                dfaTransitions[{currentStateName, symbol}] = stateMapping[closure];
            }
        }

        // Verificăm dacă starea curentă este finală
        for (const auto& state : currentSet) {
            if (finalStates.find(state) != finalStates.end()) {
                dfaFinalStates.insert(currentStateName);
                break;
            }
        }
    }

    // Returnăm AFD-ul construit
    return DeterministicFiniteAutomaton(dfaStates, dfaAlphabet, dfaTransitions, startState, dfaFinalStates);
}