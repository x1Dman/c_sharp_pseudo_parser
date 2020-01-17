//
// Created by Apple on 11/01/2020.
//

#ifndef PARSER_SYNTAXEXCEPTION_H
#define PARSER_SYNTAXEXCEPTION_H
#include <exception>
#include <string>
#include <iostream>


using namespace std;
class SyntaxException : public exception {
public:

    int ind;
    Tag tag;

    SyntaxException(int ind) {
        this->ind = ind;
    }


    const char *what() const throw() {
        string s = "Syntax error at index: " + to_string(ind) + "\n";
        cout << s << endl;
        return (char *) s.  c_str();
    }
};
#endif //PARSER_SYNTAXEXCEPTION_H
