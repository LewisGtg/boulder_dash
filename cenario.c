#include "cenario.h"

void atualiza_sprites(cenario_t * cenario)
{
    //Terminou cooldown da sprite, pode ser atualizada
    if (cenario->sprite_cd == 3)
    {
        //Alterna entre sprites do cristal, de acordo com as coordenadas na imagem
        if (cenario->cristal_y < 1)
            cenario->cristal_x++;
        else
        {
            cenario->cristal_x = 0;

            if (cenario->cristal_y < 7)
                cenario->cristal_y++;
            else
                cenario->cristal_y = 4;
        }
        //Reseta o cooldown da imagem
        cenario->sprite_cd = 0;
    }
    //Diminui cooldown
    cenario->sprite_cd += 1;
}

void atualiza_cenario(cenario_t * cenario, ALLEGRO_BITMAP * sprites)
{
    //Percorre a matriz do mapa e carrega as sprites de acordo com seu tipo
    int lin_atual = 1;
    for (int y = 0; y < cenario->lin; y++)
    {
        int col_atual = 0;
        for (int x = 0; x < cenario->col; x++)
        {
            char atual = cenario->mapa[y][x];
            switch(atual)
            {
                //Caso encontre muro
                case '#':
                    if ((y > 0 && y < cenario->lin - 1) && (x > 0 && x < cenario->col - 1))
                        al_draw_scaled_bitmap(sprites, 32, 48, 16, 16, col_atual * 32, lin_atual * 31.3, 32, 32, 0);
                    else
                        al_draw_scaled_bitmap(sprites, 0, 48, 16, 16, col_atual * 32, lin_atual * 31.3, 32, 32, 0);
                break;


                //Caso encontre pedras, tanto paradas, quanto caindo
                case 'o':
                case 48:
                case 49:
                case 50:
                case 51:
                    al_draw_scaled_bitmap(sprites, 80, 48, 16, 16, col_atual * 32, lin_atual * 31.3, 32, 32, 0);
                break;

                //Caso encontre arbusto
                case '.':
                    al_draw_scaled_bitmap(sprites, 48, 48, 16, 16, col_atual * 32, lin_atual * 31.3, 32, 32, 0);
                break;

                //Caso encontre cristais, tanto parados, quanto caindo
                case '*':
                case 65:
                case 66:
                case 67:
                case 68:
                    al_draw_scaled_bitmap(sprites, cenario->cristal_x * 16, cenario->cristal_y * 16, 16, 16, col_atual * 32, lin_atual * 31.3, 32, 32, 0);
                break;

                //Caso encontre a porta de saida
                case 's':
                    al_draw_scaled_bitmap(sprites, 16, 48, 16, 16, col_atual * 32, lin_atual * 31.3, 32, 32, 0);
                break;

                //Caso encontre espaco vazio
                case ' ':
                    al_draw_scaled_bitmap(sprites, 96, 48, 16, 16, col_atual * 32, lin_atual * 31.3, 32, 32, 0);
                break;
            }
            
            //Atualiza a coluna que devera ser desenhada
            col_atual++;
        }
        //Atualiza a linha que devera ser desenhada
        lin_atual++;
    }
    atualiza_sprites(cenario);
}

void atualiza_painel(cenario_t * cenario, player_t * player, ALLEGRO_FONT * font)
{
    //Muda de int para string os elementos
    char cristais[100];
    sprintf(cristais, "%2d", player->cristais);

    char score[100];
    sprintf(score, "%d", player->score);

    char tempo[100];
    sprintf(tempo, "%d", cenario->tempo);

    char min[100];
    sprintf(min, "%2d", cenario->min_cristais);
    
    //Escreve na tela os elementos
    al_draw_text(font, al_map_rgb(255, 255, 0), 100, 0, 0, cristais);
    al_draw_text(font, al_map_rgb(255, 255, 255), 175, 0, 0, "/");
    al_draw_text(font, al_map_rgb(255, 255, 255), 225, 0, 0, min);
    al_draw_text(font, al_map_rgb(255, 255, 255), 1100, 0, 0, score);
    al_draw_text(font, al_map_rgb(255, 255, 255), 900, 0, 0, tempo);
}

void cenario_cheatcode(cenario_t * cenario, player_t * player)
{
    //Percorre as posicoes em volta do player e transforma em cristais caso sejam arbustos
    for (int i = player->y-1; i < player->y + 2; i++)
        for (int j = player->x-1; j < player->x + 2; j++)
            if (cenario->mapa[i][j] == '.')
                cenario->mapa[i][j] = '*';
}

