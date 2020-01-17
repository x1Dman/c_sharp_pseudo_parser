//
// Created by x1Dman on 11.01.2020.
//

#include "Token.h"
#include <string>

using namespace std;

Token::Token(Tag tag, int start, int end)
        : tag(tag), start(start), end(end)
{}

string Token::tag_description(Tag tag) {
    switch (tag) {
        case DEFAULT:
            return "default";
        case CLASS:
            return "class";
        case VOID:
            return "void";
        case PUBLIC:
            return "public";
        case CLASS_VAR:
            return "class_var";
        case STATIC:
            return "static";
        case MAIN:
            return "main";
        case TRUE:
            return "true";
        case FALSE:
            return "false";
        case IDENT:
            return "identifier";
        case ARG:
            return "argument";
        case VAR_OP:
            return "var_op";
        case VAR:
            return "variable";
        case OPERATOR:
            return "operator";
        case STRING:
            return "string";
        case KEYWORD:
            return "keyword";
        case NUMBER:
            return "number";
        case COMMENT:
            return "comment";
        case IF:
            return "if";
        case ELSE:
            return "else";
        case WHILE:
            return "while";
        case FOR:
            return "for";
        case DO:
            return "do";
        case DONE:
            return "done";
        case RETURN:
            return "return";
        case THEN:
            return "then";
        case END_OF_LINE:
            return "end of line";
        case DOUBLE_PLUS:
            return "++";
        case PLUS:
            return "+";
        case MULT:
            return "*";
        case DIV:
            return "/";
        case LESS:
            return "<";
        case MORE:
            return ">";
        case DOUBLE_MINUS:
            return "--";
        case MINUS:
            return "-";
        case DOUBLE_EQUAL:
            return "==";
        case EQUAL:
            return "=";
        case BINARY_END:
            return "&";
        case BINARY_OR:
            return "|";
        case AND:
            return "&&";
        case OR:
            return "||";
        case COMMA:
            return ",";
        case COLON:
            return ":";
        case SEMICOLON:
            return ";";
        case LEFT_PAREN:
            return "(";
        case RIGHT_PAREN:
            return ")";
        case LEFT_BRACE:
            return "{";
        case RIGHT_BRACE:
            return "}";
        default:
            return "ERROR";
            break;
    }
}

Token::Token(Tag tag) {
    this->tag = tag;
    end = 0;
    start = 0;
}