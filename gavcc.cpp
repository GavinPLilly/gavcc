#include "gavcc.h"
#include <filesystem>
#include <fstream>
#include <iostream>

#include "scanner.cpp"
#include "parser.cpp"
#include "semanal.cpp"
#include "eval.cpp"

string ast_node_string(Node* ast);
string ast_node_string_verbose(Node* ast);
string node_type_string(NodeType type);

using std::cout;
using std::endl;

std::string read_file(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    std::string content;
    content.resize(std::filesystem::file_size(filename));
    file.read(content.data(), content.size());
    return content;
}

int main() {
    string s = read_file("test.c");

    cout << s << endl;

    Scanner scanner(s);
    vector<Token> tokens = scanner.scan();
    cout << to_string::tokens(tokens) << endl;

    Parser parser(tokens);
    Node* ast = parser.parse();
    cout << to_string::node(ast) << endl;

    SemAnal sem_anal(ast);
    sem_anal.sem_anal();

    Eval eval(ast);
    eval.eval();

    return 0;
}
