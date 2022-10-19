#include "Tree.h"

auto print_vector(std::vector<size_t> &v) -> void
{
    std::cout << "{ ";
    for (size_t n : v)
        std::cout << n << " ";
    std::cout << "}\n";
}

Tree::Tree(size_t dir)
{
    this->dir = dir;
    this->num_nodes = 1;

    std::vector<size_t> init_state = {1, 0, 2, 0, 3, 0, 4, 0};
    std::vector<size_t> positions = {0, 2, 4, 6};
    Node *new_node = new Node(init_state, positions, 4, NULL);
    initial_state = new_node;

    all_states.push_back(initial_state->get_state());
}

Tree::~Tree()
{
    _free(initial_state);
}

auto Tree::_free(Node *n) -> Node *
{
    if (n != NULL)
    {
        n->set_children(_free(n->get_children(0)), 0);
        n->set_children(_free(n->get_children(1)), 1);
        n->set_children(_free(n->get_children(2)), 2);
        n->set_children(_free(n->get_children(3)), 3);
        delete n;
        n = NULL;
    }

    return NULL;
}

auto Tree::final_state(std::vector<size_t> &state) -> bool
{
    if (state[0] == 3 && state[2] == 4 &&
        state[4] == 1 && state[6] == 2)
        return true;
    return false;
}

auto Tree::make_move(std::vector<size_t> &state, size_t pos) -> std::pair<std::vector<size_t>, size_t>
{
    std::vector<size_t> new_state = state;
    size_t idx = indexes[dir].move(pos);
    if (new_state[idx] != 0)
        return std::make_pair(std::vector<size_t>(), size_t(0));
    else
        std::swap(new_state[pos], new_state[idx]);
    return std::make_pair(new_state, idx);
}

auto Tree::create_Node(std::vector<size_t> &state, std::vector<size_t> &positions, Node *p, size_t r) -> Node *
{
    std::pair<std::vector<size_t>, size_t> temp = make_move(state, positions[r]);
    if (temp.first.empty())
        return NULL;
    else
    {
        num_nodes++;

        std::vector<size_t> pos = positions;
        std::vector<size_t> s = temp.first;
        pos[r] = temp.second;
        Node *new_node = new Node(s, pos, r, p);

        return new_node;
    }
}

auto Tree::delete_Node(Node *p) -> void
{
    num_nodes--;

    Node *parent = p->get_parent();
    unsigned r = parent->get_rule();
    parent->set_children(NULL, r);
    delete p;
    return;
}

auto Tree::compare_vec(std::vector<size_t> &v1, std::vector<size_t> &v2) -> bool
{
    for (int i = 0; i < v1.size(); ++i)
        if (v1[i] != v2[i])
            return false;
    return true;
}

auto Tree::is_in_tree(std::vector<size_t> &v) -> bool
{
    for (std::vector<size_t> &vec : all_states)
        if (compare_vec(v, vec))
            return true;
    return false;
}

auto Tree::get_num_nodes() -> size_t { return num_nodes; }

auto Tree::print_path(Node *p) -> void
{
    size_t level = 0;
    std::vector<size_t> state;
    while (p->get_parent() != NULL)
    {
        level++;
        state = p->get_state();
        std::cout << "Regra: R" << p->get_rule() + 1 << "\n";
        print_vector(state);
        p = p->get_parent();
    }

    std::cout << "\nNivel da solucao: " << level << "\n\n";
}

auto Tree::backtracking(size_t p) -> void
{
    std::cout << "BUSCA BACKTRACKING:\n";

    size_t level_backtracking = 0;
    size_t num_nodes = 0;
    Node *N = initial_state;
    bool failure = false;
    bool success = false;

    std::vector<size_t> aux_initial = initial_state->get_state();
    std::vector<size_t> aux_state;
    std::vector<size_t> aux_positions;

    while (!(failure || success))
    {
        if (level_backtracking >= p)
        {
            Node *p = N->get_parent();
            delete_Node(N);
            N = p;

            level_backtracking--;
        }

        if (!(N->queue_empty()))
        {
            size_t r = N->queue_front();
            N->queue_pop();

            aux_state = N->get_state();
            aux_positions = N->get_positions();
            Node *new_node = create_Node(aux_state, aux_positions, N, r);

            if (new_node == NULL)
                continue;
            else
            {
                aux_state = new_node->get_state();
                if (is_in_tree(aux_state))
                {
                    num_nodes++;
                    delete_Node(new_node);
                    continue;
                }
                else
                {
                    level_backtracking++;
                    num_nodes++;
                    all_states.push_back(aux_state);

                    N->set_children(new_node, 0);
                    N = new_node;

                    aux_state = N->get_state();
                    if (final_state(aux_state))
                        success = true;
                }
            }
        }
        else
        {
            aux_state = N->get_state();
            if (compare_vec(aux_initial, aux_state))
                failure = true;
            else
            {
                Node *p = N->get_parent();
                delete_Node(N);
                N = p;

                level_backtracking--;
            }
        }
    }

    std::cout << "\nStatus: ";
    if (failure)
        std::cout << "Falhou\n";
    else if (success)
        std::cout << "Sucesso\n";

    std::cout << "\n";
    std::cout << "Nos criados: " << num_nodes << "\n\n";

    if (success)
        print_path(N);

    return;
}

auto Tree::bfs() -> void
{
    std::cout << "BUSCA EM LARGURA:\n";

    std::vector<size_t> aux_state, aux_positions;
    size_t num_nodes, poda;
    num_nodes = poda = 0;

    std::queue<Node *> abertos, fechados;
    bool sucesso, fracasso;
    sucesso = fracasso = false;
    Node *N;
    Node *S = initial_state;
    abertos.push(S);

    while (!(sucesso || fracasso))
    {
        if (abertos.empty())
            fracasso = true;
        else
        {
            N = abertos.front();
            abertos.pop();
            aux_state = N->get_state();
            if (final_state(aux_state))
                sucesso = true;
            else
            {
                while (!N->queue_empty())
                {
                    size_t r = N->queue_front();
                    N->queue_pop();

                    aux_state = N->get_state();
                    aux_positions = N->get_positions();
                    Node *new_node = create_Node(aux_state, aux_positions, N, r);

                    if (new_node == NULL)
                        continue;
                    else
                    {
                        aux_state = new_node->get_state();
                        if (is_in_tree(aux_state))
                        {
                            poda++;
                            delete_Node(new_node);
                            continue;
                        }
                        else
                        {
                            num_nodes++;

                            all_states.push_back(aux_state);

                            N->set_children(new_node, r);
                            abertos.push(new_node);
                        }
                    }
                }
                fechados.push(N);
            }
        }
    }

    std::cout << "\nStatus: ";
    if (fracasso)
        std::cout << "Falhou\n";
    else if (sucesso)
        std::cout << "Sucesso\n";

    std::cout << "\n";
    std::cout << "Nos na arvore: " << num_nodes << "\n";
    std::cout << "Nos podados: " << poda << "\n";
    std::cout << "Nos criados: " << num_nodes + poda << "\n\n";

    if (sucesso)
        print_path(N);

    return;
}
