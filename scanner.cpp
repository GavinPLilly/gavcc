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
            char c = get_char();
            if(c >= '0' && c <= '9') {
                parse_number();
            }
            else if(c == '+') {
                tokens.push_back({ TokenType::plus});
            }
            else if(c == '=') {
                tokens.push_back({ TokenType::equals });
            }
            else if(is_whitespace(c)) {
                ;
            }
            next();
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

private:
    char get_char() {
        return chars[cur];
    }

    void next() {
        ++cur;
    }

    void parse_number() {
        char c = get_char();
        tokens.push_back({ TokenType::integer, c - '0' });
    }

    bool is_whitespace(char c) {
        switch(c) {
            case ' ':
            case '\n':
            case '\t':
                return true;
            default:
                return false;
        }
    }

};
