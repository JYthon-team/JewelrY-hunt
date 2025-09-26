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
				
				if (SDL_PointInRect(&p,&jogo->selP.botao_voltar))jogo->estado_tela = 5;//jogo->estado = JYH_LOAD_MENU;
				
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
	
	printf("Biblioteca\n");
	jogo->estado_tela = 1;
}

void JYH_PL_to_LE(JYH_GameState* jogo){
	printf("->LE\n");
	JYH_Editor temp;
	jogo->prev = jogo->estado;
	jogo->estado_tela = 0;
	jogo->estado =  JYH_LVL_EDITOR;
	jogo->edit = temp;
}
void JYH_PL_to_MM(JYH_GameState* jogo){
	printf("->MM\n");
	JYH_Menu temp;
	jogo->prev = jogo->estado;
	jogo->estado_tela = 0;
	jogo->estado = JYH_MAIN_MENU;
	jogo->menu = temp;
}
void JYH_PL_to_EX(JYH_GameState* jogo){
	printf("->EX\n");
	JYH_Level_Runner temp;
	jogo->prev = jogo->estado;
	jogo->estado_tela = 0;
	jogo->estado = JYH_LVL_EXEC;
	jogo->exec = temp;
}
void JYH_PL(JYH_GameState* jogo){
	switch(jogo->estado_tela){
		case 0://Carrega a Biblioteca de Níveis do jogador
			JYH_GameLoadSelP(jogo);
			break;
		case 1://
			JYH_GameLvlSelection_P(jogo);
			break;
		case 3://Editar Nível Específico
			JYH_PL_to_LE(jogo);
			break;
		case 4://Executar Nível específico
			JYH_PL_to_EX(jogo);
			break;
		case 5://Voltar ao Menu
			JYH_PL_to_MM(jogo);
			break;
	}
}