#include "parser.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string_view>

// Output file stream for production rules
std::ofstream* ruleOutputFile = nullptr;
bool printRules = true; // Switch to turn rule printing on/off

// Helper function to print production rules
void printProductionRule(const std::string& rule) {
    if (printRules && ruleOutputFile != nullptr) {
        *ruleOutputFile << "    " << rule << std::endl;
    }
}

// Helper function to print token and lexeme information
void printTokenInfo(const Token& token) {
    if (ruleOutputFile != nullptr) {
        std::string tokenStr;
        switch(token.type) {
            case TokenType::IDENT: tokenStr = "Identifier"; break;
            case TokenType::INT: tokenStr = "Integer"; break;
            case TokenType::REAL: tokenStr = "Real"; break;
            case TokenType::OPER: tokenStr = "Operator"; break;
            case TokenType::SEPA: tokenStr = "Separator"; break;
            case TokenType::KEYW: tokenStr = "Keyword"; break;
            case TokenType::COMM: tokenStr = "Comment"; break;
            case TokenType::UNKW: tokenStr = "Unknown"; break;
            case TokenType::END: tokenStr = "End"; break;
            default: tokenStr = "Unrecognized"; break;
        }
        *ruleOutputFile << "Token: " << tokenStr << "          Lexeme: " << token.lexeme << std::endl;
    }
}

void Parser::advanceToken(){
    if (currentToken.type != TokenType::END){
        // Print token information before advancing
        printTokenInfo(currentToken);
        currentToken = lexer.getNextToken();
    }
}

bool Parser::match(TokenType expectedType){
    return currentToken.type == expectedType;
}

bool Parser::matchLexeme(const std::string& expectedLexeme){
    return currentToken.lexeme == expectedLexeme;
}

void Parser::error(const std::string& message){
    if (ruleOutputFile != nullptr) {
        *ruleOutputFile << "Syntax error: " << message << " at token " << std::string(currentToken.lexeme) << std::endl;
    }
    std::cerr << "Syntax error: " << message << " at token " << std::string(currentToken.lexeme) << std::endl;
    throw std::runtime_error("Syntax error: " + message);
}

void Parser::initializeParserStack(){
    std::vector<std::string> keywords = {"function", "if", "else", "endif", "return", "print", "scan", "while", "endwhile", "true", "false", "integer", "boolean", "real"};
    std::vector<std::string> separators = {"(", ")", "{", "}", ";", ","};

    for (const auto& kw : keywords){
        parserStack.push(kw);
    }
    for (const auto& sep : separators){
        parserStack.push(sep);
    }
}

// R1. <Rat25S> ::= $$ <Program> $$
// Modified to be more flexible with input format
void Parser::parseRat25s(){
    printProductionRule("<Rat25S> ::= $$ <Program> $$");

    if (match(TokenType::SEPA) && currentToken.lexeme == "$$"){
        advanceToken();
        parseProgram();
        if (match(TokenType::SEPA) && currentToken.lexeme == "$$"){
            advanceToken();
        } else {
            error("Expected $$ at end of Rat25s");
        }
    } else {
        error("Expected $$ at start of Rat25s");
    }
}

// New method to handle a more flexible program structure
void Parser::parseProgram() {
    printProductionRule("<Program> ::= <Functions and Declarations and Statements>");

    // Continue parsing until we hit the closing $$ or end of file
    while (!match(TokenType::SEPA) || currentToken.lexeme != "$$") {
        if (match(TokenType::END)) {
            error("Unexpected end of file before closing $$");
        }
        else if (match(TokenType::KEYW)) {
            if (currentToken.lexeme == "function") {
                parseFunction();
            }
            else if (currentToken.lexeme == "integer" ||
                     currentToken.lexeme == "boolean" ||
                     currentToken.lexeme == "real") {
                parseDeclaration();
                if (match(TokenType::SEPA) && currentToken.lexeme == ";") {
                    advanceToken();
                } else {
                    error("Expected ';' after declaration");
                }
                     }
            else {
                parseStatement();
            }
        }
        else if (match(TokenType::IDENT)) {
            parseStatement();
        }
        else {
            error("Unexpected token in program");
        }
    }
}

// R2. <Opt Function Definitions> ::= <Function Definitions> | <Empty>
void Parser::parseOptFunctionDefinitions(){
    printProductionRule("<Opt Function Definitions> ::= <Function Definitions> | <Empty>");

    if (currentToken.type == TokenType::KEYW && currentToken.lexeme == "function"){
        parseFunctionDefinitions();
    }
    // Empty production - do nothing
}

