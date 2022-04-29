#include "allegro.h"

void must_init(bool test, const char *description)
{
    if(test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}

allegro_t * inicia_allegro()
{
    //Aloca memoria e testa
    allegro_t * allegro = malloc(sizeof(allegro_t));

    if (!allegro)
    {
        perror("Erro ao alocar estrutura allegro.");
        exit(1);
    }
    
    //Inicia a biblioteca allegro
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");

    //Inicia audio
    must_init(al_install_audio(), "audio");
    must_init(al_init_acodec_addon(), "audio codecs");
    must_init(al_reserve_samples(16), "reserve samples");

    memset(allegro->key, 0, sizeof(allegro->key));

    //TIMERS DO JOGO
    allegro->fps = al_create_timer(1.0 / 60.0);
    must_init(allegro->fps, "fps");

    allegro->tick = al_create_timer(1.0 / 10.0);
    must_init(allegro->tick, "tick");

    allegro->timer = al_create_timer(1.0 / 1.0);
    must_init(allegro->timer, "timer");

    //Fila de eventos
    allegro->queue = al_create_event_queue();
    must_init(allegro->queue, "queue");

    //Tela
    allegro->disp = al_create_display(1280, 720);
    must_init(allegro->disp, "display");

    //INICIA ADDON DE FONTES E AS FONTES
    must_init(al_init_font_addon(), "font init");
    must_init(al_init_ttf_addon(), "ttf init");
    allegro->font = al_load_font("./resources/fonts/bd_font.ttf", 40, 1);
    must_init(allegro->font, "font");

    allegro->font_text = al_load_font("./resources/fonts/arial.ttf", 20, 0);
    must_init(allegro->font, "font texto");

    //Inicia addon de imagens e inicia as sprites
    must_init(al_init_image_addon(), "image addon");
    allegro->sprites = al_load_bitmap("./resources/sprites.png");
    must_init(allegro->sprites, "sprites");

    //Registra fontes de eventos
    al_register_event_source(allegro->queue, al_get_keyboard_event_source());
    al_register_event_source(allegro->queue, al_get_display_event_source(allegro->disp));
    al_register_event_source(allegro->queue, al_get_timer_event_source(allegro->tick));
    al_register_event_source(allegro->queue, al_get_timer_event_source(allegro->fps));
    al_register_event_source(allegro->queue, al_get_timer_event_source(allegro->timer));

    //Inicia os timers
    al_start_timer(allegro->tick);
    al_start_timer(allegro->fps);
    al_start_timer(allegro->timer);

    //Retorna estrutura
    return allegro;
}