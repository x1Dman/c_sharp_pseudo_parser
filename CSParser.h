//
// Created by Apple on 11/01/2020.
//

#ifndef PARSER_CSPARSER_H
#define PARSER_CSPARSER_H
#include <string>
#include <vector>
#include <set>
#include "Token.h"
using namespace std;


class CSParser {
private:
    bool isParsed;
    int idx;
    int blocks_count;
    string parse_tree;
    Tag cur_token;
    vector<Token> tokens;
    set<string> vars;
    string code;

    // MAIN
    void main();
    // ACTIVE
    Tag get_tag();
    void next();
    void expect(Token token);
    void get_into();

    // BOOL
    bool is_operator(Tag tag);
    bool look_ahead(Tag tag);
    bool is_bool_operator(Tag tag);

    // VAR
    void var_eq(bool inCycle);
    void var_eq_c(bool inCycle);
    void variable_statement();
    void expression_var();
    void expression_var_variable();
    void expression_var_string();
    void expression_var_num();

    // COMMON
    void block();
    void statement();
    void type();
    void expression();
    void expression_variable();
    void expression_num();
    void expression_string();
    void expect_block();
    void expect_statement();

    // IF
    void if_statement();
    void if_statement_sharp();

    //CYCLE
    void cycle_statement();

public:
    CSParser(vector<Token> tokens, set<string> vars, string str);
    void parse();
    void get_parse_tree_file(string path);
};


#endif //PARSER_CSPARSER_H
