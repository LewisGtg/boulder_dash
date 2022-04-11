#include "cenario.h"

void atualiza_cenario(cenario_t cenario, ALLEGRO_BITMAP * sprites)
{
    //"Limpa" a tela
    al_clear_to_color(al_map_rgb(0, 0, 0));

    //Percorre a matriz do mapa e carrega as sprites de acordo com seu tipo
    int lin_atual = 1;
    for (int y = 0; y < cenario.lin; y++)
    {
        int col_atual = 0;
        for (int x = 0; x < cenario.col; x++)
        {
            char atual = cenario.mapa[y][x];

            switch(atual)
            {
                //Caso encontre muro
                case '#':
                    if ((y > 0 && y < cenario.lin - 1) && (x > 0 && x < cenario.col - 1))
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

                //Caso encontre cristal
                case '*':
                case 65:
                case 66:
                case 67:
                case 68:
                    al_draw_scaled_bitmap(sprites, 0, 64, 16, 16, col_atual * 32, lin_atual * 31.3, 32, 32, 0);
                break;

                //Caso encontre player
                case '@':
                    al_draw_scaled_bitmap(sprites, 0, 0, 16, 16, col_atual * 32, lin_atual * 31.3, 32, 32, 0);
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

    al_flip_display();
}

static int eh_cristal(char obj)
{
    return (obj == '*' || (obj >= 65 && obj <= 68));
}

static int eh_pedra(char obj)
{
    return (obj == 'o' || (obj >= 48 && obj <= 51));
}

void gravidade(char ** mapa, int i, int j, char obj, int min, int max)
{
    //Pedra ou cristal encontrada com espaco vazio embaixo
    if (mapa[i][j] == obj && mapa[i+1][j] == ' ')
        mapa[i][j] = min;

    //Pedra em cima de pedra, ou, cristal em cuma de crital
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
    else if ((mapa[i][j] >= min && mapa[i][j] <= max) && mapa[i+1][j] != ' ')
        mapa[i][j] = obj;
}

void verifica_gravidade(cenario_t cenario)
{
    for (int i = cenario.lin - 2; i > 0; i--)
    for (int j = 0; j < cenario.col; j++)
    {
        if(eh_pedra(cenario.mapa[i][j]))
            gravidade(cenario.mapa, i, j, 'o', 48, 51);

        if(eh_cristal(cenario.mapa[i][j]))
            gravidade(cenario.mapa, i, j, '*', 65, 68);
    }
}

