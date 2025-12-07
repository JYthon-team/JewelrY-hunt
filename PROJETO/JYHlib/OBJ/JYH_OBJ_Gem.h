#ifndef JYH_OBJ_GEM_L
#define JYH_OBJ_GEM_L
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
typedef struct JYH_Obj_Gem{	
	Uint32 type;
	SDL_Texture* txt;
	SDL_Rect r;
	Uint32 s;
	Uint32 f;
	Uint32 n_f;
}JYH_Obj_Gem;
#endif