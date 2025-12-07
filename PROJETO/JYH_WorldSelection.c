//Selecionar Mundo
#include "JYH_Header.h"
#include "JYH_WS.h"

void JYH_Destroy_WS(JYH_World_Selection* ws){
	SDL_DestroyTexture(ws->titulo.txt);
	SDL_DestroyTexture(ws->botao_V.txt);
	SDL_DestroyTexture(ws->botao_E.txt);
	SDL_DestroyTexture(ws->botao_D.txt);
	SDL_DestroyTexture(ws->txt_background);
	for(int i = 0; i < ws->n;i++)SDL_DestroyTexture(ws->mundos[i].capa);//limpa os mundos
    free(ws->mundos);
}

void JYH_WS(JYH_GameState* jogo){//Atualizar
	static SDL_Point p;
	static SDL_Rect  r;//desenho das capas do mundo

	SDL_RenderCopy(jogo->ren,jogo->ws.txt_background,NULL,NULL);
	AUX_Draw_Icon(jogo->ren,&jogo->ws.titulo);
	r.w = 300;
	r.h = 300;
	r.y = 300;
	
	AUX_Draw_Icon(jogo->ren,&jogo->ws.botao_V);

	if(jogo->ws.idx && jogo->ws.n > 3)AUX_Draw_Icon(jogo->ren,&jogo->ws.botao_E);
	if(jogo->ws.idx < jogo->ws.n -  3)AUX_Draw_Icon(jogo->ren,&jogo->ws.botao_D);
	
	SDL_SetRenderDrawColor(jogo->ren,0xff,0x00,0x00,0x00);
	for(int i = 0; i < 3; i++){//desenha os botões para selecionar o mundo
		r.x = (75)+i*375;
		SDL_RenderCopy(jogo->ren,jogo->ws.mundos[i+jogo->ws.idx].capa,NULL,&r);
	}
	
	
	if(AUX_WaitEventTimeoutCount(&(jogo->evt),&(jogo->espera))){//trocar por exercicio
		switch(jogo->evt.type){
			case SDL_MOUSEBUTTONDOWN://verifica os cliques do botão
				p.x = (int)jogo->evt.button.x; p.y = (int)jogo->evt.button.y;
				
				if (SDL_PointInRect(&p,&jogo->ws.botao_V.r)){//botão de voltar atrás
				    JYH_Trans(jogo,JYH_state_WS,JYH_state_MM);
					break;
				}else if(jogo->ws.idx && jogo->ws.n > 3 && SDL_PointInRect(&p,&jogo->ws.botao_E.r)){
					jogo->ws.idx--;
					break;
				}else if(jogo->ws.idx < jogo->ws.n-3 && SDL_PointInRect(&p,&jogo->ws.botao_D.r)){
					jogo->ws.idx++;
					break;					
				}
				
				for(int i = 0; i < 3; i++){//Botões de entrar em um mundo
					r.x = (75)+i*375;
					if(SDL_PointInRect(&p,&r)){//se existe a colisão, então vai para os níveis do mundo
                        jogo->ws.i_sel = i + jogo->ws.idx;
                        JYH_Trans(jogo,JYH_state_WS,JYH_state_LS);
						break;
					}
				}
				break;
			case SDL_QUIT:
				AUX_Empilha(&jogo->state,JYH_END_GAME);
				JYH_Destroy_WS(&jogo->ws);
				break;
		}
	}else{
		//eventos baseados em tempo
	}
}

void JYH_Load_WS(JYH_GameState* jogo){
	static char S[100];//temporario
    jogo->ws.idx = 0;

    FILE* arq = fopen(PATH_WS,"r");//arquivo fixo Windows
    
    AUX_Start_Icon(jogo->ren,&jogo->ws.titulo,IMG_WS_TITLE,(SDL_Rect){450,100,300,90},1);
    AUX_Start_Icon(jogo->ren,&jogo->ws.botao_V,IMG_B_BACK,(SDL_Rect){25,25,50,50},1);
	AUX_Start_Icon(jogo->ren,&jogo->ws.botao_E,IMG_B_LEFT,(SDL_Rect){10,300,40,90},1);
	AUX_Start_Icon(jogo->ren,&jogo->ws.botao_D,IMG_B_RIGHT,(SDL_Rect){1150,300,40,90},1);
    jogo->ws.txt_background = IMG_LoadTexture(jogo->ren,IMG_MM_BACKGROUND);
    
    assert(arq != NULL);
    assert(jogo->ws.txt_background != NULL);

    fscanf(arq,"%d",&jogo->ws.n);
    jogo->ws.mundos =(JYH_Mundo*)malloc(sizeof(JYH_Mundo)*(jogo->ws.n));

    for(int i = 0; i < jogo->ws.n;i++){//carrega as informações dos mundos pelos arquivos
        fscanf(arq,"%s",jogo->ws.mundos[i].nome);
        sprintf(S,WORLD_GET_CAPA,jogo->ws.mundos[i].nome);
        AUX_AdaptarString(S);
        jogo->ws.mundos[i].capa = IMG_LoadTexture(jogo->ren,S);
        assert(jogo->ws.mundos[i].capa != NULL);
    }
	fclose(arq);
}