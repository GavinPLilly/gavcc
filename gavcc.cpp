#include <iostream>
#include <string>

#include "scanner.cpp"
#include "parser.cpp"
#include "eval.cpp"

std::string ast_node_string(Node* ast);
std::string node_type_string(NodeType type);

int main(int argc, char** argv)
{
    std::string s = "(1 + 5) * 5";
    std::cout << s << std::endl;

    Scanner scanner(s);
    std::vector<Token> tokens = scanner.scan();
    print_tokens(tokens);

    Parser parser(tokens);
    Node* ast = parser.parse();
    std::cout << ast_node_string(ast) << std::endl;

    Eval eval(ast);
    int64_t result = eval.eval();
    std::cout << result << std::endl;

    return 0;
}

std::string ast_node_string(Node* ast) {
    if(ast == nullptr) {
        return "nullptr";
    }
    std::string s;
    s += "(Type: " + node_type_string(ast->type) + " Left: " + ast_node_string(ast->left) + " Right: " + ast_node_string(ast->right) + ")";
    return s;
}

std::string node_type_string(NodeType type) {
    if(type == NodeType::lit) {
        return "lit";
    }
    if(type == NodeType::op) {
        return "op";
    }
    return "[X]";
}
