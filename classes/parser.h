#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <iostream>
#include <stack>
#include <variant>
#include <string>
#include <fstream>

#include "Lexer.h"

class Parser {
private:
    Lexer& lexer;
    Token currentToken;
    std::stack<std::variant<std::string, TokenType>> parserStack;

    void advanceToken();
    bool match(TokenType expectedType);
    bool matchLexeme(const std::string& expectedLexeme);
    void error(const std::string& message);
    void initializeParserStack();

    // Parser methods for each grammar rule
    void parseRat25s();
    void parseProgram(); // New method for flexible program structure
    void parseOptFunctionDefinitions();
    void parseFunctionDefinitions();
    void parseFunction();
    void parseOptParameterList();
    void parseParameterList();
    void parseParameter();
    void parseQualifier();
    void parseBody();
    void parseOptDeclarationList();
    void parseDeclarationList();
    void parseDeclaration();
    void parseIDs();
    void parseStatementList();
    void parseStatement();
    void parseCompound(); // Added for R16
    void parseAssign();
    void parseIf();
    void parseReturn();
    void parsePrint();
    void parseScan();
    void parseWhile();
    void parseCondition();

    // Modified expression parsing methods (removing left recursion)
    void parseExpression();
    void parseExpressionPrime(); // New for left recursion removal
    void parseTerm();
    void parseTermPrime(); // New for left recursion removal
    void parseFactor();
    void parsePrimary();

public:
    explicit Parser(Lexer& lexer);

    // Configure the parser
    void setOutputFile(std::ofstream& outFile);
    void setRulePrinting(bool enabled);
    void fillParserStack(std::vector<std::string> tokens);

    // Run the parser
    void parse();

    // Output parse tree information
    void outputParseTree(std::ofstream& outFile);
};

#endif // PARSER_H