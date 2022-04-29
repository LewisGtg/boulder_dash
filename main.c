#include <stdio.h>
#include <stdlib.h>
#include "game.h"

int main()
{
    boulder_dash * bd = inicia_game();
    inicia_fase(bd);

    while(!bd->done)
    {
        processa_eventos(bd);
        atualiza_logica(bd);
        atualiza_display(bd);
    }

    destroi_game(bd);

    return 0;
}
    