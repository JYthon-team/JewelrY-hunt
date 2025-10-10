//Menu Principal
#include "JYH_Header.h"

//destruir

void JYH_Destroy_MM(JYH_GameState* jogo){
	SDL_DestroyTexture(jogo->mm.txt_title);
	SDL_DestroyTexture(jogo->mm.txt_worlds);
	SDL_DestroyTexture(jogo->mm.txt_selP);
	SDL_DestroyTexture(jogo->mm.txt_edit);
	SDL_DestroyTexture(jogo->mm.txt_background);
	
	SDL_DestroyTexture(jogo->mm.txt_msg_edit);
	SDL_DestroyTexture(jogo->mm.txt_msg_worlds);
	SDL_DestroyTexture(jogo->mm.txt_msg_selP);
}

//Transições

void JYH_MM_to_WS(JYH_GameState* jogo){//Menu para World Selection
    JYH_World_Selection temp;
    jogo->prev = jogo->estado;
    jogo->estado = JYH_state_WS;
    JYH_Destroy_MM(jogo);
    jogo->ws = temp;
    JYH_Load_WS(jogo);
}
void JYH_MM_to_LE(JYH_GameState* jogo){//Menu para Level Editor
	JYH_Editor temp;
	jogo->prev = jogo->estado;
	jogo->estado = JYH_state_LE;
	JYH_Destroy_MM(jogo);
	jogo->le = temp;
	JYH_Load_LE(jogo);
}
void JYH_MM_to_PL(JYH_GameState* jogo){//Menu para Player Library
	JYH_Level_Selection_P temp;
	jogo->prev = jogo->estado;
	jogo->estado = JYH_state_PL;
	JYH_Destroy_MM(jogo);
	jogo->pl = temp;
	JYH_Load_PL(jogo);
}

//Execução

void JYH_MM(JYH_GameState* jogo){
	static SDL_Point p;
	
	SDL_SetRenderDrawColor(jogo->ren,0xff,0xff,0xff,0x00);//background
	SDL_RenderClear(jogo->ren);
	
	SDL_RenderCopy(jogo->ren,jogo->mm.txt_background,NULL,NULL);
	SDL_RenderCopy(jogo->ren,jogo->mm.txt_title,NULL,&jogo->mm.title);
	
	SDL_RenderCopy(jogo->ren,jogo->mm.txt_worlds,NULL,&jogo->mm.botao_worlds);
	SDL_RenderCopy(jogo->ren,jogo->mm.txt_msg_worlds,NULL,&jogo->mm.botao_worlds);//temporario
	
	SDL_RenderCopy(jogo->ren,jogo->mm.txt_selP,NULL,&jogo->mm.botao_selP);
	SDL_RenderCopy(jogo->ren,jogo->mm.txt_msg_selP,NULL,&jogo->mm.botao_selP);//temporario
	
	SDL_RenderCopy(jogo->ren,jogo->mm.txt_edit,NULL,&jogo->mm.botao_edit);
	SDL_RenderCopy(jogo->ren,jogo->mm.txt_msg_edit,NULL,&jogo->mm.botao_edit);//temporario
	
	if(AUX_WaitEventTimeoutCount(&(jogo->evt),&(jogo->espera))){
		switch(jogo->evt.type){
			case SDL_MOUSEBUTTONDOWN://verifica os cliques do botão
				p = (SDL_Point){(int)jogo->evt.button.x,(int)jogo->evt.button.y};
				
				if      (SDL_PointInRect(&p,&jogo->mm.botao_worlds))JYH_MM_to_WS(jogo);
				else if (SDL_PointInRect(&p,&jogo->mm.botao_selP  ))JYH_MM_to_PL(jogo);
				else if (SDL_PointInRect(&p,&jogo->mm.botao_edit  ))JYH_MM_to_LE(jogo);
				break;
			case SDL_QUIT:
				jogo->estado = JYH_END_GAME;
				JYH_Destroy_MM(jogo);
				break;
		}
	}else{
		//eventos baseados em tempo
	}
	
}

//Load

void JYH_Load_MM(JYH_GameState* jogo){
	SDL_Color clr = {0x00,0x00,0x00,0x00};
	
	jogo->mm.title = (SDL_Rect){450,200,300,90};
    jogo->mm.botao_worlds = (SDL_Rect){450,450,300,30};
    jogo->mm.botao_selP = (SDL_Rect){450,500,300,30};
    jogo->mm.botao_edit = (SDL_Rect){450,550,300,30};
    
    //temporarios
    jogo->mm.txt_msg_edit = AUX_CriarTexto(jogo->ren,jogo->fnt,"Editor",clr);
    jogo->mm.txt_msg_worlds = AUX_CriarTexto(jogo->ren,jogo->fnt,"Modo Campanha",clr);
    jogo->mm.txt_msg_selP = AUX_CriarTexto(jogo->ren,jogo->fnt,"Meus Niveis",clr);
    
    #ifdef _WIN32

	jogo->mm.txt_title = IMG_LoadTexture(jogo->ren,"img\\Menu\\Titulo_JYH.png");
	jogo->mm.txt_worlds = IMG_LoadTexture(jogo->ren,"img\\Menu\\Modo_Historia_JYH.png");
	jogo->mm.txt_selP = IMG_LoadTexture(jogo->ren,"img\\Menu\\Player_Library_JYH.png");
	jogo->mm.txt_edit = IMG_LoadTexture(jogo->ren,"img\\Menu\\Editor_JYH.png");
	jogo->mm.txt_background = IMG_LoadTexture(jogo->ren,"img\\Menu\\Background_JYH.png");

    #elif __linux__

	jogo->mm.txt_title = IMG_LoadTexture(jogo->ren,"img/menu/Titulo_JYH.png");
	jogo->mm.txt_worlds = IMG_LoadTexture(jogo->ren,"./img/menu/Modo_Historia_JYH.png");
	jogo->mm.txt_selP = IMG_LoadTexture(jogo->ren,"./img/menu/Player_Library_JYH.png");
	jogo->mm.txt_edit = IMG_LoadTexture(jogo->ren,"./img/menu/Editor_JYH.png");
	jogo->mm.txt_background = IMG_LoadTexture(jogo->ren,"./img/menu/Background_JYH.png");

    #endif
    
	assert(jogo->mm.txt_title != NULL);
    assert(jogo->mm.txt_worlds != NULL);
    assert(jogo->mm.txt_selP != NULL);
    assert(jogo->mm.txt_edit != NULL);
    assert(jogo->mm.txt_background != NULL);
}
