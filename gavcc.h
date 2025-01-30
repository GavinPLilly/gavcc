#pragma once

enum class TokenType;
struct Token;
enum class NodeType;
struct Node;

std::string tokens_to_string(std::vector<Token> tokens);
std::string token_to_string(Token token);
