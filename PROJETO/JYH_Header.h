//Header Principal
#ifndef JYH_GAME
#define JYH_SCREEN_WIDTH 1200
#define JYH_SCREEN_HEIGHT 700
#define TEMPO_UPDATE 100
#define JYH_GAME
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <assert.h>

typedef struct JYH_Mundo{//estrutura de dados representando um mundo do jogo. Fica armazenada em arquivos para ser carregada para esta estrutura
	//lista dos niveis do mundo e estética na hora de seleção
	char nome[50];//Nome do mundo
    char path[50];//path para arquivo de níveis do mundo
	SDL_Texture* capa;//textura
}JYH_Mundo;
typedef struct JYH_Nivel{//estrutura de dados representando um nível do jogo. Fica armazenada em arquivos para ser carregada para esta estrutura
	//Grid do nível e posições dos objetos
	char nome[50];//nome do nível
    char path[50];//path para o arquivo do lvl
    SDL_Texture* txt_nome;
    Uint32 w,h;//tamanhos discretos
    unsigned char *mat;
}JYH_Nivel;
typedef struct JYH_Objeto{//estrutura de dados representando os objetos de uma fase.
	//Sprites + tipo do objeto e o necessário para gerênciar a atualização
}JYH_Objeto;

int AUX_WaitEventTimeoutCount(SDL_Event* evt, Uint32* ms);
SDL_Texture* AUX_CriarTexto(SDL_Renderer* ren,TTF_Font* fnt,char* str,SDL_Color clr);
void AUX_AdaptarString(char* S);

enum GAME_STATE{
	JYH_END_GAME = 0,//estado para encerrar o jogo
	JYH_state_MM,
	JYH_state_LE,
	JYH_state_EX,
	JYH_state_LS,
	JYH_state_PL,
	JYH_state_WS
};

typedef struct JYH_Menu{//Guarda os elementos necessários para o menu rodar
	SDL_Rect title;
	SDL_Texture* txt_title;
	SDL_Rect botao_worlds;//vai para modo seleção de mundos
	SDL_Texture* txt_worlds;
	SDL_Texture* txt_msg_worlds;//temporario
	
	SDL_Rect botao_edit  ;//vai para modo editor
	SDL_Texture* txt_edit;
	SDL_Texture* txt_msg_edit;//temporario
	
	SDL_Rect botao_selP  ;//vai para modo seleção de níveis do jogador
	SDL_Texture* txt_selP;
	SDL_Texture* txt_msg_selP;//temporario
	
	SDL_Texture* txt_background;
	Uint32 estado;
	//adicionar texturas dos botões
}JYH_Menu;

typedef struct JYH_Editor{//Guarda os elementos necessários para o editor funcionar
	JYH_Nivel lvl;        //Nivel a ser editado
	Uint32 last_idx;
	SDL_bool pintar;//modo pintar ativo
	SDL_bool press;//mouse pressionado
	SDL_bool drag;//modo arrasto ativo
	SDL_Rect top_bar;     //barra de cima(onde ficam os botões para sair, salvar, executar, etc)
	SDL_Texture* txt_tb;
	SDL_Rect side_bar;    //barra lateral(onde ficam os itens)
	SDL_Texture* txt_sb;
	SDL_Rect editor;      //região da tela na qual está o editor
	SDL_Rect botao_voltar;//vai para o estado anterior
	SDL_Texture* txt_voltar;
	SDL_Rect botao_run;   //vai para o estado de execução
	SDL_Texture* txt_run;
	SDL_Rect botao_salvar;//salva as alterações no nível
	SDL_Texture* txt_salvar;
	SDL_Rect botao_paint;
	SDL_Texture* txt_paint;
	//adicionar texturas
}JYH_Editor;

