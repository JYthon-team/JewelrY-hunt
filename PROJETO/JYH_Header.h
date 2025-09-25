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
#include <assert.h>

typedef struct JYH_Mundo{//estrutura de dados representando um mundo do jogo. Fica armazenada em arquivos para ser carregada para esta estrutura
	//lista dos niveis do mundo e estética na hora de seleção
	char nome[50];//Nome do mundo
	char pathW[50];//path para a lista de níveis(Windows)
    char pathL[50];//path Linux
	SDL_Texture* capa;//textura
}JYH_Mundo;
typedef struct JYH_Nivel{//estrutura de dados representando um nível do jogo. Fica armazenada em arquivos para ser carregada para esta estrutura
	//Grid do nível e posições dos objetos
	char nome[50];
	char pathW[50];
    char pathL[50];
}JYH_Nivel;
typedef struct JYH_Objeto{//estrutura de dados representando os objetos de uma fase.
	//Sprites + tipo do objeto e o necessário para gerênciar a atualização
}JYH_Objeto;

int AUX_WaitEventTimeoutCount(SDL_Event* evt, Uint32* ms);

enum GAME_STATE{
	JYH_END_GAME = 0,//estado para encerrar o jogo
	JYH_MAIN_MENU,//menu inicial
	JYH_LVL_EDITOR,//editor de niveis
	JYH_LVL_EXEC,//modo execução
	JYH_LVL_SELECTION,//seleção de níveis do jogo
	JYH_LVL_SELECTION_P,//seleção de níveis do player
	JYH_WORLD_SELECTION,//escolher mundo
	
	//Os Loads serão estados para carregar texturas e só apresentarão uma tela de loading - Implementar como estados transicionais
	
	JYH_LOAD_MENU,//carrega o menu principal
	JYH_LOAD_EDITOR,//carrega o editor
	JYH_LOAD_LEVEL,//carrega um nível para execução
	JYH_LOAD_WORLD,//carrega um mundo
	JYH_LOAD_WORLD_SELECTION,//carrega o seletor de mundos
	JYH_LOAD_PLAYER_LIBRARY//carrega a seleção de níveis do jogador
};

typedef struct JYH_Menu{//Guarda os elementos necessários para o menu rodar
    Uint32 estado_tela;//estado da tela
	SDL_Rect title;
	SDL_Rect botao_worlds;//vai para modo seleção de mundos
	SDL_Rect botao_edit  ;//vai para modo editor
	SDL_Rect botao_selP  ;//vai para modo seleção de níveis do jogador
	Uint32 estado;
	//adicionar texturas dos botões
}JYH_Menu;

typedef struct JYH_Editor{//Guarda os elementos necessários para o editor funcionar
    Uint32 estado_tela;//estado da tela
	JYH_Nivel lvl;        //Nivel a ser editado
	SDL_Rect top_bar;     //barra de cima(onde ficam os botões para sair, salvar, executar, etc)
	SDL_Rect side_bar;    //barra lateral(onde ficam os itens)
	SDL_Rect editor;      //região da tela na qual está o editor
	SDL_Rect botao_voltar;//vai para o estado anterior
	SDL_Rect botao_run;   //vai para o estado de execução
	SDL_Rect botao_salvar;//salva as alterações no nível
	//adicionar texturas
}JYH_Editor;

typedef struct JYH_Level_Runner{//Guarda os elementos necessários para a execução de um nível
    Uint32 estado_tela;
	JYH_Nivel lvl;//Nivel que está sendo jogado
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
    Uint32 estado_tela;
	Uint32 n;//quantidade de mundos
	Uint32 idx;//indice do mundo de menor indice
    Uint32 i_sel;//mundo selecionado
	JYH_Mundo* mundos;//lista de mundos
	SDL_Rect botao_voltar;//botão para voltar atrás
	SDL_Rect botao_dir;
	SDL_Rect botao_esq;
	SDL_Rect title;//"Modo Historia"
}JYH_World_Selection;

typedef struct JYH_Level_Selection_P{//Guarda os elementos necessários para a seleção de níveis criados pelo jogador
    Uint32 estado_tela;//estado da tela
	Uint32 n_niveis;//quantidade de niveis
	JYH_Nivel* niveis;//lista de niveis
	SDL_Rect title;//"Meus niveis"
	SDL_Rect botao_voltar;//botão para voltar atrás
}JYH_Level_Selection_P;

typedef struct JYH_Level_Selection{//Guarda os elementos necessários para a tela de seleção de níveis
    Uint32 estado_tela;//estado da tela
	Uint32 n_niveis;//quantidade de niveis
	JYH_Nivel* niveis;//lista de níveis
	SDL_Rect title;//nome do mundo
	char pathW[50];//path do mundo
    char pathL[50];
	SDL_Rect botao_voltar;//botão para voltar atrás
}JYH_Level_Selection;

typedef struct JYH_GameState{
	enum GAME_STATE estado;//estado do jogo
	enum GAME_STATE estado_anterior;//estado do jogo anterior
	Uint32 espera;//coordena o tempo de atualização do jogo
	Uint32 w_tela, h_tela;//dimensões da tela(caso permitirmos a customização)
	SDL_Window* win;//janela
	SDL_Renderer* ren;//renderizador
	SDL_Event evt;//evento
	union{
		JYH_Menu menu;
		JYH_Editor edit;
		JYH_Level_Runner exec;
		JYH_World_Selection worlds;
		JYH_Level_Selection sel;
		JYH_Level_Selection_P selP;
	};
}JYH_GameState;

//Inicialização e Encerramento
JYH_GameState* JYH_Init();
void JYH_EndGame(JYH_GameState* jogo);

//Estados do Jogo
void JYH_GameMenu(JYH_GameState* jogo);
void JYH_GameLvlEditor(JYH_GameState* jogo);
void JYH_GameLvlExecution(JYH_GameState* jogo);
void JYH_GameLvlSelection(JYH_GameState* jogo);
void JYH_GameLvlSelection_P(JYH_GameState* jogo);
void JYH_GameWorldSelection(JYH_GameState* jogo);

void JYH_WS(JYH_GameState* jogo);

//Estados de Transição
void JYH_GameLoadMenu(JYH_GameState* jogo);
void JYH_GameLoadEditor(JYH_GameState* jogo);
void JYH_GameLoadExec(JYH_GameState* jogo);
void JYH_GameLoadSel(JYH_GameState* jogo);
void JYH_GameLoadSelP(JYH_GameState* jogo);
void JYH_GameLoadWorlds(JYH_GameState* jogo);

//Controle principal
void JYH_GameRender(JYH_GameState* jogo);
#endif
