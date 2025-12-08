//Executar Nível
#include "JYH_Header.h"
#include <stdlib.h>

void JYH_Destroy_EX(JYH_Level_Runner* ex){
    SDL_DestroyTexture(ex->txt_gem_count);
    SDL_DestroyTexture(ex->txt_tempo);
    SDL_DestroyTexture(ex->tb.txt);
    SDL_DestroyTexture(ex->botao_V.txt);
    SDL_DestroyTexture(ex->botao_R.txt);
    SDL_DestroyTexture(ex->clock.txt);
    SDL_DestroyTexture(ex->gem.txt);
    SDL_DestroyTexture(ex->txt_defeat);
    SDL_DestroyTexture(ex->txt_victory);
    SDL_DestroyTexture(ex->txt_star);
    
    free(ex->lvl.mat);//temporario
    free(ex->lvl.objetos);
    for(int i = 0; i < N_OBJECTS; i++)SDL_DestroyTexture(ex->txts[i]);//desaloca texturas
    free(ex->txts);
    
    free(ex->lvl.l_obj);
}

void JYH_EX_Atualiza_Timer(SDL_Renderer* ren, TTF_Font* fnt, JYH_Level_Runner* ex){
	char S[10];
	SDL_Color clr = {0xff,0x00,0x00,0x00};
	Uint32 s = (ex->tempo_restante/1000)%60;
	Uint32 m = (ex->tempo_restante/1000)/60;
	sprintf(S,"%02u:%02u",m,s);
	ex->txt_tempo = AUX_CriarTexto(ren,fnt,S,clr);
	
}
void JYH_EX_Atualiza_GemCount(SDL_Renderer* ren,TTF_Font* fnt, JYH_Level_Runner* ex){
	char S[10];
	SDL_Color clr = {0xff,0x00,0x00,0x00};
	sprintf(S,"%u/%u",ex->gem_collected,ex->gem_total);
	ex->txt_gem_count = AUX_CriarTexto(ren,fnt,S,clr);
}

void JYH_DRAW_EX(SDL_Renderer* ren, JYH_Level_Runner* ex){
	SDL_SetRenderDrawColor(ren,0xff,0xff,0xff,0x00);//background
	SDL_RenderClear(ren);
	JYH_Draw_Grade_EX(ren, &ex->lvl, &ex->cam, ex->lvl.l_obj);
	
	if(ex->state == EX_VICTORY){
		SDL_Rect r = {100,200,1000,500};
		SDL_Rect c = {0,0,64,64};
		SDL_RenderCopy(ren,ex->txt_victory,NULL,&r);
		//desenhar estrelas
		
		r = (SDL_Rect){600 -108,132 ,64,64};
		c.x = 64*((ex->goal&4)==0);
		SDL_RenderCopy(ren,ex->txt_star,&c,&r);
		
		r.x = 600 - 32;
		c.x = 64*((ex->goal&2)==0);
		SDL_RenderCopy(ren,ex->txt_star,&c,&r);
		
		r.x = 600 + 48;
		c.x = 64*((ex->goal&1)==0);
		SDL_RenderCopy(ren,ex->txt_star,&c,&r);
		
	}else if(ex->state == EX_DEFEAT){
		SDL_Rect r = {100,200,1000,500};
		SDL_RenderCopy(ren,ex->txt_defeat,NULL,&r);
	}
	
    AUX_Draw_Icon(ren,&ex->tb);
    
    AUX_Draw_Icon(ren,&ex->gem);
    AUX_Draw_Icon(ren,&ex->clock);
	
    SDL_RenderCopy(ren,ex->txt_gem_count,NULL,&ex->contagem_gemas);
    SDL_RenderCopy(ren,ex->txt_tempo,NULL,&ex->contagem_tempo);
	
	//desenhar botões
	
    AUX_Draw_Icon(ren,&ex->botao_R);
    AUX_Draw_Icon(ren,&ex->botao_V);
    
    
}


