#include "allegro.h"

#ifndef __PLAY__
#define __PLAY__

typedef struct player {
    //Coordenadas em que o player se encontra
    int x, y;

    //Quantidade de cristais e a pontuacao do player
    int cristais, score;

    //True se o player estiver vivo
    bool vivo;
    
    //Coordenadas da sprite na imagem de sprites
    int sprite_x, sprite_y;

    //Tempo para trocar entre sprites
    int sprite_cooldown;

} player_t;

//Aloca memoria para o player, define sua posicao inicial, e retorna seu endereco
player_t * inicia_player();

//Desaloca memoria utilizada
void destroi_player(player_t * player);

//Desenha o player na tela e atualiza a sprite
void atualiza_player(player_t * player, ALLEGRO_BITMAP * sprites);

//Muda a posição do player
void muda_pos(player_t * player, int x, int y);

//"Mata" o player
void morte(player_t * player);

//Reseta os dados do player
int reset_player(player_t * player);

#endif