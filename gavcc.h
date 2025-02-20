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
    kw_while,
    // Literals
    id,
    integer,
    // Grouping
    lbrace,
    rbrace,
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
    block,
    stmt_while,
    stmt_decl,
    stmt_assn,
    stmt_return,
    paren_group,
    biop_plus,
    biop_minus,
    biop_mul,
    biop_div,
    unary_plus,
    unary_minus,
    lit_int,
    lit_id,
};

struct Node {
    NodeType type;
    Token token;
    vector<Node*> stmts;
    Node* body;
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
            s += to_string::token(token) + " ";
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
            case TokenType::kw_while:
                return "kw:while";
            case TokenType::id:
                return "id";
            case TokenType::lbrace:
                return "{";
            case TokenType::rbrace:
                return "}";
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
        }
        return "[UNIMP]";
    }

    string node(Node* node, string indent) {
        NodeType type = node->type;
        string s;
        s += indent + to_string::node_type(type) + ": ";
        switch(type) {
            case NodeType::lit_int:
                s += std::to_string(node->token.ival) + "\n";
                return s;
            case NodeType::lit_id:
                s += node->token.id + "\n";
                return s;
        }
        s += "\n";
        indent += "    ";
        switch(type) {
            case NodeType::prgm: 
                for(Node* stmt : node->stmts) {
                    s += to_string::node(stmt, indent);
                }
                return s;
            case NodeType::block:
                for(Node* stmt : node->stmts) {
                    s += to_string::node(stmt, indent);
                }
                return s;
            case NodeType::stmt_decl:
                s += indent + "name: " + node->id + "\n";
                return s;
            case NodeType::stmt_assn:
                s += indent + "name: " + node->id + "\n";
                s += indent  + "expr:\n";
                indent += "    ";
                s += to_string::node(node->expr, indent);
                return s;
            case NodeType::stmt_return:
                s += indent + "expr:\n";
                indent += "    ";
                s += to_string::node(node->expr, indent);
                return s;
            case NodeType::stmt_while:
                s += indent + "while:\n";
                indent += "    ";
                s += to_string::node(node->expr, indent);
                s += indent + "stmts:\n";
                s += to_string::node(node->body, indent);
                return s;
            case NodeType::paren_group:
                s += to_string::node(node->expr, indent);
                return s;
            case NodeType::biop_plus:
                s += to_string::node(node->left, indent);
                s += to_string::node(node->right, indent);
                return s;
            case NodeType::biop_minus:
                s += to_string::node(node->left, indent);
                s += to_string::node(node->right, indent);
                return s;
            case NodeType::biop_mul:
                s += to_string::node(node->left, indent);
                s += to_string::node(node->right, indent);
                return s;
            case NodeType::biop_div:
                s += to_string::node(node->left, indent);
                s += to_string::node(node->right, indent);
                return s;
            case NodeType::unary_plus:
                s += to_string::node(node->expr, indent);
                return s;
            case NodeType::unary_minus:
                s += to_string::node(node->expr, indent);
                return s;
        }
        return "UNRECOG NODE";
    }

    string node_type(NodeType type) {
        switch(type) {
            case NodeType::prgm:
                return "prgm";
            case NodeType::block:
                return "block";
            case NodeType::stmt_decl:
                return "stmt:decl";
            case NodeType::stmt_assn:
                return "stmt:assn";
            case NodeType::stmt_return:
                return "stmt:return";
            case NodeType::stmt_while:
                return "stmt:while";
            case NodeType::paren_group:
                return "paren_group";
            case NodeType::biop_plus:
                return "biop_plus";
            case NodeType::biop_minus:
                return "biop:minus";
            case NodeType::biop_mul:
                return "biop:mul";
            case NodeType::biop_div:
                return "biop:div";
            case NodeType::unary_plus:
                return "unary:plus";
            case NodeType::unary_minus:
                return "unary:minus";
            case NodeType::lit_int:
                return "lit:int";
            case NodeType::lit_id:
                return "lit:id";
        }
        return "UNRECOG NODE TYPE";
    }
}
