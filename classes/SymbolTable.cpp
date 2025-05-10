#include "SymbolTable.h"
#include <iostream>

bool SymbolTable::declare(const std::string& name, const std::string& type) {
    // Check if variable is already declared in current scope
    if (isInCurrentScope(name)) return false;
    
    // Add to current scope
    scopeStack.back()[name] = {type, currentAddress++};
    return true;
}

bool SymbolTable::exists(const std::string& name) const {
    // Check all scopes from innermost to outermost
    for (auto it = scopeStack.rbegin(); it != scopeStack.rend(); ++it) {
        if (it->count(name)) return true;
    }
    return false;
}

int SymbolTable::getAddress(const std::string& name) const {
    // Find in innermost scope first
    for (auto it = scopeStack.rbegin(); it != scopeStack.rend(); ++it) {
        if (it->count(name)) {
            return it->at(name).memoryAddress;
        }
    }
    throw std::runtime_error("Variable " + name + " not found in any scope");
}

std::string SymbolTable::getType(const std::string& name) const {
    // Find in innermost scope first
    for (auto it = scopeStack.rbegin(); it != scopeStack.rend(); ++it) {
        if (it->count(name)) {
            return it->at(name).type;
        }
    }
    throw std::runtime_error("Variable " + name + " not found in any scope");
}

void SymbolTable::print() const {
    std::cout << "\nSymbol Table:\n";
    for (size_t i = 0; i < scopeStack.size(); ++i) {
        std::cout << "Scope " << i << ":\n";
        for (const auto& [name, sym] : scopeStack[i]) {
            std::cout << "  " << name << " @ " << sym.memoryAddress << " : " << sym.type << "\n";
        }
    }
}

void SymbolTable::print(std::ostream& out) const {
    out << "\nSymbol Table:\n";
    for (size_t i = 0; i < scopeStack.size(); ++i) {
        out << "Scope " << i << ":\n";
        for (const auto& [name, sym] : scopeStack[i]) {
            out << "  " << name << " @ " << sym.memoryAddress << " : " << sym.type << "\n";
        }
    }
}

void SymbolTable::enterScope() {
    scopeStack.push_back({});
}

void SymbolTable::exitScope() {
    if (scopeStack.size() > 1) {  // Don't pop the global scope
        scopeStack.pop_back();
    }
}

bool SymbolTable::isInCurrentScope(const std::string& name) const {
    return scopeStack.back().count(name) > 0;
}