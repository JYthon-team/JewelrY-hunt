//Executar Nível
#include "JYH_Header.h"

void JYH_Destroy_EX(JYH_GameState* jogo){
    SDL_DestroyTexture(jogo->ex.txt_tb);
    SDL_DestroyTexture(jogo->ex.txt_gem);
    SDL_DestroyTexture(jogo->ex.txt_relogio);
    SDL_DestroyTexture(jogo->ex.txt_gem_count);
    SDL_DestroyTexture(jogo->ex.txt_tempo);	
    SDL_DestroyTexture(jogo->ex.txt_voltar);
    SDL_DestroyTexture(jogo->ex.txt_reiniciar);
}
void JYH_EX_to_LS(JYH_GameState* jogo){

}
void JYH_EX_to_LE(JYH_GameState* jogo){

}
void JYH_EX_to_PL(JYH_GameState* jogo){

}


void JYH_EX_goback(JYH_GameState* jogo){
	switch(jogo->prev){
        case JYH_state_LS:
            JYH_EX_to_LS(jogo);
            break;
        case JYH_state_LE:
            JYH_EX_to_LE(jogo);
            break;
        case JYH_state_PL:
            JYH_EX_to_PL(jogo);
            break;
	}
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
	
	//SDL_SetRenderDrawColor(jogo->ren,0x00,0xff,0x00,0x00);//barra do topo
	//SDL_RenderFillRect(jogo->ren,&jogo->ex.top_bar);
    SDL_RenderCopy(jogo->ren,jogo->ex.txt_tb,NULL,&jogo->ex.top_bar);
    
	
	//SDL_SetRenderDrawColor(jogo->ren,0xff,0x00,0xff,0x00);//icones
	//SDL_RenderFillRect(jogo->ren,&jogo->ex.icone_gemas);
	//SDL_RenderFillRect(jogo->ren,&jogo->ex.icone_relogio);
    SDL_RenderCopy(jogo->ren,jogo->ex.txt_gem,NULL,&jogo->ex.icone_gemas);
    SDL_RenderCopy(jogo->ren,jogo->ex.txt_relogio,NULL,&jogo->ex.icone_relogio);
	
	//SDL_SetRenderDrawColor(jogo->ren,0x00,0xff,0xff,0x00);//contadores
	//SDL_RenderFillRect(jogo->ren,&jogo->ex.contagem_gemas);
	//SDL_RenderFillRect(jogo->ren,&jogo->ex.contagem_tempo);
    SDL_RenderCopy(jogo->ren,jogo->ex.txt_gem_count,NULL,&jogo->ex.contagem_gemas);
    SDL_RenderCopy(jogo->ren,jogo->ex.txt_tempo,NULL,&jogo->ex.contagem_tempo);
	
	//desenhar botões
	
	//SDL_SetRenderDrawColor(jogo->ren,0xff,0x00,0x00,0x00);//botões
	//SDL_RenderFillRect(jogo->ren,&jogo->ex.botao_voltar);
	//SDL_RenderFillRect(jogo->ren,&jogo->ex.botao_reiniciar);
    SDL_RenderCopy(jogo->ren,jogo->ex.txt_voltar   ,NULL,&jogo->ex.botao_voltar   );
    SDL_RenderCopy(jogo->ren,jogo->ex.txt_reiniciar,NULL,&jogo->ex.botao_reiniciar);
	
	if(AUX_WaitEventTimeoutCount(&(jogo->evt),&(jogo->espera))){//trocar por exercicio
		switch(jogo->evt.type){
			case SDL_MOUSEBUTTONDOWN://verifica os cliques do botão
				p = (SDL_Point){(int)jogo->evt.button.x,(int)jogo->evt.button.y};
				
				if (SDL_PointInRect(&p,&jogo->ex.botao_voltar))JYH_EX_goback(jogo);
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

    jogo->ex.top_bar=(SDL_Rect){0,0,jogo->w_tela,100};
	jogo->ex.botao_voltar = (SDL_Rect){25,25,50,50};
	jogo->ex.botao_reiniciar =(SDL_Rect){100,25,50,50};
	jogo->ex.icone_gemas = (SDL_Rect){175,25,50,50};
	jogo->ex.contagem_gemas =(SDL_Rect){250,25,100,50};//onde é escrita a razão entre as gemas do nível e as gemas coletadas
	jogo->ex.icone_relogio = (SDL_Rect){375,25,50,50};
	jogo->ex.contagem_tempo = (SDL_Rect){450,25,100,50};//onde é escrita a contagem de tempo

    SDL_Color clr = {0xff,0x00,0x00,0x00};
    
    #ifdef _WIN32
    jogo->ex.txt_tb = IMG_LoadTexture(jogo->ren,"img\\geral\\top_bar_JYH.png");
    jogo->ex.txt_gem = IMG_LoadTexture(jogo->ren,"img\\geral\\Back_JYH.png");//trocar
    jogo->ex.txt_relogio = IMG_LoadTexture(jogo->ren,"img\\geral\\Run_JYH.png");//trocar
    jogo->ex.txt_tempo = AUX_CriarTexto(jogo->ren,jogo->fnt,"01:30",clr);
    jogo->ex.txt_gem_count = IMG_LoadTexture(jogo->ren,"img\\geral\\Back_JYH.png");//trocar
    jogo->ex.txt_voltar = IMG_LoadTexture(jogo->ren,"img\\geral\\Back_JYH.png");
    jogo->ex.txt_reiniciar = IMG_LoadTexture(jogo->ren,"img\\geral\\Save_JYH.png");//trocar
    #elif __linux__
    jogo->ex.txt_tb = IMG_LoadTexture(jogo->ren,"./img/geral/top_bar_JYH.png");
    jogo->ex.txt_gem = IMG_LoadTexture(jogo->ren,"./img/geral/Back_JYH.png");//trocar
    jogo->ex.txt_relogio = IMG_LoadTexture(jogo->ren,"./img/geral/Run_JYH.png");//trocar
    jogo->ex.txt_tempo = AUX_CriarTexto(jogo->ren,jogo->fnt,"01:30",clr);
    jogo->ex.txt_gem_count = IMG_LoadTexture(jogo->ren,"./img/geral/Back_JYH.png");//trocar
    jogo->ex.txt_voltar = IMG_LoadTexture(jogo->ren,"./img/geral/Back_JYH.png");
    jogo->ex.txt_reiniciar = IMG_LoadTexture(jogo->ren,"./img/geral/Save_JYH.png");//trocar
    #endif

	jogo->ex.timer = 0;//No Jogo Final depende do nível a ser carregado!!!
	jogo->ex.gemas_coletadas = 0;
	jogo->ex.tesouro_pego = 0;
	jogo->ex.tempo_de_jogo = 0;
}
