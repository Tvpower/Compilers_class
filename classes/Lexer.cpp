//
// Created by Tvpow on 2/8/2025.
//

#include "Lexer.h"
#include <iostream>
#include <string>
#include <cctype>


Lexer::StateTransition Lexer::handleStartState() {
    while (std::isspace(current())) {
        advance();
    }
    start = pos;
    //check for end of file
    if (current() == '\0') {
        return {State::END, [this]() {
            lastToken = {sv{}, TokenType::END};
        }};
    }
    //check for identifier start
    if (std::isalpha(current() || current() == '_')) {
        advance();
        return {State::IN_IDENTIFIER, nullptr};
    }
    //Check for comment start
    if (current() == '[' && peek() == '*') {
        advance();
        advance();
        return {State::IN_COMMENT, nullptr};
    }
    //Check for operator
    if (isOperator(current())) {
        advance();
        return {State::IN_OPERATOR, nullptr};
    }
    //Check for separator
    if (isSeparator(current())) {
        advance();
        return {State::END, [this]() {
            lastToken = {getCurrentLexeme(), TokenType::SEPA};
        }};
    }

    //If none of the last option then it's unknown character
    advance();
    return {State::ERROR, nullptr};
}



Lexer::StateTransition Lexer::handleIdentifierState() {
    return Lexer::StateTransition();
}

Lexer::StateTransition Lexer::handleIntegerState() {
    while (std::isdigit(current())) {
        advance();
    }
    if (current() == '.' && std::isdigit(peek())) {
        advance(); //if dot consume it
        return {State::IN_REAL, nullptr};
    }
    return {State::END, [this]() {
        lastToken = {getCurrentLexeme(), TokenType::INT};
    }};
}

Lexer::StateTransition Lexer::handleRealState() {
    return Lexer::StateTransition();
}

Lexer::StateTransition Lexer::handleOperatorState() {
    return Lexer::StateTransition();
}

Lexer::StateTransition Lexer::handleCommentState() {
    return Lexer::StateTransition();
}
