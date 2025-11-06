//Editor Nível
#include "JYH_Header.h"

void JYH_Destroy_LE(JYH_GameState* jogo){//desalocar
	SDL_DestroyTexture(jogo->le.botao_V.txt);
	SDL_DestroyTexture(jogo->le.botao_P.txt);
	SDL_DestroyTexture(jogo->le.botao_R.txt);
	SDL_DestroyTexture(jogo->le.botao_S.txt);
	SDL_DestroyTexture(jogo->le.botao_T.txt);
	SDL_DestroyTexture(jogo->le.botao_A.txt);
	SDL_DestroyTexture(jogo->le.tb.txt);
	SDL_DestroyTexture(jogo->le.sb.txt);
	SDL_DestroyTexture(jogo->le.lvl.txt_theme);
	
	free(jogo->le.lvl.mat);//temporario
}


void JYH_Save_lvl(JYH_GameState* jogo){
    char S[100];
    JYH_Nivel* lvl = &jogo->le.lvl;
    sprintf(S,".$JYH$MeusNiveis$%s.txt",lvl->nome);
    AUX_AdaptarString(S);
    FILE* dest = fopen(S,"w");
    printf("Salvando em %s\n",S);
    assert(dest!=NULL);
    fprintf(dest,"%d %d\n",lvl->w,lvl->h);
    fprintf(dest,"%s\n",lvl->path_theme);
    for(int i = 0; i < lvl->w; i++){
        for(int j = 0; j < lvl->h; j++){
            fprintf(dest,"%d ",lvl->mat[j*(lvl->w)+i]);
        }
        fprintf(dest,"\n");
    }
    fclose(dest);

}

void JYH_Read_lvl(JYH_GameState*jogo){
	char S[100];
    JYH_Nivel* lvl = &jogo->le.lvl;
    sprintf(S,".$JYH$MeusNiveis$%s.txt",lvl->nome);
    AUX_AdaptarString(S);
    FILE* orig =fopen(S,"r");
    assert(orig!=NULL);
    printf("Lendo %s\n",S);
    fscanf(orig,"%d %d",&lvl->w,&lvl->h);
    fscanf(orig,"%s\n",lvl->path_theme);
    for(int i = 0; i < lvl->w; i++){
        for(int j = 0; j < lvl->h; j++){
            fscanf(orig,"%d",&lvl->mat[j*(lvl->w)+i]);
        }
    }
    fclose(orig);
}

//Transições

void JYH_LE_to_PL(JYH_GameState* jogo){//editor à biblioteca do player
	JYH_Level_Selection_P temp;
	JYH_Destroy_LE(jogo);
	jogo->pl = temp;
	JYH_Load_PL(jogo);
}
void JYH_LE_to_MM(JYH_GameState* jogo){//editor ao menu inicial
	JYH_Menu temp;
	JYH_Destroy_LE(jogo);
	jogo->mm = temp;
	JYH_Load_MM(jogo);
}
void JYH_LE_goback(JYH_GameState* jogo){//é preciso saber o estado anterior na hora de sair de um nível
	AUX_Desempilha(&jogo->state);
	switch(AUX_Top(&jogo->state)){
		case JYH_state_MM:
			JYH_LE_to_MM(jogo);
			break;
		case JYH_state_PL:
			JYH_LE_to_PL(jogo);
			break;
	}
}
void JYH_LE_to_EX(JYH_GameState* jogo){
	JYH_Level_Runner temp;
	AUX_Empilha(&jogo->state,JYH_state_EX);
	JYH_Destroy_LE(jogo);
	jogo->ex = temp;
	JYH_Load_EX(jogo);
}

