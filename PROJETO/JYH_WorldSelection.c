//Selecionar Mundo
#include "JYH_Header.h"

void JYH_GameWorldSelection(JYH_GameState* jogo){//Atualizar
	static SDL_Point p;
	static SDL_Rect r;//desenho das capas do mundo

	SDL_SetRenderDrawColor(jogo->ren,0xff,0xff,0xff,0x00);//background
	SDL_RenderClear(jogo->ren);
	SDL_SetRenderDrawColor(jogo->ren,0xff,0xff,0x00,0x00);//"Modo Historia"
	SDL_RenderFillRect(jogo->ren,&jogo->worlds.title);
	
	r.w = 300;
	r.h = 300;
	r.y = 300;
	
	if(AUX_WaitEventTimeoutCount(&(jogo->evt),&(jogo->espera))){//trocar por exercicio
		switch(jogo->evt.type){
			case SDL_MOUSEBUTTONDOWN://verifica os cliques do botão
				p.x = (int)jogo->evt.button.x; p.y = (int)jogo->evt.button.y;
				
				if (SDL_PointInRect(&p,&jogo->worlds.botao_voltar)){//botão de voltar atrás
					jogo->estado = JYH_LOAD_MENU;
					break;
				}else if(jogo->worlds.idx && jogo->worlds.n_mundos > 3 && SDL_PointInRect(&p,&jogo->worlds.botao_esq)){
					jogo->worlds.idx--;
					break;
				}else if(jogo->worlds.idx < jogo->worlds.n_mundos-3 && SDL_PointInRect(&p,&jogo->worlds.botao_dir)){
					jogo->worlds.idx++;
					break;					
				}
				
				for(int i = 0; i < jogo->worlds.n_mundos; i++){//Botões de entrar em um mundo
					r.x = (75)+i*375;
					if(SDL_PointInRect(&p,&r)){//se existe a colisão, então vai para os níveis do mundo
						printf("%s\n",jogo->worlds.mundos[i + jogo->worlds.idx].nome);
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
	
	SDL_SetRenderDrawColor(jogo->ren,0xff,0x00,0x00,0x00);//cor botão 1
	SDL_RenderFillRect(jogo->ren,&jogo->worlds.botao_voltar);
	
	SDL_SetRenderDrawColor(jogo->ren,0x00,0xff,0x00,0x00);
	//só mostra botão de ir para esquerda se dá para ir para a esquerda
	if(jogo->worlds.idx && jogo->worlds.n_mundos > 3)SDL_RenderFillRect(jogo->ren,&jogo->worlds.botao_esq);
	//só desenha na direita se dá para ir para direita
	if(jogo->worlds.idx < jogo->worlds.n_mundos-3)SDL_RenderFillRect(jogo->ren,&jogo->worlds.botao_dir);
	
	SDL_SetRenderDrawColor(jogo->ren,0xff,0x00,0x00,0x00);
	for(int i = 0; i < jogo->worlds.n_mundos; i++){//desenha os botões para selecionar o mundo
		r.x = (75)+i*375;
		SDL_RenderFillRect(jogo->ren,&r);//trocar pela textura do mundo
	}
	
}

void JYH_GameLoadWorlds(JYH_GameState* jogo){
	static char S[50];//temporario
	SDL_SetRenderDrawColor(jogo->ren,0xff,0xff,0xff,0x00);//trocar por uma tela de loading
	SDL_RenderClear(jogo->ren);
	SDL_RenderPresent(jogo->ren);
	
    FILE* arq = fopen("JYH\\mundos.txt","r");//arquivo fixo
    assert(arq != NULL);
	jogo->worlds.title = (SDL_Rect){450,100,300,90};
	jogo->worlds.botao_voltar = (SDL_Rect){25,25,50,50};
	jogo->worlds.botao_esq = (SDL_Rect){10  ,300,40,90};
	jogo->worlds.botao_dir = (SDL_Rect){1150,300,40,90};
	jogo->worlds.idx = 0;

    fscanf(arq,"%d",&jogo->worlds.n_mundos);
    jogo->worlds.mundos =(JYH_Mundo*)malloc(sizeof(JYH_Mundo)*(jogo->worlds.n_mundos));
    for(int i = 0; i < jogo->worlds.n_mundos;i++){
        fscanf(arq,"%s",jogo->worlds.mundos[i].nome);
        fscanf(arq,"%s",jogo->worlds.mundos[i].path);
        fscanf(arq,"%s",S);//nome da textura
        //jogo->worlds.mundos[i].capa = 
        printf("Mundo inserido: %s e path %s\n",jogo->worlds.mundos[i].nome,jogo->worlds.mundos[i].path);
    }
	fclose(arq);
	printf("Mundos\n");
	jogo->estado = JYH_WORLD_SELECTION;
}

void JYH_GameDestroyWorlds(JYH_GameState* jogo){
    free(jogo->worlds.mundos);
}





