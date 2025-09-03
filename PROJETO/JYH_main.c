#include <SDL2/SDL.h>
#define JYH_SCREEN_WIDTH 1200
#define JYH_SCREEN_HEIGHT 700


enum GAME_STATE{
	JYH_END_GAME = 0,//estado para encerrar o jogo
	JYH_MAIN_MENU,//menu inicial
	JYH_LVL_EDITOR,//editor de niveis
	JYH_LVL_EXEC,//modo execução
	JYH_LVL_SELECTION,//seleção de níveis do jogo
	JYH_LVL_SELECTION_P,//seleção de níveis do player
	JYH_WORLD_SELECTION//escolher mundo
};

enum GAME_STATE gs;
//Assinaturas das funções dos estados do jogo
void JYH_GameMenu(SDL_Renderer* ren);
void JYH_GameLvlEditor(SDL_Renderer* ren);
void JYH_GameLvlExecution(SDL_Renderer* ren);
void JYH_GameLvlSelection(SDL_Renderer* ren);
void JYH_GameLvlSelection_P(SDL_Renderer* ren);
void JYH_GameWorldSelection(SDL_Renderer* ren);

void JYH_GameRender(SDL_Renderer* ren){
	switch(gs){
		case JYH_MAIN_MENU:
			JYH_GameMenu(ren);
			break;
		case JYH_LVL_EDITOR:
			JYH_GameLvlEditor(ren);
			break;
		case JYH_LVL_EXEC:
			JYH_GameLvlExecution(ren);
			break;
		case JYH_LVL_SELECTION:
			JYH_GameLvlSelection(ren);
			break;
		case JYH_LVL_SELECTION_P:
			JYH_GameLvlSelection_P(ren);
			break;
		case JYH_WORLD_SELECTION:
			JYH_GameWorldSelection(ren);
			break;
	}
}

int main (int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Jewelry Hunt",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,JYH_SCREEN_WIDTH, JYH_SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);
	gs = JYH_MAIN_MENU;

	while(gs)JYH_GameRender(ren);

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}

void JYH_GameMenu(SDL_Renderer* ren){
	static SDL_Event evt;
	static Uint32 espera;//usar no futuro para a implementação melhorada de esperar evento
	static SDL_Rect botao_1={450,450,300,30},botao_2={450,500,300,30},botao_3={450,550,300,30};
	static SDL_Rect nome = {450,200,300,90};
	static SDL_Point p;
	
	SDL_SetRenderDrawColor(ren,0xff,0xff,0xff,0x00);//background
	SDL_RenderClear(ren);
	
	SDL_SetRenderDrawColor(ren,0xff,0xff,0x00,0x00);
	SDL_RenderFillRect(ren,&nome);
	
	if(SDL_WaitEventTimeout(&evt,100)){//trocar por exercicio
		//evento detectado
		switch(evt.type){
			case SDL_MOUSEBUTTONDOWN://verifica os cliques do botão
				p.x = (int)evt.button.x; p.y = (int)evt.button.y;
				
				if (SDL_PointInRect(&p,&botao_1))gs = JYH_WORLD_SELECTION;
				else if (SDL_PointInRect(&p,&botao_2))gs = JYH_LVL_SELECTION_P;
				else if (SDL_PointInRect(&p,&botao_3))gs = JYH_LVL_EDITOR;
				
				break;
			case SDL_QUIT:
				gs = JYH_END_GAME;
				break;
		}
	}else{
		//eventos baseados em tempo
	}
	
	SDL_SetRenderDrawColor(ren,0xff,0x00,0x00,0x00);//cor botão 1
	SDL_RenderFillRect(ren,&botao_1);
	SDL_SetRenderDrawColor(ren,0x00,0xff,0x00,0x00);//cor botão 2
	SDL_RenderFillRect(ren,&botao_2);
	SDL_SetRenderDrawColor(ren,0x00,0x00,0xff,0x00);//cor botão 3
	SDL_RenderFillRect(ren,&botao_3);
	
	SDL_RenderPresent(ren);
}

void JYH_GameLvlEditor(SDL_Renderer* ren){//Atualizar
	static SDL_Event evt;
	static Uint32 espera;//usar no futuro para a implementação melhorada de esperar evento
	static SDL_Rect botao_1={25,25,50,50};//botões
	static SDL_Rect top_bar = {0,0,JYH_SCREEN_WIDTH,100}, side_bar = {1000,100,200,JYH_SCREEN_HEIGHT};//Estetica
	static SDL_Point p;
	SDL_SetRenderDrawColor(ren,0xff,0xff,0xff,0x00);//background
	SDL_RenderClear(ren);
	
	SDL_SetRenderDrawColor(ren,0x00,0x00,0xff,0x00);//estético
	SDL_RenderFillRect(ren,&side_bar);
	SDL_SetRenderDrawColor(ren,0x00,0xff,0x00,0x00);
	SDL_RenderFillRect(ren,&top_bar);
	
	if(SDL_WaitEventTimeout(&evt,100)){//trocar por exercicio
		//evento detectado
		switch(evt.type){
			case SDL_MOUSEBUTTONDOWN://verifica os cliques do botão
				p.x = (int)evt.button.x; p.y = (int)evt.button.y;
				
				if (SDL_PointInRect(&p,&botao_1))gs = JYH_MAIN_MENU;
				
				break;
			case SDL_QUIT:
				gs = JYH_END_GAME;
				break;
		}
	}else{
		//eventos baseados em tempo
	}
	
	SDL_SetRenderDrawColor(ren,0xff,0x00,0x00,0x00);//cor botão 1
	SDL_RenderFillRect(ren,&botao_1);
	
	SDL_RenderPresent(ren);
}