int eh_cristal(char obj)
{
    return (obj == '*' || (obj >= 65 && obj <= 68));
}

int eh_pedra(char obj)
{
    return (obj == 'o' || (obj >= 48 && obj <= 51));
}

int empurrou_pedra(char ** mapa, int dir, int x, int y)
{
    //Posicao apos a pedra
    int prox_x = x - 1;
    if (dir)
        prox_x = x + 1;

    //Caso a posicao apos a pedra seja vazia, tenta a empurrar
    if (mapa[y][prox_x] == ' ' && mapa[y+1][x] != ' ')
    {
        //Gera um numero entre 1 e 100, caso o valor seja maior que 80 a pedra sera empurrada
        int prob = rand() % 100;
        if (prob >= 80)
        {
            mapa[y][prox_x] = 'o';
            return 1;
        }
    }
    return 0;
}

int pos_valida_player(char ** mapa, int x, int y)
{
    //Testa se o player ira colidir com algum objeto
    char prox = mapa[y][x];
    return (prox != '#' && prox != 'o' && !(prox >= 48 && prox <= 51) && !(prox >= 65 && prox <= 68));
}

int pos_valida_inimigo(char ** mapa, int x, int y)
{
    //Testa se o inimigo ira colidir com algum objeto, ou outro inimigo
    char prox = mapa[y][x];
    return (prox != 'h' && prox != 'q' && prox != '.' && prox != '*' && prox != '#' && prox != 'o' && !(prox >= 48 && prox <= 51) && !(prox >= 65 && prox <= 68));
}

int colisao_player(cenario_t * cenario, player_t * player)
{
    //Percorre o vetor de inimigos
    for (int i = 0; i < cenario->qtd_inimigos; i++)
    {
        //Verifica se algum inimigo ocupa a mesma posicao que o player
        inimigo_t * inimigo = cenario->inimigos[i];
        if (inimigo->vivo && inimigo->x == player->x && inimigo->y == player->y)
            return 1;
    }
    return 0;
}

void procura_pos_inimigos(cenario_t * cenario)
{
    //Logica de movimentacao dos inimigos
    for (int i = 0; i < cenario->qtd_inimigos; i++)
    {
        inimigo_t * inimigo = cenario->inimigos[i];
        if (inimigo->vivo)
        {
            //Caso colida com algo, muda de direcao

            if (!pos_valida_inimigo(cenario->mapa, inimigo->x-1, inimigo->y) && inimigo->dir == 0) inimigo->dir = 1;

            else if (!pos_valida_inimigo(cenario->mapa, inimigo->x, inimigo->y-1) && inimigo->dir == 1) inimigo->dir = 2;

            else if (!pos_valida_inimigo(cenario->mapa, inimigo->x+1, inimigo->y) && inimigo->dir == 2) inimigo->dir = 3; 

            else if (!pos_valida_inimigo(cenario->mapa, inimigo->x, inimigo->y+1) && inimigo->dir == 3) inimigo->dir = 0;

            if (inimigo->dir == 0 && pos_valida_inimigo(cenario->mapa, inimigo->x-1, inimigo->y)) movimenta_inimigo(cenario, inimigo, inimigo->x-1, inimigo->y);

            if (inimigo->dir == 1 && pos_valida_inimigo(cenario->mapa, inimigo->x, inimigo->y-1)) movimenta_inimigo(cenario, inimigo, inimigo->x, inimigo->y-1);

            if (inimigo->dir == 2 && pos_valida_inimigo(cenario->mapa, inimigo->x+1, inimigo->y)) movimenta_inimigo(cenario, inimigo, inimigo->x+1, inimigo->y);

            if (inimigo->dir == 3 && pos_valida_inimigo(cenario->mapa, inimigo->x, inimigo->y+1)) movimenta_inimigo(cenario, inimigo, inimigo->x, inimigo->y+1);
        }
    }
}

