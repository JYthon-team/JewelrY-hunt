//Principal
#include "JYH_Header.h"

int AUX_WaitEventTimeoutCount(SDL_Event* evt, Uint32* ms){
	Uint32 antes = SDL_GetTicks();
	int temEvento = SDL_WaitEventTimeout(evt,*ms);
	(*ms) = (temEvento)?(*ms)- (SDL_GetTicks()-antes):TEMPO_UPDATE;
	return temEvento;
}

JYH_GameState* JYH_Init(){//todas as inicializações do jogo vão aqui
	//INICIAR SDL
	SDL_Init(SDL_INIT_EVERYTHING);
    JYH_GameState* jogo = (JYH_GameState*)malloc(sizeof(JYH_GameState));
    jogo->win = SDL_CreateWindow("Jewelry Hunt",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,JYH_SCREEN_WIDTH, JYH_SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    jogo->ren = SDL_CreateRenderer(jogo->win, -1, 0);
    
    //outras inicializações abaixo
    jogo->estado = JYH_LOAD_MENU;
    jogo->espera = TEMPO_UPDATE;
    jogo->w_tela = JYH_SCREEN_WIDTH;
    jogo->h_tela = JYH_SCREEN_HEIGHT;
    return jogo;
}

void JYH_EndGame(JYH_GameState* jogo){//todas as terminações do jogo vão aqui
	//outros encerramentos 
	
	//encerra o SDL
	SDL_DestroyRenderer(jogo->ren);
    SDL_DestroyWindow(jogo->win);
    free(jogo);
    SDL_Quit();
}

//Estados do jogo

void JYH_GameRender(JYH_GameState* jogo){
	switch(jogo->estado){
		//estados do jogo
		case JYH_MAIN_MENU:
			JYH_GameMenu(jogo);
			break;
		case JYH_LVL_EDITOR:
			JYH_GameLvlEditor(jogo);
			break;
		case JYH_LVL_EXEC:
			JYH_GameLvlExecution(jogo);
			break;
		case JYH_LVL_SELECTION:
			JYH_GameLvlSelection(jogo);
			break;
		case JYH_LVL_SELECTION_P:
			JYH_GameLvlSelection_P(jogo);
			break;
		case JYH_WORLD_SELECTION:
			JYH_GameWorldSelection(jogo);
			break;
		//estados de transição
		case JYH_LOAD_MENU:
			JYH_GameLoadMenu(jogo);
			break;
		case JYH_LOAD_EDITOR:
			JYH_GameLoadEditor(jogo);
			break;
		case JYH_LOAD_LEVEL:
			JYH_GameLoadExec(jogo);
			break;
		case JYH_LOAD_WORLD_SELECTION:
			JYH_GameLoadWorlds(jogo);
			break;
		case JYH_LOAD_WORLD:
			JYH_GameLoadSel(jogo);
			break;
		case JYH_LOAD_PLAYER_LIBRARY:
			JYH_GameLoadSelP(jogo);
			break;
	}
	SDL_RenderPresent(jogo->ren);
}

int main (int argc, char* args[]){
	JYH_GameState* jogo = JYH_Init();
	while(jogo->estado)JYH_GameRender(jogo);
    JYH_EndGame(jogo);
}


