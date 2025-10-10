//Selecionar Mundo
#include "JYH_Header.h"

void JYH_Destroy_WS(JYH_GameState* jogo){
	SDL_DestroyTexture(jogo->ws.txt_title);
	SDL_DestroyTexture(jogo->ws.txt_voltar);
	SDL_DestroyTexture(jogo->ws.txt_esq);
	SDL_DestroyTexture(jogo->ws.txt_dir);
	SDL_DestroyTexture(jogo->ws.txt_background);
	for(int i = 0; i < jogo->ws.n;i++)SDL_DestroyTexture(jogo->ws.mundos[i].capa);//limpa os mundos
    free(jogo->ws.mundos);
}


void JYH_WS_to_LS(JYH_GameState* jogo){
    JYH_Level_Selection temp;
    strcpy(temp.path,jogo->ws.mundos[jogo->ws.i_sel].path);//copia path para arquivo do mundo
    jogo->prev = jogo->estado;
    jogo->estado = JYH_state_LS;
    JYH_Destroy_WS(jogo);
    jogo->ls = temp;//copiao estado
    JYH_Load_LS(jogo);
}

void JYH_WS_to_MM(JYH_GameState* jogo){
    JYH_Menu temp;
    jogo->prev = jogo->estado;
    jogo->estado = JYH_state_MM;//trocar no futuro
    JYH_Destroy_WS(jogo);
    jogo->mm = temp;
    JYH_Load_MM(jogo);
}

void JYH_WS(JYH_GameState* jogo){//Atualizar
	static SDL_Point p;
	static SDL_Rect r;//desenho das capas do mundo

	SDL_RenderCopy(jogo->ren,jogo->ws.txt_background,NULL,NULL);
	SDL_RenderCopy(jogo->ren,jogo->ws.txt_title,NULL,&jogo->ws.title);
	
	r.w = 300;
	r.h = 300;
	r.y = 300;
	
	SDL_RenderCopy(jogo->ren,jogo->ws.txt_voltar,NULL,&jogo->ws.botao_voltar);

	if(jogo->ws.idx && jogo->ws.n > 3)SDL_RenderCopy(jogo->ren,jogo->ws.txt_esq,NULL,&jogo->ws.botao_esq);
	if(jogo->ws.idx < jogo->ws.n -  3)SDL_RenderCopy(jogo->ren,jogo->ws.txt_dir,NULL,&jogo->ws.botao_dir);
	
	SDL_SetRenderDrawColor(jogo->ren,0xff,0x00,0x00,0x00);
	for(int i = 0; i < 3; i++){//desenha os botões para selecionar o mundo
		r.x = (75)+i*375;
		SDL_RenderCopy(jogo->ren,jogo->ws.mundos[i+jogo->ws.idx].capa,NULL,&r);
	}
	
	
	if(AUX_WaitEventTimeoutCount(&(jogo->evt),&(jogo->espera))){//trocar por exercicio
		switch(jogo->evt.type){
			case SDL_MOUSEBUTTONDOWN://verifica os cliques do botão
				p.x = (int)jogo->evt.button.x; p.y = (int)jogo->evt.button.y;
				
				if (SDL_PointInRect(&p,&jogo->ws.botao_voltar)){//botão de voltar atrás
				    JYH_WS_to_MM(jogo);
					break;
				}else if(jogo->ws.idx && jogo->ws.n > 3 && SDL_PointInRect(&p,&jogo->ws.botao_esq)){
					jogo->ws.idx--;
					break;
				}else if(jogo->ws.idx < jogo->ws.n-3 && SDL_PointInRect(&p,&jogo->ws.botao_dir)){
					jogo->ws.idx++;
					break;					
				}
				
				for(int i = 0; i < 3; i++){//Botões de entrar em um mundo
					r.x = (75)+i*375;
					if(SDL_PointInRect(&p,&r)){//se existe a colisão, então vai para os níveis do mundo
                        jogo->ws.i_sel = i + jogo->ws.idx;
                        //jogo->estado_tela = 3;
                        JYH_WS_to_LS(jogo);
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
}

void JYH_Load_WS(JYH_GameState* jogo){
	static char S[50];//temporario
	
	jogo->ws.title = (SDL_Rect){450,100,300,90};
    jogo->ws.botao_voltar = (SDL_Rect){25,25,50,50};
    jogo->ws.botao_esq = (SDL_Rect){10  ,300,40,90};
    jogo->ws.botao_dir = (SDL_Rect){1150,300,40,90};
    jogo->ws.idx = 0;

    #ifdef _WIN32
    FILE* arq = fopen("JYH\\mundos.txt","r");//arquivo fixo Windows
	jogo->ws.txt_title = IMG_LoadTexture(jogo->ren,"img\\geral\\Modo_Campanha_JYH.png");//trocar
	jogo->ws.txt_voltar = IMG_LoadTexture(jogo->ren,"img\\geral\\Back_JYH.png");
	jogo->ws.txt_esq =  IMG_LoadTexture(jogo->ren,"img\\geral\\esquerda.png");
	jogo->ws.txt_dir = IMG_LoadTexture(jogo->ren,"img\\geral\\direita.png");
	jogo->ws.txt_background = IMG_LoadTexture(jogo->ren,"img\\Menu\\Background_JYH.png");
    #elif __linux__
    FILE* arq = fopen("./JYH/mundos.txt","r");//arquivo fixo Windows
	jogo->ws.txt_title = IMG_LoadTexture(jogo->ren,"./img/geral/Modo_Campanha_JYH.png");//trocar
	jogo->ws.txt_voltar = IMG_LoadTexture(jogo->ren,"./img/geral/Back_JYH.png");
	jogo->ws.txt_esq =  IMG_LoadTexture(jogo->ren,"./img/geral/esquerda.png");
	jogo->ws.txt_dir = IMG_LoadTexture(jogo->ren,"./img/geral/direita.png");
	jogo->ws.txt_background = IMG_LoadTexture(jogo->ren,"./img/menu/Background_JYH.png");
    #endif
    assert(arq != NULL);
    assert(jogo->ws.txt_title != NULL);
    assert(jogo->ws.txt_voltar != NULL);
    assert(jogo->ws.txt_esq != NULL);
    assert(jogo->ws.txt_dir != NULL);
    assert(jogo->ws.txt_background != NULL);

    fscanf(arq,"%d",&jogo->ws.n);
    jogo->ws.mundos =(JYH_Mundo*)malloc(sizeof(JYH_Mundo)*(jogo->ws.n));

    for(int i = 0; i < jogo->ws.n;i++){//carrega as informações dos mundos pelos arquivos
        fscanf(arq,"%s",jogo->ws.mundos[i].nome);
        fscanf(arq,"%s",jogo->ws.mundos[i].path);
        AUX_AdaptarString(jogo->ws.mundos[i].path);
        fscanf(arq,"%s",S);//nome da textura a ser carregada depois
        AUX_AdaptarString(S);
        jogo->ws.mundos[i].capa = IMG_LoadTexture(jogo->ren,S);
        assert(jogo->ws.mundos[i].capa != NULL);
    }
	fclose(arq);
}