// R3. <Function Definitions> ::= <Function> | <Function> <Function Definitions>
void Parser::parseFunctionDefinitions(){
    printProductionRule("<Function Definitions> ::= <Function> | <Function> <Function Definitions>");

    parseFunction();
    while (currentToken.type == TokenType::KEYW && currentToken.lexeme == "function"){
        parseFunction();
    }
}

// R4. <Function> ::= function <Identifier> ( <Opt Parameter List> ) <Opt Declaration List> <Body>
void Parser::parseFunction() {
    printProductionRule("<Function> ::= function <Identifier> ( <Opt Parameter List> ) <Opt Declaration List> <Body>");

    if (match(TokenType::KEYW) && currentToken.lexeme == "function") {
        advanceToken();
        if (match(TokenType::IDENT)) {
            advanceToken();
            if (match(TokenType::SEPA) && currentToken.lexeme == "(") {
                advanceToken();
                parseOptParameterList();
                if (match(TokenType::SEPA) && currentToken.lexeme == ")") {
                    advanceToken();
                    parseOptDeclarationList();
                    parseBody();
                } else {
                    error("Expected ')' after parameter list");
                }
            } else {
                error("Expected '(' after function identifier");
            }
        } else {
            error("Expected function identifier");
        }
    } else {
        error("Expected 'function' keyword");
    }
}

// R5. <Opt Parameter List> ::= <Parameter List> | <Empty>
void Parser::parseOptParameterList(){
    printProductionRule("<Opt Parameter List> ::= <Parameter List> | <Empty>");

    if (currentToken.type == TokenType::IDENT) {
        parseParameterList();
    }
    // Empty production - do nothing
}

// R6. <Parameter List> ::= <Parameter> | <Parameter> , <Parameter List>
void Parser::parseParameterList(){
    printProductionRule("<Parameter List> ::= <Parameter> | <Parameter> , <Parameter List>");

    parseParameter();
    while (match(TokenType::SEPA) && currentToken.lexeme == ",") {
        advanceToken();
        parseParameter();
    }
}

// R7. <Parameter> ::= <IDs > <Qualifier>
void Parser::parseParameter(){
    printProductionRule("<Parameter> ::= <IDs> <Qualifier>");

    parseIDs();
    parseQualifier();
}

// R8. <Qualifier> ::= integer | boolean | real
void Parser::parseQualifier() {
    printProductionRule("<Qualifier> ::= integer | boolean | real");

    if (match(TokenType::KEYW) && (currentToken.lexeme == "integer" || currentToken.lexeme == "boolean" || currentToken.lexeme == "real")) {
        advanceToken();
    } else {
        error("Expected type qualifier (integer, boolean, real)");
    }
}

// R9. <Body> ::= { < Statement List> }
void Parser::parseBody() {
    printProductionRule("<Body> ::= { <Statement List> }");

    if (match(TokenType::SEPA) && currentToken.lexeme == "{") {
        advanceToken();
        parseStatementList();
        if (match(TokenType::SEPA) && currentToken.lexeme == "}") {
            advanceToken();
        } else {
            error("Expected '}' at the end of function body");
        }
    } else {
        error("Expected '{' at the beginning of function body");
    }
}

// Back to declarations
// R10. <Opt Declaration List> ::= <Declaration List> | <Empty>
void Parser::parseOptDeclarationList(){
    printProductionRule("<Opt Declaration List> ::= <Declaration List> | <Empty>");

    if (currentToken.type == TokenType::KEYW && (currentToken.lexeme == "integer" || currentToken.lexeme == "boolean" || currentToken.lexeme == "real")){
        parseDeclarationList();
    }
    // Empty production - do nothing
}

// R11. <Declaration List> := <Declaration> ; | <Declaration> ; <Declaration List>
void Parser::parseDeclarationList(){
    printProductionRule("<Declaration List> ::= <Declaration> ; | <Declaration> ; <Declaration List>");

    parseDeclaration();
    if (match(TokenType::SEPA) && currentToken.lexeme == ";") {
        advanceToken();
        while (currentToken.type == TokenType::KEYW &&
              (currentToken.lexeme == "integer" || currentToken.lexeme == "boolean" || currentToken.lexeme == "real")) {
            parseDeclaration();
            if (match(TokenType::SEPA) && currentToken.lexeme == ";") {
                advanceToken();
            } else {
                error("Expected ';' after declaration");
            }
        }
    } else {
        error("Expected ';' after declaration");
    }
}

// R12. <Declaration> ::= <Qualifier > <IDs>
void Parser::parseDeclaration(){
    printProductionRule("<Declaration> ::= <Qualifier> <IDs>");

    parseQualifier();
    parseIDs();
}

