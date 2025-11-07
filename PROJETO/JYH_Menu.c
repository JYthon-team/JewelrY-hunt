//Menu Principal
#include "JYH_Header.h"

//destruir

void JYH_Destroy_MM(JYH_GameState* jogo){
	SDL_DestroyTexture(jogo->mm.txt_background);
	SDL_DestroyTexture(jogo->mm.titulo.txt);
	SDL_DestroyTexture(jogo->mm.botao_WS.txt);
	SDL_DestroyTexture(jogo->mm.botao_PL.txt);
	SDL_DestroyTexture(jogo->mm.botao_LE.txt);
	
	SDL_DestroyTexture(jogo->mm.txt_msg_edit);
	SDL_DestroyTexture(jogo->mm.txt_msg_worlds);
	SDL_DestroyTexture(jogo->mm.txt_msg_selP);
}

//Transições

void JYH_MM_to_WS(JYH_GameState* jogo){//Menu para World Selection
    JYH_World_Selection temp;
    AUX_Empilha(&jogo->state,JYH_state_WS);
    JYH_Destroy_MM(jogo);
    jogo->ws = temp;
    JYH_Load_WS(jogo);
}
void JYH_MM_to_LE(JYH_GameState* jogo){//Menu para Level Editor
	JYH_Editor temp;
	strcpy(temp.lvl.nome_nivel,"teste");//temporario
	AUX_Empilha(&jogo->state,JYH_state_LE);
	JYH_Destroy_MM(jogo);
	jogo->le = temp;
	JYH_Load_LE(jogo);
}
void JYH_MM_to_PL(JYH_GameState* jogo){//Menu para Player Library
	JYH_Level_Selection_P temp;
	AUX_Empilha(&jogo->state,JYH_state_PL);
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
	AUX_Draw_Icon(jogo->ren,&jogo->mm.titulo);
	
	AUX_Draw_Icon(jogo->ren,&jogo->mm.botao_WS);
	AUX_Draw_Icon(jogo->ren,&jogo->mm.botao_LE);
	AUX_Draw_Icon(jogo->ren,&jogo->mm.botao_PL);
	SDL_RenderCopy(jogo->ren,jogo->mm.txt_msg_worlds,NULL,&jogo->mm.botao_WS.r);//temporario
	
	SDL_RenderCopy(jogo->ren,jogo->mm.txt_msg_selP,NULL,&jogo->mm.botao_PL.r);//temporario
	
	SDL_RenderCopy(jogo->ren,jogo->mm.txt_msg_edit,NULL,&jogo->mm.botao_LE.r);//temporario
	
	if(AUX_WaitEventTimeoutCount(&(jogo->evt),&(jogo->espera))){
		switch(jogo->evt.type){
			case SDL_MOUSEBUTTONDOWN://verifica os cliques do botão
				p = (SDL_Point){(int)jogo->evt.button.x,(int)jogo->evt.button.y};
				
				if      (SDL_PointInRect(&p,&jogo->mm.botao_WS.r))JYH_MM_to_WS(jogo);
				else if (SDL_PointInRect(&p,&jogo->mm.botao_PL.r))JYH_MM_to_PL(jogo);
				else if (SDL_PointInRect(&p,&jogo->mm.botao_LE.r))JYH_MM_to_LE(jogo);
				break;
			case SDL_QUIT:
				AUX_Empilha(&jogo->state,JYH_END_GAME);
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
	
    //temporarios
    jogo->mm.txt_msg_edit = AUX_CriarTexto(jogo->ren,jogo->fnt,"Editor",clr);
    jogo->mm.txt_msg_worlds = AUX_CriarTexto(jogo->ren,jogo->fnt,"Modo Campanha",clr);
    jogo->mm.txt_msg_selP = AUX_CriarTexto(jogo->ren,jogo->fnt,"Meus Niveis",clr);

    jogo->mm.txt_background = IMG_LoadTexture(jogo->ren,IMG_MM_BACKGROUND);
	AUX_Start_Icon(jogo->ren,&jogo->mm.titulo  ,IMG_MM_TITLE,(SDL_Rect){350,200,500,150},1);
	AUX_Start_Icon(jogo->ren,&jogo->mm.botao_WS,IMG_MM_WS,(SDL_Rect){450,450,300,30},1);
	AUX_Start_Icon(jogo->ren,&jogo->mm.botao_PL,IMG_MM_PL,(SDL_Rect){450,500,300,30},1);
	AUX_Start_Icon(jogo->ren,&jogo->mm.botao_LE,IMG_MM_LE,(SDL_Rect){450,550,300,30},1);
}
