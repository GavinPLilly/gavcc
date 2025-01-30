#include "gavcc.h"
using i64 = int64_t;
using tt = TokenType;

class Eval {
    Node* ast;

public:
    Eval(Node* ast): ast(ast) {
    }

    int64_t eval() {
        return eval_node(ast);
    }

private:
    i64 eval_node(Node* cur) {
        if(cur->type == NodeType::lit) {
            return cur->token.ival;
        }
        i64 left = eval_node(cur->left);
        i64 right = eval_node(cur->right);
        TokenType type = cur->token.type;
        if(type == tt::plus) {
            return left + right;
        }
        if(type == tt::minus) {
            return left - right;
        }
        if(type == tt::star) {
            return left * right;
        }
        if(type == tt::div) {
            return left / right;
        }
        return -99;
    }
};
