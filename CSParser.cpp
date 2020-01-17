//
// Created by x1Dman on 10.01.2020.
//

#include <iostream>
#include <utility>
#include <fstream>
#include "CSParser.h"
#include "SyntaxException.h"
#include "ParserException.h"


/*
 * <CLASS> -> class var {<MAIN>}
 * <MAIN> -> static void main() {<BLOCK>}
 * <BLOCK> -> <VAR_DEF> <BLOCK> | <CYCLE> <BLOCK> | <IF_STATEMENT> <BLOCK> | <VAR_EQ> <BLOCK> | e
 * <VAR_EQ> -> variable = <TYPE> <VAR_EQ_C>
 * <VAR_EQ_C> -> ; | <OPERATOR> <TYPE> <VAR_EQ_C>
 * <VAR_DEF> -> var variable = <EXPRESSION_VAR>
 * <CYCLE> -> while ( <STATEMENT> ) { <BLOCK> } ; | for ( <VAR_EQ> ; <STATEMENT> ; <VAR_EQ> ) { <BLOCK> } ;
 * <IF_STATEMENT> -> if ( <STATEMENT> ) { <BLOCK> } <IF_STATEMENT'>
 * <IF_STATEMENT'> -> else { <BLOCK> } | e
 * <TYPE> -> variable | number | string
 *
 * // FOR DEFINITION
 * <EXPRESSION_VAR> -> variable <EXPRESSION_VAR_VARIABLE> | number <EXPRESSION_VAR_NUM> | string <EXPRESSION_VAR_STRING>
 * <EXPRESSION_VAR_VARIABLE> -> ; | <UNAR_OPERATOR> | <OPERATOR> <TYPE> <EXPRESSION_VAR_VARIABLE>
 * <EXPRESSION_VAR_NUM> -> ; | <UNAR_OPERATOR> | <OPERATOR> num <EXPRESSION_VAR_VARIABLE>
 * <EXPRESSION_VAR_STRING> -> ; | PLUS string <EXPRESSION_VAR_STRING>;
 * // FOR BLOCKS AND STATEMENTS
 * <EXPRESSION> -> variable <EXPRESSION_VAR> | number <EXPRESSION_NUM> | string <EXPRESSION_STRING>
 * <EXPRESSION_VAR> -> <OPERATOR> <TYPE> <EXPRESSION_VAR> | e
 * <EXPRESSION_NUM> -> number <OPERATOR> <EXPRESSION_NUM> | e
 * <EXPRESSION_STRING> -> string | string PLUS <EXPRESSION_STRING>
 *
 *
 * // INSIDE_GRAMMAR
 * <STATEMENT> -> true | false | <EXPRESSION> <BOOL_OPERATOR> <EXPRESSION>
 * <BOOL_OPERATOR> -> > | < | >= | <= | && | ||
 * <UNAR_OPERATOR> -> ++ | --
 * <OPERATOR> -> + | = | * | / | & | ++ | --
 * */



Tag CSParser::get_tag() {
    cur_token = tokens[idx].tag;
    return tokens[idx].tag;
}


CSParser::CSParser(vector<Token> tokens, set<string> vars, string str) {
    blocks_count = 0;
    isParsed = true;
    idx = 0;
    this->tokens = std::move(tokens);
    this->vars = std::move(vars);
    this->code = std::move(str);
    cur_token = this->tokens[idx].tag;
}

bool CSParser::look_ahead(Tag tag) {
    return idx + 1 < tokens.size() && tokens[idx + 1].tag == tag;
}

void CSParser::next() {
    if (idx < tokens.size()) {
        idx++;
        cur_token = tokens[idx].tag;
    } else {
        throw SyntaxException(-1);
    }
}

void CSParser::expect(Token token) {
    if (get_tag() == token.tag) {
        next();
    } else {
        isParsed = false;
    }
}


