#ifndef TREE_H
#define TREE_H

#include "Node.h"

struct moves
{
    std::function<size_t(size_t)> move;
};

class Tree
{
private:
    Node *initial_state;
    size_t dir;

    std::vector<std::vector<size_t>> all_states;
    auto compare_vec(std::vector<size_t> &v1, std::vector<size_t> &v2) -> bool;
    auto is_in_tree(std::vector<size_t> &v) -> bool;

    auto _free(Node *n) -> Node *;

    auto final_state(std::vector<size_t> &state) -> bool;
    auto make_move(std::vector<size_t> &state, size_t pos) -> std::pair<std::vector<size_t>, size_t>;
    auto create_Node(std::vector<size_t> &state, std::vector<size_t> &positions, Node *p, size_t r) -> Node *;
    auto delete_Node(Node *p) -> void;

    auto print_path(Node *p) -> void;

    moves indexes[2] = {
        {[](size_t idx)
         { return ((idx + 3) % 8 + 8) % 8; }},
        {[](size_t idx)
         { return ((idx - 3) % 8 + 8) % 8; }}};

public:
    Tree(size_t dir);
    ~Tree();

    auto backtracking(size_t p) -> void;
    auto bfs() -> void;
    auto dfs() -> void;
};

#endif // TREE_H