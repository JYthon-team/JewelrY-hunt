#ifndef JYH_OBJ_TROFEU_L
#define JYH_OBJ_TROFEU_L
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
//#include "../JYH_Nivel.h"
//#include "../JYH_EX.h"
//typedef struct JYH_Level_Runner JYH_Level_Runner;
typedef struct JYH_Obj_Trofeu{
	SDL_Texture* txt;
	SDL_Rect r;
	Uint32 s;
	Uint32 f;
	Uint32 n_f;
}JYH_Obj_Trofeu;

//void JYH_OBJ_TROFEU_START(JYH_Obj_Trofeu* obj, JYH_Level_Runner* ex, int idx);
//void JYH_OBJ_TROFEU_UPDATE(JYH_Obj_Trofeu* obj, SDL_Event* evt);
#endif