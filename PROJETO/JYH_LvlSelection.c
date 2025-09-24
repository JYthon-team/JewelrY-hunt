//Selecionar Nível
#include "JYH_Header.h"

void JYH_GameLvlSelection(JYH_GameState* jogo){//Atualizar
	static SDL_Point p;

	SDL_SetRenderDrawColor(jogo->ren,0xff,0xff,0xff,0x00);//background
	SDL_RenderClear(jogo->ren);
	SDL_SetRenderDrawColor(jogo->ren,0xff,0xff,0x00,0x00);//"Nome Mundo"
	SDL_RenderFillRect(jogo->ren,&jogo->sel.title);
	
	
	if(AUX_WaitEventTimeoutCount(&(jogo->evt),&(jogo->espera))){//trocar por exercicio
		switch(jogo->evt.type){
			case SDL_MOUSEBUTTONDOWN://verifica os cliques do botão
				p.x = (int)jogo->evt.button.x; p.y = (int)jogo->evt.button.y;
				
				if (SDL_PointInRect(&p,&jogo->sel.botao_voltar))jogo->estado = JYH_LOAD_WORLD_SELECTION;
				
				break;
			case SDL_QUIT:
				jogo->estado = JYH_END_GAME;
				break;
		}
	}else{
		//eventos baseados em tempo
	}
	
	SDL_SetRenderDrawColor(jogo->ren,0xff,0x00,0x00,0x00);//cor botão 1
	SDL_RenderFillRect(jogo->ren,&jogo->sel.botao_voltar);
}

void JYH_GameLoadSel(JYH_GameState* jogo){
	SDL_SetRenderDrawColor(jogo->ren,0xff,0xff,0xff,0x00);//trocar por uma tela de loading
	SDL_RenderClear(jogo->ren);
	SDL_RenderPresent(jogo->ren);
	
	jogo->sel.title =  (SDL_Rect){450,100,300,90};
	jogo->sel.botao_voltar = (SDL_Rect){25,25,50,50};
	jogo->sel.n_niveis = 10;//numero arbitrario temporario para testar a interface
	//jogo->sel.niveis
	
	
	
	printf("Mundo\n");
	jogo->estado = JYH_LVL_SELECTION;
}
