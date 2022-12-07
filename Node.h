#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <stddef.h>
#include <algorithm>
#include <functional>
#include <vector>
#include <queue>
#include <stack>

class Node
{
private:
    // regra que gerou o estado
    unsigned rule;
    // estado do no
    std::vector<size_t> state;
    // pai do no
    Node *parent;
    // filhos do no
    std::vector<Node*> children;
    // posicoes das pecas no estado do no
    std::vector<size_t> positions;
    // fila de regras
    std::queue<size_t> fila_de_regras;
    // pilha de regras
    std::stack<size_t> pilha_de_regras;
public:
    // estado, posicoes das pecas no estado, regra que gerou o estado, no pai
    Node(std::vector<size_t>& state, std::vector<size_t>& pos, unsigned r, Node *p);
    ~Node();

    // gets e sets
    auto get_state() -> std::vector<size_t>;
    auto get_positions() -> std::vector<size_t>;

    auto get_rule() -> unsigned;

    auto get_pos(size_t r) -> size_t;

    auto get_parent() -> Node *;
    auto set_parent(Node *Node) -> void;

    auto get_children(size_t r) -> Node *;
    auto set_children(Node *Node, size_t) -> void;

    // funcoes das filas e pilhas
    auto queue_front() -> size_t;
    auto queue_pop() -> void;
    auto queue_empty() -> bool;

    auto stack_front() -> size_t;
    auto stack_pop() -> void;
    auto stack_empty() -> bool;
};

#endif // NODE_H