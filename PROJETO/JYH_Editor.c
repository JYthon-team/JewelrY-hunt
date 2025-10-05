//Editor Nível
#include "JYH_Header.h"

void JYH_Destroy_LE(JYH_GameState* jogo){//desalocar
	SDL_DestroyTexture(jogo->edit.txt_run );
	SDL_DestroyTexture(jogo->edit.txt_salvar );
	SDL_DestroyTexture(jogo->edit.txt_sb );
	SDL_DestroyTexture(jogo->edit.txt_tb );
	SDL_DestroyTexture(jogo->edit.txt_voltar);
	free(jogo->edit.lvl.mat);//temporario
}

void JYH_Draw_Grade(JYH_GameState* jogo){
	//grade contida no retângulo {0,100,1000,600}
	//assumir nivel de  50x30
	Uint32 stepx = jogo->edit.editor.w/jogo->edit.lvl.w;
	Uint32 stepy = jogo->edit.editor.h/jogo->edit.lvl.h;
	SDL_Rect r = {0,0,stepx,stepy};
	SDL_SetRenderDrawColor(jogo->ren,0x00,0x00,0x00,0x00);
	for(int i = 0; i < jogo->edit.lvl.w; i++)SDL_RenderDrawLine(jogo->ren,i*stepx,100,i*stepx,700);
	for(int i = 0; i < jogo->edit.lvl.h; i++)SDL_RenderDrawLine(jogo->ren,0,100+i*stepy,1000,100+i*stepy);
	
	for(int i = 0; i < jogo->edit.lvl.w; i ++){
		for(int j = 0; j < jogo->edit.lvl.h; j++){
			if(jogo->edit.lvl.mat[i*(jogo->edit.lvl.w)+j]){
				r.x = i*stepx;
				r.y = 100 + j*stepy;
				SDL_RenderFillRect(jogo->ren,&r);
			}
		}
	}
	
}

