#include <iostream>

#include "scanner.cpp"

int main(int argc, char** argv)
{
    std::string s = "ga_v ga+ 3";

    Scanner scanner(s);
    std::vector<Token> tokens = scanner.scan();

    std::cout << s << std::endl;
    print_tokens(tokens);

    return 0;
}
