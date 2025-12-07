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
#include "JYH_ImagePaths.h"

#include "JYH_MM.h"
#include "JYH_LE.h"
#include "JYH_LS.h"
#include "JYH_EX.h"
#include "JYH_WS.h"
#include "JYH_PL.h"
#include "JYH_Object.h"
#include "JYH_Camera.h"
#include "JYH_Nivel.h"
#include "JYH_Icon.h"

int AUX_WaitEventTimeoutCount(SDL_Event* evt, Uint32* ms);
SDL_Texture* AUX_CriarTexto(SDL_Renderer* ren,TTF_Font* fnt,char* str,SDL_Color clr);
void AUX_AdaptarString(char* S);
void AUX_Start_Icon(SDL_Renderer* ren, JYH_Icon* i, char* txt_name, SDL_Rect r, Uint32 n);
void AUX_Draw_Icon(SDL_Renderer* ren,JYH_Icon* i);
void AUX_CriarEvento(int code, void* data1, void* data2);

enum GAME_STATE{
	JYH_END_GAME = 0,//estado para encerrar o jogo
	JYH_state_MM,
	JYH_state_LE,
	JYH_state_EX,
	JYH_state_LS,
	JYH_state_PL,
	JYH_state_WS
};
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

void JYH_Draw_Grade_Cam(SDL_Renderer* ren,JYH_Nivel* lvl, JYH_Camera* cam, JYH_Ass_Obj* obj);
void JYH_Draw_Grade_EX(SDL_Renderer* ren,JYH_Nivel* lvl, JYH_Camera* cam, JYH_Objeto** obj);

void JYH_Read_lvl(JYH_Nivel* lvl);
//Controle principal
void JYH_GameRender(JYH_GameState* jogo);
#endif
