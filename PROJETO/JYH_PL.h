//JYH_PL.h
#ifndef JYH_PL_L
#define JYH_PL_L
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "JYH_Nivel.h"
#include "JYH_Icon.h"

typedef struct JYH_Level_Selection_P{//Guarda os elementos necessários para a seleção de níveis criados pelo jogador
	Uint32 n;//quantidade de niveis
    Uint32 i_sel;
    JYH_Ass_Nivel* niveis;
    JYH_Icon titulo;
    JYH_Icon botao_V;
    SDL_Texture* txt_background;
    SDL_Texture* txt_lvl_icon;
}JYH_Level_Selection_P;
#endif