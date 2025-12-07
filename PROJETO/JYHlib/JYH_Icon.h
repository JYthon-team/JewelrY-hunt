//JYH_Icon.h
#ifndef JYH_ICON_L
#define JYH_ICON_L
typedef struct JYH_Icon{
	SDL_Texture* txt;//textura do botão
	SDL_Rect       r;//Retângulo de desenho
	Uint32         f;//frame
	Uint32         s;//estado
	Uint32       n_f;//quantidade de frames
}JYH_Icon;
#endif