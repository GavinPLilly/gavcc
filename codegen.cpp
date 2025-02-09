#include "gavcc.h"
#include <format>

using tt = TokenType;
using nt = NodeType;

class CodeGen {
    Node* ast;
    string out;
    string indent;
    bool starting_new_line = true;
public:
    CodeGen(Node* ast): ast(ast) {}

    string code_gen() {
        code_gen_node(ast);
        return out;
    }
private:
    void code_gen_node(Node* cur) {
        nt type = cur->type;
        if(type == nt::prgm) {
            for(Node* stmt : cur->stmts) {
                code_gen_node(stmt);
            }
        }
        else if(type == nt::block) {
            writeln("{");
            inc_indent();
            for(Node* stmt : cur->stmts) {
                code_gen_node(stmt);
            }
            dec_indent();
            writeln("}");
        }
        else if(type == nt::stmt_while) {
            write("while(");
            code_gen_node(cur->expr);
            write(")");
            if(cur->body->type == nt::block) {
                write(" ");
                code_gen_node(cur->body);
            }
            else {
                writeln("");
                inc_indent();
                code_gen_node(cur->body);
                dec_indent();
            }
        }
        else if(type == nt::stmt_decl) {
            writeln(format("int {};", cur->id));
        }
        else if(type == nt::stmt_assn) {
            write(format("{} = ", cur->id));
            code_gen_node(cur->expr);
            writeln(";");
        }
        else if(type == nt::stmt_return) {
            write("return ");
            code_gen_node(cur->expr);
            writeln(";");
        }
        else if(type == nt::paren_group) {
            write("(");
            code_gen_node(cur->expr);
            write(")");
        }
        else if(type == nt::biop_plus) {
            code_gen_node(cur->left);
            write(" + ");
            code_gen_node(cur->right);
        }
        else if(type == nt::biop_minus) {
            code_gen_node(cur->left);
            write(" - ");
            code_gen_node(cur->right);
        }
        else if(type == nt::biop_mul) {
            code_gen_node(cur->left);
            write(" * ");
            code_gen_node(cur->right);
        }
        else if(type == nt::biop_div) {
            code_gen_node(cur->left);
            write(" / ");
            code_gen_node(cur->right);
        }
        else if(type == nt::unary_plus) {
            write("+");
            code_gen_node(cur->expr);
        }
        else if(type == nt::unary_minus) {
            write("-");
            code_gen_node(cur->expr);
        }
        else if(type == nt::lit_int) {
            string ival = std::to_string(cur->ival);
            write(format("{}", ival));
        }
        else if(type == nt::lit_id) {
            write(format("{}", cur->id));
        }
    }

    void writeln(string s) {
        write(s + '\n');
        starting_new_line = true;
    }

    void write(string s) {
        if(starting_new_line) {
            out += indent;
            starting_new_line = false;
        }
        out += s;
    }

    void inc_indent() {
        indent += "    ";
    }

    void dec_indent() {
        indent = indent.substr(0, indent.size() - 4);
    }

};
