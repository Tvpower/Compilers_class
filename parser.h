#ifndef PARSER_H
#define PARSER_H

#include "classes/Lexer.h"
#include <vector>
#include <iostream>
#include <stack>
#include <variant>
#include <string>

class Parser {
private:
    Lexer lexer;
    Token currentToken;
    std::stack<std::variant<std::string, TokenType>> parserStack;

    void advanceToken();
    bool match(TokenType expectedType);
    bool matchLexeme(const std::string& expectedLexeme);
    void error(const std::string& message);
    void initializeParserStack();
    void parseRat25s();
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
    void parseAssign();
    void parseIf();
    void parseReturn();
    void parsePrint();
    void parseScan();
    void parseWhile();
    void parseCondition();
    void parseExpression();
    void parseTerm();
    void parseFactor();
    void parsePrimary();

public:
    explicit Parser(Lexer& lexer);
    void parse();
};

#endif // PARSER_H
