#include "classes/Lexer.h"
#include "classes/parser.h"
#include "classes/SymbolTable.h"
#include "classes/CodeGen.h"

#include <iostream>
#include <fstream>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>\n";
        return 1;
    }

    std::string inputFile = argv[1];
    std::string outputFile = argv[2];

    std::ofstream outFile(outputFile);
    if (!outFile) {
        std::cerr << "Could not open output file.\n";
        return 1;
    }

    try {
        Lexer lexer(inputFile);
        SymbolTable symbolTable;
        CodeGen codeGen;

        Parser parser(lexer, symbolTable, codeGen);
        Parser::setOutputFile(outFile); 
        parser.parse();
        parser.outputParseTree(outFile);

        symbolTable.print(outFile);
        codeGen.print(outFile);
    } catch (const std::exception& e) {
        outFile << "Exception: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