void JYH_EX(JYH_GameState* jogo){//Atualizar
	static SDL_Point p;
	JYH_DRAW_EX(jogo->ren,&jogo->ex);
	JYH_Objeto* obj1;JYH_Objeto* obj2;
	if(AUX_WaitEventTimeoutCount(&(jogo->evt),&(jogo->espera))){//trocar por exercicio
		switch(jogo->evt.type){
			case SDL_MOUSEBUTTONUP://verifica os cliques do botão
				p = (SDL_Point){(int)jogo->evt.button.x,(int)jogo->evt.button.y};
				
				if (SDL_PointInRect(&p,&jogo->ex.botao_V.r)){
					JYH_GoBack(jogo);
					return;
				}
				else if (SDL_PointInRect(&p,&jogo->ex.botao_R.r)){
					JYH_Destroy_EX(&jogo->ex);
					JYH_Load_EX(jogo);
				}
				
				break;
			case SDL_QUIT:
				AUX_Empilha(&jogo->state,JYH_END_GAME);
				JYH_Destroy_EX(&jogo->ex);
				return;
			case SDL_USEREVENT:
				switch(jogo->evt.user.code){
					case JYH_EX_COLLISION:
						obj1 = jogo->evt.user.data1;
						obj2 = jogo->evt.user.data2;
						if(obj1->type == JYH_OBJ_PLAYER && obj2->type == JYH_OBJ_GEM){
							jogo->ex.gem_collected++;
							obj2->g.r.y += 100000;//para sair da tela
							obj2->type == N_OBJECTS;
							if(jogo->ex.gem_collected == jogo->ex.gem_total)jogo->ex.goal |=2;
							JYH_EX_Atualiza_GemCount(jogo->ren,jogo->fnt,&jogo->ex);
						}
						else if(obj1->type == JYH_OBJ_PLAYER && obj2->type == JYH_OBJ_TROFEU){
							jogo->ex.state = EX_VICTORY;
						}else if(obj1->type == JYH_OBJ_PLAYER && obj2->type == JYH_OBJ_INIMIGO){
							jogo->ex.state = EX_DEFEAT;
							jogo->ex.goal = 0;
						}
						break;
				}
		}
		//Atualiza as entidades
		if(jogo->ex.state == EX_RUNNING)for(int i = 0; i < jogo->ex.lvl.qtd_obj; i++)JYH_Update_Obj(&jogo->ex.lvl.objetos[i],&(jogo->evt));
	}else{
		//eventos baseados em tempo
		jogo->espera = 10;
		if(jogo->ex.tempo_restante && jogo->ex.state == EX_RUNNING){
			jogo->ex.tempo_restante -= 10;
			if(!jogo->ex.tempo_restante)jogo->ex.goal -= 4;//retira o bit do tempo
		}
		SDL_DestroyTexture(jogo->ex.txt_tempo);
		JYH_EX_Atualiza_Timer(jogo->ren,jogo->fnt,&jogo->ex);
		AUX_CriarEvento(JYH_EX_UPDATE_FRAME,NULL,NULL);
	}
	qsort(jogo->ex.lvl.l_obj,jogo->ex.lvl.qtd_obj,sizeof(JYH_Objeto*),JYH_Comp_Obj);

}
void JYH_EX_Load_Txts(SDL_Renderer* ren, SDL_Texture*** txts){
	char S[100],nome[50];
	int temp;
	FILE* arq = fopen(PATH_OBJ,"r");
	assert(arq != NULL);
	fscanf(arq,"%u",&temp);
	(*txts) = (SDL_Texture**)malloc(sizeof(SDL_Texture*)*(N_OBJECTS));
	for(int i = 0; i < N_OBJECTS;i++){
		fscanf(arq,"%s %d",nome,&temp);
		sprintf(S,OBJ_GET_IMG,nome);
		(*txts)[i] = IMG_LoadTexture(ren,S);
		assert((*txts)[i]!= NULL);
	}
	fclose(arq);
}

