//
// Created by Tvpow on 2/8/2025.
//

#ifndef COMPILERSASSIGMENT1_LEXER_H
#define COMPILERSASSIGMENT1_LEXER_H
#include <string_view>
#include <unordered_map>
#include <array>
#include <string>

//usiong string_view for faster string operations :b
using sv = std::string_view;

class CharacterTables {
    std::array<bool, 128> isValidIdChar_;
    std::array<bool, 128> isOperatorChar_;
    std::array<bool, 128> isSeparatorChar_;

public:
    CharacterTables();

    bool isValidIdChar(char c) const {return c < 128 && isValidIdChar_[c];}
    bool isOperatorChar(char c) const {return c < 128 && isOperatorChar_[c];}
    bool isSeparatorChar(char c) const {return c < 128 && isSeparatorChar_[c];}
};

enum class TokenType {
    IDENT,
    INT,
    REAL,
    OPER,
    SEPA,
    KEYW,
    COMM,
    UNKW,
    END
};

struct Token {
    sv lexeme;
    TokenType type;
};


class Lexer {
private:
    //Buffer the entire file for faster access
    std::string buffer;
    size_t pos = 0;
    static const CharacterTables characterTables;
    static const std::unordered_map<sv, bool> keywords;

    //private helpers
    char current() const;
    char peek() const;
    void advance();
    void skipWhitespace();
    Token processIdentifier();
    Token processNumber();

public:
    explicit Lexer(const std::string& filename);
    Token getNextToken();
};



#endif //COMPILERSASSIGMENT1_LEXER_H
