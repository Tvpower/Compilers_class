#include "classes/Lexer.h"
#include <vector>
#include <iostream>
#include <string>
#include <stack>

//struct ParseNode {
//    std::string value;
//    std::vector<std::shared_ptr<ParseNode>> children;
//}


class Parser{
private:
    Lexer lexer;
    Token currentToken;
    std::stack<TokenType> parseStack;

    void advanceToken(){
        if (currentToken.type != TokenType::END){
            currentToken = lexer.getNextToken();
        }
    }

    Token peekToken(){
        return lexer.getNextToken();
    }
    
    bool match(TokenType expectedType){
        if (currentToken.type == expectedType){
            advanceToken();
            return true;
        }
        return false;
    }

    void error(const std::string& message){
        std::cerr << "Syntax error: " << message << " at token " << currentToken.lexeme << std::endl;
    }


public:
    explicit Parser(Lexer &lex) : lexer(lex) { advanceToken(); }

    void parse() {
        while (currentToken.type != TokenType::END) {
            switch (currentToken.type) {
                case TokenType::KEYW:
                    parseKeyw();
                    break;
                case TokenType::IDENT:
                    parseIdent();
                    break;
                case TokenType::INT:
                    parseInt();
                    break;
                case TokenType::REAL:
                    parseReal();
                    break;
                case TokenType::OPER:
                    parseOper();
                    break;
                case TokenType::SEPA:
                    parseSepa();
                    break;
                case TokenType::COMM:
                    parseComm();
                    break;
                case TokenType::END:
                    parseEnd();
                    break;
                default:
                    error("Unexpected token type");
                    advanceToken();  // Skip the unexpected token
                    break;
            }
        }
    }

    void parseKeyw() {

    }

    void parseIdent() {

    }

    void parseInt() {

    }

    void parseReal() {

    }

    void parseOper() {

    }

    void parseSepa() {

    }

    void parseComm() {

    }

    void parseEnd() {

    }

    void parseExpression() {

    }

    void parseTerm() {

    }
};
