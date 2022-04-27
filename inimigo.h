#include "allegro.h"

#ifndef __INI__
#define __INI__

//Estrutura de inimigos
typedef struct inimigo {
    int x, y;
    bool vivo;
    int sprite_x, sprite_y;
    int sprite_cd;
} inimigo_t;

//Aloca memoria para estrutura e a retorna
inimigo_t * inicia_inimigo(int sprite_x, int sprite_y);

//Desenha o inimigo na tela
void atualiza_inimigo(inimigo_t * player, ALLEGRO_BITMAP * sprites);

//Atualiza as coordenadas do inimigo
void move_inimigo(inimigo_t * inimigo, int x, int y);

#endif
