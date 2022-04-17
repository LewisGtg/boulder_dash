#include "allegro.h"

#ifndef __PLAY__
#define __PLAY__

typedef struct player {
    int x;
    int y;
    bool vivo;
    
    int sprite_x;
    int sprite_y;

    int sprite_cooldown;

} player_t;

//Aloca memoria para o player, define sua posicao inicial, e retorna seu endereco
player_t * inicia_player(int x, int y);

//Desenha o player na tela e atualiza a sprite
void atualiza_player(player_t * player, ALLEGRO_BITMAP * sprites);

//"Mata" o player
void morte(player_t * player);

#endif