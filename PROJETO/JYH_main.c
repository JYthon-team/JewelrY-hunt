//Principal
#include "JYH_Header.h"

JYH_GameState* JYH_Init(){//todas as inicializações do jogo vão aqui
	//INICIAR SDL
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
    JYH_GameState* jogo = (JYH_GameState*)malloc(sizeof(JYH_GameState));
    jogo->win = SDL_CreateWindow("Jewelry Hunt",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,JYH_SCREEN_WIDTH, JYH_SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    jogo->ren = SDL_CreateRenderer(jogo->win, -1, 0);
    jogo->fnt = TTF_OpenFont("tiny.ttf", 20);
    
    jogo->state.p[0] = JYH_state_MM;
    jogo->state.f = 0;
    jogo->espera = TEMPO_UPDATE;
    JYH_Load_MM(jogo);
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
	switch(AUX_Top(&jogo->state)){//Cada estado é uma tela do jogo
		case JYH_state_MM:JYH_MM(jogo);break;
		case JYH_state_LE:JYH_LE(jogo);break;
		case JYH_state_EX:JYH_EX(jogo);break;
		case JYH_state_LS:JYH_LS(jogo);break;
		case JYH_state_PL:JYH_PL(jogo);break;
		case JYH_state_WS:JYH_WS(jogo);break;
	}
	SDL_RenderPresent(jogo->ren);
}

int main (int argc, char* args[]){
	JYH_GameState* jogo = JYH_Init();
	while(AUX_Top(&jogo->state))JYH_GameRender(jogo);
    JYH_EndGame(jogo);
}


