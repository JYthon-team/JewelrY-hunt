#ifndef JYH_OBJ_GEM_L
#define JYH_OBJ_GEM_L
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
typedef struct JYH_Obj_Gem{	
	SDL_Texture* txt;
	SDL_Rect r;
	Uint32 s;
	Uint32 f;
	Uint32 n_f;
}JYH_Obj_Gem;
//void JYH_OBJ_GEM_START(JYH_Obj_Gem* obj, JYH_Level_Runner* ex, int idx);
//void JYH_OBJ_GEM_UPDATE(JYH_Obj_Gem* obj, SDL_Event* evt);
#endif