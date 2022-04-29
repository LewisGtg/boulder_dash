#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#define KEY_SEEN     1
#define KEY_RELEASED 2

#ifndef __AL__
#define __AL__

//Estrutura para conter os ponteiros utilizados pela biblioteca allegro
typedef struct allegro {
    unsigned char key[ALLEGRO_KEY_MAX];
    
    //Timers do jogo
    ALLEGRO_TIMER * fps;
    ALLEGRO_TIMER * tick;
    ALLEGRO_TIMER * timer;

    //Elementos utilizados 
    ALLEGRO_EVENT_QUEUE * queue;
    ALLEGRO_EVENT event;
    ALLEGRO_DISPLAY* disp;
    ALLEGRO_FONT* font;
    ALLEGRO_FONT* font_text;
    ALLEGRO_BITMAP* sprites;

} allegro_t;

//Inicia a biblioteca
allegro_t * inicia_allegro();

//Desaloca estruturas usadas
void destroi_allegro(allegro_t * allegro);

//Função auxiliar para testar se os ponteiros foram inicializados corretamente
void must_init(bool teste, const char * description);

#endif