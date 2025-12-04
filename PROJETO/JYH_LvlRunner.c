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
    
    free(jogo->ex.lvl.mat);//temporario
    free(jogo->ex.lvl.objetos);
    for(int i = 0; i < N_OBJECTS; i++)SDL_DestroyTexture(jogo->ex.txts[i]);//desaloca texturas
    free(jogo->ex.txts);
}

void JYH_EX_to_LS(JYH_GameState* jogo){
	JYH_Level_Selection temp;
	strcpy(temp.nome,jogo->ex.lvl.nome_mundo);
	JYH_Destroy_EX(jogo);
	jogo->ls = temp;
	JYH_Load_LS(jogo);
}

void JYH_EX_to_LE(JYH_GameState* jogo){
	JYH_Editor temp;
	temp.lvl = jogo->ex.lvl;
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

void JYH_DRAW_EX(SDL_Renderer* ren, JYH_Level_Runner* ex){
	SDL_SetRenderDrawColor(ren,0xff,0xff,0xff,0x00);//background
	SDL_RenderClear(ren);
	
	JYH_Draw_Grade_EX(ren, &ex->lvl, &ex->cam, ex->lvl.objetos);
	
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
	
	if(AUX_WaitEventTimeoutCount(&(jogo->evt),&(jogo->espera))){//trocar por exercicio
		switch(jogo->evt.type){
			case SDL_MOUSEBUTTONUP://verifica os cliques do botão
				p = (SDL_Point){(int)jogo->evt.button.x,(int)jogo->evt.button.y};
				
				if (SDL_PointInRect(&p,&jogo->ex.botao_V.r))JYH_EX_goback(jogo);
				else if (SDL_PointInRect(&p,&jogo->ex.botao_R.r)){/*Reinicia o nivel*/}
				
				break;
			case SDL_QUIT:
				AUX_Empilha(&jogo->state,JYH_END_GAME);
				JYH_Destroy_EX(jogo);
				break;
			case SDL_KEYDOWN:
				switch(jogo->evt.key.keysym.sym){//temporario
					case SDLK_UP:
						JYH_Move_Camera(&jogo->ex.cam,&jogo->ex.lvl,0,-4);
						break;
					case SDLK_DOWN:
						JYH_Move_Camera(&jogo->ex.cam,&jogo->ex.lvl,0,4);
						break;
					case SDLK_LEFT:
						JYH_Move_Camera(&jogo->ex.cam,&jogo->ex.lvl,-4,0);
						break;
					case SDLK_RIGHT:
						JYH_Move_Camera(&jogo->ex.cam,&jogo->ex.lvl,4,0);
						break;
				}
				break;
		}
	}else{
		//eventos baseados em tempo
		jogo->espera = 10;
	}
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

void JYH_EX_Start_Obj(JYH_Nivel* lvl,Uint32 z,SDL_Texture** txts){
	lvl->objetos = (JYH_Objeto*)malloc(sizeof(JYH_Objeto)*lvl->qtd_obj);

	JYH_Objeto* l_obj = lvl->objetos;
	JYH_Tile* mat = lvl->mat;
	int k = 0;
	int w = lvl->w;
	int l = (lvl->h)*(lvl->w);
	for(int i = 0; i < l; i++){
		if(mat[i].o != N_OBJECTS){
			l_obj[k].type = mat[i].o;
			l_obj[k].p = (SDL_Point){z*(i%w),z*(i/w)};
			l_obj[k].txt = txts[mat[i].o];
			l_obj[k].s = 0;//estado inicial é sempre 0
			l_obj[k].f = 0;
			l_obj[k].n_f = 4;//fazer variar com o sprite
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
	JYH_EX_Start_Obj(&jogo->ex.lvl,64,jogo->ex.txts);

	JYH_Inicia_Camera(&jogo->ex.cam,(SDL_Rect){0,0,1200,700},(SDL_Rect){0,0,1200,700},64);

	jogo->ex.contagem_gemas =(SDL_Rect){250,25,100,50};//onde é escrita a razão entre as gemas do nível e as gemas coletadas
	jogo->ex.contagem_tempo = (SDL_Rect){450,25,100,50};//onde é escrita a contagem de tempo
    jogo->ex.txt_tempo = AUX_CriarTexto(jogo->ren,jogo->fnt,"01:30",clr);
    jogo->ex.txt_gem_count = AUX_CriarTexto(jogo->ren,jogo->fnt,"2/10",clr);
    
    AUX_Start_Icon(jogo->ren,&jogo->ex.tb     ,IMG_LE_TB   ,(SDL_Rect){0,0,1200,100},1);
    AUX_Start_Icon(jogo->ren,&jogo->ex.botao_V,IMG_B_BACK  ,(SDL_Rect){25,25,50,50},1);
	AUX_Start_Icon(jogo->ren,&jogo->ex.botao_R,IMG_B_RESTART,(SDL_Rect){100,25,50,50},1);//trocar
	AUX_Start_Icon(jogo->ren,&jogo->ex.gem    ,IMG_I_GEM   ,(SDL_Rect){175,25,50,50},1);
	AUX_Start_Icon(jogo->ren,&jogo->ex.clock  ,IMG_I_CLOCK ,(SDL_Rect){375,25,50,50},1);
    
	sprintf(S,IMG_GET_THEME,jogo->ex.lvl.tema);
	jogo->ex.lvl.txt_theme = IMG_LoadTexture(jogo->ren,S);
	
	
	
	jogo->ex.timer = 0;//No Jogo Final depende do nível a ser carregado!!!
	jogo->ex.gemas_coletadas = 0;
	jogo->ex.tesouro_pego = 0;
	jogo->ex.tempo_de_jogo = 0;
}
