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
				
				if      (SDL_PointInRect(&p,&jogo->edit.botao_voltar))jogo->estado_tela = 2;
				else if (SDL_PointInRect(&p,&jogo->edit.botao_salvar))jogo->estado_tela = /*3*/1;
				else if (SDL_PointInRect(&p,&jogo->edit.botao_run   ))jogo->estado_tela = 4;
				
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
	jogo->estado_tela = 1;
	//jogo->estado = JYH_LVL_EDITOR;
}

void JYH_LE_to_PL(JYH_GameState* jogo){//editor à biblioteca do player
	printf("L->PL\n");
	JYH_Level_Selection_P temp;
	jogo->prev = jogo->estado;
	jogo->estado_tela = 0;
	jogo->estado = JYH_LVL_SELECTION_P;
	jogo->selP = temp;
}
void JYH_LE_to_MM(JYH_GameState* jogo){//editor ao menu inicial
	printf("L->MM\n");
	JYH_Menu temp;
	jogo->prev = jogo->estado;
	jogo->estado = JYH_MAIN_MENU;
	jogo->estado_tela = 0;
	jogo->menu = temp;
}

void JYH_LE_goback(JYH_GameState* jogo){//é preciso saber o estado anterior na hora de sair de um nível
	switch(jogo->prev){
		case JYH_MAIN_MENU:
			JYH_LE_to_MM(jogo);
			break;
		case JYH_LVL_SELECTION_P:
			JYH_LE_to_PL(jogo);
			break;
	}
}
void JYH_LE_to_EX(JYH_GameState* jogo){
	printf("L->EX\n");
	JYH_Level_Runner temp;
	jogo->prev = jogo->estado;
	jogo->estado_tela = 0;
	jogo->estado = JYH_LVL_EXEC;
	jogo->exec = temp;
}
void JYH_LE(JYH_GameState* jogo){
	switch(jogo->estado_tela){
		case 0://load
			JYH_GameLoadEditor(jogo);
			break;
		case 1://editor
			JYH_GameLvlEditor(jogo);
			break;
		case 2://voltar
			JYH_LE_goback(jogo);
			break;
		case 3://salvar
			printf("Salvando\n");
			jogo->estado_tela = 1;
			break;
		case 4://executar
			JYH_LE_to_EX(jogo);
			break;
	}
}