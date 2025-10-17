//Editor Nível
#include "JYH_Header.h"

void JYH_Destroy_LE(JYH_GameState* jogo){//desalocar
	SDL_DestroyTexture(jogo->le.botao_V.txt);
	SDL_DestroyTexture(jogo->le.botao_P.txt);
	SDL_DestroyTexture(jogo->le.botao_R.txt);
	SDL_DestroyTexture(jogo->le.botao_S.txt);
	SDL_DestroyTexture(jogo->le.tb.txt);
	SDL_DestroyTexture(jogo->le.sb.txt);
	
	free(jogo->le.lvl.mat);//temporario
}

//Transições

void JYH_LE_to_PL(JYH_GameState* jogo){//editor à biblioteca do player
	JYH_Level_Selection_P temp;
	JYH_Destroy_LE(jogo);
	jogo->pl = temp;
	JYH_Load_PL(jogo);
}
void JYH_LE_to_MM(JYH_GameState* jogo){//editor ao menu inicial
	JYH_Menu temp;
	JYH_Destroy_LE(jogo);
	jogo->mm = temp;
	JYH_Load_MM(jogo);
}
void JYH_LE_goback(JYH_GameState* jogo){//é preciso saber o estado anterior na hora de sair de um nível
	AUX_Desempilha(&jogo->state);
	switch(AUX_Top(&jogo->state)){
		case JYH_state_MM:
			JYH_LE_to_MM(jogo);
			break;
		case JYH_state_PL:
			JYH_LE_to_PL(jogo);
			break;
	}
}
void JYH_LE_to_EX(JYH_GameState* jogo){
	JYH_Level_Runner temp;
	AUX_Empilha(&jogo->state,JYH_state_EX);
	JYH_Destroy_LE(jogo);
	jogo->ex = temp;
	JYH_Load_EX(jogo);
}

//Interação

void JYH_Draw_Grade(JYH_GameState* jogo){
	//grade contida no retângulo {0,100,1000,600}
	//assumir nivel de  50x30
	Uint32 stepx = jogo->le.editor.w/jogo->le.lvl.w;
	Uint32 stepy = jogo->le.editor.h/jogo->le.lvl.h;
	SDL_Rect r = {0,0,stepx,stepy};
	SDL_SetRenderDrawColor(jogo->ren,0x00,0x00,0x00,0x00);
	for(int i = 0; i < jogo->le.lvl.w; i++)SDL_RenderDrawLine(jogo->ren,i*stepx,100,i*stepx,700);
	for(int i = 0; i < jogo->le.lvl.h; i++)SDL_RenderDrawLine(jogo->ren,0,100+i*stepy,1000,100+i*stepy);
	
	for(int i = 0; i < jogo->le.lvl.w; i ++){
		for(int j = 0; j < jogo->le.lvl.h; j++){
			if(jogo->le.lvl.mat[j*(jogo->le.lvl.w)+i]){
				r.x = i*stepx;
				r.y = 100 + j*stepy;
				SDL_RenderFillRect(jogo->ren,&r);//desenhar parede
			}
		}
	}
}

int JYH_Converter_Coordenada(JYH_GameState* jogo,SDL_Point* p){
	Uint32 stepx = jogo->le.editor.w/jogo->le.lvl.w;
	Uint32 stepy = jogo->le.editor.h/jogo->le.lvl.h;
	int x = (p->x)/(stepx);
	int y = (p->y - jogo->le.editor.y)/(stepy);
	return y*(jogo->le.lvl.w) + x;
}

void JYH_Coloca_Parede(JYH_GameState* jogo, SDL_Point* p){
	int idx = JYH_Converter_Coordenada(jogo,p);
	if(idx != jogo->le.last_idx){
		jogo->le.last_idx = idx;
		jogo->le.lvl.mat[idx] = !jogo->le.lvl.mat[idx];
	}
}

//Execução

