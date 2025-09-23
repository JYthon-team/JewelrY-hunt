//Seleção de Níveis do player
#include "JYH_Header.h"

void JYH_GameLvlSelection_P(JYH_GameState* jogo){//Atualizar
	static SDL_Point p;
	
	SDL_SetRenderDrawColor(jogo->ren,0xff,0xff,0xff,0x00);//background
	SDL_RenderClear(jogo->ren);
	SDL_SetRenderDrawColor(jogo->ren,0xff,0xff,0x00,0x00);//"Meus Niveis"
	SDL_RenderFillRect(jogo->ren,&jogo->selP.title);
	
	if(AUX_WaitEventTimeoutCount(&(jogo->evt),&(jogo->espera))){//trocar por exercicio
		switch(jogo->evt.type){
			case SDL_MOUSEBUTTONDOWN://verifica os cliques do botão
				p.x = (int)jogo->evt.button.x; p.y = (int)jogo->evt.button.y;
				
				if (SDL_PointInRect(&p,&jogo->selP.botao_voltar))jogo->estado = JYH_LOAD_MENU;
				
				break;
			case SDL_QUIT:
				jogo->estado = JYH_END_GAME;
				break;
		}
	}else{
		//eventos baseados em tempo
	}
	
	SDL_SetRenderDrawColor(jogo->ren,0xff,0x00,0x00,0x00);//cor botão 1
	SDL_RenderFillRect(jogo->ren,&jogo->selP.botao_voltar);
}

void JYH_GameLoadSelP(JYH_GameState* jogo){
	SDL_SetRenderDrawColor(jogo->ren,0xff,0xff,0xff,0x00);//trocar por uma tela de loading
	SDL_RenderClear(jogo->ren);
	SDL_RenderPresent(jogo->ren);
	
	jogo->selP.title = (SDL_Rect){450,100,300,90};
	jogo->selP.botao_voltar = (SDL_Rect){25,25,50,50};
	jogo->selP.n_niveis = 10;//numero arbitrario temporario para testar a interface
	//jogo->selP.niveis
	
	printf("Biblioteca\n");
	jogo->estado = JYH_LVL_SELECTION_P;
}
