#include "gavcc.h"
#include <iostream>

using nt = NodeType;
using tt = TokenType;
using std::cout;
using std::endl;

[[noreturn]]
void report_parse_error(string msg, Token token) {
    cout << msg << endl;
    exit(1);
}

[[noreturn]]
void expected_but_found(string expected, Token found) {
    cout << "Expected " << expected << ", but found " << to_string::token(found);
    exit(2);
}

[[noreturn]]
void expected_but_found(tt expected_type, Token found) {
    cout << "Expected " << to_string::token_type(expected_type) << ", but found " << to_string::token(found);
    exit(3);
}

void assert_not_eof(Token token, string msg) {
    if(token.type == tt::eof) {
        expected_but_found(msg, token);
    }
}

void assert_for(tt expected_type, Token token) {
    if(token.type != expected_type) {
        expected_but_found(expected_type, token);
    }
}

class Parser {
    const vector<Token> tokens;
    int idx = 0;

public:
    Parser(vector<Token> tokens): tokens(tokens) {}

    Node* parse() {
        Node* prgm_root = new Node;
        prgm_root->type = nt::prgm;

        while(cur().type != tt::eof) {
            prgm_root->stmts.push_back(parse_stmt());
        }
        assert_for(tt::eof, cur());

        return prgm_root;
    }

private:
    Node* parse_stmt() {
        Node* result;
        if(cur().type == tt::kw_int) {
            result = parse_stmt_decl();
        }
        else if(cur().type == tt::id) {
            result =  parse_stmt_assn();
        }
        else if(cur().type == tt::kw_return) {
            result =  parse_stmt_return();
        }
        else {
            expected_but_found("Stmt", cur());
        }
        assert_for(tt::semicolon, cur());
        next();
        return result;
    }

    Node* parse_stmt_decl() {
        assert_for(tt::kw_int, cur());
        next();

        Node* decl_root = new Node;
        decl_root->type = nt::stmt_decl;
        assert_for(tt::id, cur());
        decl_root->id = cur();
        next();
        return decl_root;
    }

    Node* parse_stmt_assn() {
        assert_for(tt::id, cur());
        Node* assn_root = new Node;
        assn_root->type = nt::stmt_assn;
        assn_root->id = cur();
        next();

        assert_for(tt::equal, cur());
        next();

        assn_root->expr = parse_expr();
        return assn_root;
    }

    Node* parse_stmt_return() {
        assert_for(tt::kw_return, cur());
        next();

        Node* return_root = new Node;
        return_root->type = nt::stmt_return;
        return_root->expr = parse_expr();
        return return_root;
    }

    Node* parse_expr() {
        Node* expr_root = parse_term();

        while(cur().type == tt::plus || cur().type == tt::minus) {
            Node* op = new Node;
            op->type = nt::op;
            op->token = cur();
            next();

            assert_not_eof(cur(), "term");
            Node * right = parse_term();

            op->left = expr_root;
            op->right = right;
            expr_root = op;
        }

        return expr_root;
    }

    Node* parse_term() {
        Node* term_root = parse_unit();
        
        while(cur().type == tt::star || cur().type == tt::div) {
            Node* op = new Node;
            op->type = nt::op;
            op->token = cur();
            next();

            assert_not_eof(cur(), "unit");
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

        assert_for(tt::rparen, cur());
        next();

        return result;
    }

    Node* parse_lit() {
        Node* result = new Node;
        if(cur().type == tt::integer) {
            result->type = nt::lit_int;
            result->token = cur();
            next();
        }
        else if(cur().type == tt::id) {
            result->type = nt::lit_id;
            result->token = cur();
            next();
        }
        else {
            expected_but_found("literal", cur());
        }
        return result;
    }

    Token cur() {
        if(idx < tokens.size()) {
            return tokens[idx];
        }
        return { .type = tt::eof };
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