void gravidade(cenario_t * cenario, player_t * player, int i, int j, char obj, int min, int max)
{
    //Verifica se um objeto caiu na cabeça do player
    if (cenario->mapa[i][j] == max && cenario->mapa[i+1][j] == '@')
    {
        al_play_sample(cenario->som_pedra, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        morte(player);
    }

    //Verifica se um objeto caiu na cabeca de algum inimigo
    for (int k = 0; k < cenario->qtd_inimigos; k++)
    {
        inimigo_t * inimigo = cenario->inimigos[k];
        if (cenario->mapa[i][j] == max && inimigo->vivo && inimigo->y == i+1 && inimigo->x == j)
        {
            cenario->mapa[inimigo->y][inimigo->x] = ' ';
            morte_inimigo(inimigo);
            al_play_sample(cenario->som_explosao, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);

            for (int l = inimigo->y-1; l < inimigo->y+2; l++)
                for (int c = inimigo->x; c < inimigo->x+2; c++)
                    if (cenario->mapa[l][c] != '#') 
                        cenario->mapa[l][c] = '*';
        }
    }

    //Pedra ou cristal encontrada com espaco vazio embaixo
    if (cenario->mapa[i][j] == obj && cenario->mapa[i+1][j] == ' ')
        cenario->mapa[i][j] = min;

    //Pedra em cima de pedra, ou, cristal em cima de crital
    else if (cenario->mapa[i][j] == obj && (cenario->mapa[i+1][j] == 'o' || cenario->mapa[i+1][j] == '*' || cenario->mapa[i+1][j] == '#'))
    {
        //Pedra ou cristal tomba para direita
        if (cenario->mapa[i][j+1] == ' ' && cenario->mapa[i+1][j+1] == ' ')
        {
            cenario->mapa[i][j] = ' ';
            cenario->mapa[i][j+1] = min + 1;
        }

        //Pedra ou cristal tomba para esquerda
        else if (cenario->mapa[i][j-1] == ' ' && cenario->mapa[i+1][j-1] == ' ')
        {
            cenario->mapa[i][j] = ' ';
            cenario->mapa[i][j-1] = min + 1;
        }
    }
    
    //Tempo de aceleracao da pedra ou cristal
    else if ((cenario->mapa[i][j] >= min && cenario->mapa[i][j] < max) && cenario->mapa[i+1][j] == ' ')
        cenario->mapa[i][j] = cenario->mapa[i][j] + 1;
    
    //Pedra ou cristal comeca cair
    else if (cenario->mapa[i][j] == max && cenario->mapa[i+1][j] == ' ')
    {
        cenario->mapa[i+1][j] = cenario->mapa[i][j];
        cenario->mapa[i][j] = ' ';
    }

    //Pedra ou cristal caiu e bateu em objeto, velocidade volta a ser 0
    else if ((cenario->mapa[i][j] >= min && cenario->mapa[i][j] <= max) && (cenario->mapa[i+1][j] != ' ') && (cenario->mapa[i+1][j] != '@') && (cenario->mapa[i+1][j] != 'h'))
    {
        al_play_sample(cenario->som_pedra_colidiu, 0.5, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
        cenario->mapa[i][j] = obj;
    }
}

void verifica_gravidade(cenario_t * cenario, player_t * player)
{
    //Percorre por todos elementos do mapa
    for (int i = cenario->lin - 2; i > 0; i--)
        for (int j = 0; j < cenario->col; j++)
        {
            //Caso encontre pedra ou cristal, aplica gravidade sobre eles
            if(eh_pedra(cenario->mapa[i][j]))
                gravidade(cenario, player, i, j, 'o', 48, 51);

            if(eh_cristal(cenario->mapa[i][j]))
                gravidade(cenario, player, i, j, '*', 65, 68);
        }
}

char ** inicia_mapa(int lin, int col)
{
    char ** mapa = malloc(sizeof(char*) * lin);
    mapa[0] = malloc(sizeof(char) * lin * col);

    if (!mapa || !mapa[0])
    {
        perror("Erro ao alocar mapa!");
        exit(1);
    }

    for (int i = 0; i < lin; i++)
        mapa[i] = mapa[0] + i * col;

    return mapa;
}

cenario_t * inicia_cenario()
{
    //Aloca memoria para o cenario e vetor de inimigos, e os testa
    cenario_t * cenario = malloc(sizeof(cenario_t));
    cenario->inimigos = malloc(sizeof(inimigo_t**) * MAX_INIMIGOS);

    if (!cenario || !cenario->inimigos)
    {
        perror("Erro ao alocar cenário.");
        exit(1);
    }

    //Incializa elementos do cenario
    inicia_sons(cenario);

    cenario->porta_aberta = false;

    cenario->qtd_inimigos = 0;
    cenario->mapa = NULL;

    cenario->cristal_x = 0;
    cenario->cristal_y = 4;

    cenario->explosao_x = 7;
    cenario->explosao_y = 4;

    cenario->sprite_cd = 0;

    return cenario;
}

void destroi_cenario(cenario_t * cenario)
{
    free(cenario->mapa[0]);
    free(cenario->mapa);

    destroi_inimigos(cenario);
    free(cenario->inimigos);
}

void inicia_sons(cenario_t * cenario)
{
    //Inicia todos os sons do cenario
    cenario->som_pedra = al_load_sample("./resources/sons/pedra.wav");
    must_init(cenario->som_pedra, "som pedra");

    cenario->som_gameover = al_load_sample("./resources/sons/gameover.wav");
    must_init(cenario->som_gameover, "som gameover");

    cenario->som_cheatcode = al_load_sample("./resources/sons/cheatcode.wav");
    must_init(cenario->som_cheatcode, "som cheatcode");

    cenario->som_abriu_porta = al_load_sample("./resources/sons/abriu_porta.wav");
    must_init(cenario->som_abriu_porta, "som abriu porta");

    cenario->som_pegou_cristal = al_load_sample("./resources/sons/pegou_cristal.wav");
    must_init(cenario->som_pegou_cristal, "som pegou cristal");

    cenario->som_pedra_colidiu = al_load_sample("./resources/sons/pedra_colidiu.wav");
    must_init(cenario->som_pedra_colidiu, "som pegou cristal");

    cenario->som_abriu_instrucoes = al_load_sample("./resources/sons/abriu_instrucao.wav");
    must_init(cenario->som_abriu_instrucoes, "som abriu instrucao");

    cenario->som_colidiu_inimigo = al_load_sample("./resources/sons/colisao.wav");
    must_init(cenario->som_colidiu_inimigo, "som colidiu inimigo");

    cenario->som_explosao = al_load_sample("./resources/sons/explosao.wav");
    must_init(cenario->som_explosao, "som explosao");

    cenario->som_concluiu_fase = al_load_sample("./resources/sons/concluiu_fase.wav");
    must_init(cenario->som_concluiu_fase, "som concluiu fase");
}

void movimenta_player(cenario_t * cenario, player_t * player)
{
    //Posicao que o player ocupava fica vazia, e a nova posicao recebe o player
    cenario->mapa[cenario->posY_player][cenario->posX_player] = ' ';
    cenario->mapa[player->y][player->x] = '@';

    //Atualiza a posicao do player na estrutura
    cenario->posY_player = player->y;
    cenario->posX_player = player->x;
}

void movimenta_inimigo(cenario_t * cenario, inimigo_t * inimigo, int new_x, int new_y)
{
    if (inimigo->vivo)
    {
        //Atualiza a posicao do inimigo no mapa
        cenario->mapa[inimigo->y][inimigo->x] = ' ';
        cenario->mapa[new_y][new_x] = inimigo->rep;

        move_inimigo(inimigo, new_x, new_y);
    }
}

void verifica_ponto(cenario_t * cenario, player_t * player)
{
    //Player pegou um cristal
    if (cenario->mapa[player->y][player->x] == '*')
    {
        al_play_sample(cenario->som_pegou_cristal, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
        player->cristais += 1;
        player->score += cenario->fator_score;
    }
}

void abre_porta(cenario_t * cenario, player_t *player)
{
    //Caso o player nao tenha aberto a porta ainda
    if (!cenario->porta_aberta)
    {
        al_play_sample(cenario->som_abriu_porta, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
        cenario->mapa[cenario->saida_y][cenario->saida_x] = 's';
        cenario->porta_aberta = true;
    }
}

int passou_fase(cenario_t * cenario, player_t * player)
{
    //Posicao do player coincide com a da saida
    return (player->x == cenario->saida_x && player->y == cenario->saida_y);
}

int tempo_acabou(cenario_t * cenario)
{
    //Tempo acabou
    if (cenario->tempo == 0)
        return 1;
    
    //Subtrai o tempo
    cenario->tempo -= 1;
    return 0;
}

void destroi_inimigos(cenario_t * cenario)
{
    //Libera memoria utilizada por todos inimigos da fase
    for (int i = 0 ; i < cenario->qtd_inimigos; i++)
        destroi_inimigo(cenario->inimigos[i]);

    cenario->qtd_inimigos = 0;       
}
