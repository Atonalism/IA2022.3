#include "Node.h"

Node::Node(std::vector<size_t> &state, std::vector<size_t> &pos, unsigned r, Node *p)
{
    this->rule = r;

    this->state.reserve(8);
    this->state = state;

    this->positions.reserve(4);
    this->positions = pos;

    // inicia todos os filhos como nulo
    this->children.reserve(4);
    for (size_t i = 0; i < 4; ++i)
        children[i] = NULL;
        
    this->parent = p;

    // preenche as ordens das regras
    for (size_t i = 0; i < 4; ++i)       // da pra melhorar
        fila_de_regras.push(i);
    for (size_t i = 4; i > 0; --i)       // da pra melhorar
        pilha_de_regras.push(i);
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

auto Node::queue_front() -> size_t { return fila_de_regras.front(); }
auto Node::queue_pop() -> void { fila_de_regras.pop(); }
auto Node::queue_empty() -> bool { return fila_de_regras.empty(); }

auto Node::stack_front() -> size_t { return pilha_de_regras.top(); }
auto Node::stack_pop() -> void { pilha_de_regras.pop(); }
auto Node::stack_empty() -> bool { return pilha_de_regras.empty(); }