//Selecionar Nível
#include "JYH_Header.h"

void JYH_DestroyLS(JYH_GameState* jogo){
	for(int i = 0; i < jogo->sel.n; i++){
		SDL_DestroyTexture(jogo->sel.niveis[i].txt_nome);
	}//desalocar detalhes da lista de níveis
	free(jogo->sel.niveis);
}

void JYH_GameLvlSelection(JYH_GameState* jogo){//Atualizar
	static SDL_Point p;
	SDL_Rect r;
	SDL_Rect r_nome;
	
	SDL_SetRenderDrawColor(jogo->ren,0xff,0xff,0xff,0x00);//background
	SDL_RenderClear(jogo->ren);
	SDL_RenderCopy(jogo->ren,jogo->sel.txt_background,NULL,&jogo->sel.r_background);
	SDL_RenderCopy(jogo->ren,jogo->sel.txt_title, NULL, &jogo->sel.title);
		SDL_RenderCopy(jogo->ren,jogo->sel.txt_voltar,NULL,&jogo->sel.botao_voltar);
	
	r.w = 64;
	r.h = 64;
	r_nome.w = 64;
	r_nome.h = 32;
	for(int i = 0; i < jogo->sel.n; i++){//desenha os botões dos níveis
		r.x = 64  + (i%9)*128;
		r.y = 300 + (i/9)*128;
		SDL_RenderCopy(jogo->ren,jogo->sel.txt_lvl_icon,NULL,&r);
		r_nome.x = r.x;
		r_nome.y = r.y+r.w;
		SDL_RenderCopy(jogo->ren,jogo->sel.niveis[i].txt_nome,NULL,&r_nome);	
	}
	
	if(AUX_WaitEventTimeoutCount(&(jogo->evt),&(jogo->espera))){//trocar por exercicio
		switch(jogo->evt.type){
			case SDL_MOUSEBUTTONDOWN://verifica os cliques do botão
				p.x = (int)jogo->evt.button.x; p.y = (int)jogo->evt.button.y;
				
				if (SDL_PointInRect(&p,&jogo->sel.botao_voltar))jogo->estado_tela = 2;
				
				for(int i = 0; i < jogo->sel.n; i++){//verifica se clicou em um mundo
					r.x = 64  + (i%9)*128;
					r.y = 300 + (i/9)*128;
					if(SDL_PointInRect(&p,&r)){
						jogo->estado_tela = 3;
						jogo->sel.i_sel = i;
						break;
					}
				}
				
				break;
			case SDL_QUIT:
				jogo->estado = JYH_END_GAME;
				JYH_DestroyLS(jogo);
				break;
		}
	}else{
		//eventos baseados em tempo
	}
}

void JYH_GameLoadSel(JYH_GameState* jogo){
	
	FILE* arq = fopen(jogo->sel.path,"r");//Assumir path sempre correto
	
	jogo->sel.title =  (SDL_Rect){450,100,300,90};
	jogo->sel.botao_voltar = (SDL_Rect){25,25,50,50};
	jogo->sel.r_background =(SDL_Rect){0,0,jogo->w_tela,jogo->h_tela};
	
	#ifdef _WIN32
	
	jogo->sel.txt_title = IMG_LoadTexture(jogo->ren,"img\\Menu\\Titulo_JYH.png");//trocar
	jogo->sel.txt_voltar = IMG_LoadTexture(jogo->ren,"img\\geral\\Back_JYH.png");
	jogo->sel.txt_background = IMG_LoadTexture(jogo->ren,"img\\Menu\\Background_JYH.png");
	jogo->sel.txt_lvl_icon  = IMG_LoadTexture(jogo->ren,"img\\Menu\\Modo_Historia_JYH.png");//trocar
	
	#elif __linux__
	
	jogo->sel.txt_title = IMG_LoadTexture(jogo->ren,"./img/menu/Titulo_JYH.png");//trocar
	jogo->sel.txt_voltar = IMG_LoadTexture(jogo->ren,"./img/geral/Back_JYH.png");
	jogo->sel.txt_background = IMG_LoadTexture(jogo->ren,"./img/menu/Background_JYH.png");
	jogo->sel.txt_lvl_icon  = IMG_LoadTexture(jogo->ren,"./img/menu/Modo_Historia_JYH.png");//trocar
	
	#endif

	fscanf(arq,"%d",&jogo->sel.n);//carrega os níveis(assumir sempre carregamento apropriado
	jogo->sel.niveis = (JYH_Nivel*)malloc(sizeof(JYH_Nivel)*jogo->sel.n);
	SDL_Color clr = {0x00,0x00,0x00,0x00};
	for(int i = 0; i < jogo->sel.n; i++){
		fscanf(arq,"%s",jogo->sel.niveis[i].nome);
		fscanf(arq,"%s",jogo->sel.niveis[i].path);
		jogo->sel.niveis[i].txt_nome = AUX_CriarTexto(jogo->ren,jogo->fnt,jogo->sel.niveis[i].nome,clr);
	}
	
	fclose(arq);
	
	jogo->estado_tela = 1;
}

void JYH_LS_to_WS(JYH_GameState* jogo){
	printf("->WS\n");
	JYH_World_Selection temp;
	jogo->prev = jogo->estado;
	jogo->estado_tela = 0;
	jogo->estado = JYH_WORLD_SELECTION;
	JYH_DestroyLS(jogo);
	jogo->worlds = temp;
	
}
void JYH_LS_to_EX(JYH_GameState* jogo){
	printf("->EX\n");
	JYH_Level_Runner temp;
	strcpy(temp.pathMundo,jogo->sel.path                        );//
	strcpy(temp.pathNivel,jogo->sel.niveis[jogo->sel.i_sel].path);
	strcpy(temp.nome     ,jogo->sel.niveis[jogo->sel.i_sel].nome);
	jogo->prev = jogo->estado;
	jogo->estado_tela = 0;
	jogo->estado = JYH_LVL_EXEC;
	JYH_DestroyLS(jogo);
	jogo->exec = temp;
}

void JYH_LS(JYH_GameState* jogo){
	switch(jogo->estado_tela){
		case 0://load
			JYH_GameLoadSel(jogo);
			break;
		case 1://Executando
			JYH_GameLvlSelection(jogo);
			break;
		case 2://Voltar à tela de seleção de mundos
			JYH_LS_to_WS(jogo);
			break;
		case 3://selecionar nível para execução
			JYH_LS_to_EX(jogo);
			break;
	}
}
