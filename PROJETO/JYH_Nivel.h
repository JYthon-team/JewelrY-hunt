//JYH_Nivel.h
#ifndef JYH_NIVEL_L
#define JYH_NIVEL_L
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "JYH_Object.h"

typedef struct JYH_Tile{
    unsigned char t;//tile
	Uint32 o;//tipo do objeto
}JYH_Tile;

typedef struct JYH_Mundo{//estrutura de dados representando um mundo do jogo. Fica armazenada em arquivos para ser carregada para esta estrutura
	//lista dos niveis do mundo e estética na hora de seleção
	char nome[50];//Nome do mundo
	SDL_Texture* capa;//textura
}JYH_Mundo;

typedef struct JYH_Ass_Nivel{//Assinatura de um nível
    char nome_nivel[50];
    SDL_Texture* txt_nome;
}JYH_Ass_Nivel;

typedef struct JYH_Nivel{//estrutura de dados representando um nível do jogo. Fica armazenada em arquivos para ser carregada para esta estrutura
	//Grid do nível e posições dos objetos
    char nome_mundo[50];
    char nome_nivel[50];
    char       tema[50];
    Uint32 qtd_obj;
    SDL_Texture* txt_nome;
    SDL_Texture* txt_theme;
    JYH_Objeto* objetos;
    int w,h;
    JYH_Tile* mat;
}JYH_Nivel;
#endif