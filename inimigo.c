#include "inimigo.h"

inimigo_t * inicia_inimigo(int rep, int sprite_x, int sprite_y)
{
    //Aloca memoria e testa
    inimigo_t * inimigo = malloc(sizeof(inimigo_t));

    if (!inimigo)
    {
        perror("Erro ao alocar inimigo.");
        exit(1);
    }

    //Inicializa estruturas do inimigo
    inimigo->rep = rep;
    inimigo->dir = 0;

    inimigo->sprite_x = sprite_x;
    inimigo->sprite_y = sprite_y;
    inimigo->sprite_cd = 0;

    inimigo->vivo = true;

    //Retorna a estrutura
    return inimigo;
}

void atualiza_inimigo(inimigo_t * inimigo, ALLEGRO_BITMAP * sprites)
{
    if (inimigo->sprite_cd == 5)
    {
        if (inimigo->sprite_y < 7)
            inimigo->sprite_y++;
        else
            inimigo->sprite_y = 4;

        inimigo->sprite_cd = 0;
    }

    inimigo->sprite_cd++;

    if (inimigo->vivo)
        al_draw_scaled_bitmap(sprites, inimigo->sprite_x * 16, inimigo->sprite_y * 16, 16, 16, inimigo->x * 32, (inimigo->y + 1) * 31.3, 32, 32, 0);
}

void move_inimigo(inimigo_t * inimigo, int x, int y)
{
    inimigo->x = x;
    inimigo->y = y;
}

void morte_inimigo(inimigo_t * inimigo)
{
    inimigo->vivo = false;
    inimigo->rep = ' ';
}