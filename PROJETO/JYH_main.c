#include <SDL2/SDL.h>
#include <stdio.h>
#define JYH_SCREEN_WIDTH 1200
#define JYH_SCREEN_HEIGHT 700
#define TEMPO_UPDATE 100

//Atualizar os comandos de compilação conforme surge a necessidade(procurar sobre Makefile no futuro)
//compilação Windows: gcc .\JYH_main.c -I .\include\ -L .\lib\ -lmingw32 -lSDL2main -lSDL2

int AUX_WaitEventTimeoutCount(SDL_Event* evt, Uint32* ms){
	Uint32 antes = SDL_GetTicks();
	int temEvento = SDL_WaitEventTimeout(evt,*ms);
	(*ms) = (temEvento)?(*ms)- (SDL_GetTicks()-antes):TEMPO_UPDATE;
	return temEvento;
}

//Estruturas de dados do jogo

typedef struct JYH_Mundo{//estrutura de dados representando um mundo do jogo. Fica armazenada em arquivos para ser carregada para esta estrutura
	//lista dos niveis do mundo e estética na hora de seleção
	char nome[30];
}JYH_Mundo;
typedef struct JYH_Nivel{//estrutura de dados representando um nível do jogo. Fica armazenada em arquivos para ser carregada para esta estrutura
	//Grid do nível e posições dos objetos
	char nome[30];
}JYH_Nivel;
typedef struct JYH_Objeto{//estrutura de dados representando os objetos de uma fase.
	//Sprites + tipo do objeto e o necessário para gerênciar a atualização
}JYH_Objeto;

//Estados do jogo

enum GAME_STATE{
	JYH_END_GAME = 0,//estado para encerrar o jogo
	JYH_MAIN_MENU,//menu inicial
	JYH_LVL_EDITOR,//editor de niveis
	JYH_LVL_EXEC,//modo execução
	JYH_LVL_SELECTION,//seleção de níveis do jogo
	JYH_LVL_SELECTION_P,//seleção de níveis do player
	JYH_WORLD_SELECTION,//escolher mundo
	
	//Os Loads serão estados para carregar texturas e só apresentarão uma tela de loading - Implementar como estados transicionais
	
	JYH_LOAD_MENU,//carrega o menu principal
	JYH_LOAD_EDITOR,//carrega o editor
	JYH_LOAD_LEVEL,//carrega um nível para execução
	JYH_LOAD_WORLD,//carrega um mundo
	JYH_LOAD_WORLD_SELECTION,//carrega o seletor de mundos
	JYH_LOAD_PLAYER_LIBRARY//carrega a seleção de níveis do jogador
};