// R13. <IDs> ::= <Identifier> | <Identifier>, <IDs>
void Parser::parseIDs(){
    printProductionRule("<IDs> ::= <Identifier> | <Identifier>, <IDs>");

    if (match(TokenType::IDENT)) {
        advanceToken();
        while(match(TokenType::SEPA) && currentToken.lexeme == ","){
            advanceToken();
            if (match(TokenType::IDENT)){
                advanceToken();
            } else {
                error("Expected an identifier after ',' in ID list");
            }
        }
    } else {
        error("Expected an Identifier");
    }
}

//Statements, body of code
// R14. <Statement List> ::= <Statement> | <Statement> <Statement List>
void Parser::parseStatementList(){
    printProductionRule("<Statement List> ::= <Statement> | <Statement> <Statement List>");

    parseStatement();
    while (currentToken.type != TokenType::END &&
           (currentToken.type != TokenType::SEPA || currentToken.lexeme != "}")){
        parseStatement();
    }
}

// R15. <Statement> ::= <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>
// Modified to also accept declarations inside function bodies
void Parser::parseStatement(){
    printProductionRule("<Statement> ::= <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While> | <Declaration>");

    if (match(TokenType::SEPA) && currentToken.lexeme == "{") {
        parseCompound();
    } else if (match(TokenType::IDENT)){
        parseAssign();
    } else if (match(TokenType::KEYW)){
        if (currentToken.lexeme == "if"){
            parseIf();
        } else if (currentToken.lexeme == "return"){
            parseReturn();
        } else if (currentToken.lexeme == "print"){
            parsePrint();
        } else if (currentToken.lexeme == "scan"){
            parseScan();
        } else if (currentToken.lexeme == "while"){
            parseWhile();
        } else if (currentToken.lexeme == "integer" || currentToken.lexeme == "boolean" || currentToken.lexeme == "real"){
            // Handle declarations inside function bodies
            parseDeclaration();
            if (match(TokenType::SEPA) && currentToken.lexeme == ";") {
                advanceToken();
            } else {
                error("Expected ';' after declaration");
            }
        } else {
            error("Unexpected keyword in statement");
        }
    } else {
        error("Invalid statement");
    }
}

// R16. <Compound> ::= { <Statement List> }
void Parser::parseCompound() {
    printProductionRule("<Compound> ::= { <Statement List> }");

    if (match(TokenType::SEPA) && currentToken.lexeme == "{") {
        advanceToken();
        parseStatementList();
        if (match(TokenType::SEPA) && currentToken.lexeme == "}") {
            advanceToken();
        } else {
            error("Expected '}' at the end of compound statement");
        }
    } else {
        error("Expected '{' at the beginning of compound statement");
    }
}

// R17. <Assign> ::= <Identifier> = <Expression> ;
void Parser::parseAssign(){
    printProductionRule("<Assign> ::= <Identifier> = <Expression> ;");

    if (match(TokenType::IDENT)){
        advanceToken();
        if (match(TokenType::OPER) && currentToken.lexeme == "="){
            advanceToken();
            parseExpression();
            if (match(TokenType::SEPA) && currentToken.lexeme == ";"){
                advanceToken();
            } else {
                error("Expected ';' after assignment");
            }
        } else {
            error("Expected an '=' in the assignment statement");
        }
    }
}

// R18. <If> ::= if ( <Condition> ) <Statement> endif |
// if ( <Condition> ) <Statement> else <Statement> endif
void Parser::parseIf(){
    printProductionRule("<If> ::= if ( <Condition> ) <Statement> endif | if ( <Condition> ) <Statement> else <Statement> endif");

    if (match(TokenType::KEYW) && currentToken.lexeme == "if") {
        advanceToken();
        if (match(TokenType::SEPA) && currentToken.lexeme == "("){
            advanceToken();
            parseCondition();
            if (match(TokenType::SEPA) && currentToken.lexeme == ")"){
                advanceToken();
                parseStatement();
                if (match(TokenType::KEYW) && currentToken.lexeme == "else"){
                    advanceToken();
                    parseStatement();
                }
                if (match(TokenType::KEYW) && currentToken.lexeme == "endif"){
                    advanceToken();
                } else {
                    error("Expected 'endif' at end of if statement");
                }
            } else {
                error("Expected ')' after condition in if statement");
            }
        } else {
            error("Expected '(' after 'if'");
        }
    }
}

