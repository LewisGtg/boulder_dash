#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    carrega_scores(bd);

    bd->instrucoes = false;
    bd->placar = false;

    bd->cheatcode = false;
    bd->code = malloc(sizeof(char) * 6);
    strcpy(bd->code, "ci1002");

    bd->done = false;
    bd->redraw = true;

    //Carrega as fases
    bd->fases[0] = "./resources/fases/fase0.txt";
    bd->fases[1] = "./resources/fases/fase1.txt";
    bd->fases[2] = "./resources/fases/fase2.txt";
    bd->fases[3] = "./resources/fases/fase3.txt";
    bd->fases[4] = "./resources/fases/fase4.txt";
    bd->fases[5] = "./resources/fases/fase5.txt";
    bd->fases[6] = "./resources/fases/fase6.txt";
    bd->fases[7] = "./resources/fases/fase7.txt";
    bd->fases[8] = "./resources/fases/fase8.txt";
    bd->fases[9] = "./resources/fases/fase9.txt";

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
            
            //Encontrou um tipo de inimigo, o inicia no vetor
            if (cenario->mapa[i][j] == 'q')
            {
                cenario->inimigos[cenario->qtd_inimigos] = inicia_inimigo('q', 5, 4);
                move_inimigo(cenario->inimigos[cenario->qtd_inimigos], j, i);
                cenario->qtd_inimigos++;
            }

            //Encontrou um tipo de inimigo, o inicia no vetor
            if (cenario->mapa[i][j] == 'h')
            {
                cenario->inimigos[cenario->qtd_inimigos] = inicia_inimigo('h', 6, 4);
                move_inimigo(cenario->inimigos[cenario->qtd_inimigos], j, i);
                cenario->qtd_inimigos++;
            }
            
        }
        fscanf(arq, "\n");
    }

    //Le o tempo e a quantidade minima de cristais do cenario
    fscanf(arq, "%d\n", &cenario->min_cristais);
    fscanf(arq, "%d\n", &cenario->fator_score);
    fscanf(arq, "%d\n", &cenario->tempo);

    cenario->porta_aberta = false;

    fclose(arq);
}

void inicia_fase(boulder_dash * bd)
{
    //Caso tenham inimigos, serao destruidos para carregar uma nova fase
    destroi_inimigos(bd->cenario);
    carrega_cenario(bd->cenario, bd->fases[bd->fase_atual]);

    //Movimenta o player para as novas coordenadas
    muda_pos(bd->rockford, bd->cenario->posX_player, bd->cenario->posY_player);
    movimenta_player(bd->cenario, bd->rockford);

    //Reseta o status do player
    reset_player(bd->rockford);
}

