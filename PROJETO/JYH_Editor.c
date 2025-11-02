//Editor Nível
#include "JYH_Header.h"

void JYH_Destroy_LE(JYH_GameState* jogo){//desalocar
	SDL_DestroyTexture(jogo->le.botao_V.txt);
	SDL_DestroyTexture(jogo->le.botao_P.txt);
	SDL_DestroyTexture(jogo->le.botao_R.txt);
	SDL_DestroyTexture(jogo->le.botao_S.txt);
	SDL_DestroyTexture(jogo->le.botao_T.txt);
	SDL_DestroyTexture(jogo->le.botao_A.txt);
	SDL_DestroyTexture(jogo->le.tb.txt);
	SDL_DestroyTexture(jogo->le.sb.txt);
	SDL_DestroyTexture(jogo->le.lvl.txt_theme);
	
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
	SDL_Rect r = {0,0,stepx,stepy*1.5};
	SDL_Rect c = {0,0,32,48};//retângulo de recorte dos tilesets
	SDL_SetRenderDrawColor(jogo->ren,0x00,0x00,0x00,0x00);
	for(int i = 0; i < jogo->le.lvl.w; i++)SDL_RenderDrawLine(jogo->ren,i*stepx,100,i*stepx,700);
	for(int i = 0; i < jogo->le.lvl.h; i++)SDL_RenderDrawLine(jogo->ren,0,100+i*stepy,1000,100+i*stepy);
	
	for(int i = 0; i < jogo->le.lvl.w; i ++){
		for(int j = 0; j < jogo->le.lvl.h; j++){
				r.x = i*stepx ;
				r.y = 100 + j*stepy - stepy*0.5;
				c.x = c.w*(jogo->le.lvl.mat[j*(jogo->le.lvl.w)+i]);
				SDL_RenderCopy(jogo->ren,jogo->le.lvl.txt_theme,&c,&r);
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
	unsigned char *mat = jogo->le.lvl.mat;
	int              w = jogo->le.lvl.w  ;
	int              h = jogo->le.lvl.h  ;
	                 mat[idx  ] |= 16;                                                                               ;//bloco em si vira parede
	if(idx%w        )mat[idx-1] |= 1 ;//bloco à esquerda tem parede
	if((idx+1)%w    )mat[idx+1] |= 4 ;//bloco à direita tem parede
	if(idx - w >= 0 )mat[idx-w] |= 8 ;//bloco acima
	if(idx + w < w*h)mat[idx+w] |= 2 ;//blocoabaixo
	
}

void JYH_Apaga_Parede(JYH_GameState* jogo, SDL_Point* p){
	int idx = JYH_Converter_Coordenada(jogo,p);
	unsigned char *mat = jogo->le.lvl.mat;
	int              w = jogo->le.lvl.w  ;
	int              h = jogo->le.lvl.h  ;
	                 mat[idx  ] -= (mat[idx  ] & 16)?16:0;                                                                               ;//bloco em si vira parede
	if(idx%w        )mat[idx-1] -= (mat[idx-1] & 1 )?1 :0;//bloco à esquerda tem parede
	if((idx+1)%w    )mat[idx+1] -= (mat[idx+1] & 4 )?4 :0;//bloco à direita tem parede
	if(idx - w >= 0 )mat[idx-w] -= (mat[idx-w] & 8 )?8 :0;//bloco acima
	if(idx + w < w*h)mat[idx+w] -= (mat[idx+w] & 2 )?2 :0;//blocoabaixo
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
	AUX_Draw_Icon(jogo->ren,&jogo->le.botao_A);
	AUX_Draw_Icon(jogo->ren,&jogo->le.botao_T);
	
	JYH_Draw_Grade(jogo);
	
	if(AUX_WaitEventTimeoutCount(&(jogo->evt),&(jogo->espera))){//trocar por exercicio
		switch(jogo->evt.type){
			case SDL_MOUSEMOTION:
				p = (SDL_Point){(int)jogo->evt.motion.x,(int)jogo->evt.motion.y};
				if(jogo->le.press && SDL_PointInRect(&p,&jogo->le.editor)){
					switch(jogo->le.pincel){
						case PINCEL_PINTANDO:JYH_Coloca_Parede(jogo,&p);break;
						case PINCEL_APAGANDO:JYH_Apaga_Parede(jogo,&p);break;
					}
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				jogo->le.press = SDL_TRUE;
				p = (SDL_Point){(int)jogo->evt.button.x,(int)jogo->evt.button.y};
				if(jogo->le.press && SDL_PointInRect(&p,&jogo->le.editor)){
					switch(jogo->le.pincel){
						case PINCEL_PINTANDO:JYH_Coloca_Parede(jogo,&p);break;
						case PINCEL_APAGANDO:JYH_Apaga_Parede(jogo,&p);break;
					}
				}
				break;
			case SDL_MOUSEBUTTONUP://verifica os cliques do botão
				p = (SDL_Point){(int)jogo->evt.button.x,(int)jogo->evt.button.y};
				jogo->le.press = SDL_FALSE;
				if      (SDL_PointInRect(&p,&jogo->le.botao_V.r)){
					JYH_LE_goback(jogo);
				}
				else if (SDL_PointInRect(&p,&jogo->le.botao_S.r)){
				
				/*Salvar Nível*/}
				else if (SDL_PointInRect(&p,&jogo->le.botao_R.r)){
					JYH_LE_to_EX(jogo);
				}
				else if (SDL_PointInRect(&p,&jogo->le.botao_P.r)){
					jogo->le.pincel = (jogo->le.pincel == PINCEL_PINTANDO)?PINCEL_DESOCUPADO:PINCEL_PINTANDO;
					jogo->le.botao_P.f = 1;
					jogo->le.botao_A.f = 0;
					
				}
				else if (SDL_PointInRect(&p,&jogo->le.botao_A.r)){
					jogo->le.pincel = (jogo->le.pincel == PINCEL_APAGANDO)?PINCEL_DESOCUPADO:PINCEL_APAGANDO;
					jogo->le.botao_P.f = 0;
					jogo->le.botao_A.f = 1;
				}
				else if (SDL_PointInRect(&p,&jogo->le.botao_T.r)){/*Menu de Temas*/}
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
	jogo->le.press = SDL_FALSE;
	
	//teste
	jogo->le.lvl.w = 25;//default
	jogo->le.lvl.h = 15;//default
	jogo->le.lvl.mat = (unsigned char*)malloc(sizeof(unsigned char)*(jogo->le.lvl.w)*(jogo->le.lvl.h));
	jogo->le.last_idx = (jogo->le.lvl.w)*(jogo->le.lvl.h);
	const int temp = (jogo->le.lvl.w)*(jogo->le.lvl.h);
	for(int i = 0; i < temp;i++)jogo->le.lvl.mat[i]= 0;
	memset(jogo->le.lvl.mat,0,sizeof(unsigned char)*(jogo->le.lvl.w)*(jogo->le.lvl.h));
	
	#ifdef _WIN32
	
	AUX_Start_Icon(jogo->ren,&jogo->le.botao_V  ,"img\\geral\\Back_JYH.png",(SDL_Rect){25,25,50,50},1);
	AUX_Start_Icon(jogo->ren,&jogo->le.botao_S  ,"img\\geral\\Save_JYH.png",(SDL_Rect){100,25,50,50},1);
	AUX_Start_Icon(jogo->ren,&jogo->le.botao_R  ,"img\\geral\\Run_JYH.png",(SDL_Rect){175,25,50,50},1);
	AUX_Start_Icon(jogo->ren,&jogo->le.botao_P  ,"img\\geral\\Paint_JYH-Sheet.png",(SDL_Rect){250,25,50,50},2);
	AUX_Start_Icon(jogo->ren,&jogo->le.botao_R  ,"img\\geral\\Run_JYH.png",(SDL_Rect){175,25,50,50},1);
	AUX_Start_Icon(jogo->ren,&jogo->le.botao_A       ,"img\\geral\\Apaga_JYH-Sheet.png",(SDL_Rect){325,25,50,50},2);
	AUX_Start_Icon(jogo->ren,&jogo->le.botao_T       ,"img\\geral\\Apaga_JYH-Sheet.png",(SDL_Rect){400,25,50,50},2);
	AUX_Start_Icon(jogo->ren,&jogo->le.tb       ,"img\\geral\\top_bar_JYH.png",(SDL_Rect){0,0,1200,100},1);
	AUX_Start_Icon(jogo->ren,&jogo->le.sb       ,"img\\geral\\side_bar_JYH.png",(SDL_Rect){1000,100,200,600},1);
	
	jogo->le.lvl.txt_theme = IMG_LoadTexture(jogo->ren,"img\\geral\\tile-Sheet.png");//sprite separado em vários de 32*48
	
	#elif __linux__
	
	AUX_Start_Icon(jogo->ren,&jogo->le.botao_V  ,"./img/geral/Back_JYH.png",(SDL_Rect){25,25,50,50},1);
	AUX_Start_Icon(jogo->ren,&jogo->le.botao_S  ,"./img/geral/Save_JYH.png",(SDL_Rect){100,25,50,50},1);
	AUX_Start_Icon(jogo->ren,&jogo->le.botao_R  ,"./img/geral/Run_JYH.png",(SDL_Rect){175,25,50,50},1);
	AUX_Start_Icon(jogo->ren,&jogo->le.botao_P  ,"./img/geral/Paint_JYH-Sheet.png",(SDL_Rect){250,25,50,50},2);
	AUX_Start_Icon(jogo->ren,&jogo->le.botao_A       ,"./img/geral/Apaga_JYH-Sheet.png",(SDL_Rect){325,25,50,50},2);
	AUX_Start_Icon(jogo->ren,&jogo->le.botao_T       ,"./img/geral/Apaga_JYH-Sheet.png",(SDL_Rect){400,25,50,50},2);
	AUX_Start_Icon(jogo->ren,&jogo->le.tb       ,"./img/geral/top_bar_JYH.png",(SDL_Rect){0,0,1200,100},1);
	AUX_Start_Icon(jogo->ren,&jogo->le.sb       ,"./img/geral/side_bar_JYH.png",(SDL_Rect){1000,100,200,600},1);
	
	jogo->le.lvl.txt_theme = IMG_LoadTexture(jogo->ren,"./img/geral/tile-Sheet.png");
	#endif
}