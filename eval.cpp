#include "gavcc.h"
#include <iostream>
#include <optional>
#include <unordered_map>
#include <format>

using i64 = int64_t;
using std::cout;
using std::endl;
using std::unordered_map;
using std::optional;
using std::nullopt;
using tt = TokenType;
using nt = NodeType;

class ScopedSymbolTable {
    vector<unordered_map<string, optional<int>>> scopes;
public:
    ScopedSymbolTable() {
        scopes.push_back({});
    }
    void add_symbol(string sym);
    void assn_value(string sym, int value);
    int get_value(string sym);
    void add_scope();
    void close_scope();
};

class Eval {
    Node* ast;
    ScopedSymbolTable scopes;

public:
    Eval(Node* ast): ast(ast) {}

    int64_t eval() {
        return eval_node(ast);
    }

private:
    i64 eval_node(Node* cur) {
        nt type = cur->type;
        if(type == nt::prgm) {
            for(Node* stmt : cur->stmts) {
                eval_node(stmt);
            }
        }
        else if(type == nt::block) {
            scopes.add_scope();
            for(Node* stmt : cur->stmts) {
                eval_node(stmt);
            }
            scopes.close_scope();
        }
        else if(type == nt::stmt_while) {
            while(eval_node(cur->expr)) {
                eval_node(cur->body);
            }
        }
        else if(type == nt::stmt_decl) {
            string id = cur->id;
            scopes.add_symbol(id);
        }
        else if(type == nt::stmt_assn) {
            string id = cur->id;
            i64 value = eval_node(cur->expr);
            scopes.assn_value(id, value);
        }
        else if(type == nt::stmt_return) {
            i64 value = eval_node(cur->expr);
            cout << value << endl;
        }
        else if(type == nt::paren_group) {
            return eval_node(cur->expr);
        }
        else if(type == nt::biop_plus) {
            i64 left = eval_node(cur->left);
            i64 right = eval_node(cur->right);
            return left + right;
        }
        else if(type == nt::biop_minus) {
            i64 left = eval_node(cur->left);
            i64 right = eval_node(cur->right);
            return left - right;
        }
        else if(type == nt::biop_mul) {
            i64 left = eval_node(cur->left);
            i64 right = eval_node(cur->right);
            return left * right;
        }
        else if(type == nt::biop_div) {
            i64 left = eval_node(cur->left);
            i64 right = eval_node(cur->right);
            return left / right;
        }
        else if(type == nt::unary_plus) {
            return eval_node(cur->expr);
        }
        else if(type == nt::unary_minus) {
            return -eval_node(cur->expr);
        }
        else if(type == nt::lit_int) {
            return cur->token.ival;
        }
        else if(type == nt::lit_id) {
            string name = cur->id;
            int value = scopes.get_value(name);
            return value;
        }
        else {
            cout << format("UNRECOGNIZED NODE TYPE: {}", to_string::node_type(type)) << endl;
        }
        return 0;
    }
};

void ScopedSymbolTable::add_symbol(string sym) {
    auto& cur_scope = scopes.back();
    if(cur_scope.contains(sym)) {
        cout << format("symbol '{}' has already been defined", sym) << endl;
        exit(10);
    }
    cur_scope.insert({sym, nullopt});
}

void ScopedSymbolTable::assn_value(string sym, int value) {
    for(auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
        if(it->contains(sym)) {
            it->at(sym) = optional(value);
            return;
        }
    }
    cout << format("symbol '{}' not found", sym) << endl;
    exit(12);
}

int ScopedSymbolTable::get_value(string sym) {
    for(auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
        if(it->contains(sym)) {
            optional<int> value = it->at(sym);
            if(!value.has_value()) {
                cout << format("symbol '{}' has not been initialized", sym) << endl;
                exit(9);
            }
            return value.value();
        }
    }
    cout << format("symbol '{}' not found", sym) << endl;
    exit(8);
}

void ScopedSymbolTable::add_scope() {
    scopes.push_back({});
}

void ScopedSymbolTable::close_scope() {
    if(scopes.size() == 1) {
        cout << "Error: tried to close global scope" << endl;
        exit(11);
    }
    scopes.pop_back();
}
