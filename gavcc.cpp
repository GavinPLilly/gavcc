#include "gavcc.h"
#include <iostream>

#include "scanner.cpp"
#include "parser.cpp"
#include "eval.cpp"

string ast_node_string(Node* ast);
string ast_node_string_verbose(Node* ast);
string node_type_string(NodeType type);

using std::cout;
using std::endl;

int main(int argc, char** argv)
{
    string s = "int a;";

    cout << s << endl;

    Scanner scanner(s);
    vector<Token> tokens = scanner.scan();
    cout << to_string::tokens(tokens) << endl;

    Parser parser(tokens);
    Node* ast = parser.parse();
    cout << to_string::node(ast) << endl;

    // Eval eval(ast);
    // int64_t result = eval.eval();
    // cout << result << endl;

    return 0;
}
