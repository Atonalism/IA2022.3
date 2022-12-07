#include "Tree.h"

// funcao auxiliar para imprimir um vetor
auto print_vector(std::vector<size_t> &v) -> void
{
    std::cout << "{ ";
    for (size_t n : v)
        std::cout << n << " ";
    std::cout << "}\n";
}

// construtor da arvore que recebe a direcao dos movimentos
Tree::Tree(size_t dir)
{
    this->dir = dir;
    this->num_nodes = 1;

    // estado inicial
    // 1: primeiro cavalo preto; 2: segundo cavalo preto;
    // 3: primeiro cavalo branco; 4: segundo cavalo branco;
    // 0: posicao vazia
    std::vector<size_t> init_state = {1, 0, 2, 0, 3, 0, 4, 0};
    // posicoes iniciais
    std::vector<size_t> positions = {0, 2, 4, 6};
    // no que contem o estado inicial
    Node *new_node = new Node(init_state, positions, 4, NULL);
    initial_state = new_node;

    all_states.push_back(initial_state->get_state());
}

// destrutor da arvore
Tree::~Tree()
{
    _free(initial_state);
}

// funcao auxiliar para o destrutor
// recursivamente desaloca todos os nos da arvore
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

// recebe um estado e retorna verdadeiro se ele for o estado final
// falso caso contrario
auto Tree::final_state(std::vector<size_t> &state) -> bool
{
    if (state[0] == 3 && state[2] == 4 &&
        state[4] == 1 && state[6] == 2)
        return true;
    return false;
}

// funcao que faz o movimento da peca
// recebe o estado que vai receber o movimento e a posicao da peca que sera movimentada
// retorna um par com o novo estado e a nova posicao da peca
auto Tree::make_move(std::vector<size_t> &state, size_t pos) -> std::pair<std::vector<size_t>, size_t>
{
    std::vector<size_t> new_state = state;
    // indice da peca apos a movimentacao
    size_t idx = indexes[dir].move(pos);
    // se a nova posicao da peca for diferente de vazio(0)
    // movimento invalido, entao retorna par vetor vazio e 0
    if (new_state[idx] != 0)
        return std::make_pair(std::vector<size_t>(), size_t(0));
    else
        std::swap(new_state[pos], new_state[idx]); // caso contrario, movimento valido, trocamos as posicoes e o movimento e feito
    return std::make_pair(new_state, idx);         // retorna o par novo estado e nova posicao
}

// funcao que cria um novo no, ou seja, um novo estado na arvore
auto Tree::create_Node(std::vector<size_t> &state, std::vector<size_t> &positions, Node *p, size_t r) -> Node *
{
    // fazemos o movimento na regra r
    std::pair<std::vector<size_t>, size_t> temp = make_move(state, positions[r]);
    // se o vetor retorna for vazio entao o movimento com a regra r e invalido
    // retorna nulo
    if (temp.first.empty())
        return NULL;
    else
    {
        // caso contrario, a regra r faz um movimento valido e um novo no e criado
        num_nodes++;

        std::vector<size_t> pos = positions;
        std::vector<size_t> s = temp.first;
        pos[r] = temp.second;
        Node *new_node = new Node(s, pos, r, p);

        return new_node;
    }
}

// funcao que deleta um no
auto Tree::delete_Node(Node *p) -> void
{
    num_nodes--;

    Node *parent = p->get_parent();
    unsigned r = parent->get_rule();
    parent->set_children(NULL, r);
    delete p;
    return;
}

// funcao que compara dois vetores
auto Tree::compare_vec(std::vector<size_t> &v1, std::vector<size_t> &v2) -> bool
{
    for (int i = 0; i < v1.size(); ++i)
        if (v1[i] != v2[i])
            return false;
    return true;
}

