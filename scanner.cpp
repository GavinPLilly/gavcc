#include <charconv>

#include "gavcc.h"

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
                tokens.push_back({ .type = tt::lparen,
                    .lexeme = std::to_string(c) });
                next();
            }
            else if(c == ')') {
                tokens.push_back({ .type = tt::rparen,
                    .lexeme = std::to_string(c) });
                next();
            }
            else if(c == '+') {
                tokens.push_back({ .type = tt::plus,
                    .lexeme = std::to_string(c) });
                next();
            }
            else if(c == '-') {
                tokens.push_back({ .type = tt::minus,
                    .lexeme = std::to_string(c) });
                next();
            }
            else if(c == '*') {
                tokens.push_back({ .type = tt::star,
                    .lexeme = std::to_string(c) });
                next();
            }
            else if(c == '/') {
                tokens.push_back({ .type = tt::div,
                    .lexeme = std::to_string(c) });
                next();
            }
            else if(c == '=') {
                tokens.push_back({ .type = tt::equal,
                    .lexeme = std::to_string(c) });
                next();
            }
            else if(c == ';') {
                tokens.push_back({ .type = tt::semicolon,
                    .lexeme = std::to_string(c) });
                next();
            }
            else if(c == EOF) {
                tokens.push_back({ .type = tt::eof });
                next();
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
            tokens.push_back({ .type = tt::id, .lexeme = id_text, .id = id_text });
        }
    }

    void scan_number() {
        int start = idx;
        int len = 0;
        char c = cur();
        while(is_digit(c)) {
            ++len;
            c = next();
        }
        string lexeme(chars, start, len);
        int value;
        std::from_chars(lexeme.data(), lexeme.data() + lexeme.size(), value);
        tokens.push_back({ .type = tt::integer, .lexeme = lexeme, .ival = value });
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
        if(id_text == "int") {
            return { .type = tt::kw_int };
        }
        if(id_text == "return") {
            return { .type = tt::kw_return };
        }
        exit(5);
    }

};
