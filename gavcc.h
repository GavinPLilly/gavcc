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
    string lexeme;
    int64_t ival;
    string id;
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
    int64_t ival;
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
        switch(type) {
            case TokenType::kw_int:
                return "kw:int";
            case TokenType::kw_return:
                return "kw:return";
            case TokenType::id:
                return "id";
            case TokenType::lparen:
                return "(";
            case TokenType::rparen:
                return ")";
            case TokenType::integer:
                return "<int>";
            case TokenType::plus:
                return "+";
            case TokenType::minus:
                return "-";
            case TokenType::star:
                return "*";
            case TokenType::div:
                return "/";
            case TokenType::equal:
                return "=";
            case TokenType::semicolon:
                return ";";
            case TokenType::eof:
                return "EOF";
            default:
                return "[UNIMP]";
        }
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
            s += indent + "biop_plus:\n";
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
            s += indent + "lit_id: " + node->id + "\n";
        }
        return s;
    }

    string node_type(NodeType type) {
        switch(type) {
            case NodeType::prgm:
                return "prgm";
            case NodeType::stmt_decl:
                return "stmt_decl";
            case NodeType::stmt_assn:
                return "stmt_assn";
            case NodeType::stmt_return:
                return "stmt_return";
            case NodeType::biop_plus:
                return "biop_plus";
            case NodeType::biop_minus:
                return "biop_minus";
            case NodeType::biop_mul:
                return "biop_mul";
            case NodeType::biop_div:
                return "biop_div";
            case NodeType::lit_int:
                return "lit_int";
            case NodeType::lit_id:
                return "lit_id";
            default:
                return "UNRECOG NODE TYPE";
        }
    }
}
