#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>

typedef struct cenario {
    int lin, col;
    char ** mapa;

    float posX_player, posY_player;
} cenario_t;

//Carrega sprites do cenario
void atualiza_cenario(cenario_t cenario, ALLEGRO_BITMAP * sprites);

//Verifica se a posicao pode ser acessada (nao possui pedra nem muro)
int pos_valida(char ** mapa, int x, int y);

//Verifica a gravidade de pedras e cristais
void verifica_gravidade(cenario_t cenario);
void gravidade(char ** mapa, int i, int j, char obj, int min, int max);