//
// Created by Apple on 11/01/2020.
//

#ifndef PARSER_LEXER_H
#define PARSER_LEXER_H


using namespace std;
#include <string>
#include <set>
#include <vector>
#include "Token.h"
class Lexer{
private:
    int cur_pos;
    std::vector<Token> tokens;
    std::string path;
    std::string s;
    std::set<std::string> vars;
    void read_file();
    Token next();
    bool is_whitespace(char c);
    void skip_whitespaces();
    bool is_symbol(char c);
    bool is_digit(char c);
    bool expect_arg(char c);
    bool expect_var();
    bool expect_quote();
    bool expect_endline();
    bool is_symbol_or_digit(char c);
    bool is_operator(char c);
    Tag get_operator(char c);
    bool is_partitioner(char c);
    Tag get_partitioner(char c);
    bool is_keyword(std::string s);
    Tag get_keyword(std::string str);
    void parse_file();
public:
    Lexer(std::string path);
    vector<Token> get_tokens();
    set<string> get_values();
    string get_code();
    void result();
};


#endif //PARSER_LEXER_H
