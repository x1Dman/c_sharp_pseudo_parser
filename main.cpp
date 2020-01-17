#include <iostream>
#include "Lexer.h"
#include "CSParser.h"

int main() {
    Lexer lex("PATH_TO_CS_FILE");
    //lex.result();
    CSParser parser(lex.get_tokens(), lex.get_values(), lex.get_code());
    parser.parse();
    parser.get_parse_tree_file("PATH_TO_PARSE_TREE_FILE");
    return 0;
}
