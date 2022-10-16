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
    unsigned rule;
    std::vector<size_t> state;
    Node *parent;
    std::vector<Node*> children;
    std::vector<size_t> positions;
    std::queue<size_t> R;
public:
    Node(std::vector<size_t>& state, std::vector<size_t>& pos, unsigned r, Node *p);
    ~Node();

    auto get_state() -> std::vector<size_t>;
    auto get_positions() -> std::vector<size_t>;

    auto get_rule() -> unsigned;

    auto get_pos(size_t r) -> size_t;

    auto get_parent() -> Node *;
    auto set_parent(Node *Node) -> void;

    auto get_children(size_t r) -> Node *;
    auto set_children(Node *Node, size_t) -> void;

    auto queue_front() -> size_t;
    auto queue_pop() -> void;
    auto queue_empty() -> bool;
};

#endif // NODE_H
