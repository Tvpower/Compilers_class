#include <chrono>
#include <iostream>
#include "Lexer.h"
#include <vector>

// Function to map TokenType to string
std::string_view tokenTypeToString(TokenType type) {

    // Using a static array for O(1) lookup instead of a switch statement
    static const std::string_view tokenTypeStrings[] = {
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

    // Convert enum to index and bounds check
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
        //get file from arguments
        std::string inputFilename = argv[1];
        std::string outputFilename = (argc >= 3) ? argv[2] : inputFilename + ".out";

        Lexer lexer(inputFilename);

        std::ofstream outFile(outputFilename);
        if (!outFile) {
            throw std::runtime_error("Could not open output file");
        }

        // Use buffered output for better performance
        std::vector<char> outBuffer(8192);
        outFile.rdbuf()->pubsetbuf(outBuffer.data(), outBuffer.size());

        //write headers:
        // Format the header with fixed widths for better alignment
        outFile << std::left << std::setw(20) << "token" << "lexeme\n";
        outFile << std::string(40, '-') << "\n";

        //Start timing
        auto start = std::chrono::high_resolution_clock::now();

        //Process tokens in a simple loop
        Token token;
        while ((token = lexer.getNextToken()).type != TokenType::END) {
            std::string tokenStr(tokenTypeToString(token.type));
            std::string lexemeStr(token.lexeme);

            outFile << std::left << std::setw(20) << tokenStr << lexemeStr << "\n";
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;

        std::string stats = "\nLexical analysis completed in " + std::to_string(elapsed.count()) + "ms\n";

        outFile << stats;
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