void processa_eventos(boulder_dash * bd)
{
    al_wait_for_event(bd->allegro->queue, &bd->allegro->event);

    //Processa o evento
    switch(bd->allegro->event.type)
    {
        case ALLEGRO_EVENT_TIMER:
            //Timer do jogo
            if(bd->allegro->event.timer.source == bd->allegro->timer)
                if (!bd->instrucoes && tempo_acabou(bd->cenario))
                    morte(bd->rockford);

            //Tick rate do jogo
            if(bd->allegro->event.timer.source == bd->allegro->tick)
            {
                //Verifica quais teclas foram apertadas

                //Teclas de movimentação, verifica se a posição requerida pode ser acessada
                if(bd->allegro->key[ALLEGRO_KEY_UP])
                {
                    if (pos_valida_player(bd->cenario->mapa, bd->rockford->x, bd->rockford->y-1)) bd->rockford->y--;
                    bd->rockford->sprite_y = 1;
                }

                else if(bd->allegro->key[ALLEGRO_KEY_DOWN])
                {
                    if (pos_valida_player(bd->cenario->mapa, bd->rockford->x, bd->rockford->y+1)) bd->rockford->y++;
                    bd->rockford->sprite_y = 2;
                }

                else if(bd->allegro->key[ALLEGRO_KEY_LEFT])
                {
                    //Verifica se a posição é valida, mas também verifica se o player esta tentando empurrar uma pedra
                    if ((pos_valida_player(bd->cenario->mapa, bd->rockford->x-1, bd->rockford->y)) ||
                        (eh_pedra(bd->cenario->mapa[bd->rockford->y][bd->rockford->x-1]) && 
                        empurrou_pedra(bd->cenario->mapa, 0, bd->rockford->x-1, bd->rockford->y)))
                        bd->rockford->x--;
                    bd->rockford->sprite_y = 1;
                }

                else if(bd->allegro->key[ALLEGRO_KEY_RIGHT] && bd->rockford->vivo)
                {
                    //Verifica se a posição é valida, mas também verifica se o player esta tentando empurrar uma pedra
                    if ((pos_valida_player(bd->cenario->mapa, bd->rockford->x+1, bd->rockford->y)) ||
                        (eh_pedra(bd->cenario->mapa[bd->rockford->y][bd->rockford->x+1]) && 
                        empurrou_pedra(bd->cenario->mapa, 1, bd->rockford->x+1, bd->rockford->y)))
                        bd->rockford->x++;
                    bd->rockford->sprite_y = 2;
                }
                else
                    bd->rockford->sprite_y = 0;


                if(bd->allegro->key[ALLEGRO_KEY_ESCAPE])
                {
                    //Player apertou esc duas vezes, jogo eh fechado
                    if (bd->placar)
                    {
                        bd->done = true;
                        return;
                    }

                    //Player apertou esc apenas uma vez, placar eh mostrado
                    salva_score(bd);
                    bd->placar = true;
                }

                //Teclas para alternar entre fases
                if(bd->allegro->key[ALLEGRO_KEY_PGDN])
                {
                    if (bd->fase_atual < QTD_FASES-1)
                        bd->fase_atual++;
                    
                    inicia_fase(bd);
                }

                if(bd->allegro->key[ALLEGRO_KEY_PGUP])
                {
                    if (bd->fase_atual > 0)
                        bd->fase_atual--;
                    
                    inicia_fase(bd);
                }

                //Abre menu de instrucoes
                if(bd->allegro->key[ALLEGRO_KEY_H] || bd->allegro->key[ALLEGRO_KEY_F1])
                {
                    al_play_sample(bd->cenario->som_abriu_instrucoes, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                    bd->instrucoes = !bd->instrucoes;
                }

                //Sequencia de caracteres para habilitar cheatcode
                if(bd->allegro->key[ALLEGRO_KEY_C])
                    retira_letra(bd->code, 'c');

                else if(bd->allegro->key[ALLEGRO_KEY_I])
                    retira_letra(bd->code, 'i');

                else if(bd->allegro->key[ALLEGRO_KEY_1])
                    retira_letra(bd->code, '1');

                else if(bd->allegro->key[ALLEGRO_KEY_0])
                    retira_letra(bd->code, '0');

                else if (bd->allegro->key[ALLEGRO_KEY_2])
                    retira_letra(bd->code, '2');

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
        //Player morreu, renicia a fase
        if (!bd->rockford->vivo)
        {
            al_play_sample(bd->cenario->som_gameover, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
            al_rest(2);
            inicia_fase(bd);
        }

        //Habilitou ou desabilitou cheatcode, reseta o codigo
        if (habilitou_cheatcode(bd))
        {
            al_play_sample(bd->cenario->som_cheatcode, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
            bd->cheatcode = !bd->cheatcode;
            strcpy(bd->code, "ci1002");
        }

        //Verifica se player pegou um cristal
        verifica_ponto(bd->cenario, bd->rockford);
        
        //Movimentacao do player e inimigo
        movimenta_player(bd->cenario, bd->rockford);
        procura_pos_inimigos(bd->cenario);

        //Aplica gravidade em elementos do cenario
        verifica_gravidade(bd->cenario, bd->rockford);

        //Cheatcode habilitado
        if (bd->cheatcode)
            cenario_cheatcode(bd->cenario, bd->rockford);

        //Inimigo colidiu com player
        if (colisao_player(bd->cenario, bd->rockford))
        {
            al_play_sample(bd->cenario->som_colidiu_inimigo, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
            morte(bd->rockford);
        }

        //Player juntou cristais necessarios
        if (bd->rockford->cristais >= bd->cenario->min_cristais)
            abre_porta(bd->cenario, bd->rockford);

        //Player passou pela porta aberta
        if (passou_fase(bd->cenario, bd->rockford))
        {
            al_play_sample(bd->cenario->som_concluiu_fase, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);

            if (bd->fase_atual < 9)
                bd->fase_atual++;
            else
                bd->fase_atual = 0;

            inicia_fase(bd);
        }
    }
}

void atualiza_display(boulder_dash * bd)
{
    //Logica ja foi atualizada, portanto pode-se desenhar 
    if(bd->redraw && al_is_event_queue_empty(bd->allegro->queue))
    {    
        al_clear_to_color(al_map_rgb(0, 0, 0));        
        
        //Mostra as instruções na tela ao invés do jogo caso tenha sido pressionado h ou F1
        if (bd->instrucoes)
            le_arquivo(bd, "./data/instrucoes.txt", 300, 25);

        //Mostra as 10 melhores pontuações caso o player tenha encerrado o jogo
        else if (bd->placar)
            le_arquivo(bd, "./data/pontuacoes.txt", 500, 25);

        //Mostra os elementos do jogo 
        else
        {
            //Desenha player e elementos do cenario
            atualiza_player(bd->rockford, bd->allegro->sprites);
            atualiza_cenario(bd->cenario, bd->allegro->sprites);

            //Desenha todos inimigos da fase
            for (int i = 0; i < bd->cenario->qtd_inimigos; i++)
                atualiza_inimigo(bd->cenario->inimigos[i], bd->allegro->sprites);

            //Desenha o painel com as informacoes da fase
            atualiza_painel(bd->cenario, bd->rockford, bd->allegro->font);    
        }
        
        al_flip_display();

        bd->redraw = false;
    }
}

void le_arquivo(boulder_dash * bd, char * nome_arq, int tab, int lh)
{
    //Abre arquivo e testa se deu certo
    FILE * arq;
    arq = fopen(nome_arq, "r");

    if (!arq)
    {
        perror("Erro na função le_arquivo");
        exit(1);
    }

    //Variavel para conter a linha do arquivo
    char linha[1024];

    int i = 1;

    //Le cada linha do arquivo e a escreve na tela
    fgets(linha, 1024, arq);
    while(!feof(arq))
    {
        al_draw_text(bd->allegro->font_text, al_map_rgb(255, 255, 255), tab, lh * i, 0, linha);
        fgets(linha, 1024, arq);
        i++;
    }

    fclose(arq);
}

void carrega_scores(boulder_dash * bd)
{
    //Abre arquivo de pontuacoes e testa
    FILE * score;
    score = fopen("./data/pontuacoes.txt", "r+");

    if (!score)
    {
        perror("Falha ao abrir arquivo de pontuação.");
        exit(1);
    }
    
    //String para pegar conteudo que nao util
    char linha_invalida[BUFSIZE];

    //Descarta a primeira linha dos dados
    fgets(linha_invalida, BUFSIZE, score);

    int i = 0;
    bd->tam_ptacoes = 0;
    
    //Carrega as pontuacoes existentes
    while(!feof(score))
    {
        //Le conteudo inutil
        fscanf(score, "%s", linha_invalida);

        //Le o score e o carrega no vetor de pontuacoes
        fscanf(score, "%d", &bd->pontuacoes[i]);

        //Carrega no max 10 pontuacoes
        if (bd->tam_ptacoes < MAX_PONTUACOES)
            bd->tam_ptacoes++;
        i++;
    }

    fclose(score);
}

//Ordena o vetor de pontuações
void insertion_sort(int vetor[], int * tam, int valor)
{   
    //Procura lugar adequado para inserir valor
    int i = 0;
    while(valor < vetor[i] && i < *tam) i++;

    //"Puxa" todos valores para esquerda para inserir novo valor
    for (int j = *tam - 1; j >= i; j--)
        if (j+1 < MAX_PONTUACOES)
            vetor[j+1] = vetor[j]; 

    //Insere novo valor
    if (i < MAX_PONTUACOES)
    {
        vetor[i] = valor;
        if (*tam < MAX_PONTUACOES)
            *tam++;
    }
        
}

void salva_score(boulder_dash * bd)
{
    //Insere o novo score de maneira ordenada 
    insertion_sort(bd->pontuacoes, &bd->tam_ptacoes, bd->rockford->score);

    //Abre arquivo de pontuacoes e testa
    FILE * score;
    score = fopen("./data/pontuacoes.txt", "w");

    if (!score)
    {
        perror("Erro ao abrir arquivo de pontuações");
        exit(1);
    }

    //Escreve as 10 melhores pontuacoes no arquivo e o fecha
    fprintf(score, "Melhores Pontuações\n");

    for (int i = 0; i < bd->tam_ptacoes; i++)
        fprintf(score, "%2d. %5d\n", i+1, bd->pontuacoes[i]);

    fclose(score);
}

int sequencia_correta(char * code, int tam)
{
    //Verifica se a sequencia esta sendo seguita ate determinado ponto do codigo
    for (int i = 0; i < tam; i++)
        if (code[i] != ' ')
            return 0;
    return 1;
}

void retira_letra(char * code, int c)
{
    //Verifica se o usuario esta escrevendo o codigo para habilitar o cheat de maneira correta, ou seja, seguindo
    //a sequencia c i 1 0 0 2

    if (c == 'c' && sequencia_correta(code, 0))
    {
        code[0] = ' ';
        return;
    }

    else if (c == 'i' && sequencia_correta(code, 1))
    {
        code[1] = ' ';
        return;
    }

    else if (c == '1' && sequencia_correta(code, 2))
    {
        code[2] = ' ';
        return;
    }

    else if (c == '0' && (sequencia_correta(code, 3) || sequencia_correta(code, 4)))
    {
        if (sequencia_correta(code, 4))
            code[4] = ' ';
        
        if (sequencia_correta(code, 3))
            code[3] = ' ';
        
        return;
    }

    else if (c == '2' && sequencia_correta(code, 5))
    {
        code[5] = ' ';
        return;
    }
    
    //Sequencia nao foi seguida, codigo eh resetado
    strcpy(code, "ci1002");
}

int habilitou_cheatcode(boulder_dash * bd)
{
    //Verifica se o codigo foi digitado por completo
    for (int i = 0; i < strlen(bd->code); i++)
        if (bd->code[i] != ' ')
            return 0;
    
    return 1;
}

void imprime_mapa(cenario_t * cenario)
{
    for (int i = 0; i < cenario->lin; i++)
    {
        for (int j = 0; j < cenario->col; j++)
            printf("%c", cenario->mapa[i][j]);
        printf("\n");
    }
}