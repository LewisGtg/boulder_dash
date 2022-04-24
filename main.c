#include <stdio.h>
#include <stdlib.h>
#include "player.h"
#include "cenario.h"
#include "allegro.h"

#define KEY_SEEN     1
#define KEY_RELEASED 2

void imprime_mapa(char ** mapa, int lin, int col)
{
    for (int i = 0; i < lin; i++)
    {
        for (int j = 0; j < col; j++)
            printf("%c", mapa[i][j]);
        printf("\n");
    }
}

int main()
{
    //Define um ponteiro para estrutura allegro e inicia seus componentes
    allegro_t * allegro = inicia_allegro();
    
    char ** fases = malloc(sizeof(char) * 100);
    fases[0] = "fase1.txt";
    fases[1] = "fase2.txt";

    //Inicia o cenário de acordo com o arquivo passado, e o carrega
    cenario_t * cenario = inicia_cenario();
    carrega_cenario(cenario, fases[0]);

    //Inicia o player
    player_t * rockford = inicia_player(cenario->posX_player, cenario->posY_player);

    //imprime_mapa(cenario->mapa, cenario->lin, cenario->col);

    bool done = false;
    bool redraw = true;

    while(1)
    {
        al_wait_for_event(allegro->queue, &allegro->event);

        switch(allegro->event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                if(allegro->event.timer.source == allegro->timer)
                    if (tempo_acabou(cenario))
                        done = true;


                if(allegro->event.timer.source == allegro->tick)
                {
                    if(allegro->key[ALLEGRO_KEY_UP])
                    {
                        if (pos_valida(cenario->mapa, rockford->x, rockford->y-1)) rockford->y--;
                    }

                    else if(allegro->key[ALLEGRO_KEY_DOWN])
                    {
                        if (pos_valida(cenario->mapa, rockford->x, rockford->y+1)) rockford->y++;
                    }

                    else if(allegro->key[ALLEGRO_KEY_LEFT])
                    {
                        if ((pos_valida(cenario->mapa, rockford->x-1, rockford->y)) ||
                           (eh_pedra(cenario->mapa[rockford->y][rockford->x-1]) && 
                           empurrou_pedra(cenario->mapa, 0, rockford->x-1, rockford->y)))
                           rockford->x--;
                    }

                    else if(allegro->key[ALLEGRO_KEY_RIGHT])
                    {
                            if ((pos_valida(cenario->mapa, rockford->x+1, rockford->y)) ||
                           (eh_pedra(cenario->mapa[rockford->y][rockford->x+1]) && 
                           empurrou_pedra(cenario->mapa, 1, rockford->x+1, rockford->y)))
                           rockford->x++;
                    }

                    if(allegro->key[ALLEGRO_KEY_ESCAPE])
                        done = true;

                    for(int i = 0; i < ALLEGRO_KEY_MAX; i++)
                        allegro->key[i] &= KEY_SEEN;

                    verifica_ponto(cenario, rockford);
                    movimenta_player(cenario, rockford);
                    verifica_gravidade(cenario, rockford);

                    if (rockford->cristais == cenario->min_cristais)
                        abre_porta(cenario, rockford);

                    if (passou_fase(cenario, rockford))
                    {
                        carrega_cenario(cenario, fases[1]);
                        reset_player(rockford);
                        muda_pos(rockford, cenario->posX_player, cenario->posY_player);
                        movimenta_player(cenario, rockford);
                    }

                    if (!rockford->vivo)
                        done = true;
                    
                }
                redraw = true;
                break;


            case ALLEGRO_EVENT_KEY_DOWN:
                allegro->key[allegro->event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
                break;

            case ALLEGRO_EVENT_KEY_UP:
                allegro->key[allegro->event.keyboard.keycode] &= KEY_RELEASED;
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
        }

        if(redraw && al_is_event_queue_empty(allegro->queue))
        {    
            al_clear_to_color(al_map_rgb(0, 0, 0));        
            
            atualiza_cenario(cenario, allegro->sprites);
            atualiza_player(rockford, allegro->sprites);
            atualiza_painel(cenario, rockford, allegro->font);
            
            al_flip_display();

            //imprime_mapa(cenario->mapa, cenario->lin, cenario->col);

            redraw = false;
        }

        if(done)
            break;
    }

    al_destroy_bitmap(allegro->sprites);
    al_destroy_font(allegro->font);
    al_destroy_display(allegro->disp);
    al_destroy_timer(allegro->fps);
    al_destroy_timer(allegro->tick);
    al_destroy_timer(allegro->timer);
    al_destroy_event_queue(allegro->queue);

    return 0;
}
