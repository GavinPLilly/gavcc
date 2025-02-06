#include "gavcc.h"
#include <iostream>
#include <optional>
#include <unordered_map>

using i64 = int64_t;
using std::cout;
using std::endl;
using std::unordered_map;
using std::optional;
using std::nullopt;
using tt = TokenType;
using nt = NodeType;

class SymbolTable {
    unordered_map<string, optional<int>> table;
public:
    void add_symbol(string sym);
    void assn_value(string sym, int value);
    int get_value(string sym);
};

class Eval {
    Node* ast;
    SymbolTable table;

public:
    Eval(Node* ast): ast(ast) {}

    int64_t eval() {
        return eval_node(ast);
    }

private:
    i64 eval_node(Node* cur) {
        if(cur->type == nt::prgm) {
            for(Node* stmt : cur->stmts) {
                eval_node(stmt);
            }
        }
        else if(cur->type == nt::stmt_decl) {
            string id = cur->id;
            table.add_symbol(id);
        }
        else if(cur->type == nt::stmt_assn) {
            string id = cur->id;
            i64 value = eval_node(cur->expr);
            table.assn_value(id, value);
        }
        else if(cur->type == nt::stmt_return) {
            i64 value = eval_node(cur->expr);
            cout << value << endl;
        }
        else if(cur->type == nt::biop_plus) {
            i64 left = eval_node(cur->left);
            i64 right = eval_node(cur->right);
            return left + right;
        }
        else if(cur->type == nt::biop_minus) {
            i64 left = eval_node(cur->left);
            i64 right = eval_node(cur->right);
            return left - right;
        }
        else if(cur->type == nt::biop_mul) {
            i64 left = eval_node(cur->left);
            i64 right = eval_node(cur->right);
            return left * right;
        }
        else if(cur->type == nt::biop_div) {
            i64 left = eval_node(cur->left);
            i64 right = eval_node(cur->right);
            return left / right;
        }
        else if(cur->type == nt::lit_int) {
            return cur->token.ival;
        }
        else if(cur->type == nt::lit_id) {
            string name = cur->id;
            int value = table.get_value(name);
            return value;
        }
        else {
            cout << "UNRECOGNIZED NODE TYPE" << endl;
        }
        return 0;
    }
};

void SymbolTable::add_symbol(string sym) {
    if(table.contains(sym)) {
        throw std::exception();
    }
    table.insert({sym, nullopt});
}

void SymbolTable::assn_value(string sym, int value) {
    if(!table.contains(sym)) {
        throw std::exception();
    }
    table.at(sym) = optional(value);
}

int SymbolTable::get_value(string sym) {
    if(!table.contains(sym)) {
        throw std::exception();
    }
    optional<int> value = table.at(sym);
    if(!value.has_value()) {
        throw std::exception();
    }
    return value.value();
}
