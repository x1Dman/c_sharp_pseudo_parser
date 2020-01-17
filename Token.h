//
// Created by Apple on 11/01/2020.
//

#ifndef PARSER_TOKEN_H
#define PARSER_TOKEN_H


#include <string>

enum Tag {
    DEFAULT,
    CLASS,
    CLASS_VAR,
    PUBLIC,
    STATIC,
    VOID,
    MAIN,
    TRUE,
    FALSE,
    IDENT,
    ARG,
    OPERATOR,
    VAR_OP,
    VAR,
    KEYWORD,
    NUMBER,
    COMMENT,
    PARTITIONER,
    STRING,
    IF,
    ELSE,
    WHILE,
    FOR,
    DO,
    LESS,
    MORE,
    DONE,
    RETURN,
    THEN,
    DOUBLE_PLUS,
    PLUS,
    MULT,
    DIV,
    DOUBLE_MINUS,
    MINUS,
    DOUBLE_EQUAL,
    EQUAL,
    BINARY_END,
    BINARY_OR,
    AND,
    OR,
    COMMA,
    COLON,
    SEMICOLON,
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    END_OF_LINE
};

class Token{
public:
    Tag tag;
    int start, end;
    Token(Tag tag, int start, int end);
    Token(Tag tag);
    static std::string tag_description(Tag tag);
};

#endif //PARSER_TOKEN_H
