#include "Node.h"

Node::Node(std::vector<size_t> &state, std::vector<size_t> &pos, unsigned r, Node *p)
{
    this->rule = r;

    this->state.reserve(8);
    this->state = state;

    this->positions.reserve(4);
    this->positions = pos;

    this->children.reserve(4);
    for (size_t i = 0; i < 4; ++i)
        children[i] = NULL;
        
    this->parent = p;

    for (size_t i = 0; i < 4; ++i)
        R.push(i);
}

Node::~Node() {}

Node *Node::get_parent() { return this->parent; }
void Node::set_parent(Node *p) { this->parent = p; }

size_t Node::get_pos(size_t r) { return positions[r]; }

unsigned Node::get_rule() { return this->rule; }

Node *Node::get_children(size_t r) { return children[r]; }
void Node::set_children(Node *p, size_t r) { children[r] = p; }

auto Node::get_state() -> std::vector<size_t> { return state; }
auto Node::get_positions() -> std::vector<size_t> { return positions; }

auto Node::queue_front() -> size_t
{
    return R.front();
}
auto Node::queue_pop() -> void
{
    R.pop();
}
auto Node::queue_empty() -> bool
{
    return R.empty();
}
