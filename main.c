#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include "player.h"

#define KEY_SEEN     1
#define KEY_RELEASED 2

char ** inicia_mapa(int lin, int col)
{
    char ** mapa = malloc(sizeof(char*) * lin);
    mapa[0] = malloc(sizeof(char) * lin * col);

    for (int i = 0; i < lin; i++)
        mapa[i] = mapa[0] + i * col;

    return mapa;
}

void carrega_mapa(char ** mapa, int lin, int col)
{
    for (int i = 0; i < lin; i++)
    {
        for (int j = 0; j < col; j++)
            fscanf(stdin, "%c", &mapa[i][j]);       
        fscanf(stdin, "\n");
    }
}

void imprime_mapa(char ** mapa, int lin, int col)
{
    for (int i = 0; i < lin; i++)
    {
        for (int j = 0; j < col; j++)
            printf("%c", mapa[i][j]);
        printf("\n");
    }
}

int pos_valida(char ** mapa, int x, int y)
{
    return (mapa[y][x] != '#' && mapa[y][x] != 'o');
}

void move_player(char ** mapa, int * pos_x, int * pos_y, int x, int y)
{
    mapa[*pos_y][*pos_x] = ' ';
    mapa[y][x] = '@';

    *pos_y = y;
    *pos_x = x;
}

int eh_cristal(char obj)
{
    return (obj == '*' || (obj >= 65 && obj <= 68));
}

int eh_pedra(char obj)
{
    return (obj == 'o' || (obj >= 48 && obj <= 51));
}

void gravidade(char ** mapa, int i, int j, char obj, int min, int max)
{
    //Pedra encontrada com espaco vazio embaixo
    if (mapa[i][j] == obj && mapa[i+1][j] == ' ')
        mapa[i][j] = min;

    //Pedra em cima de pedra
    else if (mapa[i][j] == obj && (mapa[i+1][j] == 'o' || mapa[i+1][j] == '*'))
    {
        //Pedra tomba para direita
        if (mapa[i][j+1] == ' ' && mapa[i+1][j+1] == ' ')
        {
            mapa[i][j] = ' ';
            mapa[i][j+1] = min + 1;
        }

        //Pedra tomba para esquerda
        else if (mapa[i][j-1] == ' ' && mapa[i+1][j-1] == ' ')
        {
            mapa[i][j] = ' ';
            mapa[i][j-1] = min + 1;
        }
    }
    
    //Tempo de aceleracao da pedra
    else if ((mapa[i][j] >= min && mapa[i][j] < max) && mapa[i+1][j] == ' ')
        mapa[i][j] = mapa[i][j] + 1;
    
    //Pedra comeca cair
    else if (mapa[i][j] == max && mapa[i+1][j] == ' ')
    {
        mapa[i+1][j] = mapa[i][j];
        mapa[i][j] = ' ';
    }

    //Pedra caiu e bateu em objeto, velocidade volta a ser 0
    else if ((mapa[i][j] >= min && mapa[i][j] <= max) && mapa[i+1][j] != ' ')
        mapa[i][j] = obj;
}

void verifica_gravidade(char ** mapa, int lin, int col)
{
    for (int i = lin - 2; i > 0; i--)
        for (int j = 0; j < col; j++)
        {
            if(eh_pedra(mapa[i][j]))
                gravidade(mapa, i, j, 'o', 48, 51);

            if(eh_cristal(mapa[i][j]))
                gravidade(mapa, i, j, '*', 65, 68);
        }
}

void must_init(bool test, const char *description)
{
    if(test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}

int main()
{
    int lin, col;
    char ** mapa;

    fscanf(stdin, "%d %d \n", &lin, &col);
    
    mapa = inicia_mapa(lin, col);
    carrega_mapa(mapa, lin, col);
    imprime_mapa(mapa, lin, col);

    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");

    unsigned char key[ALLEGRO_KEY_MAX];
    memset(key, 0, sizeof(key));

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    must_init(timer, "timer");

    ALLEGRO_TIMER * tick = al_create_timer(1.0 / 12.0);
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

    int vel_x = 32;
    float vel_y = 31.3;

    int aceleracao = 0;

    printf("%c\n", mapa[y][x]);

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
                        if (pos_valida(mapa, x, y-1)) y--;
                    
                    if(key[ALLEGRO_KEY_DOWN])
                        if (pos_valida(mapa, x, y+1)) y++;

                    if(key[ALLEGRO_KEY_LEFT])
                        if (pos_valida(mapa, x-1, y)) x--;

                    if(key[ALLEGRO_KEY_RIGHT])
                        if (pos_valida(mapa, x+1, y)) x++;

                    move_player(mapa, &pos_x, &pos_y, x, y);
                    verifica_gravidade(mapa, lin, col);

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
            al_clear_to_color(al_map_rgb(0, 0, 0));

			int lin_atual = 1;
            for (int y = 0; y < lin; y++)
            {
				int col_atual = 0;
                for (int x = 0; x < col; x++)
                {
                    char atual = mapa[y][x];

                    switch(atual)
                    {
                        case '#':
							if ((y > 0 && y < lin - 1) && (x > 0 && x < col - 1))
                            	al_draw_scaled_bitmap(sprites, 32, 48, 16, 16, col_atual * 32, lin_atual * 31.3, 32, 32, 0);
							else
                            	al_draw_scaled_bitmap(sprites, 0, 48, 16, 16, col_atual * 32, lin_atual * 31.3, 32, 32, 0);
                        break;

						case 'o':
                        case '0':
                        case '1':
                        case '2':
                        case '3':
                        case '4':
                        case '5':
                        case '6':
                        case '7':
                            al_draw_scaled_bitmap(sprites, 80, 48, 16, 16, col_atual * 32, lin_atual * 31.3, 32, 32, 0);
						break;

						case '.':
                            al_draw_scaled_bitmap(sprites, 48, 48, 16, 16, col_atual * 32, lin_atual * 31.3, 32, 32, 0);
						break;

						case '*':
                        case 65:
                        case 66:
                        case 67:
                        case 68:
                        case 69:
                        case 70:
                        case 71:
                        case 72:
                            al_draw_scaled_bitmap(sprites, 0, 64, 16, 16, col_atual * 32, lin_atual * 31.3, 32, 32, 0);
						break;

						case '@':
                            al_draw_scaled_bitmap(sprites, 0, 0, 16, 16, col_atual * 32, lin_atual * 31.3, 32, 32, 0);
						break;

                        case ' ':
                            al_draw_scaled_bitmap(sprites, 96, 48, 16, 16, col_atual * 32, lin_atual * 31.3, 32, 32, 0);
                        break;
                    }

					col_atual++;
                }
				lin_atual++;
            }

            al_flip_display();
            imprime_mapa(mapa, lin, col);

            redraw = false;
        }
    }

    mapa[2][2] = '0';

    if (mapa[2][2] == 48)
        printf("%c\n", mapa[2][2] - 1);

    al_destroy_bitmap(sprites);
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}
