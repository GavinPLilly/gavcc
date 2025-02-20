#include "gavcc.h"
#include <iostream>

using nt = NodeType;
using tt = TokenType;
using std::cout;
using std::endl;

[[noreturn]]
void expected_but_found(string expected, Token found) {
    cout << "Expected " << expected << ", but found " << to_string::token(found) << endl;
    exit(2);
}

[[noreturn]]
void expected_but_found(tt expected_type, Token found) {
    cout << "Expected " << to_string::token_type(expected_type) << ", but found " << to_string::token(found) << endl;
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
    uint idx = 0;

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
        tt type = cur().type;
        if(type == tt::lbrace) {
            return parse_block();
        }
        else if(type == tt::kw_while) {
            return parse_while();
        }
        else if(type == tt::kw_int) {
            result = parse_stmt_decl();
            assert_for(tt::semicolon, cur());
            next();
            return result;
        }
        else if(type == tt::id) {
            result =  parse_stmt_assn();
            assert_for(tt::semicolon, cur());
            next();
            return result;
        }
        else if(type == tt::kw_return) {
            result =  parse_stmt_return();
            assert_for(tt::semicolon, cur());
            next();
            return result;
        }
        expected_but_found("Stmt", cur());
    }

    Node* parse_block() {
        assert_for(tt::lbrace, cur());
        next();

        Node* block_root = new Node;
        block_root->type = nt::block;
        while(cur().type != tt::rbrace) {
            block_root->stmts.push_back(parse_stmt());
        }

        assert_for(tt::rbrace, cur());
        next();

        return block_root;
    }

    Node* parse_while() {
        assert_for(tt::kw_while, cur());
        next();
        assert_for(tt::lparen, cur());
        next();

        Node* while_root = new Node;
        while_root->type = nt::stmt_while;
        while_root->expr = parse_expr();

        assert_for(tt::rparen, cur());
        next();

        while_root->body = parse_stmt();

        return while_root;
    }

    Node* parse_stmt_decl() {
        assert_for(tt::kw_int, cur());
        next();

        Node* decl_root = new Node;
        decl_root->type = nt::stmt_decl;
        assert_for(tt::id, cur());
        decl_root->id = cur().id;
        next();
        return decl_root;
    }

    Node* parse_stmt_assn() {
        assert_for(tt::id, cur());
        Node* assn_root = new Node;
        assn_root->type = nt::stmt_assn;
        assn_root->id = cur().id;
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
            Node* biop = new Node;
            biop->token = cur();
            if(cur().type == tt::plus) {
                biop->type = nt::biop_plus;
            }
            else {
                biop->type = nt::biop_minus;
            }
            next();

            assert_not_eof(cur(), "term");
            Node * right = parse_term();

            biop->left = expr_root;
            biop->right = right;
            expr_root = biop;
        }

        return expr_root;
    }

    Node* parse_term() {
        Node* term_root = parse_unit();
        
        while(cur().type == tt::star || cur().type == tt::div) {
            Node* biop = new Node;
            biop->token = cur();
            if(cur().type == tt::star) {
                biop->type = nt::biop_mul;
            }
            else {
                biop->type = nt::biop_div;
            }
            next();

            assert_not_eof(cur(), "unit");
            Node* right = parse_unit();

            biop->left = term_root;
            biop->right = right;
            term_root = biop;
        }

        return term_root;
    }

    Node* parse_unit() {
        if(cur().type == tt::plus) {
            Node* result = new Node;
            result->type = nt::unary_plus;
            result->token = cur();
            next();
            result->expr = parse_unit();
            return result;
        }
        if(cur().type == tt::minus) {
            Node* result = new Node;
            result->type = nt::unary_minus;
            result->token = cur();
            next();
            result->expr = parse_unit();
            return result;
        }
        if(cur().type != tt::lparen) {
            return parse_lit();
        }

        next();
        Node* result = new Node;
        result->type = nt::paren_group;
        result->expr = parse_expr();

        assert_for(tt::rparen, cur());
        next();

        return result;
    }

    Node* parse_lit() {
        Node* result = new Node;
        if(cur().type == tt::integer) {
            result->type = nt::lit_int;
            result->token = cur();
            result->ival = cur().ival;
            next();
        }
        else if(cur().type == tt::id) {
            result->type = nt::lit_id;
            result->token = cur();
            result->id = cur().id;
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

