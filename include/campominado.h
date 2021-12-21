#ifndef MATRIZ_H

#include <stdlib.h>
#include <iostream>
#include <time.h>

#define MATRIZ_H
#define VAZIO_ '?'
#define BOMBA_ '*'

using namespace std;

class Minado{
   public:
    ~Minado();
    void executa_jogo();
    Minado(int qdt_lin, int qdt_colunas, int qdt_bombas);
   private:
    int qdt_lin;
    int qdt_col;
    int *pos_bombas;
    int qdt_bombas_total;
    int qdt_insercoes_sucedidas;
    char **campo_minado;
    bool bomba_explodiu;
    
    void aloca_vetor_pos_bombas(int qdt_bombas);
    void aloca_campo_minado(int qdt_lin, int qdt_col);
    void abre_campos_sem_bombas_ao_redor(int lin, int col);

    int busca_binaria(int alvo, int ini, int fim);

    bool campo_tem_bomba(int lin, int col);
    bool campo_ja_inserido(int lin, int col);

    bool existe_bomba(int tam, int valor);
    bool esta_dentro_campo_minado(int lin, int col);
    
    bool ganhou();
    void ganhou_partida();
    bool get_bomba_explodiu();
    void gera_posicao_bombas();
    int get_bombas_ao_redor_do_campo(int lin, int col);
    
    void imagem_bomba();
    void insere_campo_minado(int lin, int col);
    void insere_todas_bombas_no_campo_minado();

    void leitura_entradas(int* lin, int* col);
     
    void ordena_vetor(int *vetor, int tam);

    void perdeu_partida();
    void print_campo_minado();
    
    void set_bomba_explodiu(bool bomba_explodiu);
    void set_campo_minado(int lin, int col, int qdt_bombas);
    
    void trofeu();
    int transforma_para_posicao(int lin, int col);
};
#endif