// funcao que verifica se um estado esta na arvore
// usando o vetor aulixar com todos os estados da arvore
auto Tree::is_in_tree(std::vector<size_t> &v) -> bool
{
    for (std::vector<size_t> &vec : all_states)
        if (compare_vec(v, vec))
            return true;
    return false;
}

// retorna o numero de nos na arvore
auto Tree::get_num_nodes() -> size_t { return num_nodes; }

// imprime o caminho de solucao do estado objetivo ate a raiz
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

// busca backtracking
// p e o limite de profundidade
auto Tree::backtracking(size_t limit) -> void
{
    std::cout << "BUSCA BACKTRACKING:\n";

    // nivel da busca
    size_t level_backtracking = 0;
    // nos deletados
    size_t nos_deletados = 0;
    // estado inicial
    Node *N = initial_state;
    bool failure = false;
    bool success = false;

    std::vector<size_t> aux_initial = initial_state->get_state();
    std::vector<size_t> aux_state;
    std::vector<size_t> aux_positions;

    // enquanto a busca e feita
    while (!(failure || success))
    {
        // se o nivel for maior que o limite -> impasse
        if (level_backtracking >= limit)
        {
            // retornamos ao no anterior (pai) e deletamos o no impasse
            Node *p = N->get_parent();
            nos_deletados++;
            delete_Node(N);
            N = p;

            // o nivel diminui
            level_backtracking--;
        }

        // se ainda existem regras aplicaveis no estado do no atual
        if (!(N->queue_empty()))
        {
            // selecionamos a regra
            size_t r = N->queue_front();
            // tiramos ela da fila
            N->queue_pop();

            // fazemos o movimento da regra
            aux_state = N->get_state();
            aux_positions = N->get_positions();
            Node *new_node = create_Node(aux_state, aux_positions, N, r);

            // se o novo no e nulo, entao o movimento da regra r e invalido
            if (new_node == NULL)
                continue;
            else
            {
                // se o moviemnto e valido, mas ja existe na arvore
                aux_state = new_node->get_state();
                if (is_in_tree(aux_state))
                {
                    // entao o no e deletado e nao e colocado na arvore
                    nos_deletados++;
                    delete_Node(new_node);
                    continue;
                }
                else
                {
                    // se o movimento e valido e nao existe na arvore
                    // entao um novo no e criado na arvore
                    level_backtracking++;
                    all_states.push_back(aux_state);

                    N->set_children(new_node, 0);
                    N = new_node;

                    aux_state = N->get_state();
                    // se o estado atual e o final, entao sucesso
                    if (final_state(aux_state))
                        success = true;
                }
            }
        }
        else
        {
            // se o no atual nao possui mais regras
            // se o no atual voltou para o estado inicial, entao fracasso
            aux_state = N->get_state();
            if (compare_vec(aux_initial, aux_state))
                failure = true;
            else
            {
                // caso contrario, retornamos ao no anterior (pai) e deletamos o no impasse
                Node *p = N->get_parent();
                nos_deletados++;
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
    std::cout << "Nos na arvore: " << num_nodes << "\n";
    std::cout << "Nos deletados: " << nos_deletados << "\n";
    std::cout << "Nos criados: " << num_nodes + nos_deletados << "\n\n";

    if (success)
        print_path(N);

    return;
}

// funcao auxiliar para imprimir a fila de abertos
auto Tree::print_abertos(std::queue<Node *> abertos) -> void
{
    std::vector<size_t> state;
    std::cout << "Fila de abertos = { ";
    while (!abertos.empty())
    {
        state = abertos.front()->get_state();
        std::cout << "( ";
        for (size_t n : state)
            std::cout << n << " ";
        std::cout << ") ";
        abertos.pop();
    }
    std::cout << "}\n";
}

auto Tree::bfs() -> void
{
    std::cout << "BUSCA EM LARGURA:\n";

    std::vector<size_t> aux_state, aux_positions;
    // variavel que armazena o numero de podas realizados
    size_t poda = 0;
    // variavel que armazena o numero de iteracoes que a fila de abertos sera printada
    size_t iter = 0;

    // fila de abertos e fechados
    std::queue<Node *> abertos, fechados;
    bool sucesso, fracasso;
    sucesso = fracasso = false;
    Node *N;
    Node *S = initial_state;
    // inicia a fila de abertos com o estado inicial
    abertos.push(S);

    while (!(sucesso || fracasso))
    {
        if (abertos.empty())
            fracasso = true;
        else
        {
            if (iter < 16)
                print_abertos(abertos);

            // pega o primeiro elemento da fila
            N = abertos.front();
            abertos.pop();
            aux_state = N->get_state();
            // verifica se estamos no estado final
            if (final_state(aux_state))
                sucesso = true;
            else
            {
                // enquanto o estado atual tiver regras validas para gerar filhos
                while (!N->queue_empty())
                {
                    size_t r = N->queue_front();
                    N->queue_pop();

                    aux_state = N->get_state();
                    aux_positions = N->get_positions();
                    // criamos um novo no com a regra r atual
                    Node *new_node = create_Node(aux_state, aux_positions, N, r);

                    // se o novo no criado for NULL entao a regra e invalida
                    if (new_node == NULL)
                        continue;
                    else
                    {
                        // se o no ja estiver na arvore ele e podado
                        aux_state = new_node->get_state();
                        if (is_in_tree(aux_state))
                        {
                            poda++;
                            delete_Node(new_node);
                            continue;
                        }
                        else
                        {
                            // se nao tiver na arvore o novo no e inserido na arvore e na fila de abertos
                            all_states.push_back(aux_state);

                            N->set_children(new_node, r);
                            abertos.push(new_node);
                        }
                    }
                }
                // quando o no nao possui mais regras ele e colocado na fila de fechados
                fechados.push(N);
            }
        }
        iter++;
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

auto Tree::dls(size_t limit) -> int
{
    std::cout << "BUSCA EM PROFUNDIDADE:\n";
    return dls_aux(initial_state, limit, 0);
}

// funcao recursiva para a busca em profundidade limitada
auto Tree::dls_aux(Node *N, size_t limit, size_t poda) -> int
{
    std::vector<size_t> aux_state, aux_positions;
    // o novo no e colocado na fila de abertos
    std::queue<Node *> abertos;
    abertos.push(N);

    aux_state = N->get_state();

    // se o estado atual e o final, entao sucesso
    if (final_state(aux_state))
    {
        print_abertos(abertos);
        print_path(N);

        std::cout << "\n";
        std::cout << "Nos na arvore: " << num_nodes << "\n";
        std::cout << "Nos podados: " << poda << "\n";
        std::cout << "Nos criados: " << num_nodes + poda << "\n\n";
        return 1;
    }

    // enquanto o no atual tiver regras
    while (!N->queue_empty())
    {
        size_t r = N->queue_front();
        N->queue_pop();

        aux_state = N->get_state();
        aux_positions = N->get_positions();
        // um novo no com o estado gerado pela regra r e criado
        Node *new_node = create_Node(aux_state, aux_positions, N, r);

        // se o novo no for nulo entao a regra r e invalida
        if (new_node == NULL)
            continue;
        else
        {
            aux_state = new_node->get_state();
            // se o novo no criado ja estiver na arvore ele e podado
            if (is_in_tree(aux_state))
            {
                delete_Node(new_node);
                poda++;
                continue;
            }
            else
            {
                all_states.push_back(aux_state);
                // o novo no e inserido na arvore e na fila de abertos
                N->set_children(new_node, r);
                abertos.push(new_node);
            }
        }

        // se a arvore ainda estiver no limite de nivel
        if (limit > 0)
        {
            // continuamos a busca no novo no
            if (dls_aux(new_node, limit - 1, poda) != -1)
                return 1;
        }
    }

    print_abertos(abertos);

    return -1;
}