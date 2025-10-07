//Seleção de Níveis do player
#include "JYH_Header.h"

void JYH_Destroy_PL(JYH_GameState* jogo){
	for(int i = 0; i < jogo->selP.n; i++){
		SDL_DestroyTexture(jogo->selP.niveis[i].txt_nome);
	}//desalocar detalhes da lista de níveis
    SDL_DestroyTexture(jogo->selP.txt_title);
    SDL_DestroyTexture(jogo->selP.txt_background);
    SDL_DestroyTexture(jogo->selP.txt_lvl_icon);
    SDL_DestroyTexture(jogo->selP.txt_voltar);
	free(jogo->selP.niveis);
}

void JYH_Run_PL(JYH_GameState* jogo){//Atualizar
	static SDL_Point p;
    SDL_Rect r,r_nome;

    SDL_RenderCopy(jogo->ren,jogo->selP.txt_background,NULL,NULL);
    SDL_RenderCopy(jogo->ren,jogo->selP.txt_title,NULL,&jogo->selP.title);

	
	r.w = 64;
	r.h = 64;
	r_nome.w = 64;
	r_nome.h = 32;
	for(int i = 0; i < jogo->selP.n; i++){//desenha os botões dos níveis
		r.x = 64  + (i%9)*128;
		r.y = 300 + (i/9)*128;
		SDL_RenderCopy(jogo->ren,jogo->selP.txt_lvl_icon,NULL,&r);
		r_nome.x = r.x;
		r_nome.y = r.y+r.w;
		SDL_RenderCopy(jogo->ren,jogo->selP.niveis[i].txt_nome,NULL,&r_nome);	
	}

	if(AUX_WaitEventTimeoutCount(&(jogo->evt),&(jogo->espera))){//trocar por exercicio
		switch(jogo->evt.type){
			case SDL_MOUSEBUTTONDOWN://verifica os cliques do botão
				p = (SDL_Point){(int)jogo->evt.button.x,(int)jogo->evt.button.y};
				
				if (SDL_PointInRect(&p,&jogo->selP.botao_voltar))jogo->estado_tela = 5;
				
				for(int i = 0; i < jogo->sel.n; i++){//verifica se clicou em um mundo
					r.x = 64  + (i%9)*128;
					r.y = 300 + (i/9)*128;
					if(SDL_PointInRect(&p,&r)){
						jogo->estado_tela = 3;
						jogo->selP.i_sel = i;
						break;
					}
				}

				break;
			case SDL_QUIT:
				jogo->estado = JYH_END_GAME;
				JYH_Destroy_PL(jogo);
				break;
		}
	}else{
		//eventos baseados em tempo
	}
    SDL_RenderCopy(jogo->ren,jogo->selP.txt_voltar,NULL,&jogo->selP.botao_voltar);
}

void JYH_Load_PL(JYH_GameState* jogo){
	char S[50];
	jogo->selP.title = (SDL_Rect){450,100,300,90};
	jogo->selP.botao_voltar = (SDL_Rect){25,25,50,50};

    #ifdef _WIN32
    FILE* arq = fopen("JYH\\MeusNiveis\\MeusNiveis.txt","r");
    jogo->selP.txt_title = IMG_LoadTexture(jogo->ren,"img\\geral\\Biblioteca_Jogador_JYH.png");//trocar
    jogo->selP.txt_background = IMG_LoadTexture(jogo->ren,"img\\Menu\\Background_JYH.png");
    jogo->selP.txt_voltar = IMG_LoadTexture(jogo->ren,"img\\geral\\Back_JYH.png");
    #elif __linux__

    FILE* arq = fopen("./JYH/MeusNiveis/MeusNiveis.txt","r");
    jogo->selP.txt_title = IMG_LoadTexture(jogo->ren,"./img/geral/Biblioteca_Jogador_JYH.png");//trocar
    jogo->selP.txt_background = IMG_LoadTexture(jogo->ren,"./img/menu/Background_JYH.png");
    jogo->selP.txt_voltar = IMG_LoadTexture(jogo->ren,"./img/geral/Back_JYH.png");
    #endif

    assert(arq!=NULL);
    assert(jogo->selP.txt_title!=NULL);
    assert(jogo->selP.txt_background!=NULL);
    assert(jogo->selP.txt_lvl_icon!=NULL);
    assert(jogo->selP.txt_voltar!=NULL);
    
    fscanf(arq,"%d",&jogo->selP.n);
    fscanf(arq,"%s",S);//lê path windows
    AUX_AdaptarString(S);
    jogo->selP.txt_lvl_icon =  IMG_LoadTexture(jogo->ren, S);
    
    jogo->selP.niveis = (JYH_Nivel*)malloc(sizeof(JYH_Nivel)*jogo->selP.n);
    SDL_Color clr = {0x00,0x00,0x00,0x00};
    for(int i = 0;i < jogo->selP.n;i++){
        fscanf(arq,"%s",jogo->selP.niveis[i].nome);
		fscanf(arq,"%s",jogo->selP.niveis[i].path);
        AUX_AdaptarString(jogo->selP.niveis[i].path);
		jogo->selP.niveis[i].txt_nome = AUX_CriarTexto(jogo->ren,jogo->fnt,jogo->selP.niveis[i].nome,clr);
        assert(jogo->selP.niveis[i].txt_nome!=NULL);
    }
	fclose(arq);
	jogo->estado_tela = 1;
}

void JYH_PL_to_LE(JYH_GameState* jogo){
	JYH_Editor temp;
	jogo->prev = jogo->estado;
	jogo->estado_tela = 0;
	jogo->estado =  JYH_state_LE;
	JYH_Destroy_PL(jogo);
	jogo->edit = temp;
}
void JYH_PL_to_MM(JYH_GameState* jogo){
	JYH_Menu temp;
	jogo->prev = jogo->estado;
	jogo->estado_tela = 0;
	jogo->estado = JYH_state_MM;
	JYH_Destroy_PL(jogo);
	jogo->menu = temp;
}
void JYH_PL_to_EX(JYH_GameState* jogo){
	JYH_Level_Runner temp;
	jogo->prev = jogo->estado;
	jogo->estado_tela = 0;
	jogo->estado = JYH_state_EX;
	JYH_Destroy_PL(jogo);
	jogo->exec = temp;
}
void JYH_PL(JYH_GameState* jogo){
	switch(jogo->estado_tela){
		case 0://Carrega a Biblioteca de Níveis do jogador
			JYH_Load_PL(jogo);
			break;
		case 1://
			JYH_Run_PL(jogo);
			break;
		case 3://Editar Nível Específico
			JYH_PL_to_LE(jogo);
			break;
		case 4://Executar Nível específico
			JYH_PL_to_EX(jogo);
			break;
		case 5://Voltar ao Menu
			JYH_PL_to_MM(jogo);
			break;
	}
}
