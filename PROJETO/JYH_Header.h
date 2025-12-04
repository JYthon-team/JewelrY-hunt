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
#include "JYH_Object.h"
#include "JYH_ImagePaths.h"
#include "JYH_Camera.h"

typedef struct JYH_Mundo{//estrutura de dados representando um mundo do jogo. Fica armazenada em arquivos para ser carregada para esta estrutura
	//lista dos niveis do mundo e estética na hora de seleção
	char nome[50];//Nome do mundo
	SDL_Texture* capa;//textura
}JYH_Mundo;

typedef struct JYH_Ass_Nivel{//Assinatura de um nível
    char nome_nivel[50];
    SDL_Texture* txt_nome;
}JYH_Ass_Nivel;

typedef struct JYH_Tile{
	//Uint32 t;//tile a ser desenhado, objeto no tile
    unsigned char t;
	Uint32 o;//obj
}JYH_Tile;

typedef struct JYH_Nivel{//estrutura de dados representando um nível do jogo. Fica armazenada em arquivos para ser carregada para esta estrutura
	//Grid do nível e posições dos objetos
    char nome_mundo[50];
    char nome_nivel[50];
    char       tema[50];
    Uint32 qtd_obj;
    SDL_Texture* txt_nome;
    SDL_Texture* txt_theme;
    JYH_Objeto* objetos;
    int w,h;
    JYH_Tile* mat;
    
}JYH_Nivel;

int AUX_WaitEventTimeoutCount(SDL_Event* evt, Uint32* ms);
SDL_Texture* AUX_CriarTexto(SDL_Renderer* ren,TTF_Font* fnt,char* str,SDL_Color clr);
void AUX_AdaptarString(char* S);
void AUX_Start_Icon(SDL_Renderer* ren, JYH_Icon* i, char* txt_name, SDL_Rect r, Uint32 n);
void AUX_Draw_Icon(SDL_Renderer* ren,JYH_Icon* i);
void AUX_CriarEvento(int code, void* data);

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
	JYH_Icon  titulo;
	JYH_Icon  botao_WS;
	SDL_Texture* txt_msg_worlds;//temporario
	JYH_Icon  botao_LE;
	SDL_Texture* txt_msg_edit;//temporario
	JYH_Icon  botao_PL;
	SDL_Texture* txt_msg_selP;//temporario
	SDL_Texture* txt_background;
	Uint32 estado;
	//adicionar texturas dos botões
}JYH_Menu;


enum JYH_PINCEL{//mouse no editor
	PINCEL_DESOCUPADO,
	PINCEL_PINTANDO,
	PINCEL_APAGANDO,
	PINCEL_ARRASTANDO,
	PINCEL_MOVER_CAMERA
};
typedef struct JYH_Theme{
	char nome[50];
	SDL_Texture* txt;
}JYH_Theme;

typedef struct JYH_Editor{//Guarda os elementos necessários para o editor funcionar
	JYH_Nivel lvl;        //Nivel a ser editado
	JYH_Camera cam;
	SDL_bool press;//mouse pressionado
	enum JYH_PINCEL pincel;
	Uint32 n_obj;
    Uint32 sel_obj;//índice do elemento sendo arrastado
    SDL_Point mouse_tile;
    //Lista de todos os objetos(texturas) do jogo
	JYH_Ass_Obj* objetos;
	Uint32 n_theme;
	Uint32 i_theme;//tema selecionado
	JYH_Theme * temas;//lista dos temas
	JYH_Icon tb;
	JYH_Icon sb;
	JYH_Icon botao_V;//volta
	JYH_Icon botao_R;//run
	JYH_Icon botao_S;//salva
	JYH_Icon botao_P;//pinta
	JYH_Icon botao_A;//apaga
	JYH_Icon botao_T;//tema da fase
    JYH_Icon botao_ZoomIn;//Aumenta o Zoom
    JYH_Icon botao_ZoomOut;//Diminui o Zoom
    SDL_Texture* txt_frame;
    SDL_Texture* txt_sel;
}JYH_Editor;

typedef struct JYH_Level_Runner{//Guarda os elementos necessários para a execução de um nível
	JYH_Nivel lvl;//Nivel que está sendo jogado
	JYH_Camera cam;
	JYH_Icon tb;
	JYH_Icon gem;
	JYH_Icon clock;
	JYH_Icon botao_V;
	JYH_Icon botao_R;

	SDL_Rect contagem_gemas;
    SDL_Texture* txt_gem_count;
	SDL_Rect contagem_tempo;
    SDL_Texture* txt_tempo;

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
	JYH_Icon titulo;
	JYH_Icon botao_V;
	JYH_Icon botao_D;
	JYH_Icon botao_E;
	SDL_Texture* txt_background;
}JYH_World_Selection;

typedef struct JYH_Level_Selection_P{//Guarda os elementos necessários para a seleção de níveis criados pelo jogador
	Uint32 n;//quantidade de niveis
    Uint32 i_sel;
    JYH_Ass_Nivel* niveis;
    JYH_Icon titulo;
    JYH_Icon botao_V;
    SDL_Texture* txt_background;
    SDL_Texture* txt_lvl_icon;
}JYH_Level_Selection_P;

typedef struct JYH_Level_Selection{//Guarda os elementos necessários para a tela de seleção de níveis
	Uint32 n;//quantidade de niveis
	Uint32 i_sel;
	char nome[50];//nome do mundo
    JYH_Ass_Nivel* niveis;
	JYH_Icon titulo;
	JYH_Icon botao_V;
	SDL_Texture* txt_background;
	SDL_Texture* txt_lvl_icon;//icone de todos os níveis
}JYH_Level_Selection;

typedef struct JYH_Pilha{
	enum GAME_STATE p[10];
	int f;
}JYH_Pilha;

enum GAME_STATE AUX_Top(JYH_Pilha* P);
void AUX_Empilha(JYH_Pilha* P,enum GAME_STATE s);
void AUX_Desempilha(JYH_Pilha* P);


typedef struct JYH_GameState{
	JYH_Pilha state;
	Uint32 espera;//coordena o tempo de atualização do jogo
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

void JYH_Converter_TelaMundo(SDL_Point* p,JYH_Camera* cam);
void JYH_Converter_MundoTela(SDL_Point* p,JYH_Camera* cam);
void JYH_Draw_Grade_Cam(SDL_Renderer* ren,JYH_Nivel* lvl, JYH_Camera* cam, JYH_Ass_Obj* obj);
void JYH_Move_Camera(JYH_Camera* cam,JYH_Nivel* lvl,int dx,int dy);
void JYH_Inicia_Camera(JYH_Camera* cam,SDL_Rect r_box,SDL_Rect r_cam, Uint32 zoom);

void JYH_Read_lvl(JYH_Nivel* lvl);
//Controle principal
void JYH_GameRender(JYH_GameState* jogo);
#endif