void JYH_LE(JYH_GameState* jogo){//Atualizar
	static SDL_Point p;
	Uint32 idx;
	
	SDL_SetRenderDrawColor(jogo->ren,0xff,0xff,0xff,0x00);//background
	SDL_RenderClear(jogo->ren);
	
	AUX_Draw_Icon(jogo->ren,&jogo->le.tb);
	AUX_Draw_Icon(jogo->ren,&jogo->le.sb);
	
	//desenhar botões

	AUX_Draw_Icon(jogo->ren,&jogo->le.botao_V);
	AUX_Draw_Icon(jogo->ren,&jogo->le.botao_S);
	AUX_Draw_Icon(jogo->ren,&jogo->le.botao_R);
	AUX_Draw_Icon(jogo->ren,&jogo->le.botao_P);
	
	JYH_Draw_Grade(jogo);
	
	if(AUX_WaitEventTimeoutCount(&(jogo->evt),&(jogo->espera))){//trocar por exercicio
		switch(jogo->evt.type){
			case SDL_MOUSEMOTION:
				p = (SDL_Point){(int)jogo->evt.motion.x,(int)jogo->evt.motion.y};
				if(jogo->le.pintar &&  jogo->le.press && SDL_PointInRect(&p,&jogo->le.editor)){
					JYH_Coloca_Parede(jogo,&p);
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				jogo->le.press = SDL_TRUE;
				p = (SDL_Point){(int)jogo->evt.button.x,(int)jogo->evt.button.y};
				if(jogo->le.pintar &&  jogo->le.press && SDL_PointInRect(&p,&jogo->le.editor)){
					JYH_Coloca_Parede(jogo,&p);
				}
				
				break;
			case SDL_MOUSEBUTTONUP://verifica os cliques do botão
				p = (SDL_Point){(int)jogo->evt.button.x,(int)jogo->evt.button.y};
				jogo->le.press = SDL_FALSE;
				if      (SDL_PointInRect(&p,&jogo->le.botao_V.r))JYH_LE_goback(jogo);
				else if (SDL_PointInRect(&p,&jogo->le.botao_S.r)){}
				else if (SDL_PointInRect(&p,&jogo->le.botao_R.r))JYH_LE_to_EX(jogo);
				else if (SDL_PointInRect(&p,&jogo->le.botao_P.r))jogo->le.pintar = !jogo->le.pintar;
				break;
			case SDL_QUIT:
				AUX_Empilha(&jogo->state,JYH_END_GAME);
				JYH_Destroy_LE(jogo);
				break;
		}
	}else{
	/*eventos baseados em tempo*/
	}
	

}

//Load

void JYH_Load_LE(JYH_GameState* jogo){
	jogo->le.editor = (SDL_Rect){0,100,1000,600};
	jogo->le.pintar = SDL_FALSE;
	jogo->le.press = SDL_FALSE;
	jogo->le.drag = SDL_FALSE;
	
	//teste
	jogo->le.lvl.w = 25;
	jogo->le.lvl.h = 15;
	jogo->le.lvl.mat = (unsigned char*)malloc(sizeof(unsigned char)*(jogo->le.lvl.w)*(jogo->le.lvl.h));
	jogo->le.last_idx = (jogo->le.lvl.w)*(jogo->le.lvl.h);
	const int temp = (jogo->le.lvl.w)*(jogo->le.lvl.h);
	for(int i = 0; i < temp;i++)jogo->le.lvl.mat[i]= 0;
	memset(jogo->le.lvl.mat,0,sizeof(unsigned char)*(jogo->le.lvl.w)*(jogo->le.lvl.h));
	
	#ifdef _WIN32
	
	AUX_Start_Icon(jogo->ren,&jogo->le.botao_V  ,"img\\geral\\Back_JYH.png",(SDL_Rect){25,25,50,50},1);
	AUX_Start_Icon(jogo->ren,&jogo->le.botao_P  ,"img\\geral\\Paint_JYH.png",(SDL_Rect){250,25,50,50},1);
	AUX_Start_Icon(jogo->ren,&jogo->le.botao_R  ,"img\\geral\\Run_JYH.png",(SDL_Rect){175,25,50,50},1);
	AUX_Start_Icon(jogo->ren,&jogo->le.botao_S  ,"img\\geral\\Save_JYH.png",(SDL_Rect){100,25,50,50},1);
	AUX_Start_Icon(jogo->ren,&jogo->le.tb       ,"img\\geral\\top_bar_JYH.png",(SDL_Rect){0,0,1200,100},1);
	AUX_Start_Icon(jogo->ren,&jogo->le.sb       ,"img\\geral\\side_bar_JYH.png",(SDL_Rect){1000,100,200,600},1);
	
	#elif __linux__
	
	AUX_Start_Icon(jogo->ren,&jogo->le.botao_V  ,"./img/geral/Back_JYH.png",(SDL_Rect){25,25,50,50},1);
	AUX_Start_Icon(jogo->ren,&jogo->le.botao_P  ,"./img/geral/Paint_JYH.png",(SDL_Rect){250,25,50,50},1);
	AUX_Start_Icon(jogo->ren,&jogo->le.botao_R  ,"./img/geral/Run_JYH.png",(SDL_Rect){175,25,50,50},1);
	AUX_Start_Icon(jogo->ren,&jogo->le.botao_S  ,"./img/geral/Save_JYH.png",(SDL_Rect){100,25,50,50},1);
	AUX_Start_Icon(jogo->ren,&jogo->le.tb       ,"./img/geral/top_bar_JYH.png",(SDL_Rect){0,0,1200,100},1);
	AUX_Start_Icon(jogo->ren,&jogo->le.sb       ,"./img/geral/side_bar_JYH.png",(SDL_Rect){1000,100,200,600},1);
	
	#endif
}