void JYH_Run_LE(JYH_GameState* jogo){//Atualizar
	static SDL_Point p;
	int idx;
	
	SDL_SetRenderDrawColor(jogo->ren,0xff,0xff,0xff,0x00);//background
	SDL_RenderClear(jogo->ren);
	
	SDL_RenderCopy(jogo->ren,jogo->edit.txt_tb,NULL,&jogo->edit.top_bar );
	SDL_RenderCopy(jogo->ren,jogo->edit.txt_sb,NULL,&jogo->edit.side_bar);
	
	if(AUX_WaitEventTimeoutCount(&(jogo->evt),&(jogo->espera))){//trocar por exercicio
		switch(jogo->evt.type){
			case SDL_MOUSEMOTION:
				p = (SDL_Point){(int)jogo->evt.motion.x,(int)jogo->evt.motion.y};
				if(jogo->edit.pintar &&  jogo->edit.press && SDL_PointInRect(&p,&jogo->edit.editor)){
					idx = (p.x/(jogo->edit.editor.w/jogo->edit.lvl.w))*jogo->edit.lvl.w + ((p.y-100)/(jogo->edit.editor.h/jogo->edit.lvl.h));
					jogo->edit.lvl.mat[idx] = !jogo->edit.lvl.mat[idx];
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				jogo->edit.press = SDL_TRUE;
				p = (SDL_Point){(int)jogo->evt.button.x,(int)jogo->evt.button.y};
				if(jogo->edit.pintar &&  jogo->edit.press && SDL_PointInRect(&p,&jogo->edit.editor)){
					idx = (p.x/(jogo->edit.editor.w/jogo->edit.lvl.w))*jogo->edit.lvl.w + ((p.y-100)/(jogo->edit.editor.h/jogo->edit.lvl.h));
					jogo->edit.lvl.mat[idx] = !jogo->edit.lvl.mat[idx];
				}
				
				break;
			case SDL_MOUSEBUTTONUP://verifica os cliques do botão
				p = (SDL_Point){(int)jogo->evt.button.x,(int)jogo->evt.button.y};
				jogo->edit.press = SDL_FALSE;
				if      (SDL_PointInRect(&p,&jogo->edit.botao_voltar))jogo->estado_tela = 2;
				else if (SDL_PointInRect(&p,&jogo->edit.botao_salvar))jogo->estado_tela = 1;
				else if (SDL_PointInRect(&p,&jogo->edit.botao_run   ))jogo->estado_tela = 4;
				else if (SDL_PointInRect(&p,&jogo->edit.botao_paint ))jogo->edit.pintar = !jogo->edit.pintar;
				break;
			case SDL_QUIT:
				jogo->estado = JYH_END_GAME;
				JYH_Destroy_LE(jogo);
				break;
		}
	}else{
	/*eventos baseados em tempo*/
	}
	
	//desenhar botões
	
	SDL_RenderCopy(jogo->ren,jogo->edit.txt_voltar,NULL,&jogo->edit.botao_voltar);
	SDL_RenderCopy(jogo->ren,jogo->edit.txt_salvar,NULL,&jogo->edit.botao_salvar);
	SDL_RenderCopy(jogo->ren,jogo->edit.txt_run   ,NULL,&jogo->edit.botao_run);
	SDL_RenderCopy(jogo->ren,jogo->edit.txt_paint ,NULL,&jogo->edit.botao_paint);
	JYH_Draw_Grade(jogo);

}

void JYH_Load_LE(JYH_GameState* jogo){
	SDL_SetRenderDrawColor(jogo->ren,0xff,0xff,0xff,0x00);//trocar por uma tela de loading
	SDL_RenderClear(jogo->ren);
	SDL_RenderPresent(jogo->ren);
	
	jogo->edit.botao_voltar = (SDL_Rect){25,25,50,50};
	jogo->edit.botao_salvar = (SDL_Rect){100,25,50,50};
	jogo->edit.botao_run = (SDL_Rect){175,25,50,50};
	jogo->edit.botao_paint = (SDL_Rect){250,25,50,50};
	jogo->edit.editor = (SDL_Rect){0,100,jogo->w_tela - 200,jogo->h_tela - 100};
	jogo->edit.side_bar = (SDL_Rect){1000,100,200,jogo->h_tela - 100};
	jogo->edit.top_bar = (SDL_Rect){0,0,jogo->w_tela,100};
	jogo->edit.pintar = SDL_FALSE;
	jogo->edit.press = SDL_FALSE;
	jogo->edit.drag = SDL_FALSE;
	
	//teste
	jogo->edit.lvl.w = 50;
	jogo->edit.lvl.h = 30;
	jogo->edit.lvl.mat = (Uint32*)malloc(sizeof(Uint32)*(jogo->edit.lvl.w)*(jogo->edit.lvl.h));
	memset(jogo->edit.lvl.mat,0,sizeof(Uint32)*(jogo->edit.lvl.w)*(jogo->edit.lvl.h));
	
	#ifdef _WIN32
	
	jogo->edit.txt_tb = IMG_LoadTexture(jogo->ren,"img\\geral\\top_bar_JYH.png");//trocar
	jogo->edit.txt_sb = IMG_LoadTexture(jogo->ren,"img\\geral\\side_bar_JYH.png");//trocar
	jogo->edit.txt_voltar = IMG_LoadTexture(jogo->ren,"img\\geral\\Back_JYH.png");
	jogo->edit.txt_salvar = IMG_LoadTexture(jogo->ren,"img\\geral\\Save_JYH.png");//trocar
	jogo->edit.txt_run = IMG_LoadTexture(jogo->ren,"img\\geral\\Run_JYH.png");//trocar
	jogo->edit.txt_paint = IMG_LoadTexture(jogo->ren,"img\\geral\\Paint_JYH.png");//trocar
	
	#elif __linux__
	
	jogo->edit.txt_tb = IMG_LoadTexture(jogo->ren,"./img/geral/top_bar_JYH.png");
	jogo->edit.txt_sb = IMG_LoadTexture(jogo->ren,"./img/geral/side_bar_JYH.png");
	jogo->edit.txt_voltar = IMG_LoadTexture(jogo->ren,"./img/geral/Back_JYH.png");
	jogo->edit.txt_salvar = IMG_LoadTexture(jogo->ren,"./img/geral/Save_JYH.png");
	jogo->edit.txt_run = IMG_LoadTexture(jogo->ren,"./img/geral/Run_JYH.png");
	jogo->edit.txt_paint = IMG_LoadTexture(jogo->ren,"./img/geral/Paint_JYH.png");//trocar
	
	#endif
	
	jogo->estado_tela = 1;
}

void JYH_LE_to_PL(JYH_GameState* jogo){//editor à biblioteca do player
	JYH_Level_Selection_P temp;
	jogo->prev = jogo->estado;
	jogo->estado_tela = 0;
	jogo->estado = JYH_state_PL;
	JYH_Destroy_LE(jogo);
	jogo->selP = temp;
}
void JYH_LE_to_MM(JYH_GameState* jogo){//editor ao menu inicial
	JYH_Menu temp;
	jogo->prev = jogo->estado;
	jogo->estado = JYH_state_MM;
	jogo->estado_tela = 0;
	JYH_Destroy_LE(jogo);
	jogo->menu = temp;
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
	jogo->estado_tela = 0;
	jogo->estado = JYH_state_EX;
	JYH_Destroy_LE(jogo);
	jogo->exec = temp;
}
void JYH_LE(JYH_GameState* jogo){
	switch(jogo->estado_tela){
		case 0://load
			JYH_Load_LE(jogo);
			break;
		case 1://editor
			JYH_Run_LE(jogo);
			break;
		case 2://voltar
			JYH_LE_goback(jogo);
			break;
		case 3://salvar
			printf("Salvando\n");
			jogo->estado_tela = 1;
			break;
		case 4://executar
			JYH_LE_to_EX(jogo);
			break;
	}
}