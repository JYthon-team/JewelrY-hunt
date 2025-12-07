#ifndef JYH_OBJ_PLAYER_L
#define JYH_OBJ_PLAYER_L
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

typedef struct JYH_Obj_Player{
	Uint32 type;
	SDL_Texture* txt;
	SDL_Rect r;
	Uint32 s;
	Uint32 f;
	Uint32 n_f;
	//adicionar depois disso coisas específicas
	JYH_Camera* cam;
	JYH_Nivel* lvl;
	short K[4];
}JYH_Obj_Player;
#endif