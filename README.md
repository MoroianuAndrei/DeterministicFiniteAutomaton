# **Deterministic Finite Automaton (DFA)**

## 📌 **Overview**
This project focuses on transforming a **regular expression** into a **Deterministic Finite Automaton (DFA)** through a structured series of transformations. The process follows these key steps:

### **1. Regular Expression Processing**
- The regular expression is read from a file and converted into **postfix notation** for easier processing.

### **2. Nondeterministic Finite Automaton (NFA) Construction**
- The **postfix notation** is used to construct a **formal grammar**.
- Using this grammar, a **Nondeterministic Finite Automaton (NFA) with ε (lambda) transitions** is generated.

### **3. Conversion to Deterministic Finite Automaton (DFA)**
- The **NFA with ε-transitions** is systematically converted into an equivalent **DFA**.
- The DFA is then used to determine whether a given input string is **accepted or rejected**.

### **4. State Transition Representation**
- A **state transition list** is generated, illustrating how states transition based on input symbols.

## 🛠️ **Features**
✔ **Automated transformation** of a regular expression into a DFA.  
✔ **Efficient processing** of input strings to determine acceptance.  
