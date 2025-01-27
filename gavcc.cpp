#include <iostream>

#include "scanner.cpp"
#include "parser.cpp"
#include "eval.cpp"

int main(int argc, char** argv)
{
    std::string s = "1 + 3 * 5 * 2 + 2";
    std::cout << s << std::endl;

    Scanner scanner(s);
    std::vector<Token> tokens = scanner.scan();
    print_tokens(tokens);

    Parser parser(tokens);
    Node* ast = parser.parse();
    print_ast(ast);

    Eval eval(ast);
    int64_t result = eval.eval();
    std::cout << result << std::endl;

    return 0;
}
