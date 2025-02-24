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
        /*
        s0 = starting state
        s1 = integer state
        s2 = transitioning to real
        s3 = accepting real
        s4 = ERROR state
        */
    };
    State state = State::s0;

//Checking that first char is a valid integer
    if (isdigit(buffer[pos])){
        state = State::s1;
        advance();
    } else {
        return {buffer.substr(start_pos, pos - start_pos), TokenType::UNKW};
    }


//FSM, starts at state 1 and transitions to other states or ACCEPTING/ERROR

    while (pos < buffer.size()){
        switch (state){
            case State::s1: //Char is a valid integer
                if (isdigit(buffer[pos])){
                    advance();
                } else if (buffer[pos] == '.' && (pos+1 < buffer.size()) && isdigit(buffer[pos + 1])) { //has a decimal
                    state = State::s2; 
                } else if (!isspace(buffer[pos]) && !characterTables.isSeparatorChar(buffer[pos])){ //Not an int or separator -> error
                    state = State::s4;
                    goto ERROR;
                } else { //Successfully deemed an int
                    goto ACCEPT;
                }
                break;

            
            case State::s2: //Becomes a REAL, either next char is an int or becomes error

                if(isdigit(buffer[pos])){
                    state = State::s3;
                    advance();
                } else {
                    goto ERROR;
                }
                break;

            case State::s3: //Evaluating valid Real
                if (isdigit(buffer[pos])){
                    advance();
                } else if (!isspace(buffer[pos]) && !characterTables.isSeparatorChar(buffer[pos])){ //Not an int or separator, becomes error
                    goto ERROR;
                } else { //Evaluated to be acceptable real
                    goto ACCEPT;
                }
                break;
            
            case State::s4: //Automatically becomes error

                goto ERROR;
        }
    }

    ACCEPT: //If state ended on state 1 or 3, accepting state
        if (state == State::s3){
            return {buffer.substr(start_pos, pos - start_pos), TokenType::REAL};
        } else {
            return {buffer.substr(start_pos, pos - start_pos), TokenType::INT};
        }

    ERROR: //If state ended on state 4
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
