//JYH_LS.h
#ifndef JYH_LS_L
#define JYH_LS_L
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "JYH_Nivel.h"
#include "JYH_Icon.h"

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
void JYH_Destroy_LS(JYH_Level_Selection* ex);
//JYH_Load_LS(JYH_Level_Selection* ex);
#endif