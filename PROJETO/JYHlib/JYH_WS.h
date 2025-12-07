//JYH_WS.h
#ifndef JYH_WS_L
#define JYH_WS_L
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "JYH_Icon.h"

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

void JYH_Destroy_WS(JYH_World_Selection* ws);
//JYH_Load_WS(JYH_World_Selection* ws);
#endif