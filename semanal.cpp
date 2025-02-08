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

class DeclSet {
    set<string> decls;
public:
    void add_decl(string sym);
    bool is_decled(string sym);
};

class SemAnal {
    Node* ast;
    DeclSet decl_set;

public:

    SemAnal(Node* ast): ast(ast) {}

    void sem_anal() {
        sem_anal_node(ast);
    }

private:
    void sem_anal_node(Node* cur) {
        nt type = cur->type;
        if(type == nt::prgm) {
            for(const auto stmt : cur->stmts) {
                sem_anal_node(stmt);
            }
        }
        else if(type == nt::stmt_decl) {
            string sym = cur->id;
            if(decl_set.is_decled(sym)) {
                sem_anal_error(std::format("Tried to declare, but symbol '{}' is already declared", sym));
            }
            decl_set.add_decl(sym);
        }
        else if(type == nt::stmt_assn) {
            string sym = cur->id;
            if(!decl_set.is_decled(sym)) {
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
            if(!decl_set.is_decled(sym)) {
                sem_anal_error(std::format("Tried to use symbol, but symbol '{}' has not been declared", sym));
            }
        }
    }

};

void sem_anal_error(string msg) {
    cout << msg << endl;
    exit(6);
}

void DeclSet::add_decl(string sym) {
    decls.insert(sym);
}

bool DeclSet::is_decled(string sym) {
    return decls.contains(sym);
}
