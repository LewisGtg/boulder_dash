#include "player.h"
#include "cenario.h"
#include "allegro.h"
#include "inimigo.h"

#define QTD_FASES 10
#define MAX_PONTUACOES 10
#define BUFSIZE 1024

typedef struct boulder_dash{
    cenario_t * cenario;
    player_t * rockford;
    allegro_t * allegro;

    char ** fases;
    int fase_atual;

    int pontuacoes[MAX_PONTUACOES];
    int tam_ptacoes;

    bool instrucoes;
    bool placar;

    bool done;
    bool redraw;

} boulder_dash;

//Inicia os componentes do jogo
boulder_dash * inicia_game();

//Le as entradas do teclado
void processa_eventos(boulder_dash * bd);

//Atualiza a logica do jogo
void atualiza_logica(boulder_dash * bd);

//Atualiza o display
void atualiza_display(boulder_dash * bd);

//Inicia uma fase
void inicia_fase(boulder_dash * bd);

//Abre um arquivo e carrega o cenario com as informações passadas
void carrega_cenario(cenario_t * cenario, char * arquivo_cenario);

//Le um arquivo txt e exibe na tela o conteudo
void le_arquivo(boulder_dash * bd, char * nome_arq, int tab, int lh);

//Carrega as pontuações existentes
void carrega_scores(boulder_dash * bd);

//Salva uma nova pontuacao
void salva_score(boulder_dash * bd);
