#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <iostream>
#include <stack>
#include <variant>
#include <string>
#include <fstream>

#include "Lexer.h"
#include "CodeGen.h"
#include "SymbolTable.h"

class Parser {
private:
    Lexer& lexer;
    Token currentToken;
    std::stack<std::variant<std::string, TokenType>> parserStack;

    CodeGen& codeGen;
    SymbolTable& symbolTable;

    void advanceToken();
    bool match(TokenType expectedType) const;
    bool matchLexeme(const std::string& expectedLexeme) const;
    void error(const std::string& message) const;
    void initializeParserStack();
    void skipComments();

    // Grammar rule parsing methods
    void parseRat25s();
    void parseProgram();
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
    void parseCompound();
    void parseAssign();
    void parseIf();
    void parseReturn();
    void parsePrint();
    void parseScan();
    void parseWhile();
    void parseCondition();

    void parseExpression();
    void parseExpressionPrime();
    void parseTerm();
    void parseTermPrime();
    void parseFactor();
    void parsePrimary();

public:
    explicit Parser(Lexer& lexer, SymbolTable& symbolTable, CodeGen& codeGen);

    static void setOutputFile(std::ofstream& outFile);
    static void setRulePrinting(bool enabled);
    void fillParserStack(std::vector<std::string> tokens);

    void parse();
    void outputParseTree(std::ofstream& outFile) const;
};

#endif // PARSER_H
