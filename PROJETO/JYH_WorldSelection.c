//Selecionar Mundo
#include "JYH_Header.h"

void JYH_GameWorldSelection(JYH_GameState* jogo){//Atualizar
	static SDL_Point p;
	static SDL_Rect r;//desenho das capas do mundo

	SDL_SetRenderDrawColor(jogo->ren,0xff,0xff,0xff,0x00);//background
	SDL_RenderClear(jogo->ren);
	SDL_RenderCopy(jogo->ren,jogo->worlds.txt_background,NULL,&jogo->worlds.r_background);
	//SDL_SetRenderDrawColor(jogo->ren,0xff,0xff,0x00,0x00);//"Modo Historia"
	//SDL_RenderFillRect(jogo->ren,&jogo->worlds.title);
	SDL_RenderCopy(jogo->ren,jogo->worlds.txt_title,NULL,&jogo->worlds.title);
	
	r.w = 300;
	r.h = 300;
	r.y = 300;
	
	if(AUX_WaitEventTimeoutCount(&(jogo->evt),&(jogo->espera))){//trocar por exercicio
		switch(jogo->evt.type){
			case SDL_MOUSEBUTTONDOWN://verifica os cliques do botão
				p.x = (int)jogo->evt.button.x; p.y = (int)jogo->evt.button.y;
				
				if (SDL_PointInRect(&p,&jogo->worlds.botao_voltar)){//botão de voltar atrás
				jogo->estado_tela = 2;
					//jogo->estado = JYH_MAIN_MENU;
					break;
				}else if(jogo->worlds.idx && jogo->worlds.n > 3 && SDL_PointInRect(&p,&jogo->worlds.botao_esq)){
					jogo->worlds.idx--;
					break;
				}else if(jogo->worlds.idx < jogo->worlds.n-3 && SDL_PointInRect(&p,&jogo->worlds.botao_dir)){
					jogo->worlds.idx++;
					break;					
				}
				
				for(int i = 0; i < 3; i++){//Botões de entrar em um mundo
					r.x = (75)+i*375;
					if(SDL_PointInRect(&p,&r)){//se existe a colisão, então vai para os níveis do mundo
						printf("%s\n",jogo->worlds.mundos[i + jogo->worlds.idx].nome);
                        jogo->worlds.i_sel = i + jogo->worlds.idx;
                        jogo->estado_tela = 3;
						break;
					}
				}
				break;
			case SDL_QUIT:
				jogo->estado = JYH_END_GAME;
				break;
		}
	}else{
		//eventos baseados em tempo
	}

	SDL_RenderCopy(jogo->ren,jogo->worlds.txt_voltar,NULL,&jogo->worlds.botao_voltar);

	if(jogo->worlds.idx && jogo->worlds.n > 3)SDL_RenderCopy(jogo->ren,jogo->worlds.txt_esq,NULL,&jogo->worlds.botao_esq);//SDL_RenderFillRect(jogo->ren,&jogo->worlds.botao_esq);
	//só desenha na direita se dá para ir para direita
	if(jogo->worlds.idx < jogo->worlds.n -  3)SDL_RenderCopy(jogo->ren,jogo->worlds.txt_dir,NULL,&jogo->worlds.botao_dir);//SDL_RenderFillRect(jogo->ren,&jogo->worlds.botao_dir);
	
	SDL_SetRenderDrawColor(jogo->ren,0xff,0x00,0x00,0x00);
	for(int i = 0; i < 3; i++){//desenha os botões para selecionar o mundo
		r.x = (75)+i*375;
		SDL_RenderCopy(jogo->ren,jogo->worlds.mundos[i+jogo->worlds.idx].capa,NULL,&r);
	}
}