void JYH_GameLvlExecution(SDL_Renderer* ren){//Atualizar
	static SDL_Event evt;
	static Uint32 espera;//usar no futuro para a implementação melhorada de esperar evento
	static SDL_Rect botao_1={25,25,50,50};
	static SDL_Rect top_bar = {0,0,JYH_SCREEN_WIDTH,100};
	static SDL_Point p;
	SDL_SetRenderDrawColor(ren,0xff,0xff,0xff,0x00);//background
	SDL_RenderClear(ren);
	
	SDL_SetRenderDrawColor(ren,0x00,0xff,0x00,0x00);
	SDL_RenderFillRect(ren,&top_bar);
	
	if(SDL_WaitEventTimeout(&evt,100)){//trocar por exercicio
		//evento detectado
		switch(evt.type){
			case SDL_MOUSEBUTTONDOWN://verifica os cliques do botão
				p.x = (int)evt.button.x; p.y = (int)evt.button.y;
				
				if (SDL_PointInRect(&p,&botao_1))gs = JYH_MAIN_MENU;
				
				break;
			case SDL_QUIT:
				gs = JYH_END_GAME;
				break;
		}
	}else{
		//eventos baseados em tempo
	}
	
	SDL_SetRenderDrawColor(ren,0xff,0x00,0x00,0x00);//cor botão 1
	SDL_RenderFillRect(ren,&botao_1);
	
	SDL_RenderPresent(ren);
}

void JYH_GameLvlSelection(SDL_Renderer* ren){//Atualizar
	static SDL_Event evt;
	static Uint32 espera;//usar no futuro para a implementação melhorada de esperar evento
	static SDL_Rect botao_1={0,0,50,50};
	static SDL_Point p;
	SDL_SetRenderDrawColor(ren,0xff,0xff,0xff,0x00);//background
	SDL_RenderClear(ren);
	
	if(SDL_WaitEventTimeout(&evt,100)){//trocar por exercicio
		//evento detectado
		switch(evt.type){
			case SDL_MOUSEBUTTONDOWN://verifica os cliques do botão
				p.x = (int)evt.button.x; p.y = (int)evt.button.y;
				
				if (SDL_PointInRect(&p,&botao_1))gs = JYH_MAIN_MENU;
				
				break;
			case SDL_QUIT:
				gs = JYH_END_GAME;
				break;
		}
	}else{
		//eventos baseados em tempo
	}
	
	SDL_SetRenderDrawColor(ren,0xff,0x00,0x00,0x00);//cor botão 1
	SDL_RenderFillRect(ren,&botao_1);
	
	SDL_RenderPresent(ren);
}
void JYH_GameLvlSelection_P(SDL_Renderer* ren){//Atualizar
		static SDL_Event evt;
	static Uint32 espera;//usar no futuro para a implementação melhorada de esperar evento
	static SDL_Rect botao_1={0,0,50,50};
	static SDL_Point p;
	SDL_SetRenderDrawColor(ren,0xff,0xff,0xff,0x00);//background
	SDL_RenderClear(ren);
	
	if(SDL_WaitEventTimeout(&evt,100)){//trocar por exercicio
		//evento detectado
		switch(evt.type){
			case SDL_MOUSEBUTTONDOWN://verifica os cliques do botão
				p.x = (int)evt.button.x; p.y = (int)evt.button.y;
				
				if (SDL_PointInRect(&p,&botao_1))gs = JYH_MAIN_MENU;
				
				break;
			case SDL_QUIT:
				gs = JYH_END_GAME;
				break;
		}
	}else{
		//eventos baseados em tempo
	}
	
	SDL_SetRenderDrawColor(ren,0xff,0x00,0x00,0x00);//cor botão 1
	SDL_RenderFillRect(ren,&botao_1);
	
	SDL_RenderPresent(ren);
}

void JYH_GameWorldSelection(SDL_Renderer* ren){//Atualizar
			static SDL_Event evt;
	static Uint32 espera;//usar no futuro para a implementação melhorada de esperar evento
	static SDL_Rect botao_1={0,0,50,50};
	static SDL_Point p;
	SDL_SetRenderDrawColor(ren,0xff,0xff,0xff,0x00);//background
	SDL_RenderClear(ren);
	
	if(SDL_WaitEventTimeout(&evt,100)){//trocar por exercicio
		//evento detectado
		switch(evt.type){
			case SDL_MOUSEBUTTONDOWN://verifica os cliques do botão
				p.x = (int)evt.button.x; p.y = (int)evt.button.y;
				
				if (SDL_PointInRect(&p,&botao_1))gs = JYH_MAIN_MENU;
				
				break;
			case SDL_QUIT:
				gs = JYH_END_GAME;
				break;
		}
	}else{
		//eventos baseados em tempo
	}
	
	SDL_SetRenderDrawColor(ren,0xff,0x00,0x00,0x00);//cor botão 1
	SDL_RenderFillRect(ren,&botao_1);
	
	SDL_RenderPresent(ren);
}