void CSParser::block() {
    if (idx < tokens.size() && isParsed) {
        blocks_count++;
        get_into();
        try {
            switch (get_tag()) {
                case VAR:
                    parse_tree += "<VAR_EQ> <BLOCK>\n";;
                    var_eq(false);
                    get_into();
                    parse_tree += "{\n";
                    block();
                    get_into();
                    parse_tree += "}\n";
                    break;
                case VAR_OP:
                    parse_tree += "<BLOCK> -> <VAR_DEF> <BLOCK>\n";;
                    variable_statement();
                    get_into();
                    parse_tree += "{\n";
                    block();
                    get_into();
                    parse_tree += "}\n";
                    break;
                case WHILE:
                case FOR:
                    parse_tree += "<BLOCK> -> <CYCLE> <BLOCK>\n";;
                    cycle_statement();
                    get_into();
                    parse_tree += "{\n";
                    block();
                    get_into();
                    parse_tree += "}\n";
                    break;
                case IF:
                    parse_tree += "<BLOCK> -> <IF_STATEMENT> <BLOCK>\n";;
                    next();
                    if_statement();
                    get_into();
                    parse_tree += "{\n";
                    block();
                    get_into();
                    parse_tree += "}\n";
                    break;
                case DEFAULT:
                    next();
                    parse_tree += "{\n";
                    block();
                    get_into();
                    parse_tree += "}\n";
                    break;
                default:
                    parse_tree += "<BLOCK> -> e\n";;
                    break;
            }
            blocks_count--;
        } catch (SyntaxException e) {
            e.what();
        }
    }
}

void CSParser::var_eq(bool inCycle) {
    get_into();
    parse_tree += "<VAR_EQ> -> variable = <TYPE> <VAR_EQ_C>\n";;
    expect(VAR);
    expect(EQUAL);
    type();
    var_eq_c(inCycle);
    if (!inCycle) {
        expect(SEMICOLON);
    }
}

void CSParser::var_eq_c(bool inCycle) {
    get_into();
    parse_tree += "<VAR_EQ_C> -> ; | <OPERATOR> <TYPE> <VAR_EQ_C>\n";;
    switch (get_tag()) {
        default:
            if (is_operator(get_tag())) {
                next();
                type();
                var_eq_c(inCycle);
            }
    }
}

void CSParser::variable_statement() {
    get_into();
    parse_tree += "<VAR_DEF> -> var variable = <EXPRESSION_VAR>\n";;
    next();
    expect(Token(VAR));
    expect(Token(EQUAL));
    expression_var();
}

void CSParser::expression_var() {
    get_into();
    switch (get_tag()) {
        case VAR:
            parse_tree += "<EXPRESSION_VAR> -> variable <EXPRESSION_VAR_VARIABLE>\n";
            next();
            expression_var_variable();
            break;
        case NUMBER:
            parse_tree += "<EXPRESSION_VAR> -> number <EXPRESSION_VAR_NUM>\n";
            next();
            expression_var_num();
            break;
        case STRING:
            parse_tree += "<EXPRESSION_VAR> -> string <EXPRESSION_VAR_STRING>\n";
            next();
            expression_var_string();
            break;
        default:
            isParsed = false;
            cout << "EXPRESSION_VAR_ERROR\n";
            exit(0);
    }
}

bool CSParser::is_operator(Tag tag) {
    get_into();
    parse_tree += "<OPERATOR> -> + | - | \\ | ++ | --\n";
    return tag == PLUS || tag == MULT || tag == DIV || tag == DOUBLE_PLUS || tag == DOUBLE_MINUS;
}

void CSParser::expression_var_variable() {
    get_into();
    Tag t = get_tag();
    switch (t) {
        case SEMICOLON:
            parse_tree += "<EXPRESSION_VAR_VARIABLE> -> ;\n";
            next();
            break;
        case DOUBLE_PLUS:
        case DOUBLE_MINUS:
            parse_tree += "<EXPRESSION_VAR_VARIABLE> -> <UNAR_OPERATOR>\n";
            next();
            expect(SEMICOLON);
            break;
        case PLUS:
        case MINUS:
        case AND:
        case OR:
        case MULT:
        case DIV:
            parse_tree += "<EXPRESSION_VAR_VARIABLE> -> <OPERATOR> <TYPE> <EXPRESSION_VAR_VARIABLE>\n";
            next();
            type();
            expression_var_string();
            break;
        default:
            cout << "EXPRESSION_VAR_VARIABLE ERROR";
            exit(0);
    }
}

