#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <charconv>

enum class TokenType {
    identifier,
    integer,
    plus,
    minus,
    star,
    div,
    equal,
    eof,
};

struct Token {
    TokenType type;
    int64_t ival;
    std::string id_name;
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
            if(is_whitespace(c)) {
                next();
            }
            else if(is_id_start(c)) {
                parse_identifier();
            }
            else if(is_digit(c)) {
                parse_number();
            }
            else if(c == '+') {
                tokens.push_back({ TokenType::plus });
                next();
            }
            else if(c == '-') {
                tokens.push_back({ TokenType::minus });
                next();
            }
            else if(c == '*') {
                tokens.push_back({ TokenType::star });
                next();
            }
            else if(c == '/') {
                tokens.push_back({ TokenType::div });
                next();
            }
            else if(c == '=') {
                tokens.push_back({ TokenType::equal });
                next();
            }
            else if(c == EOF) {
                tokens.push_back({ TokenType::eof });
            }
            else {
                next();
            }
        }
        return tokens;
    }

private:
    char cur() {
        if(idx < chars.size()) {
            return chars[idx];
        }
        return EOF;
    }

    char next() {
        ++idx;
        return cur();
    }

    void parse_identifier() {
        int start = idx;
        int end = idx;
        char c = cur();
        while(is_id_char(c)) {
            ++end;
            c = next();
        }
        tokens.push_back({ .type = TokenType::identifier,
            .id_name = std::string(&chars[start], end - start) });
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
        tokens.push_back({ .type = TokenType::integer, .ival = value });
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

    bool is_id_start(char c) {
        return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
    }

    bool is_id_char(char c) {
        return is_id_start(c) || c == '_';
    }

    bool is_digit(char c) {
        return c >= '0' && c <= '9';
    }

};

void print_tokens(std::vector<Token> tokens)
{
    std::string s;
    for(auto token : tokens) {
        s += "(";
        switch(token.type) {
            case TokenType::identifier:
                s += "ident";
                break;
            case TokenType::integer:
                s += "int";
                break;
            case TokenType::plus:
                s += '+';
                break;
            case TokenType::minus:
                s += '-';
                break;
            case TokenType::star:
                s += '*';
                break;
            case TokenType::div:
                s += '/';
                break;
            case TokenType::equal:
                s += '=';
                break;
            case TokenType::eof:
                s += "EOF";
                break;
            default:
                s += "[X]";
                break;
        }
        s += ") ";
    }
    std::cout << s << std::endl;
}