typedef struct JYH_Menu{//Guarda os elementos necessários para o menu rodar
	SDL_Rect title;
	SDL_Rect botao_worlds;//vai para modo seleção de mundos
	SDL_Rect botao_edit  ;//vai para modo editor
	SDL_Rect botao_selP  ;//vai para modo seleção de níveis do jogador
	Uint32 estado;
	//adicionar texturas dos botões
}JYH_Menu;
typedef struct JYH_Editor{//Guarda os elementos necessários para o editor funcionar
	JYH_Nivel lvl;        //Nivel a ser editado
	SDL_Rect top_bar;     //barra de cima(onde ficam os botões para sair, salvar, executar, etc)
	SDL_Rect side_bar;    //barra lateral(onde ficam os itens)
	SDL_Rect editor;      //região da tela na qual está o editor
	SDL_Rect botao_voltar;//vai para o estado anterior
	SDL_Rect botao_run;   //vai para o estado de execução
	SDL_Rect botao_salvar;//salva as alterações no nível
	//adicionar texturas
}JYH_Editor;
typedef struct JYH_Level_Runner{//Guarda os elementos necessários para a execução de um nível
	JYH_Nivel lvl;//Nivel que está sendo jogado
	SDL_Rect top_bar;     //barra de cima(onde ficam os botões para sair e reiniciar, assim como a contagem de tempo e de gemas)
	SDL_Rect icone_gemas;
	SDL_Rect icone_relogio;
	SDL_Rect contagem_gemas;
	SDL_Rect contagem_tempo;
	SDL_Rect botao_voltar;
	SDL_Rect botao_reiniciar;
	Uint32 gemas_coletadas;//gemas coletadas até o momento no nível
	Uint32 tesouro_pego;//se o artefato do nível foi pego
	Uint32 tempo_de_jogo;//tempo que o nível está sendo jogado
	Uint32 timer;//tempo restante para sair com o artefato após pegar o tesouro
}JYH_Level_Runner;
typedef struct JYH_World_Selection{//Guarda os elementos necessários para a seleção de mundo
	Uint32 n_mundos;//quantidade de mundos
	JYH_Mundo* mundos;//lista de mundos
	SDL_Rect botao_voltar;//botão para voltar atrás
	SDL_Rect title;//"Modo Historia"
}JYH_World_Selection;
typedef struct JYH_Level_Selection{//Guarda os elementos necessários para a tela de seleção de níveis
	Uint32 n_niveis;//quantidade de niveis
	JYH_Nivel* niveis;//lista de níveis
	SDL_Rect title;//nome do cumdo
	SDL_Rect botao_voltar;//botão para voltar atrás
}JYH_Level_Selection;
typedef struct JYH_Level_Selection_P{//Guarda os elementos necessários para a seleção de níveis criados pelo jogador
	Uint32 n_niveis;//quantidade de niveis
	JYH_Nivel* niveis;//lista de niveis
	SDL_Rect title;//"Meus niveis"
	SDL_Rect botao_voltar;//botão para voltar atrás
}JYH_Level_Selection_P;


typedef struct JYH_GameState{
	enum GAME_STATE estado;//estado do jogo
	enum GAME_STATE estado_anterior;//estado do jogo anterior
	Uint32 espera;//coordena o tempo de atualização do jogo
	Uint32 w_tela, h_tela;//dimensões da tela(caso permitirmos a customização)
	SDL_Window* win;//janela
	SDL_Renderer* ren;//renderizador
	SDL_Event evt;//evento
	union{
		JYH_Menu menu;
		JYH_Editor edit;
		JYH_Level_Runner exec;
		JYH_World_Selection worlds;
		JYH_Level_Selection sel;
		JYH_Level_Selection_P selP;
	};
}JYH_GameState;

//Inicialização e Encerramento
JYH_GameState* JYH_Init();
void JYH_EndGame(JYH_GameState* jogo);

//Estados do Jogo
void JYH_GameMenu(JYH_GameState* jogo);
void JYH_GameLvlEditor(JYH_GameState* jogo);
void JYH_GameLvlExecution(JYH_GameState* jogo);
void JYH_GameLvlSelection(JYH_GameState* jogo);
void JYH_GameLvlSelection_P(JYH_GameState* jogo);
void JYH_GameWorldSelection(JYH_GameState* jogo);

//Estados de Transição
void JYH_GameLoadMenu(JYH_GameState* jogo);
void JYH_GameLoadEditor(JYH_GameState* jogo);
void JYH_GameLoadExec(JYH_GameState* jogo);
void JYH_GameLoadSel(JYH_GameState* jogo);
void JYH_GameLoadSelP(JYH_GameState* jogo);
void JYH_GameLoadWorlds(JYH_GameState* jogo);

//Controle principal
void JYH_GameRender(JYH_GameState* jogo);

int main (int argc, char* args[]){
	JYH_GameState* jogo = JYH_Init();
	while(jogo->estado)JYH_GameRender(jogo);
    JYH_EndGame(jogo);
}

//Inicialização e Encerramento

JYH_GameState jogo;//usar global temporariamente(trocar para alocação futuramente)

