//
// Created by Apple on 11/01/2020.
//

#ifndef PARSER_PARSEREXCEPTION_H
#define PARSER_PARSEREXCEPTION_H

#include <string>
#include <iostream>
#include "Token.h"
#include "SyntaxException.h"

enum PARSE_ERROR {
    IF_ERROR,
    ELSE_ERROR,
    WHILE_ERROR,
    FOR_ERROR,
    VAR_DEF_ERROR,
    EXPRESSION_ERROR,
    STATEMENT_ERROR,
    CLASS_ERROR,
    MAIN_ERROR,
    VARIABLE_ERROR,
    TYPE_ERROR
};

string return_error(enum PARSE_ERROR err) {
    switch (err) {
        case ELSE_ERROR:
            return "Else error";
        case IF_ERROR:
            return "If error";
        case EXPRESSION_ERROR:
            return "Expression error";
        case WHILE_ERROR:
            return "While error";
        case FOR_ERROR:
            return "For error";
        case VARIABLE_ERROR:
            return "<VAR_DEF> -> variable = <VAR'>";
        case VAR_DEF_ERROR:
            return "<EXPRESSION_VAR> -> variable <EXPRESSION_VAR_VARIABLE> | number <EXPRESSION_VAR_NUM> | string <EXPRESSION_VAR_STRING>";
        case CLASS_ERROR:
            return "Class error";
        case MAIN_ERROR:
            return "Main error";
        case STATEMENT_ERROR:
            return "<STATEMENT> -> true | false | <EXPRESSION> <BOOL_OPERATOR> <EXPRESSION>";
        case TYPE_ERROR:
            return "<TYPE> -> variable | number | string";
        default:
            return "NON_ILLUSTRATED_EXCEPTION";
    }
}

using namespace std;
class ParserException : public exception {
public:

    int ind;
    PARSE_ERROR error;

    ParserException(int ind, PARSE_ERROR tag) {
        this->ind = ind;
        this->error = tag;
    }


    const char *what() const throw() {
        cout << return_error(error) << endl;
        throw SyntaxException(ind);
    }
};
#endif //PARSER_PARSEREXCEPTION_H
