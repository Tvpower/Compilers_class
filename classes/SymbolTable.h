#pragma once
#include <string>
#include <unordered_map>

struct Symbol {
    std::string type;
    int memoryAddress;
};

class SymbolTable {
public:
    bool declare(const std::string& name, const std::string& type);
    bool exists(const std::string& name) const;
    int getAddress(const std::string& name) const;
    std::string getType(const std::string& name) const;
    void print() const;
    void print(std::ostream& out) const;

private:
    std::unordered_map<std::string, Symbol> table;
    int currentAddress = 10000;
};
