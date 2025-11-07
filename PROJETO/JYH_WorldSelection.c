//Selecionar Mundo
#include "JYH_Header.h"

void JYH_Destroy_WS(JYH_GameState* jogo){
	SDL_DestroyTexture(jogo->ws.titulo.txt);
	SDL_DestroyTexture(jogo->ws.botao_V.txt);
	SDL_DestroyTexture(jogo->ws.botao_E.txt);
	SDL_DestroyTexture(jogo->ws.botao_D.txt);
	SDL_DestroyTexture(jogo->ws.txt_background);
	for(int i = 0; i < jogo->ws.n;i++)SDL_DestroyTexture(jogo->ws.mundos[i].capa);//limpa os mundos
    free(jogo->ws.mundos);
}


void JYH_WS_to_LS(JYH_GameState* jogo){
    JYH_Level_Selection temp;
    strcpy(temp.path,jogo->ws.mundos[jogo->ws.i_sel].path);//copia path para arquivo do mundo
    strcpy(temp.nome,jogo->ws.mundos[jogo->ws.i_sel].nome);//copia nome do mundo
    AUX_Empilha(&jogo->state,JYH_state_LS);
    JYH_Destroy_WS(jogo);
    jogo->ls = temp;//copiao estado
    JYH_Load_LS(jogo);
}

void JYH_WS_to_MM(JYH_GameState* jogo){
    JYH_Menu temp;
    AUX_Desempilha(&jogo->state);
    JYH_Destroy_WS(jogo);
    jogo->mm = temp;
    JYH_Load_MM(jogo);
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
				    JYH_WS_to_MM(jogo);
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
                        JYH_WS_to_LS(jogo);
						break;
					}
				}
				break;
			case SDL_QUIT:
				AUX_Empilha(&jogo->state,JYH_END_GAME);
				JYH_Destroy_WS(jogo);
				break;
		}
	}else{
		//eventos baseados em tempo
	}
}

void JYH_Load_WS(JYH_GameState* jogo){
	static char S[100];//temporario
    jogo->ws.idx = 0;

    #ifdef _WIN32
    FILE* arq = fopen("JYH\\mundos.txt","r");//arquivo fixo Windows
	AUX_Start_Icon(jogo->ren,&jogo->ws.titulo,"img\\geral\\Modo_Campanha_JYH.png",(SDL_Rect){450,100,300,90},1);
	AUX_Start_Icon(jogo->ren,&jogo->ws.botao_V,"img\\botao\\Back.png",(SDL_Rect){25,25,50,50},1);
	AUX_Start_Icon(jogo->ren,&jogo->ws.botao_E,"img\\geral\\esquerda.png",(SDL_Rect){10,300,40,90},1);
	AUX_Start_Icon(jogo->ren,&jogo->ws.botao_D,"img\\geral\\direita.png",(SDL_Rect){1150,300,40,90},1);
	
	jogo->ws.txt_background = IMG_LoadTexture(jogo->ren,"img\\Menu\\Background_JYH.png");
    #elif __linux__
    FILE* arq = fopen("./JYH/mundos.txt","r");//arquivo fixo Windows
	AUX_Start_Icon(jogo->ren,&jogo->ws.titulo,"./img/geral/Modo_Campanha_JYH.png",(SDL_Rect){450,100,300,90},1);
	AUX_Start_Icon(jogo->ren,&jogo->ws.botao_V,"./img/botao/Back.png",(SDL_Rect){25,25,50,50},1);
	AUX_Start_Icon(jogo->ren,&jogo->ws.botao_E,"./img/geral/esquerda.png",(SDL_Rect){10,300,40,90},1);
	AUX_Start_Icon(jogo->ren,&jogo->ws.botao_D,"./img/geral/direita.png",(SDL_Rect){1150,300,40,90},1);
	
	jogo->ws.txt_background = IMG_LoadTexture(jogo->ren,"./img/menu/Background_JYH.png");
    #endif
    assert(arq != NULL);
    assert(jogo->ws.txt_background != NULL);

    fscanf(arq,"%d",&jogo->ws.n);
    jogo->ws.mundos =(JYH_Mundo*)malloc(sizeof(JYH_Mundo)*(jogo->ws.n));

    for(int i = 0; i < jogo->ws.n;i++){//carrega as informações dos mundos pelos arquivos
        fscanf(arq,"%s",jogo->ws.mundos[i].nome);
        sprintf(S,".$img$%s$capa.png",jogo->ws.mundos[i].nome);
        AUX_AdaptarString(S);
        jogo->ws.mundos[i].capa = IMG_LoadTexture(jogo->ren,S);
        assert(jogo->ws.mundos[i].capa != NULL);
    }
	fclose(arq);
}