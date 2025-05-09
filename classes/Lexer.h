//
// Created by Tvpow on 2/8/2025.
//

#ifndef COMPILERSASSIGMENT1_LEXER_H
#define COMPILERSASSIGMENT1_LEXER_H
#include <string_view>
#include <unordered_map>
#include <array>
#include <string>
#include <functional>
#include <stdexcept>
#include <fstream>

//usiong string_view for faster string operations :b
using sv = std::string_view;

enum class State {
    START,
    IN_IDENTIFIER,
    IN_INTEGER,
    IN_REAL,
    IN_OPERATOR,
    IN_SEPARATOR,
    IN_COMMENT,
    ERROR,
    END
};

enum class TokenType {
    IDENT,  //Jae
    INT,   //Adam
    REAL,   //Adam
    OPER,  //Mario
    SEPA,  //Jae
    KEYW,  //Mario
    COMM, // ?
    UNKW, //This is whatever
    END //This is the end of the file so dont worry
};

struct Token {
    sv lexeme;
    TokenType type;
};


class Lexer {
private:
    //Buffer the entire file for faster access
    std::string buffer;
    size_t start = 0;
    size_t pos = 0;
    State currentState = State::START;

    static const std::unordered_map<sv, bool> keywords;


    //helpers
    char current() const {
        return pos < buffer.length() ? buffer[pos] : '\0';
    }
    

    char peek() const{
      return (pos + 1) < buffer.length() ? buffer[pos + 1] : '\0';
    };

    void advance(){ pos++; };

    sv getCurrentLexeme() const {
        size_t length = pos - start;
        // Trim trailing whitespace
        while (length > 0 && std::isspace(buffer[start + length - 1])) {
            length--;
        }
        return sv{buffer.data() + start, length};
    }

    
    bool isOperator(char c) const {
        return c == '<' || c == '>' || c == '=' ||
               c == '+' || c == '-' || c == '*' || c == '/';
    }
    bool isSeparator(char c ) const {
        return c == '(' || c == ')' || c == '{' || c == '}' ||
               c == '[' || c == ']' || c == ';' || c == ',' || c == '$';
    }
    using StateTransition = std::pair<State, std::function<void()>>;
    StateTransition handleStartState();
    StateTransition handleIdentifierState();
    StateTransition handleIntegerState();
    StateTransition handleRealState();
    StateTransition handleOperatorState();
    StateTransition handleCommentState();

    Token lastToken;

public:
    explicit Lexer(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary | std::ios::ate);
        if (!file) {
            throw std::runtime_error("Could not open input.txt file");
        }

        auto size = file.tellg();
        file.seekg(0);

        buffer.resize(size);
        file.read(buffer.data(), size);
    }
    Token getNextToken();
    Token peekToken() {
        size_t savedStart = start;
        size_t savedPos = pos;
        State savedState = currentState;
        Token savedLastToken = lastToken;
    
        Token peeked = getNextToken();
    
        start = savedStart;
        pos = savedPos;
        currentState = savedState;
        lastToken = savedLastToken;
    
        return peeked;
    }    
};



#endif //COMPILERSASSIGMENT1_LEXER_H
