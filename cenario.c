#include "cenario.h"

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

                //Caso encontre mato
                case '.':
                    al_draw_scaled_bitmap(sprites, 48, 48, 16, 16, col_atual * 32, lin_atual * 31.3, 32, 32, 0);
                break;

                //Caso encontre cristais, tanto parados, quanto caindo
                case '*':
                case 65:
                case 66:
                case 67:
                case 68:
                    al_draw_scaled_bitmap(sprites, 0, 64, 16, 16, col_atual * 32, lin_atual * 31.3, 32, 32, 0);
                break;

                case 's':
                    if (cenario->min_cristais <= 0)
                        al_draw_scaled_bitmap(sprites, 16, 48, 16, 16, col_atual * 32, lin_atual * 31.3, 32, 32, 0);
                    else
                        al_draw_scaled_bitmap(sprites, 0, 48, 16, 16, col_atual * 32, lin_atual * 31.3, 32, 32, 0);
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
}

void atualiza_painel(cenario_t * cenario, player_t * player, ALLEGRO_FONT * font)
{
    char cristais[100];
    sprintf(cristais, "%d", player->cristais);

    char score[100];
    sprintf(score, "%d", player->score);

    char tempo[100];
    sprintf(tempo, "%d", cenario->tempo);
    
    al_draw_text(font, al_map_rgb(255, 255, 255), 200, 14, 0, cristais);
    al_draw_text(font, al_map_rgb(255, 255, 255), 1250, 14, 0, score);
    al_draw_text(font, al_map_rgb(255, 255, 255), 1000, 14, 0, tempo);
}

static int eh_cristal(char obj)
{
    return (obj == '*' || (obj >= 65 && obj <= 68));
}

static int eh_pedra(char obj)
{
    return (obj == 'o' || (obj >= 48 && obj <= 51));
}

int pos_valida(char ** mapa, int x, int y)
{
    char prox = mapa[y][x];
    return (prox != '#' && prox != 'o' && !(prox >= 48 && prox <= 51) && !(prox >= 65 && prox <= 68));
}

void gravidade(char ** mapa, player_t * player, int i, int j, char obj, int min, int max)
{
    //Verifica se um objeto caiu na cabeça do player
    if (mapa[i][j] == max && mapa[i+1][j] == '@')
        morte(player);

    //Pedra ou cristal encontrada com espaco vazio embaixo
    if (mapa[i][j] == obj && mapa[i+1][j] == ' ')
        mapa[i][j] = min;

    //Pedra em cima de pedra, ou, cristal em cima de crital
    else if (mapa[i][j] == obj && (mapa[i+1][j] == 'o' || mapa[i+1][j] == '*'))
    {
        //Pedra ou cristal tomba para direita
        if (mapa[i][j+1] == ' ' && mapa[i+1][j+1] == ' ')
        {
            mapa[i][j] = ' ';
            mapa[i][j+1] = min + 1;
        }

        //Pedra ou cristal tomba para esquerda
        else if (mapa[i][j-1] == ' ' && mapa[i+1][j-1] == ' ')
        {
            mapa[i][j] = ' ';
            mapa[i][j-1] = min + 1;
        }
    }
    
    //Tempo de aceleracao da pedra ou cristal
    else if ((mapa[i][j] >= min && mapa[i][j] < max) && mapa[i+1][j] == ' ')
        mapa[i][j] = mapa[i][j] + 1;
    
    //Pedra ou cristal comeca cair
    else if (mapa[i][j] == max && mapa[i+1][j] == ' ')
    {
        mapa[i+1][j] = mapa[i][j];
        mapa[i][j] = ' ';
    }

    //Pedra ou cristal caiu e bateu em objeto, velocidade volta a ser 0
    else if ((mapa[i][j] >= min && mapa[i][j] <= max) && (mapa[i+1][j] != ' ') && (mapa[i+1][j] != '@'))
        mapa[i][j] = obj;


}

void verifica_gravidade(cenario_t * cenario, player_t * player)
{
    for (int i = cenario->lin - 2; i > 0; i--)
        for (int j = 0; j < cenario->col; j++)
        {
            if(eh_pedra(cenario->mapa[i][j]))
                gravidade(cenario->mapa, player, i, j, 'o', 48, 51);

            if(eh_cristal(cenario->mapa[i][j]))
                gravidade(cenario->mapa, player, i, j, '*', 65, 68);
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
    cenario_t * cenario = malloc(sizeof(cenario_t));

    if (!cenario)
    {
        perror("Erro ao alocar cenário.");
        exit(1);
    }

    cenario->mapa = NULL;

    return cenario;
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

    //Aloca memoria para o cenario

    fscanf(arq, "%d %d \n", &cenario->lin, &cenario->col);

    if (!cenario->mapa)
        cenario->mapa = inicia_mapa(cenario->lin, cenario->col);

    for (int i = 0; i < cenario->lin; i++)
    {
        for (int j = 0; j < cenario->col; j++)
        {
            fscanf(arq, "%c", &cenario->mapa[i][j]);       
            
            if (cenario->mapa[i][j] == '@')
            {
                cenario->posY_player = i;
                cenario->posX_player = j;
            }

            if (cenario->mapa[i][j] == 's')
            {
                cenario->saida_y = i;
                cenario->saida_x = j;
            }
        }
        fscanf(arq, "\n");
    }

    fscanf(arq, "%d\n", &cenario->min_cristais);
    fscanf(arq, "%d\n", &cenario->tempo);

    fclose(arq);
}

void movimenta_player(cenario_t * cenario, player_t * player)
{
    cenario->mapa[cenario->posY_player][cenario->posX_player] = ' ';
    cenario->mapa[player->y][player->x] = '@';

    cenario->posY_player = player->y;
    cenario->posX_player = player->x;
}

void verifica_ponto(cenario_t * cenario, player_t * player)
{
    if (cenario->mapa[player->y][player->x] == '*')
    {
        player->cristais += 1;
        player->score += 10;

        cenario->min_cristais -= 12;
    }

}

int passou_fase(cenario_t * cenario, player_t * player)
{
    return (cenario->min_cristais <= 0 && player->x == cenario->saida_x && player->y == cenario->saida_y);
}

int tempo_acabou(cenario_t * cenario)
{
    if (cenario->tempo == 0)
        return 1;
    
    cenario->tempo -= 1;
    return 0;
}
