#include "allegro.h"
#include "player.h"

#ifndef __CEN__
#define __CEN__

typedef struct cenario {
    int lin, col;
    char ** mapa;

    int min_cristais;
    int fator_score;
    int tempo;

    int posX_player, posY_player;
    int saida_x, saida_y;
} cenario_t;

//Aloca memoria para o cenario e retorna o endereco
cenario_t * inicia_cenario();

//Carrega sprites do cenario
void atualiza_cenario(cenario_t * cenario, ALLEGRO_BITMAP * sprites);

//Carrega e atualiza o painel de pontuação e tempo
void atualiza_painel(cenario_t * cenario, player_t * player, ALLEGRO_FONT * font);

//Verifica se o objeto eh uma pedra
int eh_pedra(char obj);

//Verifica se a posicao pode ser acessada (nao possui pedra nem muro)
int pos_valida(char ** mapa, int x, int y);

//Retorna 1 se o player conseguiu empurrar a pedra
int empurrou_pedra(char ** mapa, int dir, int x, int y);

//Verifica a gravidade de pedras e cristais
void verifica_gravidade(cenario_t * cenario, player_t * player);
void gravidade(char ** mapa, player_t * player, int i, int j, char obj, int min, int max);

//Aloca matriz para o mapa
char ** inicia_mapa(int lin, int col);

//Carrega as informações dos mapas
void carrega_cenario(cenario_t * cenario, char * arquivo_cenario);

//Atualiza a posicao do player na matriz
void movimenta_player(cenario_t * cenario, player_t * player);

//Verifica se o player pegou um cristal
void verifica_ponto(cenario_t * cenario, player_t * player);

//Abre a porta caso o player tenha pegado os cristais necessarios
void abre_porta(cenario_t * cenario, player_t *player);

//Verifica se o player passou de fase
int passou_fase(cenario_t * cenario, player_t * player);

//Verifica se o tempo acabou
int tempo_acabou(cenario_t * cenario);

#endif