#include <allegro5/allegro5.h>
#include<stdio.h>
#include<stdlib.h>
#include "player.h"

player_t * inicia_player(int x, int y)
{
    player_t * player = malloc(sizeof(player_t));

    if (!player)
    {
        perror("Erro ao iniciar player.");
        exit(1);
    }

    player->x = x;
    player->y = y;

    player->sprite_x = 0;
    player->sprite_y = 0;

    player->sprite_cooldown = 0;

    return player;
}

void atualiza_player(player_t * player, ALLEGRO_BITMAP * sprites)
{
    //Desenha o player na tela
    al_draw_scaled_bitmap(sprites, player->sprite_x * 16, player->sprite_y * 16, 16, 16, player->x * 32, (player->y + 1) * 31.3, 32, 32, 0);
    
    //Atualiza a sprite do player
    if (player->sprite_cooldown == 13)
    {
        if (player->sprite_x < 6)
            player->sprite_x += 1;
        else
            player->sprite_x = 0;

        player->sprite_cooldown = 0;
    }

    player->sprite_cooldown++;
}

void movimenta(unsigned char key[], cenario_t * cenario, player_t * player)
{
    if(key[ALLEGRO_KEY_UP])
    {
        if (pos_valida(cenario->mapa, player->x, player->y-1)) player->y--;
    }
    
    else if(key[ALLEGRO_KEY_DOWN])
    {
        if (pos_valida(cenario->mapa, player->x, player->y+1)) player->y++;
    }

    else if(key[ALLEGRO_KEY_LEFT])
    {
        if (pos_valida(cenario->mapa, player->x-1, player->y)) player->x--;
    }

    else if(key[ALLEGRO_KEY_RIGHT])
    {
        if (pos_valida(cenario->mapa, player->x+1, player->y)) player->x++;
    }

    movimenta_player(cenario, player->x, player->y);
}