void CSParser::expression_var_num() {
    get_into();
    Tag t = get_tag();
    switch (t) {
        case SEMICOLON:
            parse_tree += "<EXPRESSION_VAR_NUM> -> ;\n";
            next();
            break;
        case DOUBLE_PLUS:
        case DOUBLE_MINUS:
            parse_tree += "<EXPRESSION_VAR_NUM> -> <UNAR_OPERATOR>\n";
            next();
            expect(SEMICOLON);
            break;
        case PLUS:
        case MINUS:
        case AND:
        case OR:
        case MULT:
        case DIV:
            parse_tree += "<EXPRESSION_VAR_NUM> -> number <EXPRESSION_VAR_number>\n";
            next();
            expect(NUMBER);
            expression_var_num();
            break;
        default:
            cout << "EXPRESSION_VAR_NUM ERROR";
            exit(0);
    }
}

void CSParser::expression_var_string() {
    get_into();
    switch (get_tag()) {
        case SEMICOLON:
            parse_tree += "<EXPRESSION_VAR_STRING> -> ;\n";
            next();
            break;
        case PLUS:
            parse_tree += "<EXPRESSION_VAR_STRING> -> PLUS string <EXPRESSION_VAR_STRING>\n";
            next();
            expect(STRING);
            expression_var_string();
            break;
        default:
            cout << "EXPRESSION_VAR_STRING ERROR";
            exit(0);
    }
}


void CSParser::cycle_statement() {
    get_into();
    switch (get_tag()) {
        case WHILE:
            parse_tree += "<CYCLE> -> while ( <STATEMENT> ) { <BLOCK> } ;\n";
            next();
            expect_statement();
            expect_block();
            break;
        case FOR:
            parse_tree += "<CYCLE> -> for ( <VAR_EQ> ; <STATEMENT> ; <VAR_EQ> ) { <BLOCK> } ;\n";
            next();
            expect(LEFT_PAREN);
            var_eq(true);
            expect(SEMICOLON);
            statement();
            expect(SEMICOLON);
            var_eq(true);
            expect(RIGHT_PAREN);
            expect_block();
            break;
        default:
            cout << "CYCLE ERROR";
            exit(0);
    }
}

void CSParser::expect_statement() {
    expect(LEFT_PAREN);
    statement();
    expect(RIGHT_PAREN);
}

void CSParser::expect_block() {
    expect(LEFT_BRACE);
    get_into();
    parse_tree += "{\n";
    block();
    get_into();
    parse_tree += "}\n";;
    expect(RIGHT_BRACE);
}


void CSParser::if_statement() {
    get_into();
    parse_tree += "<IF_STATEMENT> -> if ( <STATEMENT> ) { <BLOCK> } <IF_STATEMENT'>\n";
    expect_statement();
    expect_block();
    if_statement_sharp();
}

void CSParser::if_statement_sharp() {
    get_into();
    switch (get_tag()) {
        case ELSE:
            parse_tree += "<IF_STATEMENT'> -> else { <BLOCK> } <BLOCK> \n";
            next();
            expect_block();
            get_into();
            parse_tree += "{\n";
            block();
            get_into();
            parse_tree += "}\n";
            break;
        default:
            parse_tree += "<IF_STATEMENT'> -> e\n";
            break;
    }
}

void CSParser::type() {
    get_into();
    switch (get_tag()) {
        case NUMBER:
            parse_tree += "<TYPE> -> number\n";
            next();
            break;
        case VAR:
            parse_tree += "<TYPE> -> variable\n";
            next();
            break;
        case STRING:
            parse_tree += "<TYPE> -> string\n";
            next();
            break;
        default:
            cout << "TYPE_ERROR";
            exit(0);
    }
}


