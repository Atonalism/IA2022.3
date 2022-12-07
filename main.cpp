#include "Tree.h"

int main()
{
    Tree backtracking(0);
    backtracking.backtracking(16);
    std::cout << "-----------------------------------------------------------------------\n";
    Tree busca_em_largura(0);
    busca_em_largura.bfs();
    std::cout << "-----------------------------------------------------------------------\n";
    Tree busca_limitada(0);
    int n = busca_limitada.dls(16);
    std::cout << "\nStatus: ";
    if (n == -1)
        std::cout << "Falhou\n";
    else if (n == 1)
        std::cout << "Sucesso\n";
    std::cout << "-----------------------------------------------------------------------\n";
    return 0;
}