JYH_GameState* JYH_Init(){//todas as inicializações do jogo vão aqui
	//INICIAR SDL
	SDL_Init(SDL_INIT_EVERYTHING);
    jogo.win = SDL_CreateWindow("Jewelry Hunt",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,JYH_SCREEN_WIDTH, JYH_SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    jogo.ren = SDL_CreateRenderer(jogo.win, -1, 0);
    
    //outras inicializações abaixo
    jogo.estado = JYH_LOAD_MENU;
    jogo.espera = TEMPO_UPDATE;
    jogo.w_tela = JYH_SCREEN_WIDTH;
    jogo.h_tela = JYH_SCREEN_HEIGHT;
    return &jogo;
}

void JYH_EndGame(JYH_GameState* jogo){//todas as terminações do jogo vão aqui
	//outros encerramentos 
	
	//encerra o SDL
	SDL_DestroyRenderer(jogo->ren);
    SDL_DestroyWindow(jogo->win);
    SDL_Quit();
}

//Estados do jogo

void JYH_GameRender(JYH_GameState* jogo){
	switch(jogo->estado){
		//estados do jogo
		case JYH_MAIN_MENU:
			JYH_GameMenu(jogo);
			break;
		case JYH_LVL_EDITOR:
			JYH_GameLvlEditor(jogo);
			break;
		case JYH_LVL_EXEC:
			JYH_GameLvlExecution(jogo);
			break;
		case JYH_LVL_SELECTION:
			JYH_GameLvlSelection(jogo);
			break;
		case JYH_LVL_SELECTION_P:
			JYH_GameLvlSelection_P(jogo);
			break;
		case JYH_WORLD_SELECTION:
			JYH_GameWorldSelection(jogo);
			break;
		//estados de transição
		case JYH_LOAD_MENU:
			JYH_GameLoadMenu(jogo);
			break;
		case JYH_LOAD_EDITOR:
			JYH_GameLoadEditor(jogo);
			break;
		case JYH_LOAD_LEVEL:
			JYH_GameLoadExec(jogo);
			break;
		case JYH_LOAD_WORLD_SELECTION:
			JYH_GameLoadWorlds(jogo);
			break;
		case JYH_LOAD_WORLD:
			JYH_GameLoadSel(jogo);
			break;
		case JYH_LOAD_PLAYER_LIBRARY:
			JYH_GameLoadSelP(jogo);
			break;
	}
	SDL_RenderPresent(jogo->ren);
}

/* ESTADOS DO JOGO*/

void JYH_GameMenu(JYH_GameState* jogo){
	static SDL_Point p;
	
	SDL_SetRenderDrawColor(jogo->ren,0xff,0xff,0xff,0x00);//background
	SDL_RenderClear(jogo->ren);
	
	SDL_SetRenderDrawColor(jogo->ren,0xff,0xff,0x00,0x00);
	SDL_RenderFillRect(jogo->ren,&jogo->menu.title);
	
	if(AUX_WaitEventTimeoutCount(&(jogo->evt),&(jogo->espera))){
		switch(jogo->evt.type){
			case SDL_MOUSEBUTTONDOWN://verifica os cliques do botão
				p.x = (int)jogo->evt.button.x; p.y = (int)jogo->evt.button.y;
				
				if (SDL_PointInRect(&p,&jogo->menu.botao_worlds))jogo->estado = /*JYH_WORLD_SELECTION*/JYH_LOAD_WORLD_SELECTION;
				else if (SDL_PointInRect(&p,&jogo->menu.botao_selP))jogo->estado = /*JYH_LVL_SELECTION_P*/JYH_LOAD_PLAYER_LIBRARY;
				else if (SDL_PointInRect(&p,&jogo->menu.botao_edit))jogo->estado = /*JYH_LVL_EDITOR*/JYH_LOAD_EDITOR;
				
				break;
			case SDL_QUIT:
				jogo->estado = JYH_END_GAME;
				break;
		}
	}else{
		//eventos baseados em tempo
	}
	
	//desenhar botões
	
	SDL_SetRenderDrawColor(jogo->ren,0xff,0x00,0x00,0x00);//cor botão
	SDL_RenderFillRect(jogo->ren,&jogo->menu.botao_worlds);
	SDL_RenderFillRect(jogo->ren,&jogo->menu.botao_selP);
	SDL_RenderFillRect(jogo->ren,&jogo->menu.botao_edit);
}

void JYH_GameLvlEditor(JYH_GameState* jogo){//Atualizar
	static SDL_Point p;
	
	SDL_SetRenderDrawColor(jogo->ren,0xff,0xff,0xff,0x00);//background
	SDL_RenderClear(jogo->ren);
	
	SDL_SetRenderDrawColor(jogo->ren,0x00,0x00,0xff,0x00);//estético
	SDL_RenderFillRect(jogo->ren,&jogo->edit.side_bar);
	SDL_SetRenderDrawColor(jogo->ren,0x00,0xff,0x00,0x00);
	SDL_RenderFillRect(jogo->ren,&jogo->edit.top_bar);
	SDL_SetRenderDrawColor(jogo->ren,0xff,0xff,0x00,0x00);
	
	if(AUX_WaitEventTimeoutCount(&(jogo->evt),&(jogo->espera))){//trocar por exercicio
		switch(jogo->evt.type){
			case SDL_MOUSEBUTTONDOWN://verifica os cliques do botão
				p.x = (int)jogo->evt.button.x; p.y = (int)jogo->evt.button.y;
				
				if (SDL_PointInRect(&p,&jogo->edit.botao_voltar))jogo->estado = JYH_LOAD_MENU;
				else if (SDL_PointInRect(&p,&jogo->edit.botao_salvar)){/*Salvar Nível*/}
				else if (SDL_PointInRect(&p,&jogo->edit.botao_run))jogo->estado = JYH_LOAD_LEVEL;
				
				break;
			case SDL_QUIT:
				jogo->estado = JYH_END_GAME;
				break;
		}
	}else{
	/*eventos baseados em tempo*/}
	
	//desenhar botões
	
	SDL_SetRenderDrawColor(jogo->ren,0xff,0x00,0x00,0x00);//cor botão 1
	SDL_RenderFillRect(jogo->ren,&jogo->edit.botao_voltar);
	SDL_RenderFillRect(jogo->ren,&jogo->edit.botao_salvar);
	SDL_RenderFillRect(jogo->ren,&jogo->edit.botao_run);

}

void JYH_GameLvlExecution(JYH_GameState* jogo){//Atualizar
	static SDL_Point p;
	
	SDL_SetRenderDrawColor(jogo->ren,0xff,0xff,0xff,0x00);//background
	SDL_RenderClear(jogo->ren);
	
	SDL_SetRenderDrawColor(jogo->ren,0x00,0xff,0x00,0x00);//barra do topo
	SDL_RenderFillRect(jogo->ren,&jogo->exec.top_bar);
	
	SDL_SetRenderDrawColor(jogo->ren,0xff,0x00,0xff,0x00);//icones
	SDL_RenderFillRect(jogo->ren,&jogo->exec.icone_gemas);
	SDL_RenderFillRect(jogo->ren,&jogo->exec.icone_relogio);
	
	SDL_SetRenderDrawColor(jogo->ren,0x00,0xff,0xff,0x00);//contadores
	SDL_RenderFillRect(jogo->ren,&jogo->exec.contagem_gemas);
	SDL_RenderFillRect(jogo->ren,&jogo->exec.contagem_tempo);
	
	if(AUX_WaitEventTimeoutCount(&(jogo->evt),&(jogo->espera))){//trocar por exercicio
		switch(jogo->evt.type){
			case SDL_MOUSEBUTTONDOWN://verifica os cliques do botão
				p.x = (int)jogo->evt.button.x; p.y = (int)jogo->evt.button.y;
				
				if (SDL_PointInRect(&p,&jogo->exec.botao_voltar))jogo->estado = JYH_LOAD_MENU;
				else if (SDL_PointInRect(&p,&jogo->exec.botao_voltar)){/*Reinicia o nivel*/}
				
				break;
			case SDL_QUIT:
				jogo->estado = JYH_END_GAME;
				break;
		}
	}else{
		//eventos baseados em tempo
	}
	//desenhar botões
	
	SDL_SetRenderDrawColor(jogo->ren,0xff,0x00,0x00,0x00);//botões
	SDL_RenderFillRect(jogo->ren,&jogo->exec.botao_voltar);
	SDL_RenderFillRect(jogo->ren,&jogo->exec.botao_reiniciar);
}

void JYH_GameLvlSelection(JYH_GameState* jogo){//Atualizar
	static SDL_Point p;

	SDL_SetRenderDrawColor(jogo->ren,0xff,0xff,0xff,0x00);//background
	SDL_RenderClear(jogo->ren);
	SDL_SetRenderDrawColor(jogo->ren,0xff,0xff,0x00,0x00);//"Nome Mundo"
	SDL_RenderFillRect(jogo->ren,&jogo->sel.title);
	
	
	if(AUX_WaitEventTimeoutCount(&(jogo->evt),&(jogo->espera))){//trocar por exercicio
		switch(jogo->evt.type){
			case SDL_MOUSEBUTTONDOWN://verifica os cliques do botão
				p.x = (int)jogo->evt.button.x; p.y = (int)jogo->evt.button.y;
				
				if (SDL_PointInRect(&p,&jogo->sel.botao_voltar))jogo->estado = JYH_LOAD_WORLD_SELECTION;
				
				break;
			case SDL_QUIT:
				jogo->estado = JYH_END_GAME;
				break;
		}
	}else{
		//eventos baseados em tempo
	}
	
	SDL_SetRenderDrawColor(jogo->ren,0xff,0x00,0x00,0x00);//cor botão 1
	SDL_RenderFillRect(jogo->ren,&jogo->sel.botao_voltar);
}
void JYH_GameLvlSelection_P(JYH_GameState* jogo){//Atualizar
	static SDL_Point p;
	
	SDL_SetRenderDrawColor(jogo->ren,0xff,0xff,0xff,0x00);//background
	SDL_RenderClear(jogo->ren);
	SDL_SetRenderDrawColor(jogo->ren,0xff,0xff,0x00,0x00);//"Meus Niveis"
	SDL_RenderFillRect(jogo->ren,&jogo->selP.title);
	
	if(AUX_WaitEventTimeoutCount(&(jogo->evt),&(jogo->espera))){//trocar por exercicio
		switch(jogo->evt.type){
			case SDL_MOUSEBUTTONDOWN://verifica os cliques do botão
				p.x = (int)jogo->evt.button.x; p.y = (int)jogo->evt.button.y;
				
				if (SDL_PointInRect(&p,&jogo->selP.botao_voltar))jogo->estado = JYH_LOAD_MENU;
				
				break;
			case SDL_QUIT:
				jogo->estado = JYH_END_GAME;
				break;
		}
	}else{
		//eventos baseados em tempo
	}
	
	SDL_SetRenderDrawColor(jogo->ren,0xff,0x00,0x00,0x00);//cor botão 1
	SDL_RenderFillRect(jogo->ren,&jogo->selP.botao_voltar);
}
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

/*ESTADOS DE TRANSIÇÃO*/
//estes estados não estão planejados para aguardar eventos
//atualizar para o tamanho dos botões de ajudar de acordo com a tela
void JYH_GameLoadMenu(JYH_GameState* jogo){
	SDL_SetRenderDrawColor(jogo->ren,0xff,0xff,0xff,0x00);//trocar por uma tela de loading
	SDL_RenderClear(jogo->ren);
	SDL_RenderPresent(jogo->ren);
	
	jogo->menu.title = (SDL_Rect){450,200,300,90};
	jogo->menu.botao_worlds = (SDL_Rect){450,450,300,30};
	jogo->menu.botao_selP = (SDL_Rect){450,500,300,30};
	jogo->menu.botao_edit = (SDL_Rect){450,550,300,30};
	
	printf("Menu\n");
	jogo->estado = JYH_MAIN_MENU;
}
void JYH_GameLoadEditor(JYH_GameState* jogo){
	SDL_SetRenderDrawColor(jogo->ren,0xff,0xff,0xff,0x00);//trocar por uma tela de loading
	SDL_RenderClear(jogo->ren);
	SDL_RenderPresent(jogo->ren);
	
	jogo->edit.botao_voltar = (SDL_Rect){25,25,50,50};
	jogo->edit.botao_salvar = (SDL_Rect){100,25,50,50};
	jogo->edit.botao_run = (SDL_Rect){175,25,50,50};
	jogo->edit.editor = (SDL_Rect){0,100,jogo->w_tela - 200,jogo->h_tela - 100};
	jogo->edit.side_bar = (SDL_Rect){1000,100,200,jogo->h_tela - 100};
	jogo->edit.top_bar = (SDL_Rect){0,0,jogo->w_tela,100};
	
	printf("Editor\n");
	jogo->estado = JYH_LVL_EDITOR;
}
void JYH_GameLoadExec(JYH_GameState* jogo){
	SDL_SetRenderDrawColor(jogo->ren,0xff,0xff,0xff,0x00);//trocar por uma tela de loading
	SDL_RenderClear(jogo->ren);
	SDL_RenderPresent(jogo->ren);

	jogo->exec.botao_voltar = (SDL_Rect){25,25,50,50};
	jogo->exec.botao_reiniciar =(SDL_Rect){100,25,50,50};
	jogo->exec.icone_gemas = (SDL_Rect){175,25,50,50};
	jogo->exec.contagem_gemas =(SDL_Rect){250,25,100,50};//onde é escrita a razão entre as gemas do nível e as gemas coletadas
	jogo->exec.icone_relogio = (SDL_Rect){375,25,50,50};
	jogo->exec.contagem_tempo = (SDL_Rect){450,25,100,50};//onde é escrita a contagem de tempo
	jogo->exec.timer = 0;//No Jogo Final depende do nível a ser carregado!!!
	jogo->exec.gemas_coletadas = 0;
	jogo->exec.tesouro_pego = 0;
	jogo->exec.tempo_de_jogo = 0;
	
	printf("Execucao\n");
	jogo->estado = JYH_LVL_EXEC;
}
void JYH_GameLoadSel(JYH_GameState* jogo){
	SDL_SetRenderDrawColor(jogo->ren,0xff,0xff,0xff,0x00);//trocar por uma tela de loading
	SDL_RenderClear(jogo->ren);
	SDL_RenderPresent(jogo->ren);
	
	jogo->sel.title =  (SDL_Rect){450,100,300,90};
	jogo->sel.botao_voltar = (SDL_Rect){25,25,50,50};
	jogo->sel.n_niveis = 10;//numero arbitrario temporario para testar a interface
	//jogo->sel.niveis
	
	
	
	printf("Mundo\n");
	jogo->estado = JYH_LVL_SELECTION;
}
void JYH_GameLoadSelP(JYH_GameState* jogo){
	SDL_SetRenderDrawColor(jogo->ren,0xff,0xff,0xff,0x00);//trocar por uma tela de loading
	SDL_RenderClear(jogo->ren);
	SDL_RenderPresent(jogo->ren);
	
	jogo->selP.title = (SDL_Rect){450,100,300,90};
	jogo->selP.botao_voltar = (SDL_Rect){25,25,50,50};
	jogo->selP.n_niveis = 10;//numero arbitrario temporario para testar a interface
	//jogo->selP.niveis
	
	printf("Biblioteca\n");
	jogo->estado = JYH_LVL_SELECTION_P;
}
void JYH_GameLoadWorlds(JYH_GameState* jogo){
	SDL_SetRenderDrawColor(jogo->ren,0xff,0xff,0xff,0x00);//trocar por uma tela de loading
	SDL_RenderClear(jogo->ren);
	SDL_RenderPresent(jogo->ren);
	
	jogo->worlds.title = (SDL_Rect){450,100,300,90};
	jogo->worlds.botao_voltar = (SDL_Rect){25,25,50,50};
	jogo->worlds.n_mundos = 5;//numero arbitrario temporario para testar a interface
	//jogo->selP.mundos
	
	
	printf("Mundos\n");
	jogo->estado = JYH_WORLD_SELECTION;
}
