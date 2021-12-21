#include "campominado.h"
#include <string.h>

void joga(int tam_lin, int tam_col, int quant_bombas){
    Minado *cm = new Minado(tam_lin, tam_col, quant_bombas);
    cm->executa_jogo();
    delete cm;
}

int main(int argc, char const* argv[]){
    printf("\n");
    if(argc == 1){
        int opcao;
        printf("insira\n1 = [3x3] e 3 bombas\n2 = [10 x 10] e 30 bombas\n3 = [20 x 20] e 100 bombas\n\n");
        printf("opcao = ");
        cin>>opcao;
        if(opcao == 1) joga(3, 3, 3);
        else if(opcao == 2) joga(10, 10, 30);
        else joga(20, 20, 100);
    }else if(argc == 2 && strcmp(argv[1], "h") == 0)
        printf("entradas validas:\n./campominado\n./campominado <tamanho_linha = coluna> <quantidade de bombas>\n./campominado <tamanho_linha> <tamanho_coluna> <quantidade de bombas>\n\n");
    else if(argc == 3)
        joga(atoi(argv[1]), atoi(argv[1]), atoi(argv[2]));
    else if(argc == 4)
        joga(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]));
    else 
        printf("entradas validas:\n./campominado\n./campominado <tamanho_linha = coluna> <quantidade de bombas>\n./campominado <tamanho_linha> <tamanho_coluna> <quantidade de bombas>\n\n");
    system("pause");
}

