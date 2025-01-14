#include <iostream>

#include "scanner.cpp"

int main(int argc, char** argv)
{
    std::string s = "1 + 3 =";
    Scanner scanner(s);
    std::vector<Token> tokens = scanner.scan();
    scanner.print_tokens(tokens);

    int total = 0;
    Token prev;
    for(auto token : tokens) {
        switch(token.type) {
            case TokenType::integer:
                prev = token;
                break;
            case TokenType::plus:
                total += prev.ival;
                break;
            case TokenType::equals:
                total += prev.ival;
                std::cout << total << std::endl;
                break;
        }
    }

    return 0;
}
