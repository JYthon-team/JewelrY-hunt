#include "JYH_Header.h"

int AUX_WaitEventTimeoutCount(SDL_Event* evt, Uint32* ms){
	Uint32 antes = SDL_GetTicks();
	int temEvento = SDL_WaitEventTimeout(evt,*ms);
	(*ms) = (*ms)- (SDL_GetTicks()-antes);
	if(*ms > 10000) (*ms) = 0;//elimina underflow(trava o jogo)
	return temEvento;
}
SDL_Texture* AUX_CriarTexto(SDL_Renderer* ren,TTF_Font* fnt,char* str,SDL_Color clr){
    SDL_Surface* sfc = TTF_RenderText_Blended(fnt, str, clr);
    assert(sfc != NULL);
    SDL_Texture* txt = SDL_CreateTextureFromSurface(ren, sfc);
    assert(txt != NULL);
    SDL_FreeSurface(sfc);
    return txt;
}
void AUX_AdaptarString(char* S){//lê um path em um dado formato e apapta para a plataforma linux ou windows
	int i = 0;
	while(S[i] != '\0'){
		if(S[i] == '$'){
			#ifdef _WIN32
			S[i] = '\\';
			#elif __linux__
			S[i] = '/';
			#endif
		}
		i++;
    }
    printf(" ");//por alguma razão, quebra se não existe isto
}
void AUX_Start_Icon(SDL_Renderer* ren, JYH_Icon* i, char* txt_name, SDL_Rect r, Uint32 n){
	//printf("%s\n",txt_name);
	i->txt = IMG_LoadTexture(ren,txt_name);
	i->f   = 0;
	i->s   = 0;
	i->r   = r;
	i->n_f = n;
	assert(i->txt != NULL);
}
void AUX_Draw_Icon(SDL_Renderer* ren,JYH_Icon* i){
	static SDL_Rect c;
	c = (SDL_Rect){i->f*i->r.w,i->s*i->r.h,i->r.w,i->r.h};
	SDL_RenderCopy(ren,i->txt,&c,&i->r);
}

enum GAME_STATE AUX_Top(JYH_Pilha* P){return P->p[P->f];}
void AUX_Empilha(JYH_Pilha* P,enum GAME_STATE s){P->p[++P->f] = s;}
void AUX_Desempilha(JYH_Pilha* P){P->f--;}
