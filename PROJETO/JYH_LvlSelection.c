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
				
				if (SDL_PointInRect(&p,&jogo->sel.botao_voltar))jogo->estado_tela = 2;
				
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
	//jogo->estado = JYH_LVL_SELECTION;
	jogo->estado_tela = 1;
}

void JYH_LS_to_WS(JYH_GameState* jogo){
	printf("->WS\n");
	JYH_World_Selection temp;
	jogo->prev = jogo->estado;
	jogo->estado_tela = 0;
	jogo->estado = JYH_WORLD_SELECTION;
	jogo->worlds = temp;
	
}
void JYH_LS_to_EX(JYH_GameState* jogo){
	printf("->EX\n");
	JYH_Level_Runner temp;
	jogo->prev = jogo->estado;
	jogo->estado_tela = 0;
	jogo->estado = JYH_LVL_EXEC;
	jogo->exec = temp;
}


void JYH_LS(JYH_GameState* jogo){
	switch(jogo->estado_tela){
		case 0://load
			JYH_GameLoadSel(jogo);
			break;
		case 1://Executando
			JYH_GameLvlSelection(jogo);
			break;
		case 2://Voltar à tela de seleção de mundos
			JYH_LS_to_WS(jogo);
			break;
		case 3://selecionar nível para execução
			JYH_LS_to_EX(jogo);
			break;
		
	}
}
