#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <charconv>

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
    int idx = 0;

public:
    Scanner(std::string chars):
        chars(chars)
    {
    }

    std::vector<Token> scan()
    {
        while(idx < chars.size()) {
            char c = cur();
            if(is_digit(c)) {
                parse_number();
            }
            else if(c == '+') {
                tokens.push_back({ TokenType::plus});
                c = next();
            }
            else if(c == '=') {
                tokens.push_back({ TokenType::equals });
                c = next();
            }
            else if(is_whitespace(c)) {
                c = next();
            }
            else {
                c = next();
            }
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
    char cur() {
        return chars[idx];
    }

    char next() {
        ++idx;
        if(idx < chars.size()) {
            return cur();
        }
        return ' ';
    }

    void parse_number() {
        int start = idx;
        int end = idx;
        char c = cur();
        while(is_digit(c)) {
            ++end;
            c = next();
        }
        std::string_view number_src(chars.begin() + start, chars.begin() + end);
        int value;
        std::from_chars(number_src.begin(), number_src.end(), value);
        tokens.push_back({ TokenType::integer, value });
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

    bool is_digit(char c) {
        return c >= '0' && c <= '9';
    }

};