void CSParser::expression() {
    get_into();
    switch (get_tag()) {
        case VAR:
            parse_tree += "<EXPRESSION> -> variable <EXPRESSION_VAR>\n";
            next();
            expression_variable();
            break;
        case NUMBER:
            parse_tree += "<EXPRESSION> -> number <EXPRESSION_NUM>\n";
            next();
            expression_num();
            break;
        case STRING:
            parse_tree += "<EXPRESSION> -> string <EXPRESSION_STRING>\n";
            next();
            expression_string();
            break;
        default:
            cout << "EXPRESSION_ERROR";
            exit(0);
            break;
    }
}

void CSParser::expression_variable() {
    get_into();
    switch (get_tag()) {
        case PLUS:
        case MINUS:
            parse_tree += "<EXPRESSION_VARIABLE> -> <OPERATOR> <TYPE> <EXPRESSION_VARIABLE>\n";
            next();
            type();
            expression_variable();
            break;
        default:
            parse_tree += "<EXPRESSION_VARIABLE> -> e\n";
            break;
    }
}

void CSParser::expression_num() {
    get_into();
    switch (get_tag()) {
        case PLUS:
        case MINUS:
            parse_tree += "<EXPRESSION_NUM> -> <OPERATOR> number <EXPRESSION_NUM>\n";
            next();
            expect(NUMBER);
            expression_num();
            break;
        default:
            parse_tree += "<EXPRESSION_NUM> -> e\n";
            break;
    }
}

void CSParser::expression_string() {
    get_into();
    switch (get_tag()) {
        case PLUS:
            parse_tree += "<EXPRESSION_STRING> -> PLUS <EXPRESSION_STRING>\n";
            next();
            expect(STRING);
            expression_string();
            break;
        default:
            parse_tree += "<EXPRESSION_STRING> -> e\n";
            break;
    }
}


bool CSParser::is_bool_operator(Tag tag) {
    get_into();
    parse_tree += "<BOOL_OPERATOR> -> AND | OR | LESS | MORE | DOUBLE_EQUAL\n";
    return tag == AND || tag == OR || tag == LESS || tag == MORE || tag == DOUBLE_EQUAL;
}

void CSParser::statement() {
    get_into();
    switch (get_tag()) {
        case TRUE:
            parse_tree += "<STATEMENT> -> true\n";
            next();
            break;
        case FALSE:
            parse_tree += "<STATEMENT> -> false\n";
            next();
            break;
        default:
            parse_tree += "<STATEMENT> -> <EXPRESSION> <BOOL_OPERATOR> <EXPRESSION>\n";
            expression();
            if (is_bool_operator(get_tag())) {
                next();
                expression();
            } else {
                isParsed = false;
                cout << return_error(STATEMENT_ERROR) << endl;
                exit(0);
            }
            break;
    }
}

void CSParser::parse() {
    get_into();
    parse_tree += "<CLASS> -> class var {<MAIN>}\n";
    expect(CLASS);
    expect(VAR);
    expect(LEFT_BRACE);
    main();
    expect(RIGHT_BRACE);
    if(!isParsed){
        cout << return_error(CLASS_ERROR);
        exit(0);
    }
}

void CSParser::main() {
    get_into();
    parse_tree += "<MAIN> -> static void main() {<BLOCK>}\n";
    expect(STATIC);
    expect(VOID);
    expect(MAIN);
    expect(LEFT_PAREN);
    expect(RIGHT_PAREN);
    expect_block();
    if(!isParsed){
        cout << return_error(MAIN_ERROR);
        exit(0);
    }
}

void CSParser::get_into() {
    for (int i = 0; i < blocks_count; i++) {
        this->parse_tree.append("\t");
    }
}

void CSParser::get_parse_tree_file(string path) {
    if (isParsed) {
        ofstream out(path);
        out << parse_tree;
        out.close();
        cout << "FILE WAS CREATED";
    } else {
        cout << "PARSE ENDED WITH ERRORS!";
    }
}
