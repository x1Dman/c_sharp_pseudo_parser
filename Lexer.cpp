#include "Lexer.h"
#include <string>
#include <fstream>
#include <iostream>
#include "Token.h"
#include "SyntaxException.h"

using namespace std;

Lexer::Lexer(std::string path1) {
    cur_pos = 0;
    path = path1;
    read_file();
}

void Lexer::read_file() {
    std::ifstream file;
    file.open(path);
    std::string str;
    while (getline(file, str)) {
        s += str;
        s += '\n';
    }
    file.close();
    parse_file();
}


void Lexer::parse_file() {
    try {
        while (cur_pos < s.length()) {
            tokens.push_back(next());
        }
    } catch (SyntaxException &e) {
        std::cout << "Exception caught" << std::endl;
        std::cout << e.what() << std::endl;
    }
}

Token Lexer::next(){
    skip_whitespaces();
    Tag tag = DEFAULT;
    int start = cur_pos;
    int end = cur_pos;
    while (cur_pos < s.length() && !is_whitespace(s[cur_pos])) {
        char c = s[cur_pos];
        if (is_symbol(c)) {
            if (tag == DEFAULT) {
                tag = IDENT;
            } else if (tag != IDENT && tag != STRING && tag != COMMENT) {
                throw SyntaxException(cur_pos);
            }
        } else if (c == '"') {
            cur_pos++;
            if (expect_quote()) {
                tag = STRING;
            } else {
                throw SyntaxException(cur_pos);
            }
        } else if (c == '/' && cur_pos + 1 < s.length() && s[cur_pos + 1] == '/') {
            if (expect_endline()) {
                tag = COMMENT;
            } else {
                throw SyntaxException(cur_pos);
            }
        } else if (is_digit(c)) {
            if (tag == DEFAULT) {
                tag = NUMBER;
            } else if (tag != IDENT && tag != NUMBER) {
                throw SyntaxException(cur_pos);
            }
        } else if (is_operator(c)) {
            if (tag == DEFAULT) {
                tag = get_operator(c);
                cur_pos++;
            }
            break;
        } else if (is_partitioner(c)){
            if (tag == DEFAULT){
                tag = get_partitioner(c);
                cur_pos++;
            }
            break;
        }

        cur_pos++;
    }

    end = cur_pos - 1;

    if (tag == IDENT) {
        string token = s.substr(start, end - start + 1);
        if (is_keyword(token)) {
            tag = get_keyword(token);
        }else{
            tag = VAR;
            vars.insert(token);
        }
    }
//    cout << Token::tag_description(tag)  << " " << start << " " << end << endl;
    return Token(tag, start, end);
}

bool Lexer::is_whitespace(char c) {
    return cur_pos < s.length() && (s[cur_pos] == ' ' || s[cur_pos] == '\t' || s[cur_pos] == '\n');
}

void Lexer::skip_whitespaces() {
    while (is_whitespace(s[cur_pos])) {
        cur_pos++;
    }
}

bool Lexer::is_symbol(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool Lexer::is_digit(char c) {
    return c >= '0' && c <= '9';
}

bool Lexer::expect_arg(char c) {
    if (cur_pos + 2 >= s.length()) {
        return false;
    }
    return s[cur_pos + 1] == c && (is_symbol(s[cur_pos + 2]) || is_digit(s[cur_pos + 2]));
}

bool Lexer::expect_var() {
    if (cur_pos + 1 >= s.length()) {
        return false;
    }
    return is_symbol(s[cur_pos + 1]) || is_digit(s[cur_pos + 1]);
}

bool Lexer::expect_quote() {
    while (cur_pos < s.length()) {
        if (s[cur_pos] == '"') return true;
        cur_pos++;
    }
    return false;
}

bool Lexer::expect_endline() {
    while (cur_pos < s.length()) {
        if (s[cur_pos] == '\n' || s[cur_pos] == EOF) return true;
        cur_pos++;
    }
    return false;
}

bool Lexer::is_symbol_or_digit(char c) {
    return is_symbol(c) || is_digit(c);
}

bool Lexer::is_operator(char c) {
    return c == '+' || c == '*' || c == '/' || c == '-' || c == '=' || c == '<' || c == '>';
}

Tag Lexer::get_operator(char c) {
    if (c == '+') return PLUS;
    else if (c == '*') return MULT;
    else if (c == '/') return DIV;
    else if (c == '-') return MINUS;
    else if (c == '=' && cur_pos + 1 < s.length() && s[cur_pos + 1] == '=') {
        cur_pos++;
        return DOUBLE_EQUAL;
    }
    else if (c == '<') return LESS;
    else if (c == '>') return MORE;
    else if (c == '=') return EQUAL;
    return DEFAULT;
}

bool Lexer::is_partitioner(char c) {
    return c == ',' || c == ':' || c == ';' || c == '[' || c == ']' || c == '(' || c == ')' || c == '{' || c == '}';
}

Tag Lexer::get_partitioner(char c) {
    if (c == ',') return COMMA;
    else if (c == ':') return COLON;
    else if (c == ';') return SEMICOLON;
    else if (c == '(') return LEFT_PAREN;
    else if (c == ')') return RIGHT_PAREN;
    else if (c == '{') return LEFT_BRACE;
    else if (c == '}') return RIGHT_BRACE;
    return DEFAULT;
}

bool Lexer::is_keyword(string s) {
    return s == "class" || s == "public" || s == "static" || s == "main" || s == "void" || s == "var" || s == "if" || s == "else" || s == "for" || s == "then" || s == "while" || s == "do" || s == "done" ||
           s == "return";
}

Tag Lexer::get_keyword(string str) {
    if (str == "var") return VAR_OP;
    else if (str == "class") return CLASS;
    else if (str == "main") return MAIN;
    else if (str == "static") return STATIC;
    else if (str == "public") return PUBLIC;
    else if (str == "void") return VOID;
    else if (str == "if") return IF;
    else if (str == "else") return ELSE;
    else if (str == "while") return WHILE;
    else if (str == "for") return FOR;
    else if (str == "do") return DO;
    else if (str == "done") return DONE;
    else if (str == "return") return RETURN;
    else if (str == "then") return THEN;
    else if (str == "true") return TRUE;
    else if (str == "false") return FALSE;
    return DEFAULT;
}

void Lexer::result(){
    for (auto &e : tokens) {
        if (e.tag == DEFAULT) continue;
        std::cout << "start: " << e.start << ", end: " << e.end << ", tag: " << Token::tag_description(e.tag) << std::endl;
    }

    std::cout << "number of variables: " << vars.size() << std::endl;
    for (auto &e : vars) {
        std::cout << e << std::endl;
    }

}

vector<Token> Lexer::get_tokens() {
    return tokens;
}

set<string> Lexer::get_values() {
    return vars;
}

string Lexer::get_code() {
    return s;
}