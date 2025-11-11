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
	for(int i = 0; i < jogo->le.n_theme;i++)SDL_DestroyTexture(jogo->le.temas[i].txt);
	free(jogo->le.temas);
}

void JYH_Save_lvl(JYH_Nivel* lvl){
    char S[100];
    sprintf(S,".$JYH$MundoP$%s.txt",lvl->nome_nivel);
    AUX_AdaptarString(S);
    FILE* dest = fopen(S,"w");
    assert(dest!=NULL);
    fprintf(dest,"%d %d\n",lvl->w,lvl->h);
    fprintf(dest,"%s\n",lvl->tema);
    for(int i = 0; i < lvl->h; i++){
        for(int j = 0; j < lvl->w; j++){
            fprintf(dest,"%hhd ",lvl->mat[i*(lvl->w)+j]);
        }
        fprintf(dest,"\n");
    }
    fclose(dest);
}

void JYH_Read_lvl(JYH_Nivel* lvl){
	char S[100];
    sprintf(S,".$JYH$MundoP$%s.txt",lvl->nome_nivel);
    AUX_AdaptarString(S);
    FILE* orig =fopen(S,"r");
    if(orig != NULL){
    	fscanf(orig,"%d %d",&lvl->w,&lvl->h);
    	fscanf(orig,"%s\n",lvl->tema);
    	lvl->mat = (unsigned char*)malloc(sizeof(unsigned char)*(lvl->w)*(lvl->h));
    	for(int i = 0; i < lvl->h; i++){
        	for(int j = 0; j < lvl->w; j++){
            	fscanf(orig,"%hhd",&lvl->mat[i*(lvl->w)+j]);
        	}
    	}
    	fclose(orig);
	}else{
		lvl->w = 25;//default
		lvl->h = 25;//default
		lvl->mat = (unsigned char*)malloc(sizeof(unsigned char)*(lvl->w)*(lvl->h));
		strcpy(lvl->tema,"tema1");
		memset(lvl->mat,0,sizeof(unsigned char)*(lvl->w)*(lvl->h));
	}
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
	temp.lvl = jogo->le.lvl;
	jogo->ex = temp;
	JYH_Load_EX(jogo);
}

void JYH_Coloca_Parede(JYH_Camera* cam,JYH_Nivel* lvl, SDL_Point* p){
    JYH_Converter_TelaMundo(p,cam);
    unsigned char* mat = lvl->mat;
    int w = lvl->w;
    int h = lvl->h;
    int z = cam->zoom;
    int idx = (p->x/z)+(p->y/z)*w;
	                 mat[idx  ] |= 16;//bloco em si vira parede
	if(idx%w        )mat[idx-1] |= 1 ;//bloco à esquerda tem parede
	if((idx+1)%w    )mat[idx+1] |= 4 ;//bloco à direita tem parede
	if(idx - w >= 0 )mat[idx-w] |= 8 ;//bloco acima
	if(idx + w < w*h)mat[idx+w] |= 2 ;//blocoabaixo
	
}
void JYH_Apaga_Parede(JYH_Camera* cam,JYH_Nivel* lvl, SDL_Point* p){
    JYH_Converter_TelaMundo(p,cam);
    unsigned char * mat = lvl->mat;
    int w = lvl->w;
    int h = lvl->h;
    int z = cam->zoom;
    int idx = (p->x/z)+(p->y/z)*w;
	                 mat[idx  ] -= (mat[idx  ] & 16)?16:0;//bloco em si vira parede
	if(idx%w        )mat[idx-1] -= (mat[idx-1] & 1 )?1 :0;//bloco à esquerda tem parede
	if((idx+1)%w    )mat[idx+1] -= (mat[idx+1] & 4 )?4 :0;//bloco à direita tem parede
	if(idx - w >= 0 )mat[idx-w] -= (mat[idx-w] & 8 )?8 :0;//bloco acima
	if(idx + w < w*h)mat[idx+w] -= (mat[idx+w] & 2 )?2 :0;//blocoabaixo
}

