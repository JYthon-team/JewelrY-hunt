//JYH_Object.h, podemos importar todos os objetos
#ifndef JYH_OBJ_L
#define JYH_OBJ_L

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
typedef struct JYH_Level_Runner JYH_Level_Runner;
typedef struct JYH_Camera JYH_Camera;
typedef struct JYH_Nivel JYH_Nivel;
#include "OBJ/JYH_OBJ_Player.h"
#include "OBJ/JYH_OBJ_Gem.h"
#include "OBJ/JYH_OBJ_Trofeu.h"
#include "OBJ/JYH_OBJ_Enemy.h"

//typedef struct JYH_Level_Runner JYH_Level_Runner;
enum JYH_OBJ_TYPE{
	JYH_OBJ_PLAYER,
	JYH_OBJ_GEM,
	JYH_OBJ_TROFEU,
	JYH_OBJ_INIMIGO,
	
	N_OBJECTS
};

typedef struct {
	SDL_Texture* txt;
	SDL_Rect r;
	Uint32 s;
	Uint32 f;
	Uint32 n_f;
}JYH_Obj_Generic;

typedef struct JYH_Ass_Obj{
	char nome[50];
	SDL_Texture* txt;
    Uint32 s;//estado
    Uint32 lim;
    Uint32 qtd;
}JYH_Ass_Obj;

typedef struct {//estrutura de dados representando os objetos de uma fase.
	Uint32 type;
	union{
		JYH_Obj_Player o_p;
		JYH_Obj_Trofeu o_t;
		JYH_Obj_Gem    o_g;
		JYH_Obj_Enemy  o_e;
		JYH_Obj_Generic g;//facilita o acesso na hora de desenhar
	};
	//Sprites + tipo do objeto e o necessário para gerênciar a atualização
}JYH_Objeto;

void JYH_Update_Obj(JYH_Objeto* obj, SDL_Event* evt);
void JYH_Start_Obj(JYH_Objeto* obj,JYH_Level_Runner* ex,  int idx);
int JYH_Comp_Obj(const void* p1,const void* p2);
//void JYH_Draw_Obj(JYH_GameState* jogo,JYH_Objeto* obj);
#endif