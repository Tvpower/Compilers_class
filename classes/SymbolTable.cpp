#include "SymbolTable.h"
#include <iostream>

bool SymbolTable::declare(const std::string& name, const std::string& type) {
    if (table.count(name)) return false; // Already declared
    table[name] = {type, currentAddress++};
    return true;
}

bool SymbolTable::exists(const std::string& name) const {
    return table.count(name);
}

int SymbolTable::getAddress(const std::string& name) const {
    return table.at(name).memoryAddress;
}

std::string SymbolTable::getType(const std::string& name) const {
    return table.at(name).type;
}

void SymbolTable::print() const {
    std::cout << "\nSymbol Table:\n";
    for (const auto& [name, sym] : table) {
        std::cout << name << " @ " << sym.memoryAddress << " : " << sym.type << "\n";
    }
}

void SymbolTable::print(std::ostream& out) const {
    out << "\nSymbol Table:\n";
    for (const auto& [name, sym] : table) {
        out << name << " @ " << sym.memoryAddress << " : " << sym.type << "\n";
    }
}