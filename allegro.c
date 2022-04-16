#include "allegro.h"

void must_init(bool test, const char *description)
{
    if(test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}

allegro_t * inicia_allegro()
{
    allegro_t * allegro = malloc(sizeof(allegro_t));
    
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");

    memset(allegro->key, 0, sizeof(allegro->key));

    allegro->timer = al_create_timer(1.0 / 60.0);
    must_init(allegro->timer, "timer");

    allegro->tick = al_create_timer(1.0 / 10.0);
    must_init(allegro->tick, "tick");

    allegro->queue = al_create_event_queue();
    must_init(allegro->queue, "queue");

    allegro->disp = al_create_display(1280, 720);
    must_init(allegro->disp, "display");

    allegro->font = al_create_builtin_font();
    must_init(allegro->font, "font");

    must_init(al_init_image_addon(), "image addon");
    allegro->sprites = al_load_bitmap("sprites.png");
    must_init(allegro->sprites, "sprites");

    al_register_event_source(allegro->queue, al_get_keyboard_event_source());
    al_register_event_source(allegro->queue, al_get_display_event_source(allegro->disp));
    al_register_event_source(allegro->queue, al_get_timer_event_source(allegro->tick));
    al_register_event_source(allegro->queue, al_get_timer_event_source(allegro->timer));

    al_start_timer(allegro->tick);
    al_start_timer(allegro->timer);

    return allegro;
}