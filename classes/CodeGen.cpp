#include "CodeGen.h"
#include <iostream>

int CodeGen::emit(const std::string& op, const std::string& operand) {
    int addr = instructions.size() + 1;
    instructions.push_back({addr, op, operand});
    return addr;
}

void CodeGen::backpatch(int addr, const std::string& operand) {
    if (addr > 0 && addr <= instructions.size()) {
        instructions[addr - 1].operand = operand;
    }
}

int CodeGen::getNextAddress() const {
    return instructions.size() + 1;
}

void CodeGen::print() const {
    std::cout << "\nAssembly Code:\n";
    for (const auto& instr : instructions) {
        std::cout << instr.address << " " << instr.op;
        if (!instr.operand.empty())
            std::cout << " " << instr.operand;
        std::cout << "\n";
    }
}

void CodeGen::print(std::ostream& out) const {
    out << "\nAssembly Code:\n";
    for (const auto& instr : instructions) {
        out << instr.address << " " << instr.op;
        if (!instr.operand.empty())
            out << " " << instr.operand;
        out << "\n";
    }
}