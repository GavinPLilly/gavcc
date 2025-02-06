#pragma once
#include <vector>
#include <string>
template <typename T>
using vector = std::vector<T>;
using string = std::string;

enum class TokenType {
    // Keywords
    kw_int,
    kw_return,
    // Literals
    id,
    integer,
    // Grouping
    lparen,
    rparen,
    // Symbols
    plus,
    minus,
    star,
    div,
    equal,
    semicolon,
    eof,
};

struct Token {
    TokenType type;
    int64_t ival;
    string id_name;
};

enum class NodeType {
    prgm,
    stmt_decl,
    stmt_assn,
    stmt_return,
    biop_plus,
    biop_minus,
    biop_mul,
    biop_div,
    lit_int,
    lit_id,
};

struct Node {
    NodeType type;
    Token token;
    vector<Node*> stmts;
    string id;
    Node* expr;
    Node* left;
    Node* right;
};


namespace to_string {
    string token_type(TokenType type);
    string token(Token token);
    string tokens(vector<Token> tokens);
    string node_type(NodeType type);
    string node(Node* node, string indent="");

    string tokens(vector<Token> tokens) {
        string s;
        for(Token token : tokens) {
            s += to_string::token(token);
        }
        return s;
    }

    string token(Token token) {
        return to_string::token_type(token.type);
    }

    string token_type(TokenType type) {
        string s;
        switch(type) {
            case TokenType::kw_int:
                s += "kw:int";
                break;
            case TokenType::kw_return:
                s += "kw:return";
                break;
            case TokenType::id:
                s += "id";
                break;
            case TokenType::lparen:
                s += "(";
                break;
            case TokenType::rparen:
                s += ")";
                break;
            case TokenType::integer:
                s += "int";
                break;
            case TokenType::plus:
                s += '+';
                break;
            case TokenType::minus:
                s += '-';
                break;
            case TokenType::star:
                s += '*';
                break;
            case TokenType::div:
                s += '/';
                break;
            case TokenType::equal:
                s += '=';
                break;
            case TokenType::semicolon:
                s += ';';
                break;
            case TokenType::eof:
                s += "EOF";
                break;
            default:
                s += "[UNIMP]";
                break;
        }
        s += " ";
        return s;
    }

    string node(Node* node, string indent) {
        string s;
        if(node->type == NodeType::prgm) {
            s += indent + "prgm:\n";
            indent += "    ";
            for(Node* stmt : node->stmts) {
                s += to_string::node(stmt, indent);
            }
        }
        else if(node->type == NodeType::stmt_decl) {
            s += indent + "stmt_decl:\n";
            indent += "    ";
            s += indent + "name: " + node->id + "\n";
        }
        else if(node->type == NodeType::stmt_assn) {
            s += indent + "stmt_assn:\n";
            indent += "    ";
            s += indent + "name: " + node->id + "\n";
            s += indent  + "expr:\n";
            indent += "    ";
            s += to_string::node(node->expr, indent);
        }
        else if(node->type == NodeType::stmt_return) {
            s += indent + "stmt_return:\n";
            indent += "    ";
            s += indent + "expr:\n";
            indent += "    ";
            s += to_string::node(node->expr, indent);
        }
        else if(node->type == NodeType::biop_plus) {
            s += indent + "op:\n";
            indent += "    ";
            s += indent + token_type(node->token.type) + "\n";
            s += to_string::node(node->left, indent);
            s += to_string::node(node->right, indent);
        }
        else if(node->type == NodeType::biop_minus) {
            s += indent + "op:\n";
            indent += "    ";
            s += indent + token_type(node->token.type) + "\n";
            s += to_string::node(node->left, indent);
            s += to_string::node(node->right, indent);
        }
        else if(node->type == NodeType::biop_mul) {
            s += indent + "op:\n";
            indent += "    ";
            s += indent + token_type(node->token.type) + "\n";
            s += to_string::node(node->left, indent);
            s += to_string::node(node->right, indent);
        }
        else if(node->type == NodeType::biop_div) {
            s += indent + "op:\n";
            indent += "    ";
            s += indent + token_type(node->token.type) + "\n";
            s += to_string::node(node->left, indent);
            s += to_string::node(node->right, indent);
        }
        else if(node->type == NodeType::lit_int) {
            s += indent + "lit_int: " + std::to_string(node->token.ival) + "\n";
        }
        else if(node->type == NodeType::lit_id) {
            s += indent + "lit_id: " + to_string::token(node->token) + "\n";
        }
        return s;
    }

    string node_type(NodeType type) {
        return "NOTIMP";
    }
}
