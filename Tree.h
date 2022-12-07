#ifndef TREE_H
#define TREE_H

#include "Node.h"

// estrutura da funcao de movimento
struct moves
{
    std::function<size_t(size_t)> move;
};

class Tree
{
private:
    // ponteiro para o no raiz, ou seja, o estado inicial
    Node *initial_state;
    // direcao dos movimentos: 0 (horario),  1 (anti-horario)
    size_t dir;
    // numero de nos na arvore
    size_t num_nodes;

    // vetor que armazena todos os estados unicos da arvore
    std::vector<std::vector<size_t>> all_states;
    // funcao que compara se dois vetores sao iguais
    auto compare_vec(std::vector<size_t> &v1, std::vector<size_t> &v2) -> bool;
    // funcao que retorna se um estado ja esta na arvore
    auto is_in_tree(std::vector<size_t> &v) -> bool;

    // funcao auxiliar para desalocar os ponteiros
    auto _free(Node *n) -> Node *;

    // funcao que retorna se o estdo e o estado final
    auto final_state(std::vector<size_t> &state) -> bool;
    // funcao que faz o movimento em um estado para criar um novo
    auto make_move(std::vector<size_t> &state, size_t pos) -> std::pair<std::vector<size_t>, size_t>;
    // funcao que cria um novo no na arvore
    // estado atual, posicao atual das pecas, no pai, regra de transicao
    auto create_Node(std::vector<size_t> &state, std::vector<size_t> &positions, Node *p, size_t r) -> Node *;
    // deleta um no da arvore
    auto delete_Node(Node *p) -> void;

    // imprime o caminho de solucao
    auto print_path(Node *p) -> void;

    auto dls_aux(Node *N, size_t limit, size_t poda) -> int;
    auto print_abertos(std::queue<Node *> abertos) -> void;

    // movimentos de transicao
    // 0 (horario),  1 (anti-horario)
    moves indexes[2] = {
        {[](size_t idx)
         { return ((idx + 3) % 8 + 8) % 8; }},
        {[](size_t idx)
         { return ((idx - 3) % 8 + 8) % 8; }}};

public:
    Tree(size_t dir);
    ~Tree();

    auto get_num_nodes() -> size_t;
    auto backtracking(size_t limit) -> void;
    auto bfs() -> void;
    auto dls(size_t limit) -> int;
};

#endif // TREE_H