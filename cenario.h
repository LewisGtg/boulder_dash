#include "allegro.h"
#include "player.h"

#ifndef __CEN__
#define __CEN__

typedef struct cenario {
    int lin, col;
    char ** mapa;

    int pontos;
    int tempo;

    int posX_player, posY_player;
} cenario_t;

//Aloca memoria para o cenario e retorna o endereco
cenario_t * inicia_cenario();

//Carrega sprites do cenario
void atualiza_cenario(cenario_t * cenario, ALLEGRO_BITMAP * sprites);

//Verifica se a posicao pode ser acessada (nao possui pedra nem muro)
int pos_valida(char ** mapa, int x, int y);

//Verifica a gravidade de pedras e cristais
void verifica_gravidade(cenario_t * cenario, player_t * player);
void gravidade(char ** mapa, player_t * player, int i, int j, char obj, int min, int max);

//Aloca matriz para o mapa
char ** inicia_mapa(int lin, int col);

//Carrega as informações dos mapas
void carrega_cenario(cenario_t * cenario, char * arquivo_cenario);

//Atualiza a posicao do player na matriz
void movimenta_player(cenario_t * cenario, player_t * player);

#endif