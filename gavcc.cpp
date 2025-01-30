#include <iostream>
#include <string>

#include "scanner.cpp"
#include "parser.cpp"
#include "eval.cpp"

std::string ast_node_string(Node* ast);
std::string node_type_string(NodeType type);

int main(int argc, char** argv)
{
    std::string s = "2 - 4 + 2";
    std::cout << s << std::endl;

    Scanner scanner(s);
    std::vector<Token> tokens = scanner.scan();
    std::cout << tokens_to_string(tokens) << std::endl;

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
    if(ast->type == NodeType::lit) {
        return std::to_string(ast->token.ival);
    }
    if(ast->type == NodeType::op) {
        return "(" + token_to_string(ast->token) + " Left: " + ast_node_string(ast->left) + " Right: " + ast_node_string(ast->right) + ")";
    }
    return "[xxx]";
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
