#include "allegro.h"

#ifndef __INI__
#define __INI__

//Estrutura de inimigos
typedef struct inimigo {
    //Representacao do inimigo em ascii
    int rep;

    //Direcao que o inimigo vai se movimentar
    int dir;
    
    //Coordenadas do inimigo
    int x, y;
    bool vivo;

    //Coordenadas da sprite, e o cooldown
    int sprite_x, sprite_y;
    int sprite_cd;
} inimigo_t;

//Aloca memoria para estrutura e a retorna
inimigo_t * inicia_inimigo(int rep, int sprite_x, int sprite_y);

//Desaloca memoria da estrutura
void destroi_inimigo(inimigo_t * inimigo);

//Desenha o inimigo na tela
void atualiza_inimigo(inimigo_t * player, ALLEGRO_BITMAP * sprites);

//Atualiza as coordenadas do inimigo
void move_inimigo(inimigo_t * inimigo, int x, int y);

//Mata o inimigo
void morte_inimigo(inimigo_t * inimigo);


#endif
