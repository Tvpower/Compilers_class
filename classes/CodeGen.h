#pragma once
#include <string>
#include <vector>

struct Instruction {
    int address;
    std::string op;
    std::string operand;
};

class CodeGen {
public:
    int emit(const std::string& op, const std::string& operand = "");
    void backpatch(int addr, const std::string& operand);
    int getNextAddress() const;
    void print() const;
    void print(std::ostream& out) const;

private:
    std::vector<Instruction> instructions;
};
