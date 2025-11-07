//Selecionar Nível
#include "JYH_Header.h"

void JYH_Destroy_LS(JYH_GameState* jogo){
	for(int i = 0; i < jogo->ls.n; i++){
		SDL_DestroyTexture(jogo->ls.niveis[i].txt_nome);
	}//desalocar detalhes da lista de níveis
	SDL_DestroyTexture(jogo->ls.botao_V.txt);
	SDL_DestroyTexture(jogo->ls.titulo.txt);
	SDL_DestroyTexture(jogo->ls.txt_background);
	SDL_DestroyTexture(jogo->ls.txt_lvl_icon);
	
	free(jogo->ls.niveis);
}

void JYH_LS_to_WS(JYH_GameState* jogo){
	JYH_World_Selection temp;
	AUX_Desempilha(&jogo->state);
	JYH_Destroy_LS(jogo);
	jogo->ws = temp;
	JYH_Load_WS(jogo);
}
void JYH_LS_to_EX(JYH_GameState* jogo){
	JYH_Level_Runner temp;
	strcpy(temp.pathMundo,jogo->ls.path                        );//
	strcpy(temp.pathNivel,jogo->ls.niveis[jogo->ls.i_sel].path);
	strcpy(temp.nome     ,jogo->ls.niveis[jogo->ls.i_sel].nome);
	AUX_Empilha(&jogo->state,JYH_state_EX);
	JYH_Destroy_LS(jogo);
	jogo->ex = temp;
	JYH_Load_EX(jogo);
}

void JYH_LS(JYH_GameState* jogo){//Atualizar
	static SDL_Point p;
	SDL_Rect r;
	SDL_Rect r_nome;
	
	SDL_RenderCopy(jogo->ren,jogo->ls.txt_background,NULL,NULL);
	AUX_Draw_Icon(jogo->ren,&jogo->ls.titulo);
	AUX_Draw_Icon(jogo->ren,&jogo->ls.botao_V);
	
	r.w = 64;
	r.h = 64;
	r_nome.w = 64;
	r_nome.h = 32;
	for(int i = 0; i < jogo->ls.n; i++){//desenha os botões dos níveis
		r.x = 64  + (i%9)*128;
		r.y = 300 + (i/9)*128;
		SDL_RenderCopy(jogo->ren,jogo->ls.txt_lvl_icon,NULL,&r);
		r_nome.x = r.x;
		r_nome.y = r.y+r.w;
		SDL_RenderCopy(jogo->ren,jogo->ls.niveis[i].txt_nome,NULL,&r_nome);	
	}
	
	if(AUX_WaitEventTimeoutCount(&(jogo->evt),&(jogo->espera))){//trocar por exercicio
		switch(jogo->evt.type){
			case SDL_MOUSEBUTTONDOWN://verifica os cliques do botão
				p = (SDL_Point){(int)jogo->evt.button.x,(int)jogo->evt.button.y};
				
				if (SDL_PointInRect(&p,&jogo->ls.botao_V.r))JYH_LS_to_WS(jogo);
				
				for(int i = 0; i < jogo->ls.n; i++){//verifica se clicou em um mundo
					r.x = 64  + (i%9)*128;r.y = 300 + (i/9)*128;
					if(SDL_PointInRect(&p,&r)){
						jogo->ls.i_sel = i;
						JYH_LS_to_EX(jogo);
						break;
					}
				}
				break;
			case SDL_QUIT:
				AUX_Empilha(&jogo->state,JYH_END_GAME);
				JYH_Destroy_LS(jogo);
				break;
		}
	}else{
		//eventos baseados em tempo
	}
}

void JYH_Load_LS(JYH_GameState* jogo){
	char S[100];
	
	#ifdef _WIN32

	AUX_Start_Icon(jogo->ren,&jogo->ls.botao_V,"img\\botao\\Back.png",(SDL_Rect){25,25,50,50}   ,1);
	jogo->ls.txt_background = IMG_LoadTexture(jogo->ren,"img\\Menu\\Background_JYH.png");
	
	#elif __linux__

	AUX_Start_Icon(jogo->ren,&jogo->ls.botao_V,"./img/botao/Back.png",(SDL_Rect){25,25,50,50}   ,1);
	jogo->ls.txt_background = IMG_LoadTexture(jogo->ren,"./img/menu/Background_JYH.png");
	
	#endif
	sprintf(S,".$img$%s$lvlicon.png",jogo->ls.nome);
	AUX_AdaptarString(S);
	jogo->ls.txt_lvl_icon =  IMG_LoadTexture(jogo->ren, S);
	sprintf(S,".$img$%s$titulo.png",jogo->ls.nome);
	AUX_AdaptarString(S);
	AUX_Start_Icon(jogo->ren,&jogo->ls.titulo,S,(SDL_Rect){450,100,300,90},1);
	
	sprintf(S,".$JYH$%s$Niveis.txt",jogo->ls.nome);//Lê o arquivo
	AUX_AdaptarString(S);
	FILE* arq = fopen(S,"r");
	assert(arq != NULL);
	
	fscanf(arq,"%d",&jogo->ls.n);//carrega os níveis(assumir sempre carregamento apropriado
	
	jogo->ls.niveis = (JYH_Nivel*)malloc(sizeof(JYH_Nivel)*jogo->ls.n);

	SDL_Color clr = {0x00,0x00,0x00,0x00};
	for(int i = 0; i < jogo->ls.n; i++){
		fscanf(arq,"%s",jogo->ls.niveis[i].nome);
    
		jogo->ls.niveis[i].txt_nome = AUX_CriarTexto(jogo->ren,jogo->fnt,jogo->ls.niveis[i].nome,clr);
	}
	
	fclose(arq);
}