//Editor Nível
#include "JYH_Header.h"

enum LE_EVENT{
	JYH_LE_SAVE,
	JYH_LE_GOBACK,
	JYH_LE_RUN
};

void JYH_Destroy_LE(JYH_GameState* jogo){//desalocar
	SDL_DestroyTexture(jogo->le.botao_V.txt);
	SDL_DestroyTexture(jogo->le.botao_P.txt);
	SDL_DestroyTexture(jogo->le.botao_R.txt);
	SDL_DestroyTexture(jogo->le.botao_S.txt);
	SDL_DestroyTexture(jogo->le.botao_T.txt);
	SDL_DestroyTexture(jogo->le.botao_A.txt);
	SDL_DestroyTexture(jogo->le.tb.txt);
	SDL_DestroyTexture(jogo->le.sb.txt);
	SDL_DestroyTexture(jogo->le.txt_frame);
    SDL_DestroyTexture(jogo->le.txt_sel);
	
	free(jogo->le.lvl.mat);//temporario
	for(int i = 0; i < jogo->le.n_theme;i++)SDL_DestroyTexture(jogo->le.temas[i].txt);
	free(jogo->le.temas);
	for(int i = 0; i < jogo->le.n_obj;i++)SDL_DestroyTexture(jogo->le.objetos[i].txt);
	free(jogo->le.objetos);
}
void JYH_Draw_Sel(SDL_Renderer* ren,JYH_Editor* le){
    SDL_Point p;
    SDL_GetMouseState(&p.x,&p.y);
    JYH_Converter_TelaMundo(&p,&le->cam);
    p.x /= le->cam.zoom;
    p.y /= le->cam.zoom;
    if(p.x > le->lvl.w || p.y >le->lvl.h || p.x < 0 || p.y < 0)return;
    int idx = (p.y)*(le->lvl.w)+(p.x);
    p.x *= le->cam.zoom;
    p.y *= le->cam.zoom;
    JYH_Converter_MundoTela(&p,&le->cam);
    SDL_Rect r = {p.x,p.y,le->cam.zoom,le->cam.zoom};
    SDL_Rect c = {0,32*((le->lvl.mat[idx].t & 16)!=0),32,32};
    SDL_RenderCopy(ren,le->txt_sel,&c,&r);
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
            fprintf(dest,"%hhd ",lvl->mat[i*(lvl->w)+j].t);
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
        lvl->mat = (JYH_Tile*)malloc(sizeof(JYH_Tile)*(lvl->w)*(lvl->h));
    	for(int i = 0; i < lvl->h; i++){
        	for(int j = 0; j < lvl->w; j++){
                fscanf(orig,"%hhu",&lvl->mat[i*(lvl->w)+j].t);
                lvl->mat[i*(lvl->w)+j].o = JYH_OBJ_PLAYER;//Começa vazio
        	}
    	}
    	fclose(orig);
	}else{
		lvl->w = 25;//default
		lvl->h = 25;//default
        lvl->mat = (JYH_Tile*)malloc(sizeof(JYH_Tile)*(lvl->w)*(lvl->h));
		strcpy(lvl->tema,"tema1");
    	for(int i = 0; i < lvl->h; i++){
        	for(int j = 0; j < lvl->w; j++){
                fscanf(orig,"%hhd",&lvl->mat[i*(lvl->w)+j].t);
                lvl->mat[i*(lvl->w)+j].o = N_OBJECTS;//Começa vazio
                lvl->mat[i*(lvl->w)+j].o =         0;
        	}
    	}
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

int JYH_Coloca_Parede(JYH_Camera* cam,JYH_Nivel* lvl, SDL_Point* p){
    JYH_Converter_TelaMundo(p,cam);
    JYH_Tile* mat;
    int w = lvl->w;
    int h = lvl->h;
    int z = cam->zoom;
    int idx = (p->x/z)+(p->y/z)*w;
    if(mat[idx].t&16)return 1;//se já era uma parede, então não houve mudança
	                 mat[idx  ].t |= 16;//bloco em si vira parede
	if(idx%w        )mat[idx-1].t |= 1 ;//bloco à esquerda tem parede
	if((idx+1)%w    )mat[idx+1].t |= 4 ;//bloco à direita tem parede
	if(idx - w >= 0 )mat[idx-w].t |= 8 ;//bloco acima
	if(idx + w < w*h)mat[idx+w].t |= 2 ;//blocoabaixo
	return 0;
}
int JYH_Apaga_Parede(JYH_Camera* cam,JYH_Nivel* lvl, SDL_Point* p){
    JYH_Converter_TelaMundo(p,cam);
    JYH_Tile* mat;
    int w = lvl->w;
    int h = lvl->h;
    int z = cam->zoom;
    int idx = (p->x/z)+(p->y/z)*w;
    if(!(mat[idx].t&16))return 1;//se já era chão, então não house mudança
	                 mat[idx  ].t -= (mat[idx  ].t & 16)?16:0;//bloco em si vira parede
	if(idx%w        )mat[idx-1].t -= (mat[idx-1].t & 1 )?1 :0;//bloco à esquerda tem parede
	if((idx+1)%w    )mat[idx+1].t -= (mat[idx+1].t & 4 )?4 :0;//bloco à direita tem parede
	if(idx - w >= 0 )mat[idx-w].t -= (mat[idx-w].t & 8 )?8 :0;//bloco acima
	if(idx + w < w*h)mat[idx+w].t -= (mat[idx+w].t & 2 )?2 :0;//blocoabaixo
	return 0;
}

int JYH_ZoomIn(JYH_Nivel* lvl,JYH_Camera* cam){//Aumenta o zoom e returna em Booleano se ainda é permitido
    cam->zoom *= 2;
    Uint32 zt = (cam->zoom)*2;
    return (zt > 256);
}
int JYH_ZoomOut(JYH_Nivel* lvl,JYH_Camera* cam){
    cam->zoom /= 2;
    Uint32 zt = (cam->zoom)/2;
	return (zt*lvl->w < cam->r_cam.w || zt*lvl->h < cam->r_cam.h);//Se a câmera é maior que o mundo, então não pode da zoomOut
}

//Execução

void JYH_LE_Draw_TB(SDL_Renderer* ren,JYH_Editor* le){
	AUX_Draw_Icon(ren,&le->tb);
	//Botões
	AUX_Draw_Icon(ren,&le->botao_V);
	AUX_Draw_Icon(ren,&le->botao_S);
	AUX_Draw_Icon(ren,&le->botao_R);
	AUX_Draw_Icon(ren,&le->botao_P);
	AUX_Draw_Icon(ren,&le->botao_A);
	AUX_Draw_Icon(ren,&le->botao_T);
    AUX_Draw_Icon(ren,&le->botao_ZoomIn);
    AUX_Draw_Icon(ren,&le->botao_ZoomOut);
}
void JYH_LE_Draw_SB(SDL_Renderer* ren, JYH_Editor* le){
	//sb  = {1000,100,200,600},1
	AUX_Draw_Icon(ren,&le->sb);
	SDL_Rect r_obj = (SDL_Rect){0,0,32,32};
	SDL_Rect r_frame = (SDL_Rect){0,0,48,48};
	SDL_Rect c_obj = (SDL_Rect){0,0,32,32};
	SDL_Rect c_frame = (SDL_Rect){0,0,48,48};
	
	for(int i = 0; i < le->n_obj; i++){
		r_frame.x = le->sb.r.x + 14 + (48+14)*(i%3);
		r_frame.y = le->sb.r.y + 14 + (48+14)*(i/3);
		c_frame.y = 0;//depende do estado da célula/assinatura;
		r_obj.x = r_frame.x + 8;
		r_obj.y = r_frame.y + 8;
		SDL_RenderCopy(ren,le->txt_frame,&c_frame,&r_frame);
		SDL_RenderCopy(ren,le->objetos[i].txt,&c_obj,&r_obj);
	}
}

void JYH_Draw_LE(SDL_Renderer* ren,JYH_Editor* le){
	SDL_SetRenderDrawColor(ren,0xff,0xff,0xff,0x00);//background
	SDL_RenderClear(ren);
	JYH_Draw_Grade_Cam(ren,&le->lvl,&le->cam,le->objetos);
    JYH_Draw_Sel(ren,le);
	JYH_LE_Draw_TB(ren,le);
	JYH_LE_Draw_SB(ren,le);
}

void JYH_LE_MOUSEMOTION(JYH_Editor* le,SDL_MouseMotionEvent* evt){
	SDL_Point p = (SDL_Point){(int)evt->x,(int)evt->y};
	if(le->press && SDL_PointInRect(&p,&le->cam.r_box)){
		switch(le->pincel){
			case PINCEL_PINTANDO:
                le->botao_S.f &= JYH_Coloca_Parede(&le->cam,&le->lvl,&p);
            	break;
			case PINCEL_APAGANDO:
                le->botao_S.f &= JYH_Apaga_Parede(&le->cam,&le->lvl,&p);
                break;
			case PINCEL_MOVER_CAMERA:
                JYH_Move_Camera(&le->cam,&le->lvl,-evt->xrel,-evt->yrel);
                break;
			}
	}
}
void JYH_LE_MOUSEBUTTONDOWN(JYH_Editor* le,SDL_MouseButtonEvent* evt){
	le->press = SDL_TRUE;
	SDL_Point p = (SDL_Point){(int)evt->x,(int)evt->y};
	if(SDL_PointInRect(&p,&le->cam.r_box)){
		switch(le->pincel){
            case PINCEL_PINTANDO:
                le->botao_S.f &= JYH_Coloca_Parede(&le->cam,&le->lvl,&p);
                break;
			case PINCEL_APAGANDO:
                le->botao_S.f &= JYH_Apaga_Parede(&le->cam,&le->lvl,&p);
            	break;
			case PINCEL_DESOCUPADO:
                le->pincel = PINCEL_MOVER_CAMERA;
                break;
		}
	}
}

void JYH_LE_MOUSEBUTTONUP(JYH_Editor* le, SDL_MouseButtonEvent* evt){
	SDL_Point p = (SDL_Point){(int)evt->x,(int)evt->y};
	le->press = SDL_FALSE;
	if(le->pincel == PINCEL_MOVER_CAMERA){
		le->pincel = PINCEL_DESOCUPADO;
	}
	else if (!le->botao_S.f && SDL_PointInRect(&p,&le->botao_S.r)){
		le->botao_S.f = 1;
		AUX_CriarEvento(JYH_LE_SAVE,NULL);
	}
	else if (!le->botao_R.f && SDL_PointInRect(&p,&le->botao_R.r)){
		le->botao_R.f = 1;
		AUX_CriarEvento(JYH_LE_RUN,NULL);
	}
	else if (SDL_PointInRect(&p,&le->botao_P.r)){
		le->pincel = (le->pincel == PINCEL_PINTANDO)?PINCEL_DESOCUPADO:PINCEL_PINTANDO;
		le->botao_P.f = (le->botao_P.f)?0:1;
		le->botao_A.f = 0;
	}
	else if (SDL_PointInRect(&p,&le->botao_A.r)){
		le->pincel = (le->pincel == PINCEL_APAGANDO)?PINCEL_DESOCUPADO:PINCEL_APAGANDO;
		le->botao_P.f = 0;
		le->botao_A.f = (le->botao_A.f)?0:1;
	}
	else if (SDL_PointInRect(&p,&le->botao_T.r)){
		le->i_theme = (le->i_theme+1)%(le->n_theme);
		le->lvl.txt_theme = le->temas[le->i_theme].txt;
		strcpy(le->lvl.tema,le->temas[le->i_theme].nome);
		le->botao_S.f = 0;
	}
    else if (!le->botao_ZoomIn.f && SDL_PointInRect(&p,&le->botao_ZoomIn.r)){
        le->botao_ZoomIn.f = JYH_ZoomIn(&le->lvl,&le->cam);
        le->botao_ZoomOut.f = 0;
	}
    else if (!le->botao_ZoomOut.f && SDL_PointInRect(&p,&le->botao_ZoomOut.r)){
        le->botao_ZoomOut.f = JYH_ZoomOut(&le->lvl,&le->cam);
        le->botao_ZoomIn.f = 0;
	}
	else if(SDL_PointInRect(&p,&le->botao_V.r)){
		le->botao_V.f = 1;
		AUX_CriarEvento(JYH_LE_GOBACK,NULL);
	}
}

void JYH_LE_USEREVENT(JYH_GameState* jogo,SDL_UserEvent* evt){
	switch(evt->code){
		case JYH_LE_SAVE:
			JYH_Save_lvl(&jogo->le.lvl);
			break;
		case JYH_LE_RUN:
			JYH_LE_to_EX(jogo);
			break;
		case JYH_LE_GOBACK:
			JYH_LE_goback(jogo);
			break;
	}
}

void JYH_LE(JYH_GameState* jogo){//Atualizar
	static SDL_Point p;
	Uint32 idx;
	
	JYH_Draw_LE(jogo->ren,&jogo->le);
	
	if(AUX_WaitEventTimeoutCount(&(jogo->evt),&(jogo->espera))){//trocar por exercicio
		switch(jogo->evt.type){
			case SDL_MOUSEMOTION:
				JYH_LE_MOUSEMOTION(&jogo->le,&jogo->evt.motion);
				break;
			case SDL_MOUSEBUTTONDOWN:
				JYH_LE_MOUSEBUTTONDOWN(&jogo->le,&jogo->evt.button);
				break;
			case SDL_MOUSEBUTTONUP:
				JYH_LE_MOUSEBUTTONUP(&jogo->le,&jogo->evt.button);
				break;
			case SDL_USEREVENT:
				JYH_LE_USEREVENT(jogo,&jogo->evt.user);
				break;
			case SDL_QUIT:
				AUX_Empilha(&jogo->state,JYH_END_GAME);
				JYH_Destroy_LE(jogo);
				break;
		}
	}else{
        jogo->espera = 10;
	}
}

//Load

void JYH_LE_Load_Themes(SDL_Renderer* ren,JYH_Editor* le){
	char S[100];
	FILE* arq = fopen(PATH_THEME_LIST,"r");
	fscanf(arq,"%u",&le->n_theme);
	assert(arq != NULL);
	le->temas = (JYH_Theme*)malloc(sizeof(JYH_Theme)*(le->n_theme));
	le->i_theme = 0;
	for(int i = 0; i < le->n_theme;i++){
		fscanf(arq,"%s",le->temas[i].nome);
		sprintf(S,IMG_GET_THEME,le->temas[i].nome);
		le->temas[i].txt = IMG_LoadTexture(ren,S);
		assert(le->temas[i].txt!= NULL);
		if(!strcmp(le->temas[i].nome,le->lvl.tema)){
			le->lvl.txt_theme = le->temas[i].txt;
			le->i_theme = i;//o tema selecionado é o tema inicial do nível
		}
	}
	fclose(arq);
}

void JYH_LE_Load_Obj(SDL_Renderer* ren,JYH_Editor* le ){
	char S[100];
	FILE* arq = fopen(PATH_OBJ,"r");
	fscanf(arq,"%u",&le->n_obj);
	assert(arq != NULL);
	le->objetos = (JYH_Ass_Obj*)malloc(sizeof(JYH_Ass_Obj)*(le->n_obj));
	for(int i = 0; i < le->n_obj;i++){
		fscanf(arq,"%s",le->objetos[i].nome);
		sprintf(S,OBJ_GET_IMG,le->objetos[i].nome);
		le->objetos[i].txt = IMG_LoadTexture(ren,S);
		assert(le->objetos[i].txt!= NULL);
	}
	fclose(arq);
}

void JYH_Load_LE(JYH_GameState* jogo){
	JYH_Read_lvl(&jogo->le.lvl);//Só executa se o nível não está carregado
	JYH_Inicia_Camera(&jogo->le.cam,(SDL_Rect){0,100,1000,600},(SDL_Rect){0,0,1000,600},64);

	jogo->le.press = SDL_FALSE;
	jogo->le.pincel = PINCEL_DESOCUPADO;
	JYH_LE_Load_Themes(jogo->ren,&jogo->le);
	JYH_LE_Load_Obj(jogo->ren,&jogo->le);
	jogo->le.txt_frame = IMG_LoadTexture(jogo->ren,IMG_LE_FRAME);
    jogo->le.txt_sel = IMG_LoadTexture(jogo->ren,IMG_LE_SEL);
	AUX_Start_Icon(jogo->ren,&jogo->le.tb       ,IMG_LE_TB,(SDL_Rect){0,0,1200,100},1);
	AUX_Start_Icon(jogo->ren,&jogo->le.sb       ,IMG_LE_SB,(SDL_Rect){1000,100,200,600},1);
	
	AUX_Start_Icon(jogo->ren,&jogo->le.botao_V  ,IMG_B_BACK,(SDL_Rect){25,25,50,50},1);
	AUX_Start_Icon(jogo->ren,&jogo->le.botao_S  ,IMG_B_SAVE,(SDL_Rect){100,25,50,50},1);
	AUX_Start_Icon(jogo->ren,&jogo->le.botao_R  ,IMG_B_RUN,(SDL_Rect){175,25,50,50},1);
	AUX_Start_Icon(jogo->ren,&jogo->le.botao_P  ,IMG_B_PAINT,(SDL_Rect){250,25,50,50},2);
	AUX_Start_Icon(jogo->ren,&jogo->le.botao_A  ,IMG_B_ERASE,(SDL_Rect){325,25,50,50},2);
	AUX_Start_Icon(jogo->ren,&jogo->le.botao_T       ,IMG_B_BRUSH,(SDL_Rect){400,25,50,50},2);
	AUX_Start_Icon(jogo->ren,&jogo->le.botao_ZoomIn  ,IMG_B_ZOOMIN,(SDL_Rect){475,25,50,50},2);
	AUX_Start_Icon(jogo->ren,&jogo->le.botao_ZoomOut ,IMG_B_ZOOMOUT,(SDL_Rect){550,25,50,50},2);
    //verifica se pode dar zoomOut no inicio do jogo
	jogo->le.botao_ZoomOut.f = ((jogo->le.cam.zoom/2)*(jogo->le.lvl.w) < (jogo->le.cam.r_cam.w) || (jogo->le.cam.zoom/2)*(jogo->le.lvl.h) < (jogo->le.cam.r_cam.h));
    //no começo o nivel não tem alterações
    jogo->le.botao_S.f = 1;
}
