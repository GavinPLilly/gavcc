#include <iostream>
#include <vector>
#include <string>

enum class TokenType {
    integer,
    plus,
    equals,
};

struct Token {
    TokenType type;
    int64_t ival;
};

class Scanner {
    const std::string chars;
    std::vector<Token> tokens;
    int cur = 0;

public:
    Scanner(std::string chars):
        chars(chars)
    {
    }

    std::vector<Token> scan()
    {
        while(cur < chars.size()) {
            char c = chars[cur];
            if(c >= '0' && c <= '9') {
                tokens.push_back({ TokenType::integer, c - '0' });
            }
            else if(c == '+') {
                tokens.push_back({ TokenType::plus});
            }
            else if(c == '=') {
                tokens.push_back({ TokenType::equals });
            }
            ++cur;
        }
        return tokens;
    }

    void print_tokens(std::vector<Token> tokens)
    {
        std::string s;
        for(auto token : tokens) {
            switch(token.type) {
                case TokenType::integer:
                    s += std::to_string(token.ival);
                    break;
                case TokenType::plus:
                    s += '+';
                    break;
                case TokenType::equals:
                    s += '=';
                    break;
            }
        }
        std::cout << s << std::endl;
    }

};
