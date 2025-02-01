#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <charconv>

#include "gavcc.h"

enum class TokenType {
    // Keywords
    kw_int,
    kw_return,
    // Literals
    id,
    integer,
    // Grouping
    lparen,
    rparen,
    // Symbols
    plus,
    minus,
    star,
    div,
    equal,
    semicolon,
    eof,
};

struct Token {
    TokenType type;
    int64_t ival;
    std::string id_name;
};

using tt = TokenType;

class Scanner {
    const std::string chars;
    std::vector<Token> tokens;
    int idx = 0;

public:
    Scanner(std::string chars): chars(chars) {}

    std::vector<Token> scan() {
        while(idx < chars.size()) {
            char c = cur();
            if(is_whitespace(c)) {
                next();
            }
            else if(is_id_start(c)) {
                scan_id();
            }
            else if(is_digit(c)) {
                scan_number();
            }
            else if(c == '(') {
                tokens.push_back({ tt::lparen });
                next();
            }
            else if(c == ')') {
                tokens.push_back({ tt::rparen });
                next();
            }
            else if(c == '+') {
                tokens.push_back({ tt::plus });
                next();
            }
            else if(c == '-') {
                tokens.push_back({ tt::minus });
                next();
            }
            else if(c == '*') {
                tokens.push_back({ tt::star });
                next();
            }
            else if(c == '/') {
                tokens.push_back({ tt::div });
                next();
            }
            else if(c == '=') {
                tokens.push_back({ tt::equal });
            }
            else if(c == ';') {
                tokens.push_back({ tt::semicolon });
                next();
            }
            else if(c == EOF) {
                tokens.push_back({ tt::eof });
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

    char peek() {
        if(idx + 1 < chars.size()) {
            return chars[idx + 1];
        }
        return EOF;
    }

    void scan_id() {
        int start = idx;
        int end = idx;
        char c = cur();
        while(is_id_char(c)) {
            ++end;
            c = next();
        }
        std::string id_text(&chars[start], end - start);
        if(is_kw(id_text)) {
            tokens.push_back(scan_kw(id_text));
        }
        else {
            tokens.push_back({ .type = tt::id, .id_name = id_text });
        }
    }

    void scan_number() {
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
        tokens.push_back({ .type = tt::integer, .ival = value });
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

    bool is_kw(std::string id_text) {
        if(id_text == "int") {
            return true;
        }
        if(id_text == "return") {
            return true;
        }
        return false;
    }

    Token scan_kw(std::string id_text) {
        return { .type = tt::kw_int };
    }

};

std::string tokens_to_string(std::vector<Token> tokens) {
    std::string s;
    for(auto token : tokens) {
        s += token_to_string(token);
    }
    return s;
}

std::string token_to_string(Token token) {
    std::string s;
    switch(token.type) {
        case tt::kw_int:
            s += "kw:int";
            break;
        case tt::id:
            s += "id";
            break;
        case tt::lparen:
            s += "(";
            break;
        case tt::rparen:
            s += ")";
            break;
        case tt::integer:
            s += "int";
            break;
        case tt::plus:
            s += '+';
            break;
        case tt::minus:
            s += '-';
            break;
        case tt::star:
            s += '*';
            break;
        case tt::div:
            s += '/';
            break;
        case tt::equal:
            s += '=';
            break;
        case tt::semicolon:
            s += ';';
            break;
        case tt::eof:
            s += "EOF";
            break;
        default:
            s += "[X]";
            break;
    }
    s += " ";
    return s;
}
