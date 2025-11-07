//JYH_Object.h, podemos importar todos os objetos

typedef struct JYH_Icon{
	SDL_Texture* txt;//textura do botão
	SDL_Rect       r;//Retângulo de desenho
	Uint32         f;//frame
	Uint32         s;//estado
	Uint32       n_f;//quantidade de frames
}JYH_Icon;

enum JYH_OBJ_TYPE{
	JYH_OBJ_PLAYER,
	JYH_OBJ_GEM,
	JYH_OBJ_TROFEU,
	JYH_OBJ_INIMIGO,
	
	N_OBJECTS
};
//Struct dos Objetos
typedef struct{
}JYH_Obj_Player;
typedef struct{	
}JYH_Obj_Gem;
typedef struct{
}JYH_Obj_Trofeu;
typedef struct{
}JYH_Obj_Enemy;


typedef struct JYH_Objeto{//estrutura de dados representando os objetos de uma fase.
	Uint32 type;
	SDL_Rect hitbox;//verifica colisao nas coordenadas reais
	JYH_Icon drawbox;//aonde se desenha
	union{
		JYH_Obj_Player o_p;
		JYH_Obj_Trofeu o_t;
		JYH_Obj_Gem    o_g;
		JYH_Obj_Enemy  o_e;
	};
	//Sprites + tipo do objeto e o necessário para gerênciar a atualização
}JYH_Objeto;