//Seleção de Níveis do player
#include "JYH_Header.h"

//destruir

void JYH_Destroy_PL(JYH_GameState* jogo){

	for(int i = 0; i < jogo->pl.n; i++){
		SDL_DestroyTexture(jogo->pl.niveis[i].txt_nome);
	}

    SDL_DestroyTexture(jogo->pl.txt_background);
    SDL_DestroyTexture(jogo->pl.txt_lvl_icon);
    SDL_DestroyTexture(jogo->pl.titulo.txt);
    SDL_DestroyTexture(jogo->pl.botao_V.txt);

	free(jogo->pl.niveis);
}

//Transições

void JYH_PL_to_LE(JYH_GameState* jogo){
	JYH_Editor temp;
	AUX_Empilha(&jogo->state,JYH_state_LE);
	strcpy(temp.lvl.nome_nivel,jogo->pl.niveis[jogo->pl.i_sel].nome_nivel);
	JYH_Destroy_PL(jogo);
	jogo->le = temp;
	JYH_Load_LE(jogo);
}
void JYH_PL_to_MM(JYH_GameState* jogo){
	JYH_Menu temp;
	AUX_Desempilha(&jogo->state);
	JYH_Destroy_PL(jogo);
	jogo->mm = temp;
	JYH_Load_MM(jogo);
}
void JYH_PL_to_EX(JYH_GameState* jogo){
	JYH_Level_Runner temp;
	AUX_Empilha(&jogo->state, JYH_state_EX);
	JYH_Destroy_PL(jogo);
	jogo->ex = temp;
	JYH_Load_PL(jogo);
}

//Execução

void JYH_PL(JYH_GameState* jogo){//Atualizar
	static SDL_Point p;
    SDL_Rect r,r_nome;

    SDL_RenderCopy(jogo->ren,jogo->pl.txt_background,NULL,NULL);
    AUX_Draw_Icon(jogo->ren,&jogo->pl.titulo);
    AUX_Draw_Icon(jogo->ren,&jogo->pl.botao_V);

	r.w = 64;
	r.h = 64;
	r_nome.w = 64;
	r_nome.h = 32;
	for(int i = 0; i < jogo->pl.n; i++){//desenha os botões dos níveis
		r.x = 64  + (i%9)*128;
		r.y = 300 + (i/9)*128;
		SDL_RenderCopy(jogo->ren,jogo->pl.txt_lvl_icon,NULL,&r);
		r_nome.x = r.x;
		r_nome.y = r.y+r.w;
		SDL_RenderCopy(jogo->ren,jogo->pl.niveis[i].txt_nome,NULL,&r_nome);	
	}
	
	if(AUX_WaitEventTimeoutCount(&(jogo->evt),&(jogo->espera))){//trocar por exercicio
		switch(jogo->evt.type){
			case SDL_MOUSEBUTTONUP://verifica os cliques do botão
				p = (SDL_Point){(int)jogo->evt.button.x,(int)jogo->evt.button.y};
				
				if (SDL_PointInRect(&p,&jogo->pl.botao_V.r)){
                    JYH_PL_to_MM(jogo);
                    break;
                }
				
				for(int i = 0; i < jogo->pl.n; i++){//verifica se clicou em um mundo
					r.x = 64  + (i%9)*128;
					r.y = 300 + (i/9)*128;
					if(SDL_PointInRect(&p,&r)){
						jogo->pl.i_sel = i;
						JYH_PL_to_LE(jogo);
						break;
					}
				}

				break;
			case SDL_QUIT:
				AUX_Empilha(&jogo->state,JYH_END_GAME);
				JYH_Destroy_PL(jogo);
				break;
		}
	}else{
		//eventos baseados em tempo
	}
}

//Load

void JYH_Load_PL(JYH_GameState* jogo){
	char S[100];

    FILE* arq = fopen(PATH_PL,"r");
    
    jogo->pl.txt_background = IMG_LoadTexture(jogo->ren,IMG_MM_BACKGROUND);
    AUX_Start_Icon(jogo->ren,&jogo->pl.botao_V,IMG_B_BACK,(SDL_Rect){25,25,50,50},1);
    AUX_Start_Icon(jogo->ren,&jogo->pl.titulo,IMG_PL_TITLE,(SDL_Rect){450,100,300,90},1);
    
    assert(arq!=NULL);
    assert(jogo->pl.txt_background!=NULL);
    assert(jogo->pl.txt_lvl_icon!=NULL);
    
    fscanf(arq,"%d",&jogo->pl.n);
    fscanf(arq,"%s",S);//lê path windows
    AUX_AdaptarString(S);
    jogo->pl.txt_lvl_icon =  IMG_LoadTexture(jogo->ren, S);

    jogo->pl.niveis = (JYH_Ass_Nivel*)malloc(sizeof(JYH_Nivel)*jogo->pl.n);
    SDL_Color clr = {0x00,0x00,0x00,0x00};
    for(int i = 0;i < jogo->pl.n;i++){
        fscanf(arq,"%s",jogo->pl.niveis[i].nome_nivel);
        
		jogo->pl.niveis[i].txt_nome = AUX_CriarTexto(jogo->ren,jogo->fnt,jogo->pl.niveis[i].nome_nivel,clr);
		
        assert(jogo->pl.niveis[i].txt_nome!=NULL);
    }
	fclose(arq);
}
