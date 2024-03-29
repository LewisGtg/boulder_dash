#include "player.h"

player_t * inicia_player()
{
    //Aloca memoria para estrutura e testa
    player_t * player = malloc(sizeof(player_t));

    if (!player)
    {
        perror("Erro ao iniciar player.");
        exit(1);
    }

    //Inicializa elementos do player
    player->cristais = 0;
    player->score = 0;

    player->vivo = true;

    player->sprite_x = 0;
    player->sprite_y = 0;

    player->sprite_cooldown = 0;

    //Retorna estrutura
    return player;
}

void destroi_player(player_t * player)
{
    free(player);
    player = NULL;
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

void muda_pos(player_t * player, int x, int y)
{
    player->x = x;
    player->y = y;
}

void morte(player_t * player)
{
    player->vivo = false;
}

int reset_player(player_t * player)
{
    player->vivo = true;
    return (player->cristais = 0);
}