//Conversão Tela Mundo- Avançado
void JYH_Converter_TelaMundo(SDL_Point* p, SDL_Rect* r_editor, SDL_Rect* r_camera){
	p->x = r_camera->x + p->x - r_editor->x;
	p->y = r_camera->y + p->y - r_editor->y;
}
void JYH_Converter_MundoTela(SDL_Point* p, SDL_Rect* r_editor, SDL_Rect* r_camera){
	p->x = (p->x-r_camera->x) + r_editor->x;
	p->y = (p->y-r_camera->y) + r_editor->y;
}
void JYH_Draw_Grade_Cam(JYH_GameState* jogo){
	SDL_Point p,coord1, coord2;
	SDL_Rect r = {0,0,64,96};
	SDL_Rect c = {0,0,32,48};
	coord1 = (SDL_Point){jogo->le.r_camera.x/64                        ,                        jogo->le.r_camera.y/64};
	coord2 = (SDL_Point){(jogo->le.r_camera.x + jogo->le.r_camera.w)/64,(jogo->le.r_camera.y + jogo->le.r_camera.h)/64};
	for(int i = coord1.x; i <= coord2.x; i++){
		if (i == jogo->le.lvl.w)break;
		for(int j = coord1.y; j <= coord2.y; j++){
			if (j == jogo->le.lvl.h)break;
			p.x = i*64;
			p.y = j*64 - 32;
			JYH_Converter_MundoTela(&p,&jogo->le.r_editor,&jogo->le.r_camera);
			r.x = p.x;
			r.y = p.y;
			c.x = c.w*(jogo->le.lvl.mat[j*(jogo->le.lvl.w)+i]);
			SDL_RenderCopy(jogo->ren,jogo->le.lvl.txt_theme,&c,&r);
		}
	}
}
void JYH_Move_Camera(JYH_GameState* jogo,int dx,int dy){
	jogo->le.r_camera.x -= dx;
	jogo->le.r_camera.y -= dy;
	if(jogo->le.r_camera.x < 0)jogo->le.r_camera.x = 0;
	else if(jogo->le.r_camera.x+jogo->le.r_camera.w  > 64*jogo->le.lvl.w)jogo->le.r_camera.x = 64*jogo->le.lvl.w - jogo->le.r_camera.w -jogo->le.r_editor.x;
	if(jogo->le.r_camera.y < 0)jogo->le.r_camera.y = 0;
	else if(jogo->le.r_camera.y+jogo->le.r_camera.h  > 64*jogo->le.lvl.h)jogo->le.r_camera.y = 64*jogo->le.lvl.h - jogo->le.r_camera.h - jogo->le.r_editor.y;
}

void JYH_Coloca_Parede(JYH_GameState* jogo, SDL_Point* p){
	JYH_Converter_TelaMundo(p,&jogo->le.r_editor,&jogo->le.r_camera);
	unsigned char *mat = jogo->le.lvl.mat;
	int              w = jogo->le.lvl.w  ;
	int              h = jogo->le.lvl.h  ;
	int idx = (p->x/64)+(p->y/64)*w;
	                 mat[idx  ] |= 16;                                                                               ;//bloco em si vira parede
	if(idx%w        )mat[idx-1] |= 1 ;//bloco à esquerda tem parede
	if((idx+1)%w    )mat[idx+1] |= 4 ;//bloco à direita tem parede
	if(idx - w >= 0 )mat[idx-w] |= 8 ;//bloco acima
	if(idx + w < w*h)mat[idx+w] |= 2 ;//blocoabaixo
	
}
void JYH_Apaga_Parede(JYH_GameState* jogo, SDL_Point* p){
	JYH_Converter_TelaMundo(p,&jogo->le.r_editor,&jogo->le.r_camera);
	unsigned char *mat = jogo->le.lvl.mat;
	int              w = jogo->le.lvl.w  ;
	int              h = jogo->le.lvl.h  ;
	int idx = (p->x/64)+(p->y/64)*w;
	                 mat[idx  ] -= (mat[idx  ] & 16)?16:0;//bloco em si vira parede
	if(idx%w        )mat[idx-1] -= (mat[idx-1] & 1 )?1 :0;//bloco à esquerda tem parede
	if((idx+1)%w    )mat[idx+1] -= (mat[idx+1] & 4 )?4 :0;//bloco à direita tem parede
	if(idx - w >= 0 )mat[idx-w] -= (mat[idx-w] & 8 )?8 :0;//bloco acima
	if(idx + w < w*h)mat[idx+w] -= (mat[idx+w] & 2 )?2 :0;//blocoabaixo
}

void JYH_Draw_Grade(JYH_GameState* jogo){//Versão antiga
	//grade contida no retângulo {0,100,1000,600}
	//assumir nivel de  50x30
	Uint32 stepx = jogo->le.editor.w/jogo->le.lvl.w;
	Uint32 stepy = jogo->le.editor.h/jogo->le.lvl.h;
	SDL_Rect r = {0,0,stepx,stepy*1.5};
	SDL_Rect c = {0,0,32,48};//retângulo de recorte dos tilesets
	SDL_SetRenderDrawColor(jogo->ren,0x00,0x00,0x00,0x00);
	for(int i = 0; i < jogo->le.lvl.w; i++)SDL_RenderDrawLine(jogo->ren,i*stepx,100,i*stepx,700);
	for(int i = 0; i < jogo->le.lvl.h; i++)SDL_RenderDrawLine(jogo->ren,0,100+i*stepy,1000,100+i*stepy);
	
	for(int i = 0; i < jogo->le.lvl.w; i ++){
		for(int j = 0; j < jogo->le.lvl.h; j++){
			r.x = i*stepx ;
			r.y = 100 + j*stepy - stepy*0.5;

			c.x = c.w*(jogo->le.lvl.mat[j*(jogo->le.lvl.w)+i]);
			SDL_RenderCopy(jogo->ren,jogo->le.lvl.txt_theme,&c,&r);
		}
	}
}

