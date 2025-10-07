//Selecionar Mundo
#include "JYH_Header.h"

void JYH_Destroy_WS(JYH_GameState* jogo){
	SDL_DestroyTexture(jogo->worlds.txt_title);
	SDL_DestroyTexture(jogo->worlds.txt_voltar);
	SDL_DestroyTexture(jogo->worlds.txt_esq);
	SDL_DestroyTexture(jogo->worlds.txt_dir);
	SDL_DestroyTexture(jogo->worlds.txt_background);
	for(int i = 0; i < jogo->worlds.n;i++)SDL_DestroyTexture(jogo->worlds.mundos[i].capa);//limpa os mundos
    free(jogo->worlds.mundos);
}

void JYH_Run_WS(JYH_GameState* jogo){//Atualizar
	static SDL_Point p;
	static SDL_Rect r;//desenho das capas do mundo

	SDL_RenderCopy(jogo->ren,jogo->worlds.txt_background,NULL,NULL);
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
                        jogo->worlds.i_sel = i + jogo->worlds.idx;
                        jogo->estado_tela = 3;
						break;
					}
				}
				break;
			case SDL_QUIT:
				jogo->estado = JYH_END_GAME;
				JYH_Destroy_WS(jogo);
				break;
		}
	}else{
		//eventos baseados em tempo
	}

	SDL_RenderCopy(jogo->ren,jogo->worlds.txt_voltar,NULL,&jogo->worlds.botao_voltar);

	if(jogo->worlds.idx && jogo->worlds.n > 3)SDL_RenderCopy(jogo->ren,jogo->worlds.txt_esq,NULL,&jogo->worlds.botao_esq);
	if(jogo->worlds.idx < jogo->worlds.n -  3)SDL_RenderCopy(jogo->ren,jogo->worlds.txt_dir,NULL,&jogo->worlds.botao_dir);
	
	SDL_SetRenderDrawColor(jogo->ren,0xff,0x00,0x00,0x00);
	for(int i = 0; i < 3; i++){//desenha os botões para selecionar o mundo
		r.x = (75)+i*375;
		SDL_RenderCopy(jogo->ren,jogo->worlds.mundos[i+jogo->worlds.idx].capa,NULL,&r);
	}
}

void JYH_Load_WS(JYH_GameState* jogo){
	static char S[50];//temporario
	
	jogo->worlds.title = (SDL_Rect){450,100,300,90};
    jogo->worlds.botao_voltar = (SDL_Rect){25,25,50,50};
    jogo->worlds.botao_esq = (SDL_Rect){10  ,300,40,90};
    jogo->worlds.botao_dir = (SDL_Rect){1150,300,40,90};
    jogo->worlds.idx = 0;

    #ifdef _WIN32

    FILE* arq = fopen("JYH\\mundos.txt","r");//arquivo fixo Windows
	jogo->worlds.txt_title = IMG_LoadTexture(jogo->ren,"img\\geral\\Modo_Campanha_JYH.png");//trocar
	jogo->worlds.txt_voltar = IMG_LoadTexture(jogo->ren,"img\\geral\\Back_JYH.png");
	jogo->worlds.txt_esq =  IMG_LoadTexture(jogo->ren,"img\\geral\\esquerda.png");
	jogo->worlds.txt_dir = IMG_LoadTexture(jogo->ren,"img\\geral\\direita.png");
	jogo->worlds.txt_background = IMG_LoadTexture(jogo->ren,"img\\Menu\\Background_JYH.png");

    #elif __linux__

    FILE* arq = fopen("./JYH/mundos.txt","r");//arquivo fixo Windows
	jogo->worlds.txt_title = IMG_LoadTexture(jogo->ren,"./img/geral/Modo_Campanha_JYH.png");//trocar
	jogo->worlds.txt_voltar = IMG_LoadTexture(jogo->ren,"./img/geral/Back_JYH.png");
	jogo->worlds.txt_esq =  IMG_LoadTexture(jogo->ren,"./img/geral/esquerda.png");
	jogo->worlds.txt_dir = IMG_LoadTexture(jogo->ren,"./img/geral/direita.png");
	jogo->worlds.txt_background = IMG_LoadTexture(jogo->ren,"./img/menu/Background_JYH.png");

    #endif

    assert(arq != NULL);
    assert(jogo->worlds.txt_title != NULL);
    assert(jogo->worlds.txt_voltar != NULL);
    assert(jogo->worlds.txt_esq != NULL);
    assert(jogo->worlds.txt_dir != NULL);
    assert(jogo->worlds.txt_background != NULL);

    fscanf(arq,"%d",&jogo->worlds.n);
    jogo->worlds.mundos =(JYH_Mundo*)malloc(sizeof(JYH_Mundo)*(jogo->worlds.n));

    for(int i = 0; i < jogo->worlds.n;i++){//carrega as informações dos mundos pelos arquivos
        fscanf(arq,"%s",jogo->worlds.mundos[i].nome);
        fscanf(arq,"%s",jogo->worlds.mundos[i].path);
        AUX_AdaptarString(jogo->worlds.mundos[i].path);
        fscanf(arq,"%s",S);//nome da textura a ser carregada depois
        AUX_AdaptarString(S);
        jogo->worlds.mundos[i].capa = IMG_LoadTexture(jogo->ren,S);
        assert(jogo->worlds.mundos[i].capa != NULL);
    }
	fclose(arq);
    jogo->estado_tela = 1;
}



void JYH_WS_to_LS(JYH_GameState* jogo){
    JYH_Level_Selection temp;
    strcpy(temp.path,jogo->worlds.mundos[jogo->worlds.i_sel].path);//copia path para arquivo do mundo
    jogo->prev = jogo->estado;
    jogo->estado_tela = 0;
    jogo->estado = JYH_state_LS;
    JYH_Destroy_WS(jogo);
    jogo->sel = temp;//copiao estado
}

void JYH_WS_to_MM(JYH_GameState* jogo){
    JYH_Menu temp;
    jogo->prev = jogo->estado;
    jogo->estado_tela = 0;
    jogo->estado = JYH_state_MM;//trocar no futuro
    JYH_Destroy_WS(jogo);
    jogo->menu = temp;
}

void JYH_WS(JYH_GameState* jogo){//muda fluxo pelo estado da tela
    switch(jogo->estado_tela){
        case 0://Load da Tela
            JYH_Load_WS(jogo);
            break;
        case 1://Execução Normal da Tela
            JYH_Run_WS(jogo);
            break;
        case 2://Volta Para Menu
            JYH_WS_to_MM(jogo);
            break;
        case 3://Carrega mundo Específico
            JYH_WS_to_LS(jogo);
            break;
    }
}



