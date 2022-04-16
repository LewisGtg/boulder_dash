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
    
    //Inicia o cenário de acordo com o arquivo passado, e o carrega
    cenario_t * cenario = inicia_cenario();
    carrega_cenario(cenario, "mapa.txt");

    //Inicia o player
    player_t * rockford = inicia_player(cenario->posX_player, cenario->posY_player);

    imprime_mapa(cenario->mapa, cenario->lin, cenario->col);

    bool done = false;
    bool redraw = true;

    while(1)
    {
        al_wait_for_event(allegro->queue, &allegro->event);

        switch(allegro->event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                if(allegro->event.timer.source == allegro->tick)
                {
                    movimenta(allegro->key, cenario, rockford);
                    verifica_gravidade(cenario);

                    if(allegro->key[ALLEGRO_KEY_ESCAPE])
                        done = true;

                    for(int i = 0; i < ALLEGRO_KEY_MAX; i++)
                        allegro->key[i] &= KEY_SEEN;
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

        if(done)
            break;

        if(redraw && al_is_event_queue_empty(allegro->queue))
        {    
            al_clear_to_color(al_map_rgb(0, 0, 0));        
            
            atualiza_cenario(cenario, allegro->sprites);
            atualiza_player(rockford, allegro->sprites);
            
            al_flip_display();

            imprime_mapa(cenario->mapa, cenario->lin, cenario->col);

            redraw = false;
        }
    }

    cenario->mapa[2][2] = '0';

    if (cenario->mapa[2][2] == 48)
        printf("%c\n", cenario->mapa[2][2] - 1);

    al_destroy_bitmap(allegro->sprites);
    al_destroy_font(allegro->font);
    al_destroy_display(allegro->disp);
    al_destroy_timer(allegro->timer);
    al_destroy_event_queue(allegro->queue);

    return 0;
}
