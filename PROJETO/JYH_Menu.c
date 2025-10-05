//Menu Principal
#include "JYH_Header.h"

void JYH_Destroy_MM(JYH_GameState* jogo){
	SDL_DestroyTexture(jogo->menu.txt_title);
	SDL_DestroyTexture(jogo->menu.txt_worlds);
	SDL_DestroyTexture(jogo->menu.txt_selP);
	SDL_DestroyTexture(jogo->menu.txt_edit);
	SDL_DestroyTexture(jogo->menu.txt_background);
	
	SDL_DestroyTexture(jogo->menu.txt_msg_edit);
	SDL_DestroyTexture(jogo->menu.txt_msg_worlds);
	SDL_DestroyTexture(jogo->menu.txt_msg_selP);
}

void JYH_Run_MM(JYH_GameState* jogo){
	static SDL_Point p;
	
	SDL_SetRenderDrawColor(jogo->ren,0xff,0xff,0xff,0x00);//background
	SDL_RenderClear(jogo->ren);
	
	SDL_RenderCopy(jogo->ren,jogo->menu.txt_background,NULL,NULL);
	SDL_RenderCopy(jogo->ren,jogo->menu.txt_title,NULL,&jogo->menu.title);
	
	if(AUX_WaitEventTimeoutCount(&(jogo->evt),&(jogo->espera))){
		switch(jogo->evt.type){
			case SDL_MOUSEBUTTONDOWN://verifica os cliques do botão
				p = (SDL_Point){(int)jogo->evt.button.x,(int)jogo->evt.button.y};
				
				if      (SDL_PointInRect(&p,&jogo->menu.botao_worlds))jogo->estado_tela = 2;
				else if (SDL_PointInRect(&p,&jogo->menu.botao_selP  ))jogo->estado_tela = 3;
				else if (SDL_PointInRect(&p,&jogo->menu.botao_edit  ))jogo->estado_tela = 4;
				break;
			case SDL_QUIT:
				jogo->estado = JYH_END_GAME;
				JYH_Destroy_MM(jogo);
				break;
		}
	}else{
		//eventos baseados em tempo
	}
	
	//desenhar botões
	
	SDL_RenderCopy(jogo->ren,jogo->menu.txt_worlds,NULL,&jogo->menu.botao_worlds);
	SDL_RenderCopy(jogo->ren,jogo->menu.txt_msg_worlds,NULL,&jogo->menu.botao_worlds);//temporario
	
	SDL_RenderCopy(jogo->ren,jogo->menu.txt_selP,NULL,&jogo->menu.botao_selP);
	SDL_RenderCopy(jogo->ren,jogo->menu.txt_msg_selP,NULL,&jogo->menu.botao_selP);//temporario
	
	SDL_RenderCopy(jogo->ren,jogo->menu.txt_edit,NULL,&jogo->menu.botao_edit);
	SDL_RenderCopy(jogo->ren,jogo->menu.txt_msg_edit,NULL,&jogo->menu.botao_edit);//temporario
}

void JYH_Load_MM(JYH_GameState* jogo){
	SDL_Color clr = {0x00,0x00,0x00,0x00};
	
	jogo->menu.title = (SDL_Rect){450,200,300,90};
    jogo->menu.botao_worlds = (SDL_Rect){450,450,300,30};
    jogo->menu.botao_selP = (SDL_Rect){450,500,300,30};
    jogo->menu.botao_edit = (SDL_Rect){450,550,300,30};
    
    //temporarios
    jogo->menu.txt_msg_edit = AUX_CriarTexto(jogo->ren,jogo->fnt,"Editor",clr);
    jogo->menu.txt_msg_worlds = AUX_CriarTexto(jogo->ren,jogo->fnt,"Modo Campanha",clr);
    jogo->menu.txt_msg_selP = AUX_CriarTexto(jogo->ren,jogo->fnt,"Meus Niveis",clr);
    
    #ifdef _WIN32

	jogo->menu.txt_title = IMG_LoadTexture(jogo->ren,"img\\Menu\\Titulo_JYH.png");
	jogo->menu.txt_worlds = IMG_LoadTexture(jogo->ren,"img\\Menu\\Modo_Historia_JYH.png");
	jogo->menu.txt_selP = IMG_LoadTexture(jogo->ren,"img\\Menu\\Player_Library_JYH.png");
	jogo->menu.txt_edit = IMG_LoadTexture(jogo->ren,"img\\Menu\\Editor_JYH.png");
	jogo->menu.txt_background = IMG_LoadTexture(jogo->ren,"img\\Menu\\Background_JYH.png");

    #elif __linux__

	jogo->menu.txt_title = IMG_LoadTexture(jogo->ren,"img/menu/Titulo_JYH.png");
	jogo->menu.txt_worlds = IMG_LoadTexture(jogo->ren,"./img/menu/Modo_Historia_JYH.png");
	jogo->menu.txt_selP = IMG_LoadTexture(jogo->ren,"./img/menu/Player_Library_JYH.png");
	jogo->menu.txt_edit = IMG_LoadTexture(jogo->ren,"./img/menu/Editor_JYH.png");
	jogo->menu.txt_background = IMG_LoadTexture(jogo->ren,"./img/menu/Background_JYH.png");

    #endif
    
	assert(jogo->menu.txt_title != NULL);
    assert(jogo->menu.txt_worlds != NULL);
    assert(jogo->menu.txt_selP != NULL);
    assert(jogo->menu.txt_edit != NULL);
    assert(jogo->menu.txt_background != NULL);

	jogo->estado_tela = 1;
}

void JYH_MM_to_WS(JYH_GameState* jogo){//Menu para World Selection
    JYH_World_Selection temp;
    jogo->prev = jogo->estado;
    jogo->estado_tela = 0;
    jogo->estado = JYH_state_WS;
    JYH_Destroy_MM(jogo);
    jogo->worlds = temp;
}

void JYH_MM_to_LE(JYH_GameState* jogo){//Menu para Level Editor
	JYH_Editor temp;
	jogo->prev = jogo->estado;
	jogo->estado_tela = 0;
	jogo->estado = JYH_state_LE;
	JYH_Destroy_MM(jogo);
	jogo->edit = temp;
}

void JYH_MM_to_PL(JYH_GameState* jogo){//Menu para Player Library
	JYH_Level_Selection_P temp;
	jogo->prev = jogo->estado;
	jogo->estado_tela = 0;
	jogo->estado = JYH_state_PL;
	JYH_Destroy_MM(jogo);
	jogo->selP = temp;
}

void JYH_MM(JYH_GameState* jogo){//Menu
    switch(jogo->estado_tela){
        case 0://load
            JYH_Load_MM(jogo);
            break;
        case 1://exec
            JYH_Run_MM(jogo);
            break;
        case 2://vai para mundos
            JYH_MM_to_WS(jogo);
            break;
        case 3://vai para biblioteca
			JYH_MM_to_PL(jogo);
            break;
        case 4://vai para editor
			JYH_MM_to_LE(jogo);
            break;
    }
}



