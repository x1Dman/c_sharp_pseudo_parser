#include <iostream>
#include "Lexer.h"
#include "CSParser.h"

int main() {
    Lexer lex("/Users/Apple/Desktop/Study/5_sem/TheoryOfFormalLanguage/csTest.cs");
    //lex.result();
    CSParser parser(lex.get_tokens(), lex.get_values(), lex.get_code());
    parser.parse();
    parser.get_parse_tree_file("/Users/Apple/Desktop/Study/5_sem/TheoryOfFormalLanguage/parseTree.txt");
    return 0;
}
