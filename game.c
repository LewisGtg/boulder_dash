#include <stdio.h>
#include <stdlib.h>
#include "game.h"

boulder_dash * inicia_game()
{
    //Aloca memoria para o jogo e vetor de strings
    boulder_dash * bd = malloc(sizeof(boulder_dash));
    bd->fases = malloc(sizeof(char**) * QTD_FASES);

    //Testa os mallocs
    if (!bd || !bd->fases)
    {
        perror("Erro ao alocar jogo.");
        exit(1);
    }

    //Inicia estruturas
    bd->allegro = inicia_allegro();
    bd->cenario = inicia_cenario();
    bd->rockford = inicia_player();

    bd->instrucoes = false;
    bd->placar = false;

    bd->done = false;
    bd->redraw = true;

    bd->fases[0] = "fase0.txt";
    bd->fases[1] = "fase1.txt";
    bd->fases[2] = "fase2.txt";
    bd->fases[3] = "fase3.txt";
    bd->fases[4] = "fase4.txt";
    bd->fases[5] = "fase5.txt";
    bd->fases[6] = "fase6.txt";
    bd->fases[7] = "fase7.txt";
    bd->fases[8] = "fase8.txt";
    bd->fases[9] = "fase9.txt";

    bd->fase_atual = 0;

    return bd;
}

void carrega_cenario(cenario_t * cenario, char * arquivo_cenario)
{
    //Abre arquivo e testa 
    FILE * arq;
    arq = fopen(arquivo_cenario, "r");


    if (!arq)
    {
        perror("Erro ao abrir cenário.");
        exit(1);
    }

    //Le tamanho do mapa
    fscanf(arq, "%d %d \n", &cenario->lin, &cenario->col);

    //Caso nenhum mapa tenha sido incializado, inicia um
    if (!cenario->mapa)
        cenario->mapa = inicia_mapa(cenario->lin, cenario->col);

    //Percorre a matriz de caracteres do arquivo e copia elementos para o mapa
    for (int i = 0; i < cenario->lin; i++)
    {
        for (int j = 0; j < cenario->col; j++)
        {
            fscanf(arq, "%c", &cenario->mapa[i][j]);       
            
            //Salva a posição do player
            if (cenario->mapa[i][j] == '@')
            {
                cenario->posY_player = i;
                cenario->posX_player = j;
            }

            //Salva a posição da saida e a transforma em um muro
            if (cenario->mapa[i][j] == 's')
            {
                cenario->saida_y = i;
                cenario->saida_x = j;
                cenario->mapa[i][j] = '#';
            }
        }
        fscanf(arq, "\n");
    }

    //Le o tempo e a quantidade minima de cristais do cenario
    fscanf(arq, "%d\n", &cenario->min_cristais);
    fscanf(arq, "%d\n", &cenario->tempo);

    cenario->fator_score++;

    fclose(arq);
}

void inicia_fase(boulder_dash * bd)
{
    //Passos necessários para iniciar uma nova fase
    carrega_cenario(bd->cenario, bd->fases[bd->fase_atual]);
    muda_pos(bd->rockford, bd->cenario->posX_player, bd->cenario->posY_player);
    movimenta_player(bd->cenario, bd->rockford);
    reset_player(bd->rockford);
}

