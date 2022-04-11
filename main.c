#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include "player.h"
#include "cenario.h"

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

void move_player(char ** mapa, int * pos_x, int * pos_y, int x, int y)
{
    mapa[*pos_y][*pos_x] = ' ';
    mapa[y][x] = '@';

    *pos_y = y;
    *pos_x = x;
}

void must_init(bool test, const char *description)
{
    if(test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}

int main()
{
    cenario_t * cenario = carrega_cenario("mapa1.txt");

    imprime_mapa(cenario->mapa, cenario->lin, cenario->col);

    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");

    unsigned char key[ALLEGRO_KEY_MAX];
    memset(key, 0, sizeof(key));

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    must_init(timer, "timer");

    ALLEGRO_TIMER * tick = al_create_timer(1.0 / 10.0);
    must_init(tick, "tick");

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "queue");

    ALLEGRO_DISPLAY* disp = al_create_display(1280, 720);
    must_init(disp, "display");

    ALLEGRO_FONT* font = al_create_builtin_font();
    must_init(font, "font");

    must_init(al_init_image_addon(), "image addon");
    ALLEGRO_BITMAP* sprites = al_load_bitmap("sprites.png");
    must_init(sprites, "mysha");

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(tick));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool done = false;
    bool redraw = true;
    ALLEGRO_EVENT event;

    int pos_x = 3;
    int pos_y = 2;

    int x = pos_x;
    int y = pos_y;

    printf("%c\n", cenario->mapa[y][x]);

    al_start_timer(tick);
    al_start_timer(timer);

    while(1)
    {
        al_wait_for_event(queue, &event);

        switch(event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                if(event.timer.source == tick)
                {
                    if(key[ALLEGRO_KEY_UP])
                    {
                        if (pos_valida(cenario->mapa, x, y-1)) y--;
                    }
                    
                    else if(key[ALLEGRO_KEY_DOWN])
                    {
                        if (pos_valida(cenario->mapa, x, y+1)) y++;
                    }

                    else if(key[ALLEGRO_KEY_LEFT])
                    {
                        if (pos_valida(cenario->mapa, x-1, y)) x--;
                    }

                    else if(key[ALLEGRO_KEY_RIGHT])
                    {
                        if (pos_valida(cenario->mapa, x+1, y)) x++;
                    }

                    move_player(cenario->mapa, &pos_x, &pos_y, x, y);
                    verifica_gravidade(cenario);

                    if(key[ALLEGRO_KEY_ESCAPE])
                        done = true;

                    for(int i = 0; i < ALLEGRO_KEY_MAX; i++)
                        key[i] &= KEY_SEEN;
                }
                redraw = true;
                break;


            case ALLEGRO_EVENT_KEY_DOWN:
                key[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
                break;

            case ALLEGRO_EVENT_KEY_UP:
                key[event.keyboard.keycode] &= KEY_RELEASED;
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
        }

        if(done)
            break;

        if(redraw && al_is_event_queue_empty(queue))
        {            
            atualiza_cenario(cenario, sprites);
            imprime_mapa(cenario->mapa, cenario->lin, cenario->col);

            redraw = false;
        }
    }

    cenario->mapa[2][2] = '0';

    if (cenario->mapa[2][2] == 48)
        printf("%c\n", cenario->mapa[2][2] - 1);

    al_destroy_bitmap(sprites);
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}
