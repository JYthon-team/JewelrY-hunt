//Executar Nível
#include "JYH_Header.h"

void JYH_Destroy_EX(JYH_GameState* jogo){
	
}

void JYH_EX_goback(JYH_GameState* jogo){
	/*switch(estado->prev){
	}*/
	//temporario durante o debug
	JYH_Menu temp;
	jogo->prev = jogo->estado;
	jogo->estado = JYH_state_MM;
	JYH_Destroy_EX(jogo);
	jogo->mm = temp;
	JYH_Load_MM(jogo);
}

void JYH_EX(JYH_GameState* jogo){//Atualizar
	static SDL_Point p;
	
	SDL_SetRenderDrawColor(jogo->ren,0xff,0xff,0xff,0x00);//background
	SDL_RenderClear(jogo->ren);
	
	SDL_SetRenderDrawColor(jogo->ren,0x00,0xff,0x00,0x00);//barra do topo
	SDL_RenderFillRect(jogo->ren,&jogo->ex.top_bar);
	
	SDL_SetRenderDrawColor(jogo->ren,0xff,0x00,0xff,0x00);//icones
	SDL_RenderFillRect(jogo->ren,&jogo->ex.icone_gemas);
	SDL_RenderFillRect(jogo->ren,&jogo->ex.icone_relogio);
	
	SDL_SetRenderDrawColor(jogo->ren,0x00,0xff,0xff,0x00);//contadores
	SDL_RenderFillRect(jogo->ren,&jogo->ex.contagem_gemas);
	SDL_RenderFillRect(jogo->ren,&jogo->ex.contagem_tempo);
	
	//desenhar botões
	
	SDL_SetRenderDrawColor(jogo->ren,0xff,0x00,0x00,0x00);//botões
	SDL_RenderFillRect(jogo->ren,&jogo->ex.botao_voltar);
	SDL_RenderFillRect(jogo->ren,&jogo->ex.botao_reiniciar);
	
	if(AUX_WaitEventTimeoutCount(&(jogo->evt),&(jogo->espera))){//trocar por exercicio
		switch(jogo->evt.type){
			case SDL_MOUSEBUTTONDOWN://verifica os cliques do botão
				p = (SDL_Point){(int)jogo->evt.button.x,(int)jogo->evt.button.y};
				
				if (SDL_PointInRect(&p,&jogo->ex.botao_voltar))JYH_EX_goback(jogo);//jogo->estado_tela = 2;
				else if (SDL_PointInRect(&p,&jogo->ex.botao_voltar)){/*Reinicia o nivel*/}
				
				break;
			case SDL_QUIT:
				jogo->estado = JYH_END_GAME;
				JYH_Destroy_EX(jogo);
				break;
		}
	}else{
		//eventos baseados em tempo
	}

}

void JYH_Load_EX(JYH_GameState* jogo){
	SDL_SetRenderDrawColor(jogo->ren,0xff,0xff,0xff,0x00);//trocar por uma tela de loading
	SDL_RenderClear(jogo->ren);
	SDL_RenderPresent(jogo->ren);

	jogo->ex.botao_voltar = (SDL_Rect){25,25,50,50};
	jogo->ex.botao_reiniciar =(SDL_Rect){100,25,50,50};
	jogo->ex.icone_gemas = (SDL_Rect){175,25,50,50};
	jogo->ex.contagem_gemas =(SDL_Rect){250,25,100,50};//onde é escrita a razão entre as gemas do nível e as gemas coletadas
	jogo->ex.icone_relogio = (SDL_Rect){375,25,50,50};
	jogo->ex.contagem_tempo = (SDL_Rect){450,25,100,50};//onde é escrita a contagem de tempo
	jogo->ex.timer = 0;//No Jogo Final depende do nível a ser carregado!!!
	jogo->ex.gemas_coletadas = 0;
	jogo->ex.tesouro_pego = 0;
	jogo->ex.tempo_de_jogo = 0;
}