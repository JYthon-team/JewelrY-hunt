//JYH_EX.h
#ifndef JYH_EX_L
#define JYH_EX_L
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "JYH_Nivel.h"
#include "JYH_Object.h"
#include "JYH_Icon.h"
#include "JYH_Camera.h"

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

	SDL_Texture** txts;

	Uint32 gemas_coletadas;//gemas coletadas até o momento no nível
	Uint32 tesouro_pego;//se o artefato do nível foi pego
	Uint32 tempo_de_jogo;//tempo que o nível está sendo jogado
	Uint32 timer;//tempo restante para sair com o artefato após pegar o tesouro
}JYH_Level_Runner;
#endif