int JYH_ZoomIn(JYH_Nivel* lvl,JYH_Camera* cam){//Aumenta o zoom e returna em Booleano se ainda é permitido
    cam->zoom *= 2;
    Uint32 zt = (cam->zoom)*2;
    return (2*zt > cam->r_cam.h || 2*zt > cam->r_cam.w);
}
int JYH_ZoomOut(JYH_Nivel* lvl,JYH_Camera* cam){
    cam->zoom /= 2;
    Uint32 zt = (cam->zoom)/2;
	return (zt*lvl->w < cam->r_cam.w || zt*lvl->h < cam->r_cam.h);//Se a câmera é maior que o mundo, então não pode da zoomOut
}

//Execução

void JYH_LE(JYH_GameState* jogo){//Atualizar
	static SDL_Point p;
	Uint32 idx;
	
	SDL_SetRenderDrawColor(jogo->ren,0xff,0xff,0xff,0x00);//background
	SDL_RenderClear(jogo->ren);
	
	JYH_Draw_Grade_Cam(jogo->ren,&jogo->le.lvl,&jogo->le.cam);

	AUX_Draw_Icon(jogo->ren,&jogo->le.tb);
	AUX_Draw_Icon(jogo->ren,&jogo->le.sb);
	
	//desenhar botões

	AUX_Draw_Icon(jogo->ren,&jogo->le.botao_V);
	AUX_Draw_Icon(jogo->ren,&jogo->le.botao_S);
	AUX_Draw_Icon(jogo->ren,&jogo->le.botao_R);
	AUX_Draw_Icon(jogo->ren,&jogo->le.botao_P);
	AUX_Draw_Icon(jogo->ren,&jogo->le.botao_A);
	AUX_Draw_Icon(jogo->ren,&jogo->le.botao_T);
    AUX_Draw_Icon(jogo->ren,&jogo->le.botao_ZoomIn);
    AUX_Draw_Icon(jogo->ren,&jogo->le.botao_ZoomOut);
	
	if(AUX_WaitEventTimeoutCount(&(jogo->evt),&(jogo->espera))){//trocar por exercicio
		switch(jogo->evt.type){
			case SDL_MOUSEMOTION:
				p = (SDL_Point){(int)jogo->evt.motion.x,(int)jogo->evt.motion.y};
				if(jogo->le.press && SDL_PointInRect(&p,&jogo->le.cam.r_box)){
					switch(jogo->le.pincel){
						case PINCEL_PINTANDO:JYH_Coloca_Parede(&jogo->le.cam,&jogo->le.lvl,&p);break;
						case PINCEL_APAGANDO:JYH_Apaga_Parede(&jogo->le.cam,&jogo->le.lvl,&p);break;
						case PINCEL_MOVER_CAMERA:JYH_Move_Camera(&jogo->le.cam,&jogo->le.lvl,-jogo->evt.motion.xrel,-jogo->evt.motion.yrel);break;
					}
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				jogo->le.press = SDL_TRUE;
				p = (SDL_Point){(int)jogo->evt.button.x,(int)jogo->evt.button.y};
				if(jogo->le.press && SDL_PointInRect(&p,&jogo->le.cam.r_box)){
					switch(jogo->le.pincel){
                        case PINCEL_PINTANDO:JYH_Coloca_Parede(&jogo->le.cam,&jogo->le.lvl,&p);break;
						case PINCEL_APAGANDO:JYH_Apaga_Parede(&jogo->le.cam,&jogo->le.lvl,&p);break;

						case PINCEL_DESOCUPADO:jogo->le.pincel = PINCEL_MOVER_CAMERA;break;
					}
				}
				else if      (SDL_PointInRect(&p,&jogo->le.botao_V.r)){
					JYH_LE_goback(jogo);
				}
				break;
			case SDL_MOUSEBUTTONUP://verifica os cliques do botão
				p = (SDL_Point){(int)jogo->evt.button.x,(int)jogo->evt.button.y};
				jogo->le.press = SDL_FALSE;
				if(jogo->le.pincel == PINCEL_MOVER_CAMERA){
					jogo->le.pincel = PINCEL_DESOCUPADO;
				}
				else if (SDL_PointInRect(&p,&jogo->le.botao_S.r)){
				    JYH_Save_lvl(&jogo->le.lvl);
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
				else if (SDL_PointInRect(&p,&jogo->le.botao_T.r)){
					jogo->le.i_theme = (jogo->le.i_theme+1)%(jogo->le.n_theme);
					jogo->le.lvl.txt_theme = jogo->le.temas[jogo->le.i_theme].txt;
					strcpy(jogo->le.lvl.tema,jogo->le.temas[jogo->le.i_theme].nome);
					/*Menu de Temas*/
				}
                else if (!jogo->le.botao_ZoomIn.f && SDL_PointInRect(&p,&jogo->le.botao_ZoomIn.r)){
                	jogo->le.botao_ZoomOut.f = JYH_ZoomOut(&jogo->le.lvl,&jogo->le.cam);
                	jogo->le.botao_ZoomOut.f = 0;
				}
                else if (!jogo->le.botao_ZoomOut.f && SDL_PointInRect(&p,&jogo->le.botao_ZoomOut.r)){
                	jogo->le.botao_ZoomOut.f = JYH_ZoomOut(&jogo->le.lvl,&jogo->le.cam);
                	jogo->le.botao_ZoomIn.f = 0;
				}
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
	printf("Iniciando\n");
	char S[100];
	JYH_Read_lvl(&jogo->le.lvl);//Só executa se o nível não está carregado
	JYH_Inicia_Camera(&jogo->le.cam,(SDL_Rect){0,100,1000,600},(SDL_Rect){0,0,1000,600},64);
	
	jogo->le.press = SDL_FALSE;
	jogo->le.pincel = PINCEL_DESOCUPADO;
	printf("Lendo temas\n");
	printf("%s\n",PATH_THEME_LIST);
	FILE* arq = fopen(PATH_THEME_LIST,"r");
	
	fscanf(arq,"%u",&jogo->le.n_theme);
	assert(arq != NULL);
	jogo->le.temas = (JYH_Theme*)malloc(sizeof(JYH_Theme)*(jogo->le.n_theme));
	jogo->le.i_theme = 0;
	for(int i = 0; i <jogo->le.n_theme;i++){
		fscanf(arq,"%s",jogo->le.temas[i].nome);
		sprintf(S,IMG_GET_THEME,jogo->le.temas[i].nome);
		jogo->le.temas[i].txt = IMG_LoadTexture(jogo->ren,S);
		if(!strcmp(jogo->le.temas[i].nome,jogo->le.lvl.tema)){
			jogo->le.lvl.txt_theme = jogo->le.temas[i].txt;
			jogo->le.i_theme = i;//o tema selecionado é o tema inicial do nível
		}
	}
	fclose(arq);
	printf("Fechei o arquivo\n");
	
	AUX_Start_Icon(jogo->ren,&jogo->le.tb       ,IMG_LE_TB,(SDL_Rect){0,0,1200,100},1);
	AUX_Start_Icon(jogo->ren,&jogo->le.sb       ,IMG_LE_SB,(SDL_Rect){1000,100,200,600},1);
	
	AUX_Start_Icon(jogo->ren,&jogo->le.botao_V  ,IMG_B_BACK,(SDL_Rect){25,25,50,50},1);
	AUX_Start_Icon(jogo->ren,&jogo->le.botao_S  ,IMG_B_SAVE,(SDL_Rect){100,25,50,50},1);
	AUX_Start_Icon(jogo->ren,&jogo->le.botao_R  ,IMG_B_RUN,(SDL_Rect){175,25,50,50},1);
	AUX_Start_Icon(jogo->ren,&jogo->le.botao_P  ,IMG_B_PAINT,(SDL_Rect){250,25,50,50},2);
	AUX_Start_Icon(jogo->ren,&jogo->le.botao_A  ,IMG_B_ERASE,(SDL_Rect){325,25,50,50},2);
	AUX_Start_Icon(jogo->ren,&jogo->le.botao_T       ,IMG_B_ERASE,(SDL_Rect){400,25,50,50},2);//trocar
	AUX_Start_Icon(jogo->ren,&jogo->le.botao_ZoomIn  ,IMG_B_ZOOMIN,(SDL_Rect){475,25,50,50},2);
	AUX_Start_Icon(jogo->ren,&jogo->le.botao_ZoomOut ,IMG_B_ZOOMOUT,(SDL_Rect){550,25,50,50},2);
	
}
