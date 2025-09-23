//Menu Principal
#include "JYH_Header.h"

void JYH_GameMenu(JYH_GameState* jogo){
	static SDL_Point p;
	
	SDL_SetRenderDrawColor(jogo->ren,0xff,0xff,0xff,0x00);//background
	SDL_RenderClear(jogo->ren);
	
	SDL_SetRenderDrawColor(jogo->ren,0xff,0xff,0x00,0x00);
	SDL_RenderFillRect(jogo->ren,&jogo->menu.title);
	
	if(AUX_WaitEventTimeoutCount(&(jogo->evt),&(jogo->espera))){
		switch(jogo->evt.type){
			case SDL_MOUSEBUTTONDOWN://verifica os cliques do botão
				p.x = (int)jogo->evt.button.x; p.y = (int)jogo->evt.button.y;
				
				if (SDL_PointInRect(&p,&jogo->menu.botao_worlds))jogo->estado = /*JYH_WORLD_SELECTION*/JYH_LOAD_WORLD_SELECTION;
				else if (SDL_PointInRect(&p,&jogo->menu.botao_selP))jogo->estado = /*JYH_LVL_SELECTION_P*/JYH_LOAD_PLAYER_LIBRARY;
				else if (SDL_PointInRect(&p,&jogo->menu.botao_edit))jogo->estado = /*JYH_LVL_EDITOR*/JYH_LOAD_EDITOR;
				
				break;
			case SDL_QUIT:
				jogo->estado = JYH_END_GAME;
				break;
		}
	}else{
		//eventos baseados em tempo
	}
	
	//desenhar botões
	
	SDL_SetRenderDrawColor(jogo->ren,0xff,0x00,0x00,0x00);//cor botão
	SDL_RenderFillRect(jogo->ren,&jogo->menu.botao_worlds);
	SDL_RenderFillRect(jogo->ren,&jogo->menu.botao_selP);
	SDL_RenderFillRect(jogo->ren,&jogo->menu.botao_edit);
}

void JYH_GameLoadMenu(JYH_GameState* jogo){
	SDL_SetRenderDrawColor(jogo->ren,0xff,0xff,0xff,0x00);//trocar por uma tela de loading
	SDL_RenderClear(jogo->ren);
	SDL_RenderPresent(jogo->ren);
	
	jogo->menu.title = (SDL_Rect){450,200,300,90};
	jogo->menu.botao_worlds = (SDL_Rect){450,450,300,30};
	jogo->menu.botao_selP = (SDL_Rect){450,500,300,30};
	jogo->menu.botao_edit = (SDL_Rect){450,550,300,30};
	
	printf("Menu\n");
	jogo->estado = JYH_MAIN_MENU;
}



