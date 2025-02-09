#include "gavcc.h"
#include <format>
#include <unordered_set>
#include <iostream>

using std::string;
template <typename T>
using set = std::unordered_set<T>;
using std::cout;
using std::endl;
using nt = NodeType;
using tt = TokenType;

void sem_anal_error(string msg);

class ScopedDeclSet {
    vector<set<string>> scopes;
public:
    ScopedDeclSet() {
        scopes.push_back({});
    }
    void add_decl(string sym);
    bool is_decled(string sym);
    void add_scope();
    void close_scope();
};

class SemAnal {
    Node* ast;
    ScopedDeclSet scopes;

public:

    SemAnal(Node* ast): ast(ast) {}

    void sem_anal() {
        sem_anal_node(ast);
    }

private:
    void sem_anal_node(Node* cur) {
        nt type = cur->type;
        if(type == nt::prgm) {
            for(Node* stmt : cur->stmts) {
                sem_anal_node(stmt);
            }
        }
        else if(type == nt::block) {
            scopes.add_scope();
            for(Node * stmt : cur->stmts) {
                sem_anal_node(stmt);
            }
            scopes.close_scope();
        }
        else if(type == nt::stmt_decl) {
            string sym = cur->id;
            if(scopes.is_decled(sym)) {
                sem_anal_error(std::format("Tried to declare, but symbol '{}' is already declared", sym));
            }
            scopes.add_decl(sym);
        }
        else if(type == nt::stmt_assn) {
            string sym = cur->id;
            if(!scopes.is_decled(sym)) {
                sem_anal_error(std::format("Tried to assign value, but symbol '{}' has not been declared", sym));
            }
            sem_anal_node(cur->expr);
        }
        else if(type == nt::stmt_return) {
            sem_anal_node(cur->expr);
        }
        else if(type == nt::biop_plus
                || type == nt::biop_minus
                || type == nt::biop_mul
                || type == nt::biop_div) {
            sem_anal_node(cur->left);
            sem_anal_node(cur->right);
        }
        else if(type == nt::unary_plus
                || type == nt::unary_minus) {
            sem_anal_node(cur->expr);
        }
        else if(type == nt::lit_int) {
            // nothing
        }
        else if(type == nt::lit_id) {
            string sym = cur->id;
            if(!scopes.is_decled(sym)) {
                sem_anal_error(std::format("Tried to use symbol, but symbol '{}' has not been declared", sym));
            }
        }
    }

};

void sem_anal_error(string msg) {
    cout << msg << endl;
    exit(6);
}

void ScopedDeclSet::add_decl(string sym) {
    scopes.back().insert(sym);
}

bool ScopedDeclSet::is_decled(string sym) {
    for(auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
        if(it->contains(sym)) {
            return true;
        }
    }
    return false;
}

void ScopedDeclSet::add_scope() {
    scopes.push_back({});
}

void ScopedDeclSet::close_scope() {
    if(scopes.size() == 1) {
        cout << "Error: tried to destroy global scope" << endl;
        exit(7);
    }
    scopes.pop_back();
}
