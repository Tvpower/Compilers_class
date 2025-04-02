#include "parser.h"

class Parser{
private:
    Lexer lexer;
    Token currentToken;
    std::stack<std::variant<std::string, TokenType>> parserStack;

    void advanceToken(){
        if (currentToken.type != TokenType::END){
            currentToken = lexer.getNextToken();
        }
    }
    
    bool match(TokenType expectedType){
        return currentToken.type == expectedType;
    }

    bool Parser::matchLexeme(const std::string& expectedLexeme){
        return currentToken.lexeme == expectedLexeme;
    }

    void error(const std::string& message){
        std::cerr << "Syntax error: " << message << " at token " << currentToken.lexeme << std::endl;
    }

    void initializeParserStack(){
        std::vector<std::string> keywords = {"function", "if", "else", "endif", "return", "print", "scan", "while", "endwhile", "true", "false", "integer", "boolean", "real"};
        std::vector<std::string> separators = {"(", ")", "{", "}", ";", ","};

        for (const auto& kw : keywords){
            parserStack.push(kw);
        }
        for (const auto& sep : separators){
            parserStack.push(sep);
        }
    }

// R1. <Rat25S> ::= $$ <Opt Function Definitions> $$ <Opt Declaration List> $$ <Statement List> $$
    void parseRat25s(){
        if (match(TokenType::SEPA) && currentToken.lexeme == "$$"){
            advanceToken();
            parseOptFunctionDefinitions();
            if (match(TokenType::SEPA) && currentToken.lexeme == "$$"){
                advanceToken();
                parseOptDeclarationList();
                if (match(TokenType::SEPA) && currentToken.lexeme == "$$"){
                    advanceToken();
                    parseStatementList();
                    if (match(TokenType::SEPA) && currentToken.lexeme == "$$"){
                        advanceToken();
                    } else {
                        error("Expected $$ at end of Rat25s");
                    }
                } else {
                    error("Expected $$ before Statement List");
                }
            } else {
                error("Expected $$ before Declaration List");
            }
        } else {
            error("Expected $$ at start of Rat25s");
        }
    }

// R2. <Opt Function Definitions> ::= <Function Definitions> | <Empty>
    void parseOptFunctionDefinitions(){
        if (currentToken.type == TokenType::KEYW && currentToken.lexeme == "function"){
            parseFunctionDefinitions();
        }
    }

// R3. <Function Definitions> ::= <Function> | <Function> <Function Definitions>
    void parseFunctionDefinitions(){
        parseFunction();
        while (currentToken.type == TokenType::KEYW && currentToken.lexeme == "function"){
            parseFunction();
        }
    }

// R4. <Function> ::= function <Identifier> ( <Opt Parameter List> ) <Opt Declaration List> <Body>
    void parseFunction() {
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
    void parseOptParameterList(){
        if (currentToken.type == TokenType::IDENT) {
            parseParameterList();
        }
    }

// R6. <Parameter List> ::= <Parameter> | <Parameter> , <Parameter List>
    void parseParameterList(){
        parseParameter();
        while (match(TokenType::SEPA) && currentToken.lexeme == ",") {
            advanceToken();
            parseParameter();
        }
    }

// R7. <Parameter> ::= <IDs > <Qualifier>
    void parseParameter(){
        if (match(TokenType::IDENT)) {
            advanceToken();
            parseQualifier();
        } else {
            error("Expected identifier in parameter");
        }
    }

// R8. <Qualifier> ::= integer | boolean | real
    void parseQualifier() {
        if (match(TokenType::KEYW) && (currentToken.lexeme == "integer" || currentToken.lexeme == "boolean" || currentToken.lexeme == "real")) {
            advanceToken();
        } else {
            error("Expected type qualifier (integer, boolean, real)");
        }
    }

// R9. <Body> ::= { < Statement List> }
// R16. <Compound> ::= { <Statement List> }
    void parseBody() {
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
    void parseOptDeclarationList(){
        if (currentToken.type == TokenType::KEYW && (currentToken.lexeme == "integer" || currentToken.lexeme == "boolean" || currentToken.lexeme == "real")){
            parseDeclarationList();
        }
    }

// R11. <Declaration List> := <Declaration> ; | <Declaration> ; <Declaration List>
    void parseDeclarationList(){
        parseDeclaration();
        while (match(TokenType::SEPA) && currentToken.lexeme == ";"){
            advanceToken();
            if (currentToken.type == TokenType::KEYW){
                parseDeclaration();
            }
        }
    }

// R12. <Declaration> ::= <Qualifier > <IDs>
    void parseDeclaration(){
        parseQualifier();
        parseIDs();
    }

// R13. <IDs> ::= <Identifier> | <Identifier>, <IDs>
    void parseIDs(){
        if (match(TokenType::IDENT)) {
            advanceToken();
            while(match(TokenType::SEPA) && currentToken.lexeme == ","){
                advanceToken();
                if (match(TokenType::IDENT)){
                    advanceToken();
                } else {
                    error("Expected and identifier after ',' in Declaration list. ");
                }
            }
        } else {
            error("Expected an Identifier. ");
        }
    }

//Statements, body of code
// R14. <Statement List> ::= <Statement> | <Statement> <Statement List>
    void parseStatementList(){
        parseStatement();
        while (currentToken.type != TokenType::SEPA || currentToken.lexeme != "}"){
            parseStatement();
        }
    }

// R15. <Statement> ::= <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>
    void parseStatement(){
        if (currentToken.type == TokenType::IDENT){
            parseAssign();
        } else if (currentToken.type == TokenType::KEYW){
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
            } else {
                error("Unexpected keyword in statement");
            }
        } else {
            error("Invalid statement");
        }
    }

// R17. <Assign> ::= <Identifier> = <Expression> ;
    void parseAssign(){
        if (match(TokenType::IDENT)){
            advanceToken();
            if (match(TokenType::OPER) && currentToken.lexeme == "="){//When writing these, this program does not like '(item)', use "(item)" instead
                advanceToken();
                parseExpression();
                if (match(TokenType::SEPA) && currentToken.lexeme == ";"){
                    advanceToken();
                } else {
                    error("Expected ';' after assignment");
                }
            } else {
                error("Expected an '=' in the assignment statement. ");
            }
        }
    }
// R18. <If> ::= if ( <Condition> ) <Statement> endif |
// if ( <Condition> ) <Statement> else <Statement> endif
    void parseIf(){
        match(TokenType::KEYW);
        if (match(TokenType::SEPA) && currentToken.lexeme == "("){
            advanceToken();
            parseExpression();
            if (match(TokenType::SEPA) && currentToken.lexeme == ")"){
                advanceToken();
                parseStatement();
                if (match(TokenType::KEYW) && currentToken.lexeme == "else"){
                    advanceToken();
                    parseStatement();
                }
                if (!match(TokenType::KEYW) || currentToken.lexeme != "endif"){
                    error("Expected 'endif' at end of if statement");
                }
            }
        }
    }

// R19. <Return> ::= return ; | return <Expression> ;
    void parseReturn(){
        match(TokenType::KEYW);
        if (match(TokenType::SEPA) && currentToken.lexeme == ";") {
            advanceToken();
        } else {
            parseExpression();
            match(TokenType::SEPA);
        }
    }
    
// R20. <Print> ::= print ( <Expression> );
    void parsePrint() {
        if (match(TokenType::KEYW) && currentToken.lexeme == "print") {
            advanceToken();
            if (match(TokenType::SEPA) && currentToken.lexeme == "(") {
                advanceToken();
                parseExpression(); // R25
                if (match(TokenType::SEPA) && currentToken.lexeme == ")") {
                    advanceToken();
                    if (!match(TokenType::SEPA) || currentToken.lexeme != ";") {
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
    void parseScan() {
        if (match(TokenType::KEYW) && currentToken.lexeme == "scan") {
            advanceToken();
            if (match(TokenType::SEPA) && currentToken.lexeme == "(") {
                advanceToken();
                parseIDs(); // R13
                if (match(TokenType::SEPA) && currentToken.lexeme == ")") {
                    advanceToken();
                    if (!match(TokenType::SEPA) || currentToken.lexeme != ";") {
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

// R22. <While> ::= while ( <Condition> ) <Statement> endwhile
    void parseWhile() {
        if (match(TokenType::KEYW) && currentToken.lexeme == "while") {
            advanceToken();
            if (match(TokenType::SEPA) && currentToken.lexeme == "(") {
                advanceToken();
                parseCondition(); // R23
                if (match(TokenType::SEPA) && currentToken.lexeme == ")") {
                    advanceToken();
                    parseStatement(); // R15
                    if (!match(TokenType::KEYW) || currentToken.lexeme != "endwhile") {
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


    void parseCondition() {
        parseExpression(); // Left side of the condition
        if (match(TokenType::OPER) && (currentToken.lexeme == "==" || currentToken.lexeme == "!=" || currentToken.lexeme == ">" || currentToken.lexeme == "<" || currentToken.lexeme == "<=" || currentToken.lexeme == "=>")) {
            advanceToken();
            parseExpression(); // Right side of the condition
        } else {
            error("Expected relational operator in condition");
        }
    }
    
// R25. <Expression> ::= <Expression> + <Term> | <Expression> - <Term> | <Term>
    void parseExpression() {
        parseTerm();
        while (currentToken.type == TokenType::OPER && (currentToken.lexeme == "+" || currentToken.lexeme == "-")){
            advanceToken();
            parseTerm();
        }
    }

// R26. <Term> ::= <Term> * <Factor> | <Term> / <Factor> | <Factor>
    void parseTerm(){
        parseFactor();
        while (currentToken.type == TokenType::OPER && currentToken.lexeme == "*" || currentToken.lexeme == "/"){ // Given priority because it recursively calls back here before addition and subtraction
            advanceToken();
            parseFactor();
        }
    }

// R27. <Factor> ::= - <Primary> | <Primary>
    void parseFactor(){
        if (match(TokenType::OPER) && currentToken.lexeme == "-") {
            advanceToken();
            parsePrimary(); // R28
        } else {
            parsePrimary(); // R28
        }
    }

// R28. <Primary> ::= <Identifier> | <Integer> | <Identifier> ( <IDs> ) | ( <Expression> ) | <Real> | true | false
    void parsePrimary(){
        if (match(TokenType::IDENT)){
            advanceToken();
        } else if (match(TokenType::INT) || match(TokenType::REAL)){
            advanceToken();
        } else if (match(TokenType::SEPA) && currentToken.lexeme == "("){
            advanceToken();
            parseExpression();
            if (!match(TokenType::SEPA) || currentToken.lexeme != ")"){
                error("Expected a matching ')' after sub-expression");
            }
        } else if (match(TokenType::KEYW) && (currentToken.lexeme == "true" || currentToken.lexeme == "false")) {
            advanceToken();
        } else {
            error("Expected an identifier, number, or sub-expression");
        }
    }



public:
    explicit Parser(Lexer& lexer) : lexer(lexer), currentToken(lexer.getNextToken()){
        initializeParserStack();
    }

    void fillParserStack(std::vector<std::string> tokens) {
        for (int i = tokens.size() - 1; i >= 0; --i) {
            //if (tokens.type == TokenType::COMM) Have to not push comments
            parserStack.push(tokens[i]);
        }
    } //DO THIS FIRST, Store the tokens in reverse order to pop off the stack in order


    void parse(){
        parseRat25s();
    } // Error finder

    void outputParseTree(std::ofstream& outFile){

    }
};
