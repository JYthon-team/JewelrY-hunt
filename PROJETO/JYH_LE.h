//JYH_LE.h
#ifndef JYH_LE_L
#define JYH_LE_L
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "JYH_Nivel.h"
#include "JYH_Object.h"
#include "JYH_Camera.h"
#include "JYH_Icon.h"
typedef struct JYH_Nivel JYH_Nivel;
typedef struct JYH_Object JYH_Object;
typedef struct JYH_Camera JYH_Camera;
typedef struct JYH_Icon   JYH_Icon;

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

void JYH_Destroy_LE(JYH_Editor* ex);
//JYH_Load_LE(JYH_Editor* ex);
#endif