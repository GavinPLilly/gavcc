#include <string>
#include <vector>

#include "gavcc.h"

enum class NodeType {
    prgm,
    stmt_decl,
    stmt_assn,
    stmt_return,
    op,
    lit,
};

struct Node {
    NodeType type;
    Token token;
    std::vector<Node*> stmts;
    Token id;
    Node* expr;
    Node* left;
    Node* right;
};

using nt = NodeType;
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
        Node* prgm_root = new Node;
        prgm_root->type = nt::prgm;

        while(cur().type != tt::eof) {
            prgm_root->stmts.push_back(parse_stmt());
        }

        if(cur().type != tt::eof) {
            expected_but_found("eof", cur());
        }

        return prgm_root;
    }

private:
    Node* parse_stmt() {
        if(cur().type == tt::kw_int) {
            return parse_stmt_decl();
        }
        else if(cur().type == tt::id) {
            return parse_stmt_assn();
        }
        else if(cur().type == tt::kw_return) {
            return parse_stmt_return();
        }
        if(cur().type != tt::semicolon) {
            expected_but_found(";", cur());
        }
        expected_but_found("Stmt", cur());
    }

    Node* parse_stmt_decl() {
        if(cur().type != tt::kw_int) {
            expected_but_found("kw_int", cur());
        }
        next();

        Node* decl_root = new Node;
        decl_root->type = nt::stmt_decl;
        if(cur().type != tt::id) {
            expected_but_found("id", cur());
        }
        decl_root->id = cur();
        decl_root->expr = parse_expr();
        return decl_root;
    }

    Node* parse_stmt_assn() {
        if(cur().type != tt::id) {
            expected_but_found("id", cur());
        }
        Node* assn_root = new Node;
        assn_root->type = nt::stmt_assn;
        assn_root->id = cur();
        next();

        if(cur().type != tt::equal) {
            expected_but_found("=", cur());
        }
        next();

        assn_root->expr = parse_expr();
        return assn_root;
    }

    Node* parse_stmt_return() {
        if(cur().type != tt::kw_return) {
            expected_but_found("kw_return", cur());
        }
        next();

        Node* return_root = new Node;
        return_root->type = nt::stmt_return;
        return_root->expr = parse_expr();
        return return_root;
    }

    Node* parse_expr() {
        Node* expr_root = parse_term();

        while(cur().type != tt::eof && (cur().type == tt::plus || cur().type == tt::minus)) {
            Node* op = new Node;
            op->type = nt::op;
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
            op->type = nt::op;
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
        result->type = nt::lit;
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
