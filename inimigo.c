#include "inimigo.h"

inimigo_t * inicia_inimigo(int sprite_x, int sprite_y)
{
    inimigo_t * inimigo = malloc(sizeof(inimigo_t));

    if (!inimigo)
    {
        perror("Erro ao alocar inimigo.");
        exit(1);
    }

    inimigo->sprite_x = sprite_x;
    inimigo->sprite_y = sprite_y;
    inimigo->sprite_cd = 100;

    inimigo->vivo = true;


    return inimigo;
}

void atualiza_inimigo(inimigo_t * inimigo, ALLEGRO_BITMAP * sprites)
{
    al_draw_scaled_bitmap(sprites, inimigo->sprite_x * 16, inimigo->sprite_y * 16, 16, 16, inimigo->x * 32, (inimigo->y + 1) * 31.3, 32, 32, 0);
}

void move_inimigo(inimigo_t * inimigo, int x, int y)
{
    inimigo->x = x;
    inimigo->y = y;
}