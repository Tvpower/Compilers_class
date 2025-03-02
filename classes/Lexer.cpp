//
// Created by Tvpow on 2/8/2025.
//

#include "Lexer.h"
#include <iostream>
#include <string>
#include <cctype>


Token Lexer::getNextToken() {
    currentState = State::START;

    while (currentState != State::END && currentState != State::ERROR) {
        StateTransition transition;

        switch (currentState) {
            case State::START:
                transition = handleStartState();
                break;
            case State::IN_IDENTIFIER:
                transition = handleIdentifierState();
                break;
            case State::IN_INTEGER:
                transition = handleIntegerState();
                break;
            case State::IN_REAL:
                transition = handleRealState();
                break;
            case State::IN_OPERATOR:
                transition = handleOperatorState();
                break;
            case State::IN_COMMENT:
                transition = handleCommentState();
                break;
            default:
                return {getCurrentLexeme(), TokenType::UNKW};
        }
        currentState = transition.first;

        //If theres an action to perform during transition, do it
        if (transition.second) {
            transition.second();
        }
    }
    if (currentState == State::ERROR) {
        return {getCurrentLexeme(), TokenType::UNKW};
    }

    return lastToken;
}


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
    if (std::isalpha(current()) || current() == '_') {
        advance();
        return {State::IN_IDENTIFIER, nullptr};
    }
    //Check for comment start
    if (current() == '[' && peek() == '*') {
        advance();
        advance();
        return {State::IN_COMMENT, nullptr};
    }
    //Numbers
    if (isdigit(current())){
        advance();
        return {State::IN_INTEGER, nullptr};
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
    while(isalpha(current()) || isdigit(current()) || current() == '_'){
        advance();
    }
    if (Lexer::keywords.find(getCurrentLexeme()) != Lexer::keywords.end()){
        return {State::END, [this]() {
            lastToken = {getCurrentLexeme(), TokenType::KEYW};
        }};
    }
    else {
        return {State::END, [this]() {
            lastToken = {getCurrentLexeme(), TokenType::IDENT};
        }};
    }
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
    while (std::isdigit(current())){
        advance();
    }
    return {State::END, [this]() {
        lastToken = {getCurrentLexeme(), TokenType::REAL}; //Need to ensure this returns full number before and after '.' and not just the digits after
    }};
}

Lexer::StateTransition Lexer::handleOperatorState() {
    if (isOperator(current())){
        if (peek() == '=' || peek() == '/'){ // For operators such as ==, <=, //
            advance(); // extra advance
        }
    }

    advance();
    return {State::END, [this](){
        lastToken = {getCurrentLexeme(), TokenType::OPER};
    }};
}

Lexer::StateTransition Lexer::handleCommentState() {
    while (!(current() == '*' && peek() == ']')) {
        if (current() == '\0') { // Check for EOF in comment
            return {State::ERROR, nullptr};
        }
        advance();
    }

    // Consume the closing '*]'
    advance(); // for '*'
    advance(); // for ']'

    return {State::END, [this]() {
        lastToken = {getCurrentLexeme(), TokenType::COMM};
    }};
}
const std::unordered_map<sv, bool> Lexer::keywords = {
    {"function", true},
    {"integer", true},
    {"boolean", true},
    {"real", true},
    {"if", true},
    {"else", true},
    {"endif", true},
    {"while", true},
    {"endwhile", true},
    {"return", true},
    {"scan", true},
    {"print", true},
    {"true", true},
    {"false", true}
};