typedef struct JYH_Level_Runner{//Guarda os elementos necessários para a execução de um nível
	JYH_Nivel lvl;//Nivel que está sendo jogado
	char pathNivel[50];
	char pathMundo[50];
	char nome[50];
	SDL_Rect top_bar;     //barra de cima(onde ficam os botões para sair e reiniciar, assim como a contagem de tempo e de gemas)
	SDL_Rect icone_gemas;
	SDL_Rect icone_relogio;
	SDL_Rect contagem_gemas;
	SDL_Rect contagem_tempo;
	SDL_Rect botao_voltar;
	SDL_Rect botao_reiniciar;
	Uint32 gemas_coletadas;//gemas coletadas até o momento no nível
	Uint32 tesouro_pego;//se o artefato do nível foi pego
	Uint32 tempo_de_jogo;//tempo que o nível está sendo jogado
	Uint32 timer;//tempo restante para sair com o artefato após pegar o tesouro
}JYH_Level_Runner;

typedef struct JYH_World_Selection{//Guarda os elementos necessários para a seleção de mundo
	Uint32 n;//quantidade de mundos
	Uint32 idx;//indice do mundo de menor indice
    Uint32 i_sel;//mundo selecionado
	JYH_Mundo* mundos;//lista de mundos
	SDL_Rect botao_voltar;//botão para voltar atrás
	SDL_Texture* txt_voltar;
	SDL_Rect botao_dir;
	SDL_Texture* txt_dir;
	SDL_Rect botao_esq;
	SDL_Texture* txt_esq;
	SDL_Rect title;//"Modo Historia"
	SDL_Texture* txt_title;
	SDL_Texture* txt_background;
}JYH_World_Selection;

typedef struct JYH_Level_Selection_P{//Guarda os elementos necessários para a seleção de níveis criados pelo jogador
	Uint32 n;//quantidade de niveis
    Uint32 i_sel;
	JYH_Nivel* niveis;//lista de niveis
	SDL_Rect title;//"Meus niveis"
    SDL_Texture* txt_title;
	SDL_Rect botao_voltar;//botão para voltar atrás
    SDL_Texture* txt_voltar;
    SDL_Texture* txt_background;
    SDL_Texture* txt_lvl_icon;
}JYH_Level_Selection_P;

typedef struct JYH_Level_Selection{//Guarda os elementos necessários para a tela de seleção de níveis
	Uint32 n;//quantidade de niveis
	Uint32 i_sel;
	char path[50];//path do mundo
	JYH_Nivel* niveis;//lista de níveis
	SDL_Rect title;//nome do mundo
	SDL_Texture* txt_title;
	SDL_Rect botao_voltar;//botão para voltar atrás
	SDL_Texture* txt_voltar;
	SDL_Texture* txt_background;
	SDL_Texture* txt_lvl_icon;//icone de todos os níveis
}JYH_Level_Selection;

typedef struct JYH_GameState{
	enum GAME_STATE estado;//estado do jogo
	Uint32 espera;//coordena o tempo de atualização do jogo
	Uint32 prev;
	Uint32 w_tela, h_tela;//dimensões da tela(caso permitirmos a customização)
	TTF_Font* fnt;
	SDL_Window* win;//janela
	SDL_Renderer* ren;//renderizador
	SDL_Event evt;//evento
	union{
		JYH_Menu              mm;
		JYH_Editor            le;
		JYH_Level_Runner      ex;
		JYH_World_Selection   ws;
		JYH_Level_Selection   ls;
		JYH_Level_Selection_P pl;
	};
}JYH_GameState;

//Inicialização e Encerramento
JYH_GameState* JYH_Init();
void JYH_EndGame(JYH_GameState* jogo);

void JYH_MM(JYH_GameState* jogo);
void JYH_WS(JYH_GameState* jogo);
void JYH_LS(JYH_GameState* jogo);
void JYH_EX(JYH_GameState* jogo);
void JYH_PL(JYH_GameState* jogo);
void JYH_LE(JYH_GameState* jogo);

void JYH_Load_WS(JYH_GameState* jogo);
void JYH_Load_MM(JYH_GameState* jogo);
void JYH_Load_LS(JYH_GameState* jogo);
void JYH_Load_EX(JYH_GameState* jogo);
void JYH_Load_PL(JYH_GameState* jogo);
void JYH_Load_LE(JYH_GameState* jogo);


//Controle principal
void JYH_GameRender(JYH_GameState* jogo);
#endif
