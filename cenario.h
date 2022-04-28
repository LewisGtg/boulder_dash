#include "allegro.h"
#include "player.h"
#include "inimigo.h"

#ifndef __CEN__
#define __CEN__

#define MAX_INIMIGOS 20

typedef struct cenario {
    int lin, col;
    char ** mapa;
    bool porta_aberta;

    int min_cristais;
    int fator_score;
    int tempo;

    inimigo_t ** inimigos;
    int qtd_inimigos;

    int posX_player, posY_player;
    int saida_x, saida_y;

    ALLEGRO_SAMPLE * som_pedra;
    ALLEGRO_SAMPLE * som_pedra_colidiu;
    ALLEGRO_SAMPLE * som_colidiu_inimigo;
    ALLEGRO_SAMPLE * som_gameover;
    ALLEGRO_SAMPLE * som_abriu_porta;
    ALLEGRO_SAMPLE * som_pegou_cristal;
    ALLEGRO_SAMPLE * som_abriu_instrucoes;
    ALLEGRO_SAMPLE * som_cheatcode;
} cenario_t;

//Aloca memoria para o cenario e retorna o endereco
cenario_t * inicia_cenario();

//Carrega sprites do cenario
void atualiza_cenario(cenario_t * cenario, ALLEGRO_BITMAP * sprites);

//Carrega e atualiza o painel de pontuação e tempo
void atualiza_painel(cenario_t * cenario, player_t * player, ALLEGRO_FONT * font);

//Transforma as gramas em volta do player em cristais
void cenario_cheatcode(cenario_t * cenario, player_t * player);

//Verifica se o objeto eh uma pedra
int eh_pedra(char obj);

//Verifica se a posicao pode ser acessada pelo player(nao possui pedra nem muro)
int pos_valida_player(char ** mapa, int x, int y);

//Verifica se a posicao pode ser acessada por algum inimigo
int pos_valida_inimigo(char ** mapa, int x, int y);

//Verifica se algum inimigo colidiu com o player
int colisao_player(cenario_t * cenario, player_t * player);

//Retorna 1 se o player conseguiu empurrar a pedra
int empurrou_pedra(char ** mapa, int dir, int x, int y);

//Verifica a gravidade de pedras e cristais
void verifica_gravidade(cenario_t * cenario, player_t * player);
void gravidade(cenario_t * cenario, player_t * player, int i, int j, char obj, int min, int max);

//Aloca matriz para o mapa
char ** inicia_mapa(int lin, int col);

//Procura posicoes validas para os inimigos e os movimenta
void procura_pos_inimigos(cenario_t * cenario);

//Atualiza a posicao do player no mapa
void movimenta_player(cenario_t * cenario, player_t * player);

//Atualiza a posicao do inimigo no mapa
void movimenta_inimigo(cenario_t * cenario, inimigo_t * inimigo, int new_x, int new_y);

//Verifica se o player pegou um cristal
void verifica_ponto(cenario_t * cenario, player_t * player);

//Abre a porta caso o player tenha pegado os cristais necessarios
void abre_porta(cenario_t * cenario, player_t *player);

//Verifica se o player passou de fase
int passou_fase(cenario_t * cenario, player_t * player);

//Verifica se o tempo acabou
int tempo_acabou(cenario_t * cenario);

//Limpa o vetor de inimigos
void destroi_inimigos(cenario_t * cenario);

#endif