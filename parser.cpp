#include <string>
#include <vector>

#include "gavcc.h"

enum class NodeType {
    lit,
    op,
};

struct Node {
    NodeType type;
    Token token;
    Node* left;
    Node* right;
};

class Parser {
    const std::vector<Token> tokens;
    int idx = 0;

public:
    Parser(std::vector<Token> tokens):
        tokens(tokens)
    {
    }

    Node* parse() {
        return parse_expr();
    }

private:
    Node* parse_expr() {
        Node* left;
        Node* op;
        Node* right;
        Token t;

        left = parse_term();

        t = cur();
        if(t.type == TokenType::eof || !(t.type == TokenType::plus || t.type == TokenType::minus)) {
            return left;
        }
        op = new Node;
        op->type = NodeType::op;
        op->token = t;
        next();

        t = cur();
        if(t.type == TokenType::eof) {
            std::cout << "Expected term" << std::endl;
            exit(1);
        }

        right = parse_expr();

        op->left = left;
        op->right = right;
        return op;
    }

    Node* parse_term() {
        Node* left;
        Node* op;
        Node* right;
        Token t;

        left = parse_lit();

        t = cur();
        if(t.type == TokenType::eof || !(t.type == TokenType::star || t.type == TokenType::div)) {
            return left;
        }

        op = new Node;
        op->type = NodeType::op;
        op->token = t;
        next();

        t = cur();
        if(t.type == TokenType::eof) {
            std::cout << "Expected term" << std::endl;
            exit(1);
        }

        right = parse_term();

        op->left = left;
        op->right = right;
        return op;
    }

    Node* parse_lit() {
        Token t = cur();
        if(t.type != TokenType::integer) {
            std::cout << "Expected literal" << std::endl;
            exit(1);
        }
        Node* result = new Node;
        result->type = NodeType::lit;
        result->token = t;
        next();
        return result;
    }

    Token cur() {
        if(idx < tokens.size()) {
            return tokens[idx];
        }
        return { .type = TokenType::eof };
    }

    Token next() {
        ++idx;
        return cur();
    }

    Token peek() {
        if(idx + 1 < tokens.size()) {
            return tokens[idx + 1];
        }
        return make_eof_token();
    }

    Token make_eof_token() {
        return { .type = TokenType::eof };
    }

};

std::string op_str(TokenType type) {
    if(type == TokenType::plus) {
        return "+";
    }
    if(type == TokenType::star) {
        return "*";
    }
    return "UNKNOWN OP";
}

void print_ast(Node* root) {
    auto left = ((root->left)->token).ival;
    TokenType op = (root->token).type;
    auto right = ((root->right)->token).ival;

    std::string s;
    s += std::to_string(left);
    s += op_str(op);
    s += std::to_string(right);

    std::cout << s << std::endl;
}
