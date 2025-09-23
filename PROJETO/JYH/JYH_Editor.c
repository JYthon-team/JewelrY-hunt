//Editor Nível
#include "JYH_Header.h"

void JYH_GameLvlEditor(JYH_GameState* jogo){//Atualizar
	static SDL_Point p;
	
	SDL_SetRenderDrawColor(jogo->ren,0xff,0xff,0xff,0x00);//background
	SDL_RenderClear(jogo->ren);
	
	SDL_SetRenderDrawColor(jogo->ren,0x00,0x00,0xff,0x00);//estético
	SDL_RenderFillRect(jogo->ren,&jogo->edit.side_bar);
	SDL_SetRenderDrawColor(jogo->ren,0x00,0xff,0x00,0x00);
	SDL_RenderFillRect(jogo->ren,&jogo->edit.top_bar);
	SDL_SetRenderDrawColor(jogo->ren,0xff,0xff,0x00,0x00);
	
	if(AUX_WaitEventTimeoutCount(&(jogo->evt),&(jogo->espera))){//trocar por exercicio
		switch(jogo->evt.type){
			case SDL_MOUSEBUTTONDOWN://verifica os cliques do botão
				p.x = (int)jogo->evt.button.x; p.y = (int)jogo->evt.button.y;
				
				if (SDL_PointInRect(&p,&jogo->edit.botao_voltar))jogo->estado = JYH_LOAD_MENU;
				else if (SDL_PointInRect(&p,&jogo->edit.botao_salvar)){/*Salvar Nível*/}
				else if (SDL_PointInRect(&p,&jogo->edit.botao_run))jogo->estado = JYH_LOAD_LEVEL;
				
				break;
			case SDL_QUIT:
				jogo->estado = JYH_END_GAME;
				break;
		}
	}else{
	/*eventos baseados em tempo*/}
	
	//desenhar botões
	
	SDL_SetRenderDrawColor(jogo->ren,0xff,0x00,0x00,0x00);//cor botão 1
	SDL_RenderFillRect(jogo->ren,&jogo->edit.botao_voltar);
	SDL_RenderFillRect(jogo->ren,&jogo->edit.botao_salvar);
	SDL_RenderFillRect(jogo->ren,&jogo->edit.botao_run);

}

void JYH_GameLoadEditor(JYH_GameState* jogo){
	SDL_SetRenderDrawColor(jogo->ren,0xff,0xff,0xff,0x00);//trocar por uma tela de loading
	SDL_RenderClear(jogo->ren);
	SDL_RenderPresent(jogo->ren);
	
	jogo->edit.botao_voltar = (SDL_Rect){25,25,50,50};
	jogo->edit.botao_salvar = (SDL_Rect){100,25,50,50};
	jogo->edit.botao_run = (SDL_Rect){175,25,50,50};
	jogo->edit.editor = (SDL_Rect){0,100,jogo->w_tela - 200,jogo->h_tela - 100};
	jogo->edit.side_bar = (SDL_Rect){1000,100,200,jogo->h_tela - 100};
	jogo->edit.top_bar = (SDL_Rect){0,0,jogo->w_tela,100};
	
	printf("Editor\n");
	jogo->estado = JYH_LVL_EDITOR;
}

