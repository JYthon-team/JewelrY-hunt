//Editor Nível
#include "JYH_Header.h"

void JYH_Destroy_LE(JYH_GameState* jogo){//desalocar
	SDL_DestroyTexture(jogo->le.txt_run );
	SDL_DestroyTexture(jogo->le.txt_salvar );
	SDL_DestroyTexture(jogo->le.txt_sb );
	SDL_DestroyTexture(jogo->le.txt_tb );
	SDL_DestroyTexture(jogo->le.txt_voltar);
	free(jogo->le.lvl.mat);//temporario
}

//Transições

void JYH_LE_to_PL(JYH_GameState* jogo){//editor à biblioteca do player
	JYH_Level_Selection_P temp;
	jogo->prev = jogo->estado;
	jogo->estado = JYH_state_PL;
	JYH_Destroy_LE(jogo);
	jogo->pl = temp;
	JYH_Load_PL(jogo);
}
void JYH_LE_to_MM(JYH_GameState* jogo){//editor ao menu inicial
	JYH_Menu temp;
	jogo->prev = jogo->estado;
	jogo->estado = JYH_state_MM;
	JYH_Destroy_LE(jogo);
	jogo->mm = temp;
	JYH_Load_MM(jogo);
}
void JYH_LE_goback(JYH_GameState* jogo){//é preciso saber o estado anterior na hora de sair de um nível
	switch(jogo->prev){
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
	jogo->prev = jogo->estado;
	jogo->estado = JYH_state_EX;
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
	
	SDL_RenderCopy(jogo->ren,jogo->le.txt_tb,NULL,&jogo->le.top_bar );
	SDL_RenderCopy(jogo->ren,jogo->le.txt_sb,NULL,&jogo->le.side_bar);
	
	//desenhar botões
	
	SDL_RenderCopy(jogo->ren,jogo->le.txt_voltar,NULL,&jogo->le.botao_voltar);
	SDL_RenderCopy(jogo->ren,jogo->le.txt_salvar,NULL,&jogo->le.botao_salvar);
	SDL_RenderCopy(jogo->ren,jogo->le.txt_run   ,NULL,&jogo->le.botao_run);
	SDL_RenderCopy(jogo->ren,jogo->le.txt_paint ,NULL,&jogo->le.botao_paint);
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
				if      (SDL_PointInRect(&p,&jogo->le.botao_voltar))JYH_LE_goback(jogo);//jogo->estado_tela = 2;
				else if (SDL_PointInRect(&p,&jogo->le.botao_salvar))jogo->estado = JYH_state_LE;//jogo->estado_tela = 1;
				else if (SDL_PointInRect(&p,&jogo->le.botao_run   ))JYH_LE_to_EX(jogo);//jogo->estado_tela = 4;
				else if (SDL_PointInRect(&p,&jogo->le.botao_paint ))jogo->le.pintar = !jogo->le.pintar;
				break;
			case SDL_QUIT:
				jogo->estado = JYH_END_GAME;
				JYH_Destroy_LE(jogo);
				break;
		}
	}else{
	/*eventos baseados em tempo*/
	}
	

}

//Load

void JYH_Load_LE(JYH_GameState* jogo){
	SDL_SetRenderDrawColor(jogo->ren,0xff,0xff,0xff,0x00);//trocar por uma tela de loading
	SDL_RenderClear(jogo->ren);
	SDL_RenderPresent(jogo->ren);
	
	jogo->le.botao_voltar = (SDL_Rect){25,25,50,50};
	jogo->le.botao_salvar = (SDL_Rect){100,25,50,50};
	jogo->le.botao_run = (SDL_Rect){175,25,50,50};
	jogo->le.botao_paint = (SDL_Rect){250,25,50,50};
	jogo->le.editor = (SDL_Rect){0,100,jogo->w_tela - 200,jogo->h_tela - 100};
	jogo->le.side_bar = (SDL_Rect){1000,100,200,jogo->h_tela - 100};
	jogo->le.top_bar = (SDL_Rect){0,0,jogo->w_tela,100};
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
	
	jogo->le.txt_tb = IMG_LoadTexture(jogo->ren,"img\\geral\\top_bar_JYH.png");
	jogo->le.txt_sb = IMG_LoadTexture(jogo->ren,"img\\geral\\side_bar_JYH.png");
	jogo->le.txt_voltar = IMG_LoadTexture(jogo->ren,"img\\geral\\Back_JYH.png");
	jogo->le.txt_salvar = IMG_LoadTexture(jogo->ren,"img\\geral\\Save_JYH.png");
	jogo->le.txt_run = IMG_LoadTexture(jogo->ren,"img\\geral\\Run_JYH.png");
	jogo->le.txt_paint = IMG_LoadTexture(jogo->ren,"img\\geral\\Paint_JYH.png");
	
	#elif __linux__
	
	jogo->le.txt_tb = IMG_LoadTexture(jogo->ren,"./img/geral/top_bar_JYH.png");
	jogo->le.txt_sb = IMG_LoadTexture(jogo->ren,"./img/geral/side_bar_JYH.png");
	jogo->le.txt_voltar = IMG_LoadTexture(jogo->ren,"./img/geral/Back_JYH.png");
	jogo->le.txt_salvar = IMG_LoadTexture(jogo->ren,"./img/geral/Save_JYH.png");
	jogo->le.txt_run = IMG_LoadTexture(jogo->ren,"./img/geral/Run_JYH.png");
	jogo->le.txt_paint = IMG_LoadTexture(jogo->ren,"./img/geral/Paint_JYH.png");
	
	#endif
}