void JYH_GameLoadWorlds(JYH_GameState* jogo){
	static char S[50];//temporario
	SDL_SetRenderDrawColor(jogo->ren,0xff,0xff,0xff,0x00);//trocar por uma tela de loading
	SDL_RenderClear(jogo->ren);
	SDL_RenderPresent(jogo->ren);
	
    FILE* arq = fopen("JYH\\mundos.txt","r");//arquivo fixo Windows
    //FILE* arq = fopen("JYH/mundos.txt","r");//linux
    assert(arq != NULL);
	jogo->worlds.title = (SDL_Rect){450,100,300,90};
	jogo->worlds.txt_title = IMG_LoadTexture(jogo->ren,"img\\Menu\\Titulo_JYH.png");//trocar
	jogo->worlds.botao_voltar = (SDL_Rect){25,25,50,50};
	jogo->worlds.txt_voltar = IMG_LoadTexture(jogo->ren,"img\\geral\\Back_JYH.png");
	jogo->worlds.botao_esq = (SDL_Rect){10  ,300,40,90};
	jogo->worlds.txt_esq =  IMG_LoadTexture(jogo->ren,"img\\geral\\esquerda.png");
	assert(jogo->worlds.txt_esq != NULL);
	jogo->worlds.botao_dir = (SDL_Rect){1150,300,40,90};
	jogo->worlds.txt_dir = IMG_LoadTexture(jogo->ren,"img\\geral\\direita.png");
	jogo->worlds.r_background = (SDL_Rect){0,0,jogo->w_tela,jogo->h_tela};
	jogo->worlds.txt_background = IMG_LoadTexture(jogo->ren,"img\\Menu\\Background_JYH.png");
	jogo->worlds.idx = 0;

    fscanf(arq,"%d",&jogo->worlds.n);
    jogo->worlds.mundos =(JYH_Mundo*)malloc(sizeof(JYH_Mundo)*(jogo->worlds.n));

    for(int i = 0; i < jogo->worlds.n;i++){//carrega as informações dos mundos pelos arquivos
        fscanf(arq,"%s",jogo->worlds.mundos[i].nome);
        fscanf(arq,"%s",jogo->worlds.mundos[i].pathW);
        fscanf(arq,"%s",jogo->worlds.mundos[i].pathL);
        fscanf(arq,"%s",S);//nome da textura a ser carregada depois
        
        jogo->worlds.mundos[i].capa = IMG_LoadTexture(jogo->ren,S);
        assert(jogo->worlds.mundos[i].capa != NULL);
        
        printf("Mundo inserido: %s e path %s\n",jogo->worlds.mundos[i].nome,jogo->worlds.mundos[i].pathW);
    }
	fclose(arq);
	printf("Mundos\n");
    jogo->estado_tela = 1;
}

void JYH_GameDestroyWorlds(JYH_GameState* jogo){
	SDL_DestroyTexture(jogo->worlds.txt_title);
	SDL_DestroyTexture(jogo->worlds.txt_voltar);
	SDL_DestroyTexture(jogo->worlds.txt_esq);
	SDL_DestroyTexture(jogo->worlds.txt_dir);
	SDL_DestroyTexture(jogo->worlds.txt_background);
	for(int i = 0; i < jogo->worlds.n;i++)SDL_DestroyTexture(jogo->worlds.mundos[i].capa);//limpa os mundos
    free(jogo->worlds.mundos);
}

void JYH_WS_to_LS(JYH_GameState* jogo){
    JYH_Level_Selection temp;
    printf("->LS\n");
    strcpy(temp.pathW,jogo->worlds.mundos[jogo->worlds.i_sel].pathW);//copia path para arquivo do mundo
    strcpy(temp.pathL,jogo->worlds.mundos[jogo->worlds.i_sel].pathL);
    JYH_GameDestroyWorlds(jogo);//elimina o que foi alocado
    jogo->prev = jogo->estado;
    jogo->estado_tela = 0;
    jogo->estado = JYH_LVL_SELECTION;
    jogo->sel = temp;//copiao estado
}

void JYH_WS_to_MM(JYH_GameState* jogo){
	printf("->MM\n");
    JYH_Menu temp;
    JYH_GameDestroyWorlds(jogo);
    jogo->prev = jogo->estado;
    jogo->estado_tela = 0;
    jogo->estado = JYH_MAIN_MENU;//trocar no futuro
    jogo->menu = temp;
}

void JYH_WS(JYH_GameState* jogo){//muda fluxo pelo estado da tela
    switch(jogo->estado_tela){
        case 0://Load da Tela
            JYH_GameLoadWorlds(jogo);
            break;
        case 1://Execução Normal da Tela
            JYH_GameWorldSelection(jogo);
            break;
        case 2://Volta Para Menu
            JYH_WS_to_MM(jogo);
            break;
        case 3://Carrega mundo Específico
            JYH_WS_to_LS(jogo);
            break;
    }
}



