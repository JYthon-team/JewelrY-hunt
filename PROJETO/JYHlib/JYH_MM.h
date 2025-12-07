//JYH_MM.h
#ifndef JYH_MM_L
#define JYH_MM_L
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "JYH_Icon.h"

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

void JYH_Destroy_MM(JYH_Menu* mm);
//JYH_Load_MM(JYH_Menu* mm);
#endif