int JYH_Converter_Coordenada(JYH_GameState* jogo,SDL_Point* p){
	Uint32 stepx = jogo->le.editor.w/jogo->le.lvl.w;
	Uint32 stepy = jogo->le.editor.h/jogo->le.lvl.h;
	int x = (p->x)/(stepx);
	int y = (p->y - jogo->le.editor.y)/(stepy);
	return y*(jogo->le.lvl.w) + x;
}

//Execução

void JYH_LE(JYH_GameState* jogo){//Atualizar
	static SDL_Point p;
	Uint32 idx;
	
	SDL_SetRenderDrawColor(jogo->ren,0xff,0xff,0xff,0x00);//background
	SDL_RenderClear(jogo->ren);
	
    //JYH_Draw_Grade(jogo);

	//Experimental, Câmera

	JYH_Draw_Grade_Cam(jogo);

	AUX_Draw_Icon(jogo->ren,&jogo->le.tb);
	AUX_Draw_Icon(jogo->ren,&jogo->le.sb);
	
	//desenhar botões

	AUX_Draw_Icon(jogo->ren,&jogo->le.botao_V);
	AUX_Draw_Icon(jogo->ren,&jogo->le.botao_S);
	AUX_Draw_Icon(jogo->ren,&jogo->le.botao_R);
	AUX_Draw_Icon(jogo->ren,&jogo->le.botao_P);
	AUX_Draw_Icon(jogo->ren,&jogo->le.botao_A);
	AUX_Draw_Icon(jogo->ren,&jogo->le.botao_T);
	
	if(AUX_WaitEventTimeoutCount(&(jogo->evt),&(jogo->espera))){//trocar por exercicio
		switch(jogo->evt.type){
			case SDL_MOUSEMOTION:
				p = (SDL_Point){(int)jogo->evt.motion.x,(int)jogo->evt.motion.y};
				if(jogo->le.press && SDL_PointInRect(&p,&jogo->le.editor)){
					switch(jogo->le.pincel){
						case PINCEL_PINTANDO:JYH_Coloca_Parede(jogo,&p);break;
						case PINCEL_APAGANDO:JYH_Apaga_Parede(jogo,&p);break;
						case PINCEL_MOVER_CAMERA:JYH_Move_Camera(jogo,jogo->evt.motion.xrel,jogo->evt.motion.yrel);break;
					}
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				jogo->le.press = SDL_TRUE;
				p = (SDL_Point){(int)jogo->evt.button.x,(int)jogo->evt.button.y};
				if(jogo->le.press && SDL_PointInRect(&p,&jogo->le.editor)){
					switch(jogo->le.pincel){
						case PINCEL_PINTANDO:JYH_Coloca_Parede(jogo,&p);break;
						case PINCEL_APAGANDO:JYH_Apaga_Parede(jogo,&p);break;
						case PINCEL_DESOCUPADO:jogo->le.pincel = PINCEL_MOVER_CAMERA;break;
					}
				}
				break;
			case SDL_MOUSEBUTTONUP://verifica os cliques do botão
				p = (SDL_Point){(int)jogo->evt.button.x,(int)jogo->evt.button.y};
				jogo->le.press = SDL_FALSE;
				if(jogo->le.pincel == PINCEL_MOVER_CAMERA){
					jogo->le.pincel = PINCEL_DESOCUPADO;
				}
				else if      (SDL_PointInRect(&p,&jogo->le.botao_V.r)){
					JYH_LE_goback(jogo);
				}
				else if (SDL_PointInRect(&p,&jogo->le.botao_S.r)){
				    JYH_Save_lvl(jogo);
				}
				else if (SDL_PointInRect(&p,&jogo->le.botao_R.r)){
					JYH_LE_to_EX(jogo);
				}
				else if (SDL_PointInRect(&p,&jogo->le.botao_P.r)){
					jogo->le.pincel = (jogo->le.pincel == PINCEL_PINTANDO)?PINCEL_DESOCUPADO:PINCEL_PINTANDO;
					jogo->le.botao_P.f = (jogo->le.botao_P.f)?0:1;
					jogo->le.botao_A.f = 0;
					
				}
				else if (SDL_PointInRect(&p,&jogo->le.botao_A.r)){
					jogo->le.pincel = (jogo->le.pincel == PINCEL_APAGANDO)?PINCEL_DESOCUPADO:PINCEL_APAGANDO;
					jogo->le.botao_P.f = 0;
					jogo->le.botao_A.f = (jogo->le.botao_A.f)?0:1;
				}
				else if (SDL_PointInRect(&p,&jogo->le.botao_T.r)){/*Menu de Temas*/}
				break;
			case SDL_QUIT:
				AUX_Empilha(&jogo->state,JYH_END_GAME);
				JYH_Destroy_LE(jogo);
				break;
		}
	}else{
        jogo->espera = 10;
	/*eventos baseados em tempo*/
	}
	

}

//Load

void JYH_Load_LE(JYH_GameState* jogo){
	jogo->le.editor = (SDL_Rect){0,100,1000,600};
	jogo->le.press = SDL_FALSE;
	
	//teste
	jogo->le.lvl.w = 25;//default
	jogo->le.lvl.h = 15;//default
	jogo->le.lvl.mat = (unsigned char*)malloc(sizeof(unsigned char)*(jogo->le.lvl.w)*(jogo->le.lvl.h));
    strcpy(jogo->le.lvl.path_theme,".$img$geral$tile-Sheet.png");
    strcpy(jogo->le.lvl.nome,"teste");
	jogo->le.last_idx = (jogo->le.lvl.w)*(jogo->le.lvl.h);
	const int temp = (jogo->le.lvl.w)*(jogo->le.lvl.h);
	for(int i = 0; i < temp;i++)jogo->le.lvl.mat[i]= 0;
	memset(jogo->le.lvl.mat,0,sizeof(unsigned char)*(jogo->le.lvl.w)*(jogo->le.lvl.h));
	
	//teste câmera
	jogo->le.r_editor = (SDL_Rect){0,100,1000,600};
	jogo->le.r_camera = (SDL_Rect){0,0,1000,600};

	#ifdef _WIN32
	
	AUX_Start_Icon(jogo->ren,&jogo->le.botao_V  ,"img\\geral\\Back_JYH.png",(SDL_Rect){25,25,50,50},1);
	AUX_Start_Icon(jogo->ren,&jogo->le.botao_S  ,"img\\geral\\Save_JYH.png",(SDL_Rect){100,25,50,50},1);
	AUX_Start_Icon(jogo->ren,&jogo->le.botao_R  ,"img\\geral\\Run_JYH.png",(SDL_Rect){175,25,50,50},1);
	AUX_Start_Icon(jogo->ren,&jogo->le.botao_P  ,"img\\geral\\Paint_JYH-Sheet.png",(SDL_Rect){250,25,50,50},2);
	AUX_Start_Icon(jogo->ren,&jogo->le.botao_R  ,"img\\geral\\Run_JYH.png",(SDL_Rect){175,25,50,50},1);
	AUX_Start_Icon(jogo->ren,&jogo->le.botao_A       ,"img\\geral\\Apaga_JYH-Sheet.png",(SDL_Rect){325,25,50,50},2);
	AUX_Start_Icon(jogo->ren,&jogo->le.botao_T       ,"img\\geral\\Apaga_JYH-Sheet.png",(SDL_Rect){400,25,50,50},2);
	AUX_Start_Icon(jogo->ren,&jogo->le.tb       ,"img\\geral\\top_bar_JYH.png",(SDL_Rect){0,0,1200,100},1);
	AUX_Start_Icon(jogo->ren,&jogo->le.sb       ,"img\\geral\\side_bar_JYH.png",(SDL_Rect){1000,100,200,600},1);
	
	jogo->le.lvl.txt_theme = IMG_LoadTexture(jogo->ren,"img\\geral\\tile-Sheet.png");//sprite separado em vários de 32*48
	
	#elif __linux__
	
	AUX_Start_Icon(jogo->ren,&jogo->le.botao_V  ,"./img/geral/Back_JYH.png",(SDL_Rect){25,25,50,50},1);
	AUX_Start_Icon(jogo->ren,&jogo->le.botao_S  ,"./img/geral/Save_JYH.png",(SDL_Rect){100,25,50,50},1);
	AUX_Start_Icon(jogo->ren,&jogo->le.botao_R  ,"./img/geral/Run_JYH.png",(SDL_Rect){175,25,50,50},1);
	AUX_Start_Icon(jogo->ren,&jogo->le.botao_P  ,"./img/geral/Paint_JYH-Sheet.png",(SDL_Rect){250,25,50,50},2);
	AUX_Start_Icon(jogo->ren,&jogo->le.botao_A       ,"./img/geral/Apaga_JYH-Sheet.png",(SDL_Rect){325,25,50,50},2);
	AUX_Start_Icon(jogo->ren,&jogo->le.botao_T       ,"./img/geral/Apaga_JYH-Sheet.png",(SDL_Rect){400,25,50,50},2);
	AUX_Start_Icon(jogo->ren,&jogo->le.tb       ,"./img/geral/top_bar_JYH.png",(SDL_Rect){0,0,1200,100},1);
	AUX_Start_Icon(jogo->ren,&jogo->le.sb       ,"./img/geral/side_bar_JYH.png",(SDL_Rect){1000,100,200,600},1);
	
	jogo->le.lvl.txt_theme = IMG_LoadTexture(jogo->ren,"./img/geral/tile-Sheet.png");
	#endif
}
