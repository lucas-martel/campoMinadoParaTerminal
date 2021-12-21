#include "campominado.h"

#pragma region publico
Minado::Minado(int qdt_lin, int qdt_col, int qdt_bombas_total){
    this->qdt_lin = qdt_lin;
    this->qdt_col = qdt_col;
    this->qdt_bombas_total = qdt_bombas_total;
    this->qdt_insercoes_sucedidas = 0;
    set_bomba_explodiu(false);
    aloca_campo_minado(qdt_lin, qdt_col);
    aloca_vetor_pos_bombas(this->qdt_bombas_total);
    gera_posicao_bombas();
}

Minado::~Minado(){}

void Minado::executa_jogo(){
    printf("INICIO PARTIDA******************\n");
    int lin, col;
    while(!this->bomba_explodiu){
        print_campo_minado();
        leitura_entradas(&lin, &col);
        insere_campo_minado(lin, col);
        if(get_bomba_explodiu())
            perdeu_partida();
        else if(ganhou())
            ganhou_partida();
    }
}
#pragma endregion
#pragma region privado
void Minado:: aloca_vetor_pos_bombas(int qdt_bombas){
    if(qdt_bombas == 0) exit(0);
    this->pos_bombas = (int *) malloc(sizeof(int) * qdt_bombas);
    if(!this->pos_bombas) exit(0);
}

void Minado:: aloca_campo_minado(int qdt_lin, int qdt_col){
    this->campo_minado = (char**) malloc(sizeof(char*) * qdt_lin);
    if(!this->campo_minado) exit(0);
    for(int i = 0; i < qdt_lin; i++){
        this->campo_minado[i] = (char *) malloc(sizeof(char) * qdt_col);
        for(int j = 0; j < qdt_col; j++) this->campo_minado[i][j] = VAZIO_;
    }
}

void Minado::abre_campos_sem_bombas_ao_redor(int lin, int col){
    if(!esta_dentro_campo_minado(lin, col) || campo_ja_inserido(lin, col)) return;
    int qdt_b = get_bombas_ao_redor_do_campo(lin, col);
    set_campo_minado(lin, col, qdt_b);
    if(qdt_b != 0) return; 
    if(!campo_tem_bomba(lin - 1, col)) 
        abre_campos_sem_bombas_ao_redor(lin - 1, col);
    if(!campo_tem_bomba(lin + 1, col))
        abre_campos_sem_bombas_ao_redor(lin + 1, col);
    if(!campo_tem_bomba(lin, col - 1))
        abre_campos_sem_bombas_ao_redor(lin, col - 1);
    if(!campo_tem_bomba(lin, col + 1))
        abre_campos_sem_bombas_ao_redor(lin, col + 1);
    if(!campo_tem_bomba(lin, col + 1))
        abre_campos_sem_bombas_ao_redor(lin - 1, col - 1);
    if(!campo_tem_bomba(lin, col + 1))
        abre_campos_sem_bombas_ao_redor(lin + 1, col - 1);
    if(!campo_tem_bomba(lin, col + 1))
        abre_campos_sem_bombas_ao_redor(lin - 1, col + 1);
    if(!campo_tem_bomba(lin, col + 1))
        abre_campos_sem_bombas_ao_redor(lin + 1, col + 1);
}

int Minado::busca_binaria(int alvo, int ini, int fim){
    if(ini > fim) return -1;
    int meio = (ini + fim) / 2;
    if(this->pos_bombas[meio] < alvo) return busca_binaria(alvo, meio+1, fim);
    else if(this->pos_bombas[meio] > alvo) return busca_binaria(alvo, ini, meio-1);
    else return meio;
}

bool Minado::campo_tem_bomba(int lin, int col){
    int pos = transforma_para_posicao(lin, col);
    if(busca_binaria(pos, 0, this->qdt_bombas_total -1) == -1)
        return false;
    return true;
}

bool Minado::campo_ja_inserido(int lin, int col){
    if(this->campo_minado[lin][col] != VAZIO_) return true;
    return false;
}

bool Minado:: existe_bomba(int tam, int valor){
    for(int i = 0; i < tam; i++)
        if(pos_bombas[i] == valor) return true;
    return false;
}

bool Minado::esta_dentro_campo_minado(int lin, int col){
    if(lin < 0 || col < 0 || lin >= this->qdt_lin || col >= this->qdt_col)
        return false;
    return true;
}

bool Minado::ganhou(){
    if((this->qdt_lin * this->qdt_col) - this->qdt_insercoes_sucedidas == qdt_bombas_total)
        return true;
    return false;
}

void Minado::ganhou_partida(){
    insere_todas_bombas_no_campo_minado();
    print_campo_minado();
    printf("PARABENS, VOCE NAO EXPLODIU NENHUMA *\n");
    trofeu();
    set_bomba_explodiu(true);
}

bool Minado::get_bomba_explodiu(){
    return this->bomba_explodiu;
}

void Minado:: gera_posicao_bombas(){
    if(!pos_bombas) exit(0);
    srand(time(NULL));
    int aux;
    int limite = qdt_lin * qdt_col;
    for(int i = 0; i < qdt_bombas_total; i++){
        aux = rand() % limite;
        while (busca_binaria(aux, 0, i) != -1)
            aux = rand() % limite;
        pos_bombas[i] = aux;
        ordena_vetor(pos_bombas, i + 1);
    }
}