void processa_eventos(boulder_dash * bd)
{
    al_wait_for_event(bd->allegro->queue, &bd->allegro->event);

    switch(bd->allegro->event.type)
    {
        case ALLEGRO_EVENT_TIMER:
            //Timer do jogo
            if(bd->allegro->event.timer.source == bd->allegro->timer)
                if (tempo_acabou(bd->cenario))
                    bd->done = true;

            //Tick rate do jogo
            if(bd->allegro->event.timer.source == bd->allegro->tick)
            {
                //Verifica quais teclas foram apertadas

                //Teclas de movimentação, verifica se a posição requerida pode ser acessada
                if(bd->allegro->key[ALLEGRO_KEY_UP])
                {
                    if (pos_valida(bd->cenario->mapa, bd->rockford->x, bd->rockford->y-1)) bd->rockford->y--;
                }

                else if(bd->allegro->key[ALLEGRO_KEY_DOWN])
                {
                    if (pos_valida(bd->cenario->mapa, bd->rockford->x, bd->rockford->y+1)) bd->rockford->y++;
                }

                else if(bd->allegro->key[ALLEGRO_KEY_LEFT])
                {
                    //Verifica se a posição é valida, mas também verifica se o player esta tentando empurrar uma pedra
                    if ((pos_valida(bd->cenario->mapa, bd->rockford->x-1, bd->rockford->y)) ||
                        (eh_pedra(bd->cenario->mapa[bd->rockford->y][bd->rockford->x-1]) && 
                        empurrou_pedra(bd->cenario->mapa, 0, bd->rockford->x-1, bd->rockford->y)))
                        bd->rockford->x--;
                }

                else if(bd->allegro->key[ALLEGRO_KEY_RIGHT])
                {
                    //Verifica se a posição é valida, mas também verifica se o player esta tentando empurrar uma pedra
                    if ((pos_valida(bd->cenario->mapa, bd->rockford->x+1, bd->rockford->y)) ||
                        (eh_pedra(bd->cenario->mapa[bd->rockford->y][bd->rockford->x+1]) && 
                        empurrou_pedra(bd->cenario->mapa, 1, bd->rockford->x+1, bd->rockford->y)))
                        bd->rockford->x++;
                }


                //Fecha o jogo
                if(bd->allegro->key[ALLEGRO_KEY_ESCAPE])
                    bd->done = true;

                //Teclas para alternar entre fases

                if(bd->allegro->key[ALLEGRO_KEY_PGDN])
                {
                    if (bd->fase_atual < QTD_FASES)
                        bd->fase_atual++;
                    
                    inicia_fase(bd);
                }

                if(bd->allegro->key[ALLEGRO_KEY_PGUP])
                {
                    if (bd->fase_atual > 0)
                        bd->fase_atual--;
                    
                    inicia_fase(bd);
                }

                if(bd->allegro->key[ALLEGRO_KEY_H])
                {
                    bd->instrucoes = !bd->instrucoes;
                }

                for(int i = 0; i < ALLEGRO_KEY_MAX; i++)
                    bd->allegro->key[i] &= KEY_SEEN;
                
            }
            bd->redraw = true;
            break;


        case ALLEGRO_EVENT_KEY_DOWN:
            bd->allegro->key[bd->allegro->event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
            break;

        case ALLEGRO_EVENT_KEY_UP:
            bd->allegro->key[bd->allegro->event.keyboard.keycode] &= KEY_RELEASED;
            break;

        //Usuario fechou a tela do jogo
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            bd->done = true;
            break;
    }
}

void atualiza_logica(boulder_dash * bd)
{
    //Tick rate
    if(bd->allegro->event.timer.source == bd->allegro->tick)
    {
        //Verifica se player pegou um cristal
        verifica_ponto(bd->cenario, bd->rockford);
        
        movimenta_player(bd->cenario, bd->rockford);
        verifica_gravidade(bd->cenario, bd->rockford);

        //Player juntou cristais necessarios
        if (bd->rockford->cristais == bd->cenario->min_cristais)
            abre_porta(bd->cenario, bd->rockford);

        //Player passou pela porta aberta
        if (passou_fase(bd->cenario, bd->rockford))
        {
            bd->fase_atual++;
            inicia_fase(bd);
        }

        //Player morreu, termina jogo
        if (!bd->rockford->vivo)
            bd->done = true;
    }
}

void atualiza_display(boulder_dash * bd)
{
    if(bd->redraw && al_is_event_queue_empty(bd->allegro->queue))
    {    
        al_clear_to_color(al_map_rgb(0, 0, 0));        
        
        if (bd->instrucoes)
            le_arquivo(bd, "instrucoes.txt");
        else
        {
            atualiza_cenario(bd->cenario, bd->allegro->sprites);
            atualiza_player(bd->rockford, bd->allegro->sprites);
            atualiza_painel(bd->cenario, bd->rockford, bd->allegro->font);    
        }
        
        al_flip_display();

        bd->redraw = false;
    }
}

void le_arquivo(boulder_dash * bd, char * nome_arq)
{
    FILE * arq;
    arq = fopen(nome_arq, "r");

    if (!arq)
    {
        perror("Erro na função le_arquivo");
        exit(1);
    }

    char linha[1024];

    int i = 1;

    while(!feof(arq))
    {
        fgets(linha, 1024, arq);
        al_draw_text(bd->allegro->font_text, al_map_rgb(255, 255, 255), 10, 100 * i, 0, linha);
        i++;
    }

    fclose(arq);
}