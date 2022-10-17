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
        std::vector<size_t> pos = positions;
        std::vector<size_t> s = temp.first;
        pos[r] = temp.second;
        Node *new_node = new Node(s, pos, r, p);

        return new_node;
    }
}

auto Tree::delete_Node(Node *p) -> void
{
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

auto Tree::print_path(Node *p) -> void
{
    std::vector<size_t> state;
    while (p->get_parent() != NULL)
    {
        state = p->get_state();
        std::cout << "Rule: R" << p->get_rule() + 1 << "\n";
        print_vector(state);
        std::cout << "\n";
        p = p->get_parent();
    }
}

auto Tree::backtracking(size_t p) -> void
{
    size_t level_backtracking = 0;
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
                    delete new_node;
                    continue;
                }
                else
                {
                    level_backtracking++;
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

    std::cout << "\n";
    if (failure)
        std::cout << "Falhou\n";
    else if (success)
        std::cout << "Sucesso!\n";

    std::cout << "Nivel = " << level_backtracking << "\n\n";

    if(success)
        print_path(N);

    return;
}