// R19. <Return> ::= return ; | return <Expression> ;
void Parser::parseReturn(){
    printProductionRule("<Return> ::= return ; | return <Expression> ;");

    if (match(TokenType::KEYW) && currentToken.lexeme == "return") {
        advanceToken();
        if (match(TokenType::SEPA) && currentToken.lexeme == ";") {
            advanceToken();
        } else {
            parseExpression();
            if (match(TokenType::SEPA) && currentToken.lexeme == ";") {
                advanceToken();
            } else {
                error("Expected ';' after return expression");
            }
        }
    }
}

// R20. <Print> ::= print ( <Expression> );
void Parser::parsePrint() {
    printProductionRule("<Print> ::= print ( <Expression> );");

    if (match(TokenType::KEYW) && currentToken.lexeme == "print") {
        advanceToken();
        if (match(TokenType::SEPA) && currentToken.lexeme == "(") {
            advanceToken();
            parseExpression();
            if (match(TokenType::SEPA) && currentToken.lexeme == ")") {
                advanceToken();
                if (match(TokenType::SEPA) && currentToken.lexeme == ";") {
                    advanceToken();
                } else {
                    error("Expected ';' after print statement");
                }
            } else {
                error("Expected ')' after expression in print statement");
            }
        } else {
            error("Expected '(' after 'print'");
        }
    }
}

// R21. <Scan> ::= scan ( <IDs> );
void Parser::parseScan() {
    printProductionRule("<Scan> ::= scan ( <IDs> );");

    if (match(TokenType::KEYW) && currentToken.lexeme == "scan") {
        advanceToken();
        if (match(TokenType::SEPA) && currentToken.lexeme == "(") {
            advanceToken();
            parseIDs();
            if (match(TokenType::SEPA) && currentToken.lexeme == ")") {
                advanceToken();
                if (match(TokenType::SEPA) && currentToken.lexeme == ";") {
                    advanceToken();
                } else {
                    error("Expected ';' after scan statement");
                }
            } else {
                error("Expected ')' after IDs in scan statement");
            }
        } else {
            error("Expected '(' after 'scan'");
        }
    }
}

// R22. <While> ::= while ( <Condition> ) <Statement> endwhile [;]
// Modified to handle optional semicolon after endwhile
void Parser::parseWhile() {
    printProductionRule("<While> ::= while ( <Condition> ) <Statement> endwhile [;]");

    if (match(TokenType::KEYW) && currentToken.lexeme == "while") {
        advanceToken();
        if (match(TokenType::SEPA) && currentToken.lexeme == "(") {
            advanceToken();
            parseCondition();
            if (match(TokenType::SEPA) && currentToken.lexeme == ")") {
                advanceToken();
                parseStatement();
                if (match(TokenType::KEYW) && currentToken.lexeme == "endwhile") {
                    advanceToken();
                    // Handle optional semicolon after endwhile
                    if (match(TokenType::SEPA) && currentToken.lexeme == ";") {
                        advanceToken();
                    }
                } else {
                    error("Expected 'endwhile' after statement in while loop");
                }
            } else {
                error("Expected ')' after condition in while loop");
            }
        } else {
            error("Expected '(' after 'while'");
        }
    }
}

// R23. <Condition> ::= <Expression> <Relop> <Expression>
void Parser::parseCondition() {
    printProductionRule("<Condition> ::= <Expression> <Relop> <Expression>");

    parseExpression(); // Left side of the condition
    if (match(TokenType::OPER) && (currentToken.lexeme == "==" || currentToken.lexeme == "!=" ||
        currentToken.lexeme == ">" || currentToken.lexeme == "<" ||
        currentToken.lexeme == "<=" || currentToken.lexeme == ">=")) {
        advanceToken();
        parseExpression(); // Right side of the condition
    } else {
        error("Expected relational operator in condition");
    }
}

// R24. <Relop> ::= == | != | > | < | <= | >=
// Note: This is handled in parseCondition()

// Removing left recursion from the Expression grammar
// Original: R25. <Expression> ::= <Expression> + <Term> | <Expression> - <Term> | <Term>
// Modified: R25. <Expression> ::= <Term> <Expression'>
void Parser::parseExpression() {
    printProductionRule("<Expression> ::= <Term> <Expression'>");

    parseTerm();
    parseExpressionPrime();
}

// R25a. <Expression'> ::= + <Term> <Expression'> | - <Term> <Expression'> | ε
void Parser::parseExpressionPrime() {
    printProductionRule("<Expression'> ::= + <Term> <Expression'> | - <Term> <Expression'> | ε");

    if (match(TokenType::OPER) && (currentToken.lexeme == "+" || currentToken.lexeme == "-")) {
        std::string op = std::string(currentToken.lexeme); // Save the operator
        advanceToken();
        parseTerm();
        parseExpressionPrime();
    }
    // ε case - do nothing
}

