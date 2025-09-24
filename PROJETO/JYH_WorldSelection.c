//Selecionar Mundo
#include "JYH_Header.h"

void JYH_GameWorldSelection(JYH_GameState* jogo){//Atualizar
	static SDL_Point p;

	SDL_SetRenderDrawColor(jogo->ren,0xff,0xff,0xff,0x00);//background
	SDL_RenderClear(jogo->ren);
	SDL_SetRenderDrawColor(jogo->ren,0xff,0xff,0x00,0x00);//"Modo Historia"
	SDL_RenderFillRect(jogo->ren,&jogo->worlds.title);
	
	if(AUX_WaitEventTimeoutCount(&(jogo->evt),&(jogo->espera))){//trocar por exercicio
		switch(jogo->evt.type){
			case SDL_MOUSEBUTTONDOWN://verifica os cliques do botão
				p.x = (int)jogo->evt.button.x; p.y = (int)jogo->evt.button.y;
				
				if (SDL_PointInRect(&p,&jogo->worlds.botao_voltar))jogo->estado = JYH_LOAD_MENU;
				
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
}

void JYH_GameLoadWorlds(JYH_GameState* jogo){
	SDL_SetRenderDrawColor(jogo->ren,0xff,0xff,0xff,0x00);//trocar por uma tela de loading
	SDL_RenderClear(jogo->ren);
	SDL_RenderPresent(jogo->ren);
	
    FILE* arq = fopen("mundos.txt","r");
    assert(arq != NULL);
	jogo->worlds.title = (SDL_Rect){450,100,300,90};
	jogo->worlds.botao_voltar = (SDL_Rect){25,25,50,50};

    fscanf(arq,"%d",&jogo->worlds.n_mundos);
    jogo->worlds.mundos =(JYH_Mundo*)malloc(sizeof(JYH_Mundo)*(jogo->worlds.n_mundos));
    for(int i = 0; i < jogo->worlds.n_mundos;i++){
        fscanf(arq,"%s",jogo->worlds.mundos[i].nome);
        printf("Mundo inserido: %s\n",jogo->worlds.mundos[i].nome);
    }

	fclose(arq);
	printf("Mundos\n");
	jogo->estado = JYH_WORLD_SELECTION;
}

void JYH_GameDestroyWorlds(JYH_GameState* jogo){
    free(jogo->worlds.mundos);
}