int Minado::get_bombas_ao_redor_do_campo(int lin, int col){
    int qdt_bombas_ao_redor = 0;
    int lin_inc = lin + 1;
    int lin_dec = lin - 1;
    int col_inc = col + 1;
    int col_dec = col - 1;
    if(esta_dentro_campo_minado(lin_dec, col) && campo_tem_bomba(lin_dec, col))
        qdt_bombas_ao_redor++; // cima
    if(esta_dentro_campo_minado(lin_inc, col) && campo_tem_bomba(lin_inc, col))
        qdt_bombas_ao_redor++; // baixo
    if(esta_dentro_campo_minado(lin, col_inc) && campo_tem_bomba(lin, col_inc))
        qdt_bombas_ao_redor++; // direita
    if(esta_dentro_campo_minado(lin, col_dec) && campo_tem_bomba(lin, col_dec))
        qdt_bombas_ao_redor++; // esquerda
    if(esta_dentro_campo_minado(lin_dec, col_dec) && campo_tem_bomba(lin_dec, col_dec))
        qdt_bombas_ao_redor++; // diagonal esquerda superior
    if(esta_dentro_campo_minado(lin_inc, col_dec) && campo_tem_bomba(lin_inc, col_dec))
        qdt_bombas_ao_redor++; // diagonal esquerda inferior
    if(esta_dentro_campo_minado(lin_dec, col_inc) && campo_tem_bomba(lin_dec, col_inc))
        qdt_bombas_ao_redor++; // diagonal direita superior
    if(esta_dentro_campo_minado(lin_inc, col_inc) && campo_tem_bomba(lin_inc, col_inc))
        qdt_bombas_ao_redor++; // diagonal direita inferior
    return qdt_bombas_ao_redor;
}

void Minado::imagem_bomba(){
    printf("\n      *    ");
    printf("\n     *     ");
    printf("\n    ***   ");
    printf("\n  *******  ");
    printf("\n  *******  ");
    printf("\n  *******  ");
    printf("\n    ***   ");
    printf("\n");
}

void Minado::insere_campo_minado(int lin, int col){
    if(campo_tem_bomba(lin, col))
         set_bomba_explodiu(true);
    else
        abre_campos_sem_bombas_ao_redor(lin, col);
}

void Minado::insere_todas_bombas_no_campo_minado(){
    for(int i = 0; i < this->qdt_lin; i++)
        for(int j = 0; j < this->qdt_col; j++)
            if(campo_tem_bomba(i, j))
                set_campo_minado(i, j, -1);        
}

void Minado::leitura_entradas(int* lin, int* col){
    bool leia = true;
    while(leia){
        printf("linha e coluna: ");
        cin>>*lin>>*col;
        *lin -= 1;
        *col -= 1;
        if(esta_dentro_campo_minado(*lin, *col) && !campo_ja_inserido(*lin, *col))
            leia = false;
        else printf("posicao invalida, tente novamente\n");
    }
}

void Minado:: ordena_vetor(int *vetor, int tam){
    for(int i = 0; i < tam; i++)
        for(int j = i+1; j < tam; j++)
            if(vetor[i] > vetor[j]){
                int aux = vetor[i];
                vetor[i] = vetor[j];
                vetor[j] = aux;
            }
}

void Minado::perdeu_partida(){
    insere_todas_bombas_no_campo_minado();
    print_campo_minado();
    printf("infelizmente, voce explodiu uma * :(\n");
    imagem_bomba();
}

void Minado::print_campo_minado(){
   //system("cls");
    int tamanho = this->qdt_lin * this->qdt_col;
    int campos_sem_bomba = tamanho - qdt_bombas_total;
    printf("\ncampos[%d/%d]\n\nLinhas\n", this->qdt_insercoes_sucedidas, campos_sem_bomba);

    for(int i = 0; i < this->qdt_lin; i++){
        printf("%d-> ", i+1);
        if(i < 9) printf(" ");
        for(int j = 0; j < this->qdt_col; j++){
            printf("%c ", campo_minado[i][j]);
            if(j > 7) printf(" ");
        }
        printf("\n");
    }
    printf("    ");
    for(int i = 0; i < this->qdt_col; i++){
        if(i > 8) printf("  ");
        else printf(" ");
        printf("^");
    } 
        printf("\n     ");
    for(int i = 0; i < this->qdt_col; i++){
        printf("%d ", i+1);
    }
    printf("colunas\n\n");
}

void Minado:: set_bomba_explodiu(bool bomba_explodiu){
    this->bomba_explodiu = bomba_explodiu;
}

void Minado::set_campo_minado(int lin, int col, int qdt_bombas){
    if(qdt_bombas != -1){  
        this->campo_minado[lin][col] = qdt_bombas + '0';
        qdt_insercoes_sucedidas++;
    }
    else
        this->campo_minado[lin][col] = BOMBA_;
}

void Minado::trofeu(){
    printf("\n                *********               ");
    printf("\n              *************             ");
    printf("\n           *******************          ");
    printf("\n        ************************        ");
    printf("\n        ************************        ");
    printf("\n********* ******************** *********");
    printf("\n********* ******************** *********");
    printf("\n***       ********    ********       ***");
    printf("\n***       ******   **   ******       ***");
    printf("\n ***      *****  ****    *****      *** ");
    printf("\n  ***     ****     **     ****     ***  ");
    printf("\n   ***    ****     **     ****    ***   ");
    printf("\n   ***   *****     **     *****  ***    ");
    printf("\n     ***  *****    **    *****  ***     ");
    printf("\n      *** *******      ******* ***      ");
    printf("\n       ** ******************** **       ");
    printf("\n        * ******************** **       ");
    printf("\n          *******************           ");
    printf("\n           ******************           ");
    printf("\n             **************             ");
    printf("\n               ***********              ");
    printf("\n                *********               ");
    printf("\n              *************             ");
    printf("\n");
}

int Minado::transforma_para_posicao(int lin, int col){
    return (lin * this->qdt_col) + col;
}
#pragma endregion