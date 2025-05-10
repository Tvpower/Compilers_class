#pragma once
#include <string>
#include <unordered_map>
#include <vector>

struct Symbol {
    std::string type;
    int memoryAddress;
};

class SymbolTable {
private:
    // Stack of symbol tables for different scopes
    std::vector<std::unordered_map<std::string, Symbol>> scopeStack;
    int currentAddress;

public:
    SymbolTable() : currentAddress(10000) {
        // Initialize with global scope
        scopeStack.push_back({});
    }

    bool declare(const std::string& name, const std::string& type);
    bool exists(const std::string& name) const;
    int getAddress(const std::string& name) const;
    std::string getType(const std::string& name) const;
    void print() const;
    void print(std::ostream& out) const;

    // New methods for scope management
    void enterScope();
    void exitScope();
    bool isInCurrentScope(const std::string& name) const;
};
