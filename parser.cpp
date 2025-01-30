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

using tt = TokenType;

[[noreturn]]
void report_parse_error(std::string msg, Token token) {
    std::cout << msg << std::endl;
    exit(1);
}

[[noreturn]]
void expected_but_found(std::string expected, Token found) {
    std::cout << "Expected " << expected << ", but found " << token_to_string(found);
    exit(2);
}

class Parser {
    const std::vector<Token> tokens;
    int idx = 0;

public:
    Parser(std::vector<Token> tokens): tokens(tokens) {}

    Node* parse() {
        return parse_expr();
        if(cur().type != tt::eof) {
            expected_but_found("eof", cur());
        }
    }

private:
    Node* parse_expr() {
        Node* expr_root = parse_term();

        while(cur().type != tt::eof && (cur().type == tt::plus || cur().type == tt::minus)) {
            Node* op = new Node;
            op->type = NodeType::op;
            op->token = cur();
            next();

            if(cur().type == tt::eof) {
                expected_but_found("term", cur());
            }
            Node * right = parse_term();

            op->left = expr_root;
            op->right = right;
            expr_root = op;
        }

        return expr_root;
    }

    Node* parse_term() {
        Node* term_root = parse_unit();
        
        while(cur().type != tt::eof && (cur().type == tt::star || cur().type == tt::div)) {
            Node* op = new Node;
            op->type = NodeType::op;
            op->token = cur();
            next();

            if(cur().type == tt::eof) {
                expected_but_found("unit", cur());
            }
            Node* right = parse_unit();

            op->left = term_root;
            op->right = right;
            term_root = op;
        }

        return term_root;
    }

    Node* parse_unit() {
        if(cur().type != tt::lparen) {
            return parse_lit();
        }

        next();
        Node* result = parse_expr();

        if(cur().type != tt::rparen) {
            expected_but_found("right parenthesis", cur());
        }
        next();

        return result;
    }

    Node* parse_lit() {
        Token t = cur();
        if(t.type != TokenType::integer) {
            expected_but_found("literal", cur());
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
