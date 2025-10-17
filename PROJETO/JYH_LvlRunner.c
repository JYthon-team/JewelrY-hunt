//Executar Nível
#include "JYH_Header.h"

void JYH_Destroy_EX(JYH_GameState* jogo){
    SDL_DestroyTexture(jogo->ex.txt_gem_count);
    SDL_DestroyTexture(jogo->ex.txt_tempo);
    SDL_DestroyTexture(jogo->ex.tb.txt);
    SDL_DestroyTexture(jogo->ex.botao_V.txt);
    SDL_DestroyTexture(jogo->ex.botao_R.txt);
    SDL_DestroyTexture(jogo->ex.clock.txt);
    SDL_DestroyTexture(jogo->ex.gem.txt);
}
void JYH_EX_to_LS(JYH_GameState* jogo){
	JYH_Level_Selection temp;
	strcpy(temp.path,jogo->ex.pathMundo);
	JYH_Destroy_EX(jogo);
	jogo->ls = temp;
	JYH_Load_LS(jogo);
}
void JYH_EX_to_LE(JYH_GameState* jogo){
	JYH_Editor temp;
	strcpy(temp.path,jogo->ex.pathNivel);
	temp.lvl = jogo->ex.lvl;//copia o nível
	JYH_Destroy_EX(jogo);
	jogo->le = temp;
	JYH_Load_LE(jogo);
}
void JYH_EX_to_PL(JYH_GameState* jogo){
	JYH_Level_Selection_P temp;
	JYH_Destroy_EX(jogo);
	jogo->pl = temp;
	JYH_Load_PL(jogo);
}

void JYH_EX_goback(JYH_GameState* jogo){
	AUX_Desempilha(&jogo->state);
	switch(AUX_Top(&jogo->state)){
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
}

void JYH_EX(JYH_GameState* jogo){//Atualizar
	static SDL_Point p;
	
	SDL_SetRenderDrawColor(jogo->ren,0xff,0xff,0xff,0x00);//background
	SDL_RenderClear(jogo->ren);
	
    AUX_Draw_Icon(jogo->ren,&jogo->ex.tb);
    
    AUX_Draw_Icon(jogo->ren,&jogo->ex.gem);
    AUX_Draw_Icon(jogo->ren,&jogo->ex.clock);
	
    SDL_RenderCopy(jogo->ren,jogo->ex.txt_gem_count,NULL,&jogo->ex.contagem_gemas);
    SDL_RenderCopy(jogo->ren,jogo->ex.txt_tempo,NULL,&jogo->ex.contagem_tempo);
	
	//desenhar botões
	
    AUX_Draw_Icon(jogo->ren,&jogo->ex.botao_R);
    AUX_Draw_Icon(jogo->ren,&jogo->ex.botao_V);
	
	if(AUX_WaitEventTimeoutCount(&(jogo->evt),&(jogo->espera))){//trocar por exercicio
		switch(jogo->evt.type){
			case SDL_MOUSEBUTTONDOWN://verifica os cliques do botão
				p = (SDL_Point){(int)jogo->evt.button.x,(int)jogo->evt.button.y};
				
				if (SDL_PointInRect(&p,&jogo->ex.botao_V.r))JYH_EX_goback(jogo);
				else if (SDL_PointInRect(&p,&jogo->ex.botao_R.r)){/*Reinicia o nivel*/}
				
				break;
			case SDL_QUIT:
				AUX_Empilha(&jogo->state,JYH_END_GAME);
				JYH_Destroy_EX(jogo);
				break;
		}
	}else{
		//eventos baseados em tempo
	}

}

void JYH_Load_EX(JYH_GameState* jogo){
	SDL_Color clr = {0xff,0x00,0x00,0x00};

	jogo->ex.contagem_gemas =(SDL_Rect){250,25,100,50};//onde é escrita a razão entre as gemas do nível e as gemas coletadas
	jogo->ex.contagem_tempo = (SDL_Rect){450,25,100,50};//onde é escrita a contagem de tempo
    jogo->ex.txt_tempo = AUX_CriarTexto(jogo->ren,jogo->fnt,"01:30",clr);
    jogo->ex.txt_gem_count = AUX_CriarTexto(jogo->ren,jogo->fnt,"2/10",clr);
    
    #ifdef _WIN32
    AUX_Start_Icon(jogo->ren,&jogo->ex.tb     ,"img\\geral\\top_bar_JYH.png",(SDL_Rect){0,0,1200,100},1);
	AUX_Start_Icon(jogo->ren,&jogo->ex.botao_V,"img\\geral\\Back_JYH.png"   ,(SDL_Rect){25,25,50,50},1);
	AUX_Start_Icon(jogo->ren,&jogo->ex.botao_R,"img\\geral\\Save_JYH.png"   ,(SDL_Rect){100,25,50,50},1);//trocar
	AUX_Start_Icon(jogo->ren,&jogo->ex.gem    ,"img\\geral\\Back_JYH.png"   ,(SDL_Rect){175,25,50,50},1);//trocar
	AUX_Start_Icon(jogo->ren,&jogo->ex.clock  ,"img\\geral\\Run_JYH.png"    ,(SDL_Rect){375,25,50,50},1);//trocar
    #elif __linux__
    AUX_Start_Icon(jogo->ren,&jogo->ex.tb     ,"./img/geral/top_bar_JYH.png",(SDL_Rect){0,0,1200,100},1);
	AUX_Start_Icon(jogo->ren,&jogo->ex.botao_V,"./img/geral/Back_JYH.png"   ,(SDL_Rect){25,25,50,50},1);
	AUX_Start_Icon(jogo->ren,&jogo->ex.botao_R,"./img/geral/Save_JYH.png"   ,(SDL_Rect){100,25,50,50},1);//trocar
	AUX_Start_Icon(jogo->ren,&jogo->ex.gem    ,"./img/geral/Back_JYH.png"   ,(SDL_Rect){175,20,50,50},1);//trocar
	AUX_Start_Icon(jogo->ren,&jogo->ex.clock  ,"./img/geral/Run_JYH.png"    ,(SDL_Rect){375,25,50,50},1);//trocar
    #endif

	jogo->ex.timer = 0;//No Jogo Final depende do nível a ser carregado!!!
	jogo->ex.gemas_coletadas = 0;
	jogo->ex.tesouro_pego = 0;
	jogo->ex.tempo_de_jogo = 0;
}