void JYH_EX_Start_Obj(JYH_Level_Runner* ex,Uint32 z,SDL_Texture** txts){
	ex->lvl.objetos = (JYH_Objeto*)malloc(sizeof(JYH_Objeto)*(ex->lvl.qtd_obj));
	ex->lvl.l_obj = (JYH_Objeto**)malloc(sizeof(JYH_Objeto*)*(ex->lvl.qtd_obj));

	JYH_Objeto* l_obj = ex->lvl.objetos;
	JYH_Tile* mat = ex->lvl.mat;
	int k = 0;
	int w = ex->lvl.w;
	int l = (ex->lvl.h)*(ex->lvl.w);
	for(int i = 0; i < l; i++)if(mat[i].o == JYH_OBJ_PLAYER){
		l_obj[k].type = mat[i].o;
		ex->lvl.l_obj[k] = &l_obj[k];
		JYH_Start_Obj(&l_obj[k],ex,i);
		k++;
		mat[i].o = N_OBJECTS;
		break;//só existe 1 player
	}
	for(int i = 0; i < l; i++){
		if(mat[i].o != N_OBJECTS){
			l_obj[k].type = mat[i].o;
			ex->lvl.l_obj[k] = &l_obj[k];
			JYH_Start_Obj(&l_obj[k],ex,i);
			k++;
			mat[i].o = N_OBJECTS;
		}
	}
}


void JYH_Load_EX(JYH_GameState* jogo){
	char S[100];
	SDL_Color clr = {0xff,0x00,0x00,0x00};
	JYH_Read_lvl(&jogo->ex.lvl);
	JYH_EX_Load_Txts(jogo->ren,&jogo->ex.txts);
	JYH_Inicia_Camera(&jogo->ex.cam,(SDL_Rect){0,100,1200,700},(SDL_Rect){0,0,1200,700},64);
	jogo->ex.gem_total = 0;
	jogo->ex.state = EX_RUNNING;
	jogo->ex.goal = 5;//3 bits marcados, começa 101,(tempo, joias,completar o nível)
	JYH_EX_Start_Obj(&jogo->ex,64,jogo->ex.txts);

	jogo->ex.contagem_gemas =(SDL_Rect){250,25,100,50};//onde é escrita a razão entre as gemas do nível e as gemas coletadas
	jogo->ex.contagem_tempo = (SDL_Rect){450,25,100,50};//onde é escrita a contagem de tempo
    
    AUX_Start_Icon(jogo->ren,&jogo->ex.tb     ,IMG_LE_TB   ,(SDL_Rect){0,0,1200,100},1);
    AUX_Start_Icon(jogo->ren,&jogo->ex.botao_V,IMG_B_BACK  ,(SDL_Rect){25,25,50,50},1);
	AUX_Start_Icon(jogo->ren,&jogo->ex.botao_R,IMG_B_RESTART,(SDL_Rect){100,25,50,50},1);//trocar
	AUX_Start_Icon(jogo->ren,&jogo->ex.gem    ,IMG_I_GEM   ,(SDL_Rect){175,25,50,50},1);
	AUX_Start_Icon(jogo->ren,&jogo->ex.clock  ,IMG_I_CLOCK ,(SDL_Rect){375,25,50,50},1);
    
	sprintf(S,IMG_GET_THEME,jogo->ex.lvl.tema);
	jogo->ex.lvl.txt_theme = IMG_LoadTexture(jogo->ren,S);
	jogo->ex.txt_star = IMG_LoadTexture(jogo->ren,IMG_STAR);
	jogo->ex.txt_defeat = AUX_CriarTexto(jogo->ren,jogo->fnt,"Derrota!",clr);
	jogo->ex.txt_victory = AUX_CriarTexto(jogo->ren,jogo->fnt,"Vitoria!",clr);

	jogo->ex.gem_collected = 0;
	jogo->ex.trofeu_pego = 0;
	jogo->ex.tempo_restante = 90000;//no futuro o timer faz parte do nível
	
	JYH_EX_Atualiza_GemCount(jogo->ren,jogo->fnt,&jogo->ex);
	
	JYH_EX_Atualiza_Timer(jogo->ren,jogo->fnt,&jogo->ex);
}
