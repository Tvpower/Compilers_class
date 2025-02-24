//
// Created by Tvpow on 2/8/2025.
//

#include "Lexer.h"
#include <iostream>
#include <string>
#include <cctype>

Token Lexer::processNumber(){
    size_t start_pos = pos;
    enum class State {
        s0, s1, s2, s3, s4
    };
    State state = State::s0;

    if (isdigit(buffer[pos])){
        state = State::s1;
        advance();
    } else {
        return {buffer.substr(start_pos, pos - start_pos), TokenType::UNKW};
    }

    while (pos < buffer.size()){
        switch (state){
            case State::s1:
                if (isdigit(buffer[pos])){
                    advance();
                } else if (buffer[pos] == '.' && (pos+1 < buffer.size()) && isdigit(buffer[pos + 1])) {
                    state = State::s2;
                } else if (!isspace(buffer[pos]) && !characterTables.isSeparatorChar(buffer[pos])){
                    state = State::s4;
                    goto ERROR;
                } else {
                    goto ACCEPT;
                }
                break;

            case State::s2:
                if(isdigit(buffer[pos])){
                    advance();
                } else {
                    goto ERROR;
                }
                break;

            case State::s3:
                if (isdigit(buffer[pos])){
                    advance();
                } else if (!isspace(buffer[pos]) && !characterTables.isSeparatorChar(buffer[pos])){
                    goto ERROR;
                } else {
                    goto ACCEPT;
                }
                break;

            case State::s4:
                goto ERROR;
        }
    }

    ACCEPT:
    {
        return {buffer.substr(start_pos, pos - start_pos), TokenType::INT};
    }

    ERROR:
        return {buffer.substr(start_pos, pos - start_pos), TokenType::UNKW};
}

Lexer::StateTransition Lexer::handleStartState() {
    return Lexer::StateTransition();
}

Lexer::StateTransition Lexer::handleIdentifierState() {
    return Lexer::StateTransition();
}

Lexer::StateTransition Lexer::handleIntegerState() {
    return Lexer::StateTransition();
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