// Removing left recursion from the Term grammar
// Original: R26. <Term> ::= <Term> * <Factor> | <Term> / <Factor> | <Factor>
// Modified: R26. <Term> ::= <Factor> <Term'>
void Parser::parseTerm() {
    printProductionRule("<Term> ::= <Factor> <Term'>");

    parseFactor();
    parseTermPrime();
}

// R26a. <Term'> ::= * <Factor> <Term'> | / <Factor> <Term'> | ε
void Parser::parseTermPrime() {
    printProductionRule("<Term'> ::= * <Factor> <Term'> | / <Factor> <Term'> | ε");

    if (match(TokenType::OPER) && (currentToken.lexeme == "*" || currentToken.lexeme == "/")) {
        std::string op = std::string(currentToken.lexeme); // Save the operator
        advanceToken();
        parseFactor();
        parseTermPrime();
    }
    // ε case - do nothing
}

// R27. <Factor> ::= - <Primary> | <Primary>
void Parser::parseFactor() {
    printProductionRule("<Factor> ::= - <Primary> | <Primary>");

    if (match(TokenType::OPER) && currentToken.lexeme == "-") {
        advanceToken();
        parsePrimary();
    } else {
        parsePrimary();
    }
}

// R28. <Primary> ::= <Identifier> | <Integer> | <Identifier> ( <IDs> ) | ( <Expression> ) | <Real> | true | false
void Parser::parsePrimary() {
    printProductionRule("<Primary> ::= <Identifier> | <Integer> | <Identifier> ( <IDs> ) | ( <Expression> ) | <Real> | true | false");

    if (match(TokenType::IDENT)) {
        advanceToken();
        // Check for function call syntax
        if (match(TokenType::SEPA) && currentToken.lexeme == "(") {
            advanceToken();
            parseIDs(); // Function call arguments
            if (match(TokenType::SEPA) && currentToken.lexeme == ")") {
                advanceToken();
            } else {
                error("Expected ')' after function call arguments");
            }
        }
    } else if (match(TokenType::INT) || match(TokenType::REAL)) {
        advanceToken();
    } else if (match(TokenType::SEPA) && currentToken.lexeme == "(") {
        advanceToken();
        parseExpression();
        if (match(TokenType::SEPA) && currentToken.lexeme == ")") {
            advanceToken();
        } else {
            error("Expected a matching ')' after sub-expression");
        }
    } else if (match(TokenType::KEYW) && (currentToken.lexeme == "true" || currentToken.lexeme == "false")) {
        advanceToken();
    } else {
        error("Expected an identifier, number, or sub-expression");
    }
}

Parser::Parser(Lexer& lexer) : lexer(lexer), currentToken(lexer.getNextToken()) {
    initializeParserStack();
}

void Parser::fillParserStack(std::vector<std::string> tokens) {
    // Clear the stack first (in case it already has elements)
    while (!parserStack.empty()) {
        parserStack.pop();
    }

    // Push tokens in reverse order so the first token is on top of the stack
    for (int i = tokens.size() - 1; i >= 0; --i) {
        parserStack.push(tokens[i]);
    }

    // Debug output to check stack content
    std::cout << "Parser stack filled with " << tokens.size() << " tokens." << std::endl;
}

void Parser::setOutputFile(std::ofstream& outFile) {
    ruleOutputFile = &outFile;
}

void Parser::setRulePrinting(bool enabled) {
    printRules = enabled;
}

void Parser::parse() {
    try {
        parseRat25s();
        if (ruleOutputFile != nullptr) {
            *ruleOutputFile << "Parsing completed successfully!" << std::endl;
        }
    } catch (const std::exception& e) {
        if (ruleOutputFile != nullptr) {
            *ruleOutputFile << "Parsing failed: " << e.what() << std::endl;
        }
        throw; // Re-throw the exception to be caught by the main program
    }
}

void Parser::outputParseTree(std::ofstream& outFile) {
    outFile << "\nParse Tree Summary:" << std::endl;
    outFile << "===================" << std::endl;
    outFile << "The parser applied a recursive descent parsing algorithm" << std::endl;
    outFile << "following the Rat25S grammar (with left recursion removed)" << std::endl;
    outFile << "to analyze the input program." << std::endl;

    // Output information about remaining tokens in the parser stack
    outFile << "\nRemaining items in parser stack: " << parserStack.size() << std::endl;
}