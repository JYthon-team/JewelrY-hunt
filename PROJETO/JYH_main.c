//Principal
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

JYH_GameState* JYH_Init(){//todas as inicializações do jogo vão aqui
	//INICIAR SDL
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
    JYH_GameState* jogo = (JYH_GameState*)malloc(sizeof(JYH_GameState));
    jogo->win = SDL_CreateWindow("Jewelry Hunt",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,JYH_SCREEN_WIDTH, JYH_SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    jogo->ren = SDL_CreateRenderer(jogo->win, -1, 0);
    jogo->fnt = TTF_OpenFont("tiny.ttf", 20);
    
    //outras inicializações abaixo
    jogo->estado = /*JYH_MAIN_MENU*/JYH_state_MM;
    jogo->estado_tela = 0;
    jogo->espera = TEMPO_UPDATE;
    jogo->w_tela = JYH_SCREEN_WIDTH;
    jogo->h_tela = JYH_SCREEN_HEIGHT;
    return jogo;
}

void JYH_EndGame(JYH_GameState* jogo){//todas as terminações do jogo vão aqui
	//outros encerramentos 
	//encerra o SDL
	TTF_CloseFont(jogo->fnt);
	SDL_DestroyRenderer(jogo->ren);
    SDL_DestroyWindow(jogo->win);
    free(jogo);
    SDL_Quit();
}

//Estados do jogo

void JYH_GameRender(JYH_GameState* jogo){
	switch(jogo->estado){//Cada estado é uma tela do jogo
		case /*JYH_MAIN_MENU*/JYH_state_MM:JYH_MM(jogo);break;
		case /*JYH_LVL_EDITOR*/JYH_state_LE:JYH_LE(jogo);break;
		case /*JYH_LVL_EXEC*/JYH_state_EX:JYH_EX(jogo);break;
		case /*JYH_LVL_SELECTION*/JYH_state_LS:JYH_LS(jogo);break;
		case /*JYH_LVL_SELECTION_P*/JYH_state_PL:JYH_PL(jogo);break;
		case /*JYH_WORLD_SELECTION*/JYH_state_WS:JYH_WS(jogo);break;
	}
	SDL_RenderPresent(jogo->ren);
}

int main (int argc, char* args[]){
	JYH_GameState* jogo = JYH_Init();
	while(jogo->estado)JYH_GameRender(jogo);
    JYH_EndGame(jogo);
}


