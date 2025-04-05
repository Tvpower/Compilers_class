#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <string>
#include "Lexer.h"
#include "parser.h"

// Function to map TokenType to string for debugging
std::string tokenTypeToString(TokenType type) {
    static const std::string tokenTypeStrings[] = {
        "identifier",  // IDENT
        "integer",     // INT
        "real",        // REAL
        "operator",    // OPER
        "separator",   // SEPA
        "keyword",     // KEYW
        "comment",     // COMM
        "unknown",     // UNKW
        "end"          // END
    };

    auto index = static_cast<size_t>(type);
    if (index < std::size(tokenTypeStrings)) {
        return tokenTypeStrings[index];
    }
    return "unrecognized";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file> [output_file]\n";
        return 1;
    }

    try {
        // Get file from arguments
        std::string inputFilename = argv[1];
        std::string outputFilename = (argc >= 3) ? argv[2] : inputFilename + ".out";

        // Create the lexer
        Lexer lexer(inputFilename);

        // Create output file
        std::ofstream outFile(outputFilename);
        if (!outFile) {
            throw std::runtime_error("Could not open output file: " + outputFilename);
        }

        // Set up buffered output for better performance
        std::vector<char> outBuffer(8192);
        outFile.rdbuf()->pubsetbuf(outBuffer.data(), outBuffer.size());

        // Write a header to the output file
        outFile << "Syntax Analysis of " << inputFilename << std::endl;
        outFile << "===================================" << std::endl << std::endl;

        // Create the parser with the lexer
        Parser parser(lexer);

        // Configure the parser
        parser.setOutputFile(outFile);
        parser.setRulePrinting(true); // Enable rule printing

        // Record parsing time for performance metrics
        auto startTime = std::chrono::high_resolution_clock::now();

        try {
            // Run the parser
            parser.parse();

            // Output parse tree if successful
            parser.outputParseTree(outFile);

            // Calculate and display parsing time
            auto endTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

            outFile << "\nParsing completed in " << duration.count() << " milliseconds." << std::endl;
            std::cout << "Parsing successful! Output written to " << outputFilename << std::endl;
            std::cout << "Parsing completed in " << duration.count() << " milliseconds." << std::endl;

        } catch (const std::exception& e) {
            // Calculate and display parsing time even when there's an error
            auto endTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

            outFile << "\nParsing failed in " << duration.count() << " milliseconds." << std::endl;
            std::cerr << "Parsing failed: " << e.what() << std::endl;
            std::cerr << "Parsing failed in " << duration.count() << " milliseconds." << std::endl;
            std::cerr << "Details written to " << outputFilename << std::endl;
            return 1;
        }

        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}