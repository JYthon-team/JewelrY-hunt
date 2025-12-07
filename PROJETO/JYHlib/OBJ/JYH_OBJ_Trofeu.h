#ifndef JYH_OBJ_TROFEU_L
#define JYH_OBJ_TROFEU_L
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
typedef struct JYH_Obj_Trofeu{
	Uint32 type;
	SDL_Texture* txt;
	SDL_Rect r;
	Uint32 s;
	Uint32 f;
	Uint32 n_f;
}JYH_Obj_Trofeu;
#endif