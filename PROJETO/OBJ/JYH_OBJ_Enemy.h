#ifndef JYH_OBJ_ENEMY_L
#define JYH_OBJ_ENEMY_L
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
typedef struct JYH_Obj_Enemy{
	SDL_Texture* txt;
	SDL_Rect r;
	Uint32 s;
	Uint32 f;
	Uint32 n_f;
	
	SDL_Rect* target;//o jogador
	JYH_Nivel* lvl;
}JYH_Obj_Enemy;
#endif