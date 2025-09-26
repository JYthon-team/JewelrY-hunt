//Executar Nível
#include "JYH_Header.h"

void JYH_GameLvlExecution(JYH_GameState* jogo){//Atualizar
	static SDL_Point p;
	
	SDL_SetRenderDrawColor(jogo->ren,0xff,0xff,0xff,0x00);//background
	SDL_RenderClear(jogo->ren);
	
	SDL_SetRenderDrawColor(jogo->ren,0x00,0xff,0x00,0x00);//barra do topo
	SDL_RenderFillRect(jogo->ren,&jogo->exec.top_bar);
	
	SDL_SetRenderDrawColor(jogo->ren,0xff,0x00,0xff,0x00);//icones
	SDL_RenderFillRect(jogo->ren,&jogo->exec.icone_gemas);
	SDL_RenderFillRect(jogo->ren,&jogo->exec.icone_relogio);
	
	SDL_SetRenderDrawColor(jogo->ren,0x00,0xff,0xff,0x00);//contadores
	SDL_RenderFillRect(jogo->ren,&jogo->exec.contagem_gemas);
	SDL_RenderFillRect(jogo->ren,&jogo->exec.contagem_tempo);
	
	if(AUX_WaitEventTimeoutCount(&(jogo->evt),&(jogo->espera))){//trocar por exercicio
		switch(jogo->evt.type){
			case SDL_MOUSEBUTTONDOWN://verifica os cliques do botão
				p.x = (int)jogo->evt.button.x; p.y = (int)jogo->evt.button.y;
				
				if (SDL_PointInRect(&p,&jogo->exec.botao_voltar))jogo->estado_tela = 2/*jogo->estado = JYH_LOAD_MENU*/;
				else if (SDL_PointInRect(&p,&jogo->exec.botao_voltar)){/*Reinicia o nivel*/}
				
				break;
			case SDL_QUIT:
				jogo->estado = JYH_END_GAME;
				break;
		}
	}else{
		//eventos baseados em tempo
	}
	//desenhar botões
	
	SDL_SetRenderDrawColor(jogo->ren,0xff,0x00,0x00,0x00);//botões
	SDL_RenderFillRect(jogo->ren,&jogo->exec.botao_voltar);
	SDL_RenderFillRect(jogo->ren,&jogo->exec.botao_reiniciar);
}

void JYH_GameLoadExec(JYH_GameState* jogo){
	SDL_SetRenderDrawColor(jogo->ren,0xff,0xff,0xff,0x00);//trocar por uma tela de loading
	SDL_RenderClear(jogo->ren);
	SDL_RenderPresent(jogo->ren);

	jogo->exec.botao_voltar = (SDL_Rect){25,25,50,50};
	jogo->exec.botao_reiniciar =(SDL_Rect){100,25,50,50};
	jogo->exec.icone_gemas = (SDL_Rect){175,25,50,50};
	jogo->exec.contagem_gemas =(SDL_Rect){250,25,100,50};//onde é escrita a razão entre as gemas do nível e as gemas coletadas
	jogo->exec.icone_relogio = (SDL_Rect){375,25,50,50};
	jogo->exec.contagem_tempo = (SDL_Rect){450,25,100,50};//onde é escrita a contagem de tempo
	jogo->exec.timer = 0;//No Jogo Final depende do nível a ser carregado!!!
	jogo->exec.gemas_coletadas = 0;
	jogo->exec.tesouro_pego = 0;
	jogo->exec.tempo_de_jogo = 0;
	
	printf("Execucao\n");
	jogo->estado_tela = 1;
}



void JYH_EX_goback(JYH_GameState* jogo){
	/*switch(estado->prev){
		
		
		
		
	}*/
	printf("L->MM\n");
	//temporario durante o debug
	JYH_Menu temp;
	jogo->prev = jogo->estado;
	jogo->estado_tela = 0;
	jogo->estado = JYH_MAIN_MENU;
	jogo->menu = temp;
}

void JYH_EX(JYH_GameState* jogo){
	switch(jogo->estado_tela){
		case 0:
			JYH_GameLoadExec(jogo);
			break;
		case 1:
			JYH_GameLvlExecution(jogo);
			break;
		case 2:
			JYH_EX_goback(jogo);
			